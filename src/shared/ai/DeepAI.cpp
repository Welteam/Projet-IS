#include "DeepAI.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include "state.h"
#include "engine.h"
#include "engine/Cordinate.cpp"
#include "engine/DisplayAttack.cpp"
#include <chrono>

using namespace std::chrono;
using namespace state;
using namespace std;
using namespace engine;

vector<pair<Character, vector<pair<GameObject, int>>>> attackScore;
vector<pair<Character, vector<pair<GameObject, int>>>> movementScore;
vector<pair<Character, pair<GameObject, int>>> maxAttackScores;
vector<pair<Character, pair<GameObject, int>>> maxMovementScores;
vector<pair<Character, pair<GameObject, int>>> secondMaxAttackScores;
vector<pair<Character, pair<GameObject, int>>> secondMaxMovementScores;

vector<pair<Character, pair<GameObject, int>>> bestActions;
vector<pair<Character, pair<GameObject, int>>> secondBestActions;

vector<vector<pair<Character, pair<GameObject, int>>>> listBestActions;
vector<vector<pair<Character, pair<GameObject, int>>>> listsecondBestActions;

int passTurn = 1;

void ai::DeepAI::run(Engine &e) {
    bool terminateTurn = false;

    while(!terminateTurn){
        for(auto unit : e.getGameState().getActivePlayer().getUnits())
            calculateUnitAttack(e, unit);

        for(auto unit : e.getGameState().getActivePlayer().getUnits())
            calculateUnitMove(e, unit);

        /** TESTING BEST MOVEMENT AND BEST ATTACK **/
        for(auto maxAttackScore : maxAttackScores)
            cout << "best attack (" <<maxAttackScore.second.first.getX() << ", " <<  maxAttackScore.second.first.getY() <<") = " << maxAttackScore.second.second << endl;
        for(auto secondMaxAttackScore : secondMaxAttackScores)
            cout << "second best attack (" << secondMaxAttackScore.second.first.getX() << ", " << secondMaxAttackScore.second.first.getY() << ") = " << secondMaxAttackScore.second.second << endl;
        for(auto maxMovementScore : maxMovementScores)
            cout << "best move (" <<maxMovementScore.second.first.getX() << ", " <<  maxMovementScore.second.first.getY() <<") = " << maxMovementScore.second.second << endl;
        for(auto secondMaxMovementScore : secondMaxMovementScores)
            cout << "second best move (" << secondMaxMovementScore.second.first.getX() << ", " << secondMaxMovementScore.second.first.getY() << ") = " << secondMaxMovementScore.second.second << endl;

        terminateTurn = playActions(e, terminateTurn);

        int undoCommands = maxAttackScores.size()*2;

        for(int i = 0; i < undoCommands; i++){
            e.undoCommands();
        }

    }
    /** END THE TURN **/
    shared_ptr<Command> newTurnCommand = make_shared<NewTurnCommand>();
    e.addCommand(newTurnCommand, 1);
    e.runCommands();
}

bool ai::DeepAI::playActions(Engine &e, bool terminateTurn){
    for(int i = 0; i < static_cast<int>(maxMovementScores.size()); i++){
        pair<Character, pair<GameObject, int>> maxAttackScore = maxAttackScores.at(i);
        pair<Character, pair<GameObject, int>> maxMovementScore = maxMovementScores.at(i);
        if(passTurn > maxAttackScore.second.second && passTurn > maxMovementScore.second.second){
            bestActions.push_back(maxAttackScore);
            terminateTurn = true;
        } else if(maxAttackScore.second.second > maxMovementScore.second.second){
            terminateTurn = false;
            for(const auto& unit : e.getGameState().getActivePlayer().getUnits()){
                if(unit.getX() == maxAttackScore.first.getX() && unit.getY() == maxAttackScore.first.getY()){
                    e.getGameState().setSelectedUnit(make_shared<Character>(unit));
                }
            }
            if(e.getGameState().getSelectedUnit() != nullptr){
                bestActions.push_back(maxAttackScore);
                shared_ptr<Command> attack = make_shared<AttackCommand>(e.getGameState().getSelectedUnit(),
                                                                        maxAttackScore.second.first.getX(), maxAttackScore.second.first.getY());
                e.addCommand(attack, 1);
                e.runCommands();
                e.getGameState().unselectedUnit();
            }
        } else if (maxAttackScore.second.second < maxMovementScore.second.second){
            terminateTurn = false;
            Character tempUnit;
            for(const auto& unit : e.getGameState().getActivePlayer().getUnits()){
                if(unit.getX() == maxMovementScore.first.getX() && unit.getY() == maxMovementScore.first.getY()){
                    e.getGameState().setSelectedUnit(make_shared<Character>(unit));
                    tempUnit = unit;
                }
            }
            if(e.getGameState().getSelectedUnit() != nullptr) {
                bestActions.push_back(maxMovementScore);
                shared_ptr<Command> move = make_shared<MoveCommand>(e.getGameState().getSelectedUnit(),
                                                                    maxMovementScore.second.first.getX(),
                                                                    maxMovementScore.second.first.getY());
                e.addCommand(move, 1);
                if(!e.runCommands()){
                    e.undoCommands();
                    e.getGameState().unselectedUnit();
                    calculateUnitMove(e, tempUnit);
                    e.getGameState().setSelectedUnit(make_shared<Character>(tempUnit));
                    cout << "best move (" <<maxMovementScores.at(maxMovementScores.size()-1).second.first.getX() << ", " <<  maxMovementScores.at(maxMovementScores.size()-1).second.first.getY() <<") = " << maxMovementScores.at(maxMovementScores.size()-1).second.second << endl;
                    std::cout << "("<< e.getGameState().getSelectedUnit()->getX() << ", "<< e.getGameState().getSelectedUnit()->getY() <<")" << std::endl;
                    shared_ptr<Command> move2 = make_shared<MoveCommand>(e.getGameState().getSelectedUnit(),
                                                                        maxMovementScores.at(maxMovementScores.size()-1).second.first.getX(),
                                                                        maxMovementScores.at(maxMovementScores.size()-1).second.first.getY());
                    e.addCommand(move2, 1);
                    e.runCommands();
                    bestActions.pop_back();
                    bestActions.push_back(maxMovementScores.at(maxMovementScores.size()-1));
                    maxMovementScores.pop_back();
                }
                e.getGameState().unselectedUnit();
            }
        }
    }
    attackScore.clear();
    movementScore.clear();
    maxAttackScores.clear();
    maxMovementScores.clear();
    secondMaxAttackScores.clear();
    secondMaxMovementScores.clear();
    listBestActions.push_back(bestActions);
    bestActions.clear();
    return terminateTurn;
}


void ai::DeepAI::calculateUnitAttack(Engine &e, Character unit){
    pair<Character, pair<GameObject, int>> maxAttackScore{};
    pair<Character, pair<GameObject, int>> secondMaxAttackScore{};
    if(!unit.getHasAttacked()){
        e.getGameState().setSelectedUnit(make_shared<Character>(unit));
        vector<pair<GameObject, int>> targetsAndScores;
        vector<int> attackField = DisplayAttack::createField(e.getGameState().getSelectedUnit().get(), e.getGameState().getWorld(), e.getGameState().getGameObjects());
        for(int i = 0; i < static_cast<int>(attackField.size()); i++){
            if(attackField[i] == 1) {
                GameObject target{i % e.getGameState().getWorld().getYMax(), i / e.getGameState().getWorld().getYMax()};
                vector<engine::Node> damageArea = DisplayAttack::createDamageArea(
                        i % e.getGameState().getWorld().getYMax(), i / e.getGameState().getWorld().getYMax(),
                        e.getGameState().getSelectedUnit().get(), e.getGameState().getWorld());
                int score = 0;
                vector<Character> enemies;
                // Pour chaque case pouvant être être touché, on regarde si on touche un enemi
                for (auto node : damageArea) {
                    if (e.getGameState().getActivePlayer().getId() == 2) {
                        enemies = e.getGameState().getPlayer1().getUnits();
                    } else {
                        enemies = e.getGameState().getPlayer2().getUnits();
                    }
                    // Pour chaque enemi touché : score = dommage causés + 10 par enemi tué + ...si l'enemi à moins de vie
                    for (auto enemy : enemies) {
                        if (node.x == enemy.getX() && node.y == enemy.getY()) {
                            score += unit.getWeapon().getDamage();
                            score += 100 / enemy.getHp();
                            if (enemy.getHp() <= unit.getWeapon().getDamage()) {
                                score += 10;
                            }
                            // Score multiplié par 2 si enemi touchable par une autre unité
                            for (auto scores : attackScore) {
                                for (auto position : scores.second) {
                                    if (node.x == position.first.getX() && node.y == position.first.getY()) {
                                        score = score * 2;
                                    }
                                }
                            }
                        }
                    }
                }
                if (score > 0) {
                    pair<GameObject, int> targetAndScore{target, score};
                    targetsAndScores.push_back(targetAndScore);
                    if(score > maxAttackScore.second.second){
                        secondMaxAttackScore.first = maxAttackScore.first;
                        secondMaxAttackScore.second.first = maxAttackScore.second.first;
                        secondMaxAttackScore.second.second = maxAttackScore.second.second;

                        maxAttackScore.first = unit;
                        maxAttackScore.second.first = target;
                        maxAttackScore.second.second = score;
                    } else if (score > secondMaxAttackScore.second.second){
                        secondMaxAttackScore.first = unit;
                        secondMaxAttackScore.second.first = target;
                        secondMaxAttackScore.second.second = score;
                    }
                }
            }
        }
        if(!targetsAndScores.empty()){
            pair<Character, vector<pair<GameObject, int>>> unitAndSCore{unit, targetsAndScores};
            attackScore.push_back(unitAndSCore);
            maxAttackScores.push_back(maxAttackScore);
            secondMaxAttackScores.push_back(secondMaxAttackScore);
        } else {
            maxAttackScore.first = unit;
            maxAttackScore.second.first = GameObject{unit.getX(), unit.getY()};;
            maxAttackScore.second.second = 0;
            maxAttackScores.push_back(maxAttackScore);

            secondMaxAttackScore.first = unit;
            secondMaxAttackScore.second.first = GameObject{unit.getX(), unit.getY()};;
            secondMaxAttackScore.second.second = 0;
            secondMaxAttackScores.push_back(secondMaxAttackScore);
        }
    } else {
        maxAttackScore.first = unit;
        maxAttackScore.second.first = GameObject{unit.getX(), unit.getY()};;
        maxAttackScore.second.second = 0;
        maxAttackScores.push_back(maxAttackScore);

        secondMaxAttackScore.first = unit;
        secondMaxAttackScore.second.first = GameObject{unit.getX(), unit.getY()};;
        secondMaxAttackScore.second.second = 0;
        secondMaxAttackScores.push_back(secondMaxAttackScore);
    }
    e.getGameState().unselectedUnit();
}

void ai::DeepAI::calculateUnitMove(Engine &e, Character unit){
    pair<Character, pair<GameObject, int>> maxMovementScore{};
    pair<Character, pair<GameObject, int>> secondMaxMovementScore{};
    vector<GameObject> gameObjects = e.getGameState().getGameObjects();
    bool alreadyCalculAstar = false;
    if(unit.getPm() > 0){
        e.getGameState().setSelectedUnit(make_shared<Character>(unit));
        vector<pair<GameObject, int>> movementsAndScores;
        for(int x = unit.getX()-unit.getPm(); x <= unit.getX()+unit.getPm(); x++) {
            for (int y = unit.getY()-unit.getPm(); y <= unit.getY()+unit.getPm(); y++) {
                if ((unit.getX()-abs(x)) + (unit.getY()-abs(y)) <= unit.getPm()) {
                    vector<Node> nodesForAchievableMove = Cordinate::aStar(Node{.x = unit.getX(), .y = unit.getY()}, Node{.x = x, .y = y}, e.getGameState().getWorld(), e.getGameState().getGameObjects(), unit.getPm());
                    if(nodesForAchievableMove.size() > 1){
                        GameObject move{x, y};
                        vector<Character> enemies;
                        int distanceFromNearestEnemi = 1000;
                        int score = 0;
                        if (e.getGameState().getActivePlayer().getId() == 2) {
                            enemies = e.getGameState().getPlayer1().getUnits();
                        } else {
                            enemies = e.getGameState().getPlayer2().getUnits();
                        }
                        if(unit.getHasAttacked()){
                            score = 5;
                            for (auto enemy : enemies) {
                                bool couldBeAttack = false;
                                shared_ptr<Character> enemy_ptr = make_shared<Character>(enemy);
                                vector<int> attackField = DisplayAttack::createField(enemy_ptr.get(), e.getGameState().getWorld(), e.getGameState().getGameObjects());
                                for(int i = 0; i < static_cast<int>(attackField.size()); i++) {
                                    if (attackField[i] == 1) {
                                        vector<engine::Node> damageArea = DisplayAttack::createDamageArea(
                                                i % e.getGameState().getWorld().getYMax(), i / e.getGameState().getWorld().getYMax(),
                                                enemy_ptr.get(), e.getGameState().getWorld());
                                        for (auto node : damageArea) {
                                            if (node.x == x && node.y == y){
                                                score--;
                                                couldBeAttack  = true;
                                                break;
                                            }
                                        }
                                        if(couldBeAttack){
                                            break;
                                        }
                                    }
                                }
                            }
                        } else {
                            if(!alreadyCalculAstar){
                                for (const auto& enemy : enemies) {
                                    vector<Node> nodesFromEnemy = Cordinate::aStar(Node{.x = unit.getX(), .y = unit.getY()}, Node{.x = enemy.getX(), .y = enemy.getY()}, e.getGameState().getWorld(), gameObjects);
                                    if(nodesFromEnemy.size() > 1){
                                        if(static_cast<int>(nodesFromEnemy.size()) < distanceFromNearestEnemi){
                                            if(unit.getWeapon().getDamage() > static_cast<int>(nodesFromEnemy.size())){
                                                score = unit.getWeapon().getDamage()-nodesFromEnemy.size();
                                            } else {
                                                score = 5;
                                            }
                                            distanceFromNearestEnemi = nodesFromEnemy.size();
                                            if(static_cast<int>(nodesFromEnemy.size()) > unit.getPm()){
                                                move = GameObject{nodesFromEnemy.at(unit.getPm()).x, nodesFromEnemy.at(unit.getPm()).y};
                                            } else {
                                                move = GameObject{nodesFromEnemy.at(nodesFromEnemy.size()-1).x, nodesFromEnemy.at(nodesFromEnemy.size()-1).y};
                                            }
                                            alreadyCalculAstar = true;
                                            for(auto moveTaken : maxMovementScores){
                                                cout << moveTaken.second.first.getX() << move.getX() << moveTaken.second.first.getY() << move.getY() << endl;
                                                if(moveTaken.second.first.getX()  == move.getX() && moveTaken.second.first.getY() == move.getY()){
                                                    alreadyCalculAstar = false;
                                                } else if(maxMovementScore.second.first.getX()  == move.getX() &&  maxMovementScore.second.first.getY() == move.getY()){
                                                    alreadyCalculAstar = false;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        if (score > 0) {
                            pair<GameObject, int> targetAndScore{move, score};
                            movementsAndScores.push_back(targetAndScore);
                            gameObjects.push_back(move);
                            if(score > maxMovementScore.second.second){
                                secondMaxMovementScore.first = maxMovementScore.first;
                                secondMaxMovementScore.second.first = maxMovementScore.second.first;
                                secondMaxMovementScore.second.second = maxMovementScore.second.second;

                                maxMovementScore.first = unit;
                                maxMovementScore.second.first = move;
                                maxMovementScore.second.second = score;
                                alreadyCalculAstar = false;
                            } else if (score >= secondMaxMovementScore.second.second){
                                secondMaxMovementScore.first = unit;
                                secondMaxMovementScore.second.first = move;
                                secondMaxMovementScore.second.second = score;
                                alreadyCalculAstar = true;
                            }
                        }
                    }
                }
            }
        }
        if(!movementsAndScores.empty()){
            pair<Character, vector<pair<GameObject, int>>> unitAndScore{unit, movementsAndScores};
            movementScore.push_back(unitAndScore);
            maxMovementScores.push_back(maxMovementScore);
            secondMaxMovementScores.push_back(secondMaxMovementScore);
        } else {
            maxMovementScore.first = unit;
            maxMovementScore.second.first = GameObject{unit.getX(), unit.getY()};;
            maxMovementScore.second.second = 0;
            maxMovementScores.push_back(maxMovementScore);

            secondMaxMovementScore.first = unit;
            secondMaxMovementScore.second.first = GameObject{unit.getX(), unit.getY()};;
            secondMaxMovementScore.second.second = 0;
            secondMaxMovementScores.push_back(secondMaxMovementScore);
        }
    } else {
        maxMovementScore.first = unit;
        maxMovementScore.second.first = GameObject{unit.getX(), unit.getY()};;
        maxMovementScore.second.second = 0;
        maxMovementScores.push_back(maxMovementScore);

        secondMaxMovementScore.first = unit;
        secondMaxMovementScore.second.first = GameObject{unit.getX(), unit.getY()};;
        secondMaxMovementScore.second.second = 0;
        secondMaxMovementScores.push_back(secondMaxMovementScore);
    }
    e.getGameState().unselectedUnit();
}

