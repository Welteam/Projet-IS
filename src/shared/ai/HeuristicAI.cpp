#include "HeuristicAI.h"
#include <random>
#include <iostream>
#include <vector>
#include "state.h"
#include "engine.h"
#include "engine/Cordinate.cpp"
#include "engine/DisplayAttack.cpp"

using namespace state;
using namespace std;
using namespace engine;

void ai::HeuristicAI::run(Engine &e) {
    /* First -> if a unit can make damage, score is equal to damage.
     * For example : if you can make -30 on the hp, the score of the action is 30.
     * +10 bonus if the enemy can be kill or have less hp than another.
     *
     * Second -> if a unit can't make damage to an enemy,
     * score for moving is equal to the distance from the nearest enemy, and
     * score is higher if you can be close to an enemy.
     * For example : enemy is 9 case in front of you,
     * if you move 5 pm behind you, the score is -5, if you go in front, it's 5.
     * If you can be close to an allies, it's +1/2 per case.
     *
     * But if the unit has attacked, his movementScore is even bigger
     * if he can't find a movement where I cannot be attacked */
    bool terminateTurn = false;

    while(!terminateTurn){
        /****************************/
        /** LOCAL VARIABLES NEEDED **/
        /****************************/
        // Create a vector of scoreAllies attack that will store every case where score is
        vector<pair<Character, vector<pair<GameObject, int>>>> attackScore;
        vector<pair<Character, vector<pair<GameObject, int>>>> movementScore;
        pair<Character, pair<GameObject, int>> maxAttackScore{};
        pair<Character, pair<GameObject, int>> maxMovementScore{};
        int passTurn = 1;


        /********************************/
        /** Evaluating attacking score **/
        /********************************/
        // Parcours chaque unité de l'IA pour leur attribuer des scores à leurs cibles potentiels
        for(auto unit : e.getGameState().getActivePlayer().getUnits()){
            if(!unit.getHasAttacked()){
                e.getGameState().setSelectedUnit(make_shared<Character>(unit));
                vector<pair<GameObject, int>> targetsAndScores;
                vector<int> attackField = DisplayAttack::createField(e.getGameState().getSelectedUnit().get(), e.getGameState().getWorld(), e.getGameState().getGameObjects());
                for(int i = 0; i < attackField.size(); i++){
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
                                        score = +10;
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
                                maxAttackScore.first = unit;
                                maxAttackScore.second.first = target;
                                maxAttackScore.second.second = score;
                            }
                        }
                    }
                }
                if(targetsAndScores.size() > 0){
                    pair<Character, vector<pair<GameObject, int>>> unitAndSCore{unit, targetsAndScores};
                    attackScore.push_back(unitAndSCore);
                }
            }
            e.getGameState().unselectedUnit();
        }



        /*******************************/
        /** Evaluating movement score **/
        /*******************************/
        // Parcours chaque unité de l'IA pour leur attribuer des scores à leurs déplacements potentiels
        for(auto unit : e.getGameState().getActivePlayer().getUnits()) {
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
                                        for(int i = 0; i < attackField.size(); i++) {
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
                                    for (auto enemy : enemies) {
                                        vector<Node> nodesFromEnemy = Cordinate::aStar(Node{.x = x, .y = y}, Node{.x = enemy.getX(), .y = enemy.getY()}, e.getGameState().getWorld(), e.getGameState().getGameObjects());
                                        if(nodesFromEnemy.size() > 1){
                                            if(nodesFromEnemy.size() < distanceFromNearestEnemi){
                                                score = unit.getWeapon().getDamage()-nodesFromEnemy.size();
                                                distanceFromNearestEnemi = nodesFromEnemy.size();
                                            }
                                        }
                                    }
                                }
                                if (score > 0) {
                                    pair<GameObject, int> targetAndScore{move, score};
                                    movementsAndScores.push_back(targetAndScore);
                                    if(score > maxMovementScore.second.second){
                                        maxMovementScore.first = unit;
                                        maxMovementScore.second.first = move;
                                        maxMovementScore.second.second = score;
                                    }
                                }
                            }
                        }
                    }
                }
                if(movementsAndScores.size() > 0){
                    pair<Character, vector<pair<GameObject, int>>> unitAndScore{unit, movementsAndScores};
                    movementScore.push_back(unitAndScore);
                }
            }
            e.getGameState().unselectedUnit();
        }



        /** TESTING EVALUATION OF ATTACK SCORE **/
        /*for(auto score : attackScore){
            cout << "score of unit in position : (" << score.first.getX() << ", " << score.first.getY() << ")" << endl;
            for(auto position : score.second){
                cout << "target at : (" << position.first.getX() << ", " << position.first.getY() << ") has a score of " << position.second << endl;
            }
            cout << "" << endl;
        }
        if(attackScore.empty()){
            cout << "All scores for attack is 0" << endl;
        }*/



        /** TESTING EVALUATION OF MOVE SCORE **/
        /*for(auto score : movementScore){
            cout << "score of unit in position : (" << score.first.getX() << ", " << score.first.getY() << ")" << endl;
            for(auto position : score.second){
                cout << "move to : (" << position.first.getX() << ", " << position.first.getY() << ") has a score of " << position.second << endl;
            }
            cout << "" << endl;
        }
        if(movementScore.empty()){
            cout << "All scores for movement is 0" << endl;
        }*/



        /** TESTING BEST MOVEMENT AND BEST ATTACK **/
        cout << "best attack (" <<maxAttackScore.second.first.getX() << ", " <<  maxAttackScore.second.first.getY() <<") = " << maxAttackScore.second.second << endl;
        cout << "best move (" <<maxMovementScore.second.first.getX() << ", " <<  maxMovementScore.second.first.getY() <<") = " << maxMovementScore.second.second << endl;

        if(passTurn > maxAttackScore.second.second && passTurn > maxMovementScore.second.second){
            terminateTurn = true;
        } else if(maxAttackScore.second.second > maxMovementScore.second.second){
            for(auto unit : e.getGameState().getActivePlayer().getUnits()){
                if(unit.getX() == maxAttackScore.first.getX() && unit.getY() == maxAttackScore.first.getY()){
                    e.getGameState().setSelectedUnit(make_shared<Character>(unit));
                }
            }
            if(e.getGameState().getSelectedUnit() != nullptr){
                shared_ptr<Command> attack = make_shared<AttackCommand>(e.getGameState().getSelectedUnit(),
                        maxAttackScore.second.first.getX(), maxAttackScore.second.first.getY());
                e.addCommand(attack, 1);
                e.runCommands();
                e.getGameState().unselectedUnit();
            }
        } else if (maxAttackScore.second.second < maxMovementScore.second.second){
            for(auto unit : e.getGameState().getActivePlayer().getUnits()){
                if(unit.getX() == maxMovementScore.first.getX() && unit.getY() == maxMovementScore.first.getY()){
                    e.getGameState().setSelectedUnit(make_shared<Character>(unit));
                }
            }
            if(e.getGameState().getSelectedUnit() != nullptr) {
                shared_ptr<Command> move = make_shared<MoveCommand>(e.getGameState().getSelectedUnit(),
                                                                    maxMovementScore.second.first.getX(),
                                                                    maxMovementScore.second.first.getY());
                e.addCommand(move, 1);
                e.runCommands();
                e.getGameState().unselectedUnit();
            }
        }
    }


    /** END THE TURN **/
    shared_ptr<Command> newTurnCommand = make_shared<NewTurnCommand>();
    e.addCommand(newTurnCommand, 1);
    e.runCommands();

}
