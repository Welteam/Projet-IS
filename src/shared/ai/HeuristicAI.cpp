#include "HeuristicAI.h"
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
        // TODO make a vector of the two variables below
        vector<pair<Character, pair<GameObject, int>>> maxAttackScores{};
        vector<pair<Character, pair<GameObject, int>>> maxMovementScores{};

        int passTurn = 1;


        auto start = high_resolution_clock::now();
        /********************************/
        /** Evaluating attacking score **/
        /********************************/
        // Parcours chaque unité de l'IA pour leur attribuer des scores à leurs cibles potentiels
        for(auto unit : e.getGameState().getActivePlayer().getUnits()){
            pair<Character, pair<GameObject, int>> maxAttackScore{};
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
                            // TODO : when score equal to previous score, add it to the vector
                            if(score > maxAttackScore.second.second){
                                // TODO : empty vector in this case before pushBack()
                                maxAttackScore.first = unit;
                                maxAttackScore.second.first = target;
                                maxAttackScore.second.second = score;
                            }
                        }
                    }
                }
                if(!targetsAndScores.empty()){
                    pair<Character, vector<pair<GameObject, int>>> unitAndSCore{unit, targetsAndScores};
                    attackScore.push_back(unitAndSCore);
                    maxAttackScores.push_back(maxAttackScore);
                } else {
                    maxAttackScore.first = unit;
                    maxAttackScore.second.first = GameObject{unit.getX(), unit.getY()};;
                    maxAttackScore.second.second = 0;
                    maxAttackScores.push_back(maxAttackScore);
                }
            } else {
                maxAttackScore.first = unit;
                maxAttackScore.second.first = GameObject{unit.getX(), unit.getY()};;
                maxAttackScore.second.second = 0;
                maxAttackScores.push_back(maxAttackScore);
            }
            e.getGameState().unselectedUnit();
        }
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "Time taken by attack calculation : "
             << duration.count()/1000 << " milliseconds" << endl;


        auto start2 = high_resolution_clock::now();
        /*******************************/
        /** Evaluating movement score **/
        /*******************************/
        // Parcours chaque unité de l'IA pour leur attribuer des scores à leurs déplacements potentiels
        for(auto unit : e.getGameState().getActivePlayer().getUnits()) {
            pair<Character, pair<GameObject, int>> maxMovementScore{};
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
                                                /*int distanceEucli = sqrt((enemy.getX() - x)*(enemy.getX() - x) + (enemy.getY() - y)*(enemy.getY() - y));
                                                if(distanceEucli < distanceFromNearestEnemi){
                                                score = unit.getWeapon().getDamage()-distanceEucli;
                                                distanceFromNearestEnemi = distanceEucli;
                                                }*/
                                            vector<Node> nodesFromEnemy = Cordinate::aStar(Node{.x = unit.getX(), .y = unit.getY()}, Node{.x = enemy.getX(), .y = enemy.getY()}, e.getGameState().getWorld(), e.getGameState().getGameObjects());
                                            if(nodesFromEnemy.size() > 1){
                                                if(static_cast<int>(nodesFromEnemy.size()) < distanceFromNearestEnemi){
                                                    score = unit.getWeapon().getDamage()-nodesFromEnemy.size();
                                                    distanceFromNearestEnemi = nodesFromEnemy.size();
                                                    if(static_cast<int>(nodesFromEnemy.size()) > unit.getPm()){
                                                        move = GameObject{nodesFromEnemy.at(unit.getPm()).x, nodesFromEnemy.at(unit.getPm()).y};
                                                    } else {
                                                        move = GameObject{nodesFromEnemy.at(nodesFromEnemy.size()-1).x, nodesFromEnemy.at(nodesFromEnemy.size()-1).y};
                                                    }
                                                    alreadyCalculAstar = true;
                                                    for(auto moveTaken : maxMovementScores)
                                                        if(moveTaken.second.first.getX()  == move.getX() && moveTaken.second.first.getY() == move.getY())
                                                            alreadyCalculAstar = false;
                                                }
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
                if(!movementsAndScores.empty()){
                    pair<Character, vector<pair<GameObject, int>>> unitAndScore{unit, movementsAndScores};
                    movementScore.push_back(unitAndScore);
                    maxMovementScores.push_back(maxMovementScore);
                } else {
                    maxMovementScore.first = unit;
                    maxMovementScore.second.first = GameObject{unit.getX(), unit.getY()};;
                    maxMovementScore.second.second = 0;
                    maxMovementScores.push_back(maxMovementScore);
                }
            } else {
                maxMovementScore.first = unit;
                maxMovementScore.second.first = GameObject{unit.getX(), unit.getY()};;
                maxMovementScore.second.second = 0;
                maxMovementScores.push_back(maxMovementScore);
            }
            e.getGameState().unselectedUnit();
        }

        auto stop2 = high_resolution_clock::now();
        auto duration2 = duration_cast<microseconds>(stop2 - start2);
        cout << "Time taken by move calculation : "
             << duration2.count()/1000 << " milliseconds" << endl;



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


        // TODO : Pick a random score from both best scores vector


        /** TESTING BEST MOVEMENT AND BEST ATTACK **/
        for(auto maxAttackScore : maxAttackScores)
        cout << "best attack (" <<maxAttackScore.second.first.getX() << ", " <<  maxAttackScore.second.first.getY() <<") = " << maxAttackScore.second.second << endl;
        for(auto maxMovementScore : maxMovementScores)
        cout << "best move (" <<maxMovementScore.second.first.getX() << ", " <<  maxMovementScore.second.first.getY() <<") = " << maxMovementScore.second.second << endl;

        for(int i = 0; i < static_cast<int>(maxMovementScores.size()); i++){
            pair<Character, pair<GameObject, int>> maxAttackScore = maxAttackScores.at(i);
            pair<Character, pair<GameObject, int>> maxMovementScore = maxMovementScores.at(i);
            if(passTurn > maxAttackScore.second.second && passTurn > maxMovementScore.second.second){
                terminateTurn = true;
            } else if(maxAttackScore.second.second > maxMovementScore.second.second){
                terminateTurn = false;
                for(const auto& unit : e.getGameState().getActivePlayer().getUnits()){
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
                terminateTurn = false;
                for(const auto& unit : e.getGameState().getActivePlayer().getUnits()){
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
    }


    /** END THE TURN **/
    shared_ptr<Command> newTurnCommand = make_shared<NewTurnCommand>();
    e.addCommand(newTurnCommand, 1);
    e.runCommands();

}

void ai::HeuristicAI::run_thread(engine::Engine &e, std::mutex &input_lock) {
    bool terminateTurn = false;
    input_lock.lock();

    while(!terminateTurn){



        /****************************/
        /** LOCAL VARIABLES NEEDED **/
        /****************************/
        // Create a vector of scoreAllies attack that will store every case where score is
        vector<pair<Character, vector<pair<GameObject, int>>>> attackScore;
        vector<pair<Character, vector<pair<GameObject, int>>>> movementScore;
        // TODO make a vector of the two variables below
        vector<pair<Character, pair<GameObject, int>>> maxAttackScores{};
        vector<pair<Character, pair<GameObject, int>>> maxMovementScores{};

        int passTurn = 1;


        auto start = high_resolution_clock::now();
        /********************************/
        /** Evaluating attacking score **/
        /********************************/
        // Parcours chaque unité de l'IA pour leur attribuer des scores à leurs cibles potentiels
        for(auto unit : e.getGameState().getActivePlayer().getUnits()){
            pair<Character, pair<GameObject, int>> maxAttackScore{};
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
                            // TODO : when score equal to previous score, add it to the vector
                            if(score > maxAttackScore.second.second){
                                // TODO : empty vector in this case before pushBack()
                                maxAttackScore.first = unit;
                                maxAttackScore.second.first = target;
                                maxAttackScore.second.second = score;
                            }
                        }
                    }
                }
                if(!targetsAndScores.empty()){
                    pair<Character, vector<pair<GameObject, int>>> unitAndSCore{unit, targetsAndScores};
                    attackScore.push_back(unitAndSCore);
                    maxAttackScores.push_back(maxAttackScore);
                } else {
                    maxAttackScore.first = unit;
                    maxAttackScore.second.first = GameObject{unit.getX(), unit.getY()};;
                    maxAttackScore.second.second = 0;
                    maxAttackScores.push_back(maxAttackScore);
                }
            } else {
                maxAttackScore.first = unit;
                maxAttackScore.second.first = GameObject{unit.getX(), unit.getY()};;
                maxAttackScore.second.second = 0;
                maxAttackScores.push_back(maxAttackScore);
            }
            e.getGameState().unselectedUnit();
        }
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "Time taken by attack calculation : "
             << duration.count()/1000 << " milliseconds" << endl;

        auto start2 = high_resolution_clock::now();
        /*******************************/
        /** Evaluating movement score **/
        /*******************************/
        // Parcours chaque unité de l'IA pour leur attribuer des scores à leurs déplacements potentiels
        for(auto unit : e.getGameState().getActivePlayer().getUnits()) {
            pair<Character, pair<GameObject, int>> maxMovementScore{};
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
                                            /*int distanceEucli = sqrt((enemy.getX() - x)*(enemy.getX() - x) + (enemy.getY() - y)*(enemy.getY() - y));
                                            if(distanceEucli < distanceFromNearestEnemi){
                                            score = unit.getWeapon().getDamage()-distanceEucli;
                                            distanceFromNearestEnemi = distanceEucli;
                                            }*/
                                            vector<Node> nodesFromEnemy = Cordinate::aStar(Node{.x = unit.getX(), .y = unit.getY()}, Node{.x = enemy.getX(), .y = enemy.getY()}, e.getGameState().getWorld(), e.getGameState().getGameObjects());
                                            if(nodesFromEnemy.size() > 1){
                                                if(static_cast<int>(nodesFromEnemy.size()) < distanceFromNearestEnemi){
                                                    score = unit.getWeapon().getDamage()-nodesFromEnemy.size();
                                                    distanceFromNearestEnemi = nodesFromEnemy.size();
                                                    if(static_cast<int>(nodesFromEnemy.size()) > unit.getPm()){
                                                        move = GameObject{nodesFromEnemy.at(unit.getPm()).x, nodesFromEnemy.at(unit.getPm()).y};
                                                    } else {
                                                        move = GameObject{nodesFromEnemy.at(nodesFromEnemy.size()-1).x, nodesFromEnemy.at(nodesFromEnemy.size()-1).y};
                                                    }
                                                    alreadyCalculAstar = true;
                                                    for(auto moveTaken : maxMovementScores)
                                                        if(moveTaken.second.first.getX()  == move.getX() && moveTaken.second.first.getY() == move.getY())
                                                            alreadyCalculAstar = false;
                                                }
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
                if(!movementsAndScores.empty()){
                    pair<Character, vector<pair<GameObject, int>>> unitAndScore{unit, movementsAndScores};
                    movementScore.push_back(unitAndScore);
                    maxMovementScores.push_back(maxMovementScore);
                } else {
                    maxMovementScore.first = unit;
                    maxMovementScore.second.first = GameObject{unit.getX(), unit.getY()};;
                    maxMovementScore.second.second = 0;
                    maxMovementScores.push_back(maxMovementScore);
                }
            } else {
                maxMovementScore.first = unit;
                maxMovementScore.second.first = GameObject{unit.getX(), unit.getY()};;
                maxMovementScore.second.second = 0;
                maxMovementScores.push_back(maxMovementScore);
            }
            e.getGameState().unselectedUnit();
        }

        auto stop2 = high_resolution_clock::now();
        auto duration2 = duration_cast<microseconds>(stop2 - start2);
        cout << "Time taken by move calculation : "
             << duration2.count()/1000 << " milliseconds" << endl;


        /** TESTING BEST MOVEMENT AND BEST ATTACK **/
        for(auto maxAttackScore : maxAttackScores)
            cout << "best attack (" <<maxAttackScore.second.first.getX() << ", " <<  maxAttackScore.second.first.getY() <<") = " << maxAttackScore.second.second << endl;
        for(auto maxMovementScore : maxMovementScores)
            cout << "best move (" <<maxMovementScore.second.first.getX() << ", " <<  maxMovementScore.second.first.getY() <<") = " << maxMovementScore.second.second << endl;

        for(int i = 0; i < static_cast<int>(maxMovementScores.size()); i++){
            pair<Character, pair<GameObject, int>> maxAttackScore = maxAttackScores.at(i);
            pair<Character, pair<GameObject, int>> maxMovementScore = maxMovementScores.at(i);
            if(passTurn > maxAttackScore.second.second && passTurn > maxMovementScore.second.second){
                terminateTurn = true;
            } else if(maxAttackScore.second.second > maxMovementScore.second.second){
                terminateTurn = false;
                for(const auto& unit : e.getGameState().getActivePlayer().getUnits()){
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
                terminateTurn = false;
                for(const auto& unit : e.getGameState().getActivePlayer().getUnits()){
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
    }


    /** END THE TURN **/
    shared_ptr<Command> newTurnCommand = make_shared<NewTurnCommand>();
    e.addCommand(newTurnCommand, 1);
    e.runCommands();
    input_lock.unlock();
}
