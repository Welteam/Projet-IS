#include <iostream>
#include <SFML/Graphics.hpp>
#include "render/Animation.hpp"
#include "render/AnimatedSprite.hpp"
#include <state.h>
#include "render.h"
#include "engine.h"
#include "client.h"
#include "ai.h"
#include "engine/Cordinate.cpp"
#include "engine/DisplayAttack.cpp"
#include <cstring>
#include <thread>
#include <fstream>
#include <zconf.h>

using namespace std;
using namespace state;
using namespace render;
using namespace engine;
using namespace ai;
using namespace client;

/****************************/
/***** GLOBAL VARIABLES *****/
/****************************/
bool iaTurn = false;
int oldMouseEventX = 0;
int oldMouseEventY = 0;
int mouseEventX = 0;
int mouseEventY = 0;

bool endOfGame = false;

void handleInputs(sf::RenderWindow &window, const unique_ptr<Scene>& scene, const shared_ptr<Engine>& e);



int main(int argc,char* argv[])
{

    if (argc == 2 && strcmp(argv[1], "thread") != 0) {

        /****************************/
        /*** Local Data GameState ***/
        /****************************/


        /// 1. Intancier GameState
        GameState gameState{};
        /// 2. Create Window SFML
        sf::RenderWindow window(sf::VideoMode(640, 640), "ZCOM from main.cpp");
        window.setFramerateLimit(60);
        sf::View view(sf::FloatRect(0, 0, 320, 320));
        /// 3. Intanciate Scene
        unique_ptr<Scene> scene(new Scene(window, view));
        /// 4. Register Scene -> GameState
        gameState.registerObserver(scene.get());
        /// 5. Charger la carte World dans GameState
        gameState.setWorld(World{"../res/map2.txt"});
        /// 6. Charger players dans GameState
        gameState.setPlayer1(Player{1, gameState.getWorld().getSpawnUnits1(), gameState.getWorld().getSpawnTowers1(), gameState.getWorld().getSpawnApparitionAreas1()});
        gameState.setPlayer2(Player{2, gameState.getWorld().getSpawnUnits2(), gameState.getWorld().getSpawnTowers2(), gameState.getWorld().getSpawnApparitionAreas2()});
        gameState.setActivePlayer(gameState.getPlayer1());
        /// 7. Create engine
        shared_ptr<Engine> engine = make_shared<Engine>(gameState);


        if (strcmp(argv[1], "hello") == 0) {
            cout << "Bonjour le monde !" << endl;
        }


        /****************************/
        /********** RENDER **********/
        /****************************/

        else if (strcmp(argv[1], "render") == 0) {
            cout << "Bienvenue sur render" << endl;
            while (window.isOpen()) {
                sf::Event event{};
                while (window.pollEvent(event)) {
                    switch (event.type) {
                        case sf::Event::Closed:
                            window.close();
                            break;
                        default:
                            scene.get()->updateAll();
                            break;
                    }
                }
            }
        }


        /****************************/
        /********** ENGINE **********/
        /****************************/

        else if (strcmp(argv[1], "engine") == 0) {

            cout << "Bienvenue sur engine !" << endl << endl;
            cout << "Clic gauche pour selectionner un soldat rouge." << endl;
            cout << "Déplacer la souris pour voir ces déplacements, puis cliquer sur une des cases pour le déplacer."<< endl << endl;
            cout << "Double clic pour passer en mode attaque." << endl;
            cout << "Appuyer sur T pour changer de tour et laisser votre adversaire jouer." << endl << endl;
            cout << "Sinon appuyer sur D pour la demo (mais cela ruinerai votre expérience)." << endl;

            while (window.isOpen()) {
                handleInputs(window, scene, engine);
                engine->runCommands();
            }
        }


        /*****************************/
        /********* RANDOM AI *********/
        /*****************************/

        else if (strcmp(argv[1], "random_ai") == 0) {
            cout << "Bienvenue sur random_ai !" << endl<< "Appuyer sur T pour changer de tour et laisser votre l'IA jouer." << endl;

            unique_ptr<AI> ai;
            ai.reset(new RandomAI);

            while (window.isOpen()) {
                if(!iaTurn){
                    handleInputs(window, scene, engine);
                    engine->runCommands();
                } else {
                    ai->run(*engine);
                    iaTurn = false;
                }
            }
        }


        /******************************/
        /******** HEURISTIC AI ********/
        /******************************/

        else if (strcmp(argv[1], "heuristic_ai") == 0) {
            cout << "Bienvenue sur heuristic_ai !" << endl << "Appuyer sur T pour changer de tour et laisser votre l'IA jouer." << endl;
            cout << "Clic gauche pour selectionner un soldat rouge." << endl;
            cout << "Déplacer la souris pour voir ces déplacements, puis cliquer sur une des cases pour le déplacer."<< endl << endl;
            cout << "Double clic pour passer en mode attaque." << endl;
            cout << "Sinon appuyer sur D pour la demo (mais cela ruinerai votre expérience)." << endl;

            unique_ptr<AI> ai;
            ai.reset(new HeuristicAI);

            while (window.isOpen()) {
                if(!iaTurn){
                    handleInputs(window, scene, engine);
                    engine->runCommands();
                } else {
                    ai->run(*engine);
                    iaTurn = false;
                }
            }
        }


        /******************************/
        /********** ROLLBACK **********/
        /******************************/

        else if (!strcmp(argv[1], "rollback")) {
            cout << "Bienvenue sur rollback !" << endl << "Appuyer sur T pour changer de tour et laisser votre l'IA jouer." << endl;
            cout << "Appuyer sur R pour rollback la dernière commande" << endl;
            bool firstLaunchDemoRollback = false;

            unique_ptr<AI> ai;
            ai.reset(new HeuristicAI);

            while (window.isOpen()) {
                if(!iaTurn){
                    if(!firstLaunchDemoRollback){
                        handleInputs(window, scene, engine);
                        engine->getGameState().unselectedUnit();
                        shared_ptr<Command> newTurnCommand = make_shared<NewTurnCommand>();
                        engine->addCommand(newTurnCommand, 1);
                        iaTurn = true;
                        engine->runCommands();
                        firstLaunchDemoRollback = true;
                    } else {
                        handleInputs(window, scene, engine);
                        engine->runCommands();
                    }
                } else {
                    ai->run(*engine);
                    iaTurn = false;
                    /// UNDO COMMANDS UNTIL IS EMPTY
                    while(!engine->getPreviousCommands().empty()){
                        engine->undoCommands();
                        this_thread::sleep_for(std::chrono::milliseconds(100));
                    }
                }
            }
        }


        /*****************************/
        /********** DEEP AI **********/
        /*****************************/

        else if (!strcmp(argv[1], "deep_ai")) {
            cout << "Bienvenue sur deep_ai !" << endl << "Appuyer sur T pour changer de tour et laisser votre l'IA jouer." << endl;
            cout << "Clic gauche pour selectionner un soldat rouge." << endl;
            cout << "Déplacer la souris pour voir ces déplacements, puis cliquer sur une des cases pour le déplacer."<< endl << endl;
            cout << "Double clic pour passer en mode attaque." << endl;
            cout << "Sinon appuyer sur D pour la demo (mais cela ruinerai votre expérience)." << endl;
            cout << "Appuyer sur R pour rollback la dernière commande" << endl;

            unique_ptr<AI> ai;
            ai.reset(new DeepAI);

            while (window.isOpen()) {
                if(!iaTurn){
                    handleInputs(window, scene, engine);
                    engine->runCommands();
                } else {
                    ai->run(*engine);
                    iaTurn = false;
                }
            }
        }

        /******************************/
        /*********** RECORD ***********/
        /******************************/

        else if (strcmp(argv[1], "record") == 0) {
            cout << "Bienvenue sur record !" << endl << "Appuyer sur T pour changer de tour et laisser votre l'IA jouer." << endl;
            cout << "Clic gauche pour selectionner un soldat rouge." << endl;
            cout << "Déplacer la souris pour voir ces déplacements, puis cliquer sur une des cases pour le déplacer."<< endl << endl;
            cout << "Double clic pour passer en mode attaque." << endl;
            cout << "Sinon appuyer sur D pour la demo (mais cela ruinerai votre expérience)." << endl;

            engine = make_shared<Engine>(gameState, true);

            unique_ptr<AI> ai;
            ai.reset(new HeuristicAI);

            while (window.isOpen()) {
                if(!iaTurn){
                    handleInputs(window, scene, engine);
                    engine->runCommands();
                } else {
                    ai->run(*engine);
                    iaTurn = false;
                }
            }
        }

        /****************************/
        /*********** PLAY ***********/
        /****************************/

        if (!strcmp(argv[1], "play")) {

            thread eng([engine] {
                ifstream ifsMap("replay.txt", ifstream::in);
                Json::Reader reader;
                Json::Value obj;
                if (reader.parse(ifsMap, obj)) {
                    const Json::Value &cmds = obj["commands"];
                    for (unsigned int i = 0; i < cmds.size() - 1; i++) {
                        if (engine->getGameState().getPlayer1().getUnits().size() > 0 &&
                                engine->getGameState().getPlayer2().getUnits().size() > 0) {
                            switch ((CommandTypeId) (cmds[i]["CommandTypeId"].asInt())) {

                                case engine::CommandTypeId::ATTACK_CMD: {
                                    for(const auto& unit : engine->getGameState().getActivePlayer().getUnits()){
                                        if((unit.getX() == cmds[i]["selectedUnitX"].asInt() && unit.getY() == cmds[i]["selectedUnitY"].asInt())){
                                            engine->getGameState().setSelectedUnit(make_shared<Character>(unit));
                                            engine->addCommand(make_shared<AttackCommand>(engine->getGameState().getSelectedUnit(),
                                                    cmds[i]["targetX"].asInt(), cmds[i]["targetY"].asInt()), 0);
                                        }
                                    }
                                    break;
                                }

                                case engine::CommandTypeId::NEW_TURN_CMD: {
                                    engine->addCommand(make_shared<NewTurnCommand>(), 0);
                                    usleep(1000000);
                                    break;
                                }

                                case engine::CommandTypeId::MOVE_CMD: {
                                    for(const auto& unit : engine->getGameState().getActivePlayer().getUnits()){
                                        if((unit.getX() == cmds[i]["selectedUnitX"].asInt() && unit.getY() == cmds[i]["selectedUnitY"].asInt())){
                                            engine->getGameState().setSelectedUnit(make_shared<Character>(unit));
                                            engine->addCommand(make_shared<MoveCommand>(engine->getGameState().getSelectedUnit(),
                                                                                          cmds[i]["destinationX"].asInt(), cmds[i]["destinationY"].asInt()), 0);
                                        }
                                    }
                                    break;
                                }
                                default:
                                    break;
                            }
                            engine->runCommands();
                            engine->getGameState().unselectedUnit();

                        }
                    }

                }
                endOfGame = true;
                ifsMap.close();
                return;
            });

            while (window.isOpen()) {
                sf::Event event{};
                while (window.pollEvent(event)) {
                    switch (event.type) {
                        case sf::Event::Closed:
                            window.close();
                            break;
                        default:
                            break;
                    }
                }
                if (endOfGame) {
                    cout << "the end of the commands recorded" << endl;
                    window.close();
                }

            }
            eng.join();
        }

        /*****************************/
        /********** NETWORK **********/
        /*****************************/

        if (!strcmp(argv[1], "network")) {
            cout << "network" << endl;

            Client client1;
            client1.connectNetwork();
        }

    }

    /****************************/
    /********** THREAD **********/
    /****************************/

    else if (!strcmp(argv[1], "thread")) {
        cout << "thread" << endl;
        Client client1;
        client1.run();
    }
    else {
        cout << "I don't understand" << endl << "you can say hello, render, engine, random_ai, heuristic_ai, rollback, deep_ai..." << endl;
    }
    return 0;
}




/******************************/
/** Mouse and keyboard EVENT **/
/******************************/

void handleInputs(sf::RenderWindow &window, const unique_ptr<Scene>& scene, const shared_ptr<Engine>& e){
    sf::Event event{};
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:

                /*************************************/
                /** Press R -> Rollback one command **/
                /*************************************/
                if (event.key.code == sf::Keyboard::R)
                {
                    e->undoCommands();
                }

                /***************************/
                /** Press U -> Update all **/
                /***************************/
                if (event.key.code == sf::Keyboard::U)
                {
                    scene.get()->updateAll();
                }

                /*****************************/
                /** Press T -> end the turn **/
                /*****************************/
                if (event.key.code == sf::Keyboard::T)
                {
                    e->getGameState().unselectedUnit();
                    shared_ptr<Command> newTurnCommand = make_shared<NewTurnCommand>();
                    e->addCommand(newTurnCommand, 1);
                    iaTurn = true;
                }

                /******************************/
                /** DEMO of different action **/
                /******************************/
                if (event.key.code == sf::Keyboard::D)
                {
                    std::cout << "Lancement du mode démo ne pas bouger votre souris..." << std::endl;
                    for(const auto& unit : e->getGameState().getActivePlayer().getUnits()){
                        if(unit.getX() == 1 && unit.getY() == 4){
                            e->getGameState().setSelectedUnit(make_shared<Character>(unit));
                        }
                    }
                    if(e->getGameState().getSelectedUnit() != nullptr){
                        Node depart = {.x =  e->getGameState().getSelectedUnit().get()->getX(), .y = e->getGameState().getSelectedUnit().get()->getY()};
                        Node destination = {.x = 3, .y = 6};
                        scene->updateTrajectory(Cordinate::aStar(depart, destination, e->getGameState().getWorld(), e->getGameState().getGameObjects(), e->getGameState().getSelectedUnit().get()->getPm()));
                        shared_ptr<Command> move = make_shared<MoveCommand>(e->getGameState().getSelectedUnit(), 3, 6);
                        e->addCommand(move, 1);
                        e->runCommands();
                    } else {
                        cout << "L'unité démo ne peut pas être sélectionée." << endl << "Relancer le jeu s'il vous plaît" << endl;
                    }
                    for(const auto& unit : e->getGameState().getActivePlayer().getUnits()){
                        if(unit.getX() == 3 && unit.getY() == 6){
                            e->getGameState().setSelectedUnit(make_shared<Character>(unit));
                        }
                    }
                    if(e->getGameState().getSelectedUnit() != nullptr){
                        for(int i = 0; i < 4; i++){
                            this_thread::sleep_for(std::chrono::milliseconds(400));
                            e->getGameState().setAttackMode(true);
                            vector<int> attackField = DisplayAttack::createField(e->getGameState().getSelectedUnit().get(),
                                    e->getGameState().getWorld(), e->getGameState().getGameObjects());
                            if(attackField[9+ 5 * e->getGameState().getWorld().getYMax()] == 1){
                                scene->updateTrajectory(DisplayAttack::createDamageArea(9, 5,
                                        e->getGameState().getSelectedUnit().get(), e->getGameState().getWorld()));
                            } else{
                                scene->updateTrajectory(vector<Node>{});
                            }
                            this_thread::sleep_for(std::chrono::milliseconds(600));
                            shared_ptr<Command> attack = make_shared<AttackCommand>(e->getGameState().getSelectedUnit(), 9, 5);
                            e->addCommand(attack, 1);
                            e->runCommands();
                        }
                    } else {
                        cout << "L'unité démo ne peut pas être sélectionée." << endl << "Relancer le jeu s'il vous plaît" << endl;
                    }
                    for(int i = 0; i < 4; i++){
                        this_thread::sleep_for(std::chrono::milliseconds(600));
                        e->getGameState().unselectedUnit();
                        shared_ptr<Command> newTurnCommand = make_shared<NewTurnCommand>();
                        e->addCommand(newTurnCommand, 1);
                        e->runCommands();
                    }

                }
                break;
            case sf::Event::KeyReleased:
                break;
            case sf::Event::MouseButtonPressed:
                // TODO: Directly show attack if pm = 0

                /******************************/
                /**** LEFT CLICK HANDLING *****/
                /******************************/
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    int x = (event.mouseButton.x - (((1 - window.getView().getViewport().width)*window.getSize().x)/2))/(window.getView().getViewport().width*window.getSize().x)*20; // NOLINT(bugprone-narrowing-conversions)
                    int y = (event.mouseButton.y - (((1 - window.getView().getViewport().height)*window.getSize().y)/2))/(window.getView().getViewport().height*window.getSize().y)*20;
                    bool foundNewUnit = false;
                    if(e->getGameState().getSelectedUnit() != nullptr){
                        /// UNSELECT UNIT AND SELECT NEW ONE
                        for(const auto& unit : e->getGameState().getActivePlayer().getUnits()){
                            if((unit.getX() == x && unit.getY() == y)){
                                if(x != e->getGameState().getSelectedUnit().get()->getX() || y != e->getGameState().getSelectedUnit().get()->getY()){
                                    foundNewUnit = true;
                                    e->getGameState().setSelectedUnit(make_shared<Character>(unit));
                                    if(e->getGameState().getSelectedUnit()->getPm() == 0){
                                        if(!e->getGameState().getSelectedUnit()->getHasAttacked()){
                                            e->getGameState().setAttackMode(true);
                                        }
                                    }
                                }
                            }
                        }
                        if(!foundNewUnit){
                            if(e->getGameState().getAttackMode()){
                                /// ATTACK WITH SELECTED UNIT
                                if(x != e->getGameState().getSelectedUnit().get()->getX() || y != e->getGameState().getSelectedUnit().get()->getY()){
                                    shared_ptr<Command> attack = make_shared<AttackCommand>(e->getGameState().getSelectedUnit(), x, y);
                                    e->addCommand(attack, 1);
                                    e->getGameState().unselectedUnit();
                                } else {
                                    e->getGameState().unselectedUnit();
                                }
                            } else{
                                if(e->getGameState().getSelectedUnit().get()->getX() == x && e->getGameState().getSelectedUnit().get()->getY() == y){
                                    if(e->getGameState().getSelectedUnit()->getHasAttacked()){
                                        e->getGameState().unselectedUnit();
                                    } else {
                                        /// MAKE UNIT IN ATTACK MODE
                                        e->getGameState().setAttackMode(true);
                                    }
                                } else {
                                    /// MOVE SELECTED UNIT
                                    shared_ptr<Command> move = make_shared<MoveCommand>(e->getGameState().getSelectedUnit(), x, y);
                                    e->addCommand(move, 1);
                                    e->getGameState().unselectedUnit();
                                }
                            }
                        }
                    } else {
                        /// SELECT AN UNIT
                        for(const auto& unit : e->getGameState().getActivePlayer().getUnits()){
                            if(unit.getX() == x && unit.getY() == y){
                                e->getGameState().setSelectedUnit(make_shared<Character>(unit));
                                if(e->getGameState().getSelectedUnit()->getPm() == 0){
                                    if(!e->getGameState().getSelectedUnit()->getHasAttacked()){
                                        e->getGameState().setAttackMode(true);
                                    }
                                }
                            }
                        }
                    }
                }
                /// UNSELECT UNIT
                if (event.mouseButton.button == sf::Mouse::Right) {
                    e->getGameState().unselectedUnit();
                }
                break;
            case sf::Event::MouseButtonReleased:
                break;
            case sf::Event::MouseMoved:

                /******************************/
                /**** MOUSE MOVE HANDLING *****/
                /******************************/
                if(e->getGameState().getSelectedUnit() != nullptr){
                    mouseEventX = (event.mouseMove.x - (((1 - window.getView().getViewport().width)*window.getSize().x)/2))/(window.getView().getViewport().width*window.getSize().x)*20;
                    mouseEventY = (event.mouseMove.y - (((1 - window.getView().getViewport().height)*window.getSize().y)/2))/(window.getView().getViewport().height*window.getSize().y)*20;
                    if(oldMouseEventX != mouseEventX || oldMouseEventY != mouseEventY){
                        if(!e->getGameState().getAttackMode()){
                            /// DRAW PATTERN OF POSSIBLE MOVEMENT
                            Node depart = {.x =  e->getGameState().getSelectedUnit().get()->getX(), .y = e->getGameState().getSelectedUnit().get()->getY()};
                            Node destination = {.x = mouseEventX, .y = mouseEventY};
                            scene->updateTrajectory(Cordinate::aStar(depart, destination, e->getGameState().getWorld(), e->getGameState().getGameObjects(), e->getGameState().getSelectedUnit().get()->getPm()));
                        } else if(!e->getGameState().getSelectedUnit().get()->getHasAttacked()) {
                            /// DRAW PATTERN OF POSSIBLE TARGETS
                            vector<int> attackField = DisplayAttack::createField(e->getGameState().getSelectedUnit().get(),
                                    e->getGameState().getWorld(), e->getGameState().getGameObjects());
                            if(attackField[mouseEventX+ mouseEventY * e->getGameState().getWorld().getYMax()] == 1){
                                scene->updateTrajectory(DisplayAttack::createDamageArea(mouseEventX, mouseEventY, e->getGameState().getSelectedUnit().get(), e->getGameState().getWorld()));
                            } else{
                                scene->updateTrajectory(vector<Node>{});
                            }
                        }
                        oldMouseEventX = mouseEventX;
                        oldMouseEventY = mouseEventY;
                    }

                }
                break;
            case sf::Event::MouseEntered:
                break;
            case sf::Event::MouseLeft:
                if(e->getGameState().getSelectedUnit() == nullptr){
                    scene->updateTrajectory(vector<Node>{});
                } else {
                    if(!e->getGameState().getAttackMode()){
                        scene->updateTrajectory(vector<Node>{Node{.x = e->getGameState().getSelectedUnit().get()->getX(), .y = e->getGameState().getSelectedUnit().get()->getY()}});
                    } else {
                        scene->updateTrajectory(vector<Node>{});
                    }
                }
                break;
            default:
                scene.get()->updateAll();
                break;
        }
    }
  }
