#include "Client.h"
#include "../render.h"
#include "state.h"
#include "ai.h"
#include "engine.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <zconf.h>
#include <SFML/Network.hpp>
#include <SFML/Window/Event.hpp>
#include "engine/Cordinate.cpp"
#include "engine/DisplayAttack.cpp"

using namespace client;
using namespace render;
using namespace engine;
using namespace std;
using namespace state;
using namespace ai;

<<<<<<< Updated upstream
bool clientAITurn = false;

=======
>>>>>>> Stashed changes
Client::Client(){

}

void Client::run(){

    /// 1. Intancier GameState
    GameState gameState{};
    /// 2. Create Window SFML
    sf::RenderWindow window(sf::VideoMode(640, 640), "ZCOM from Client.cpp");
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
    this->engine = make_shared<Engine>(gameState);



    // Call our AI computer
    ai->restrictArea = false;
    cerr << "render running" << endl;
    shared_ptr<Engine> enginePtr = engine;
    shared_ptr<AI> aiPtr = ai;
    mutex *input_lock = new std::mutex;

    thread eng([this, &input_lock] {
        while(1){
            clientAITurn = true;
            input_lock->lock();
            ai->run_thread(*engine, input_lock);
            input_lock->unlock();
            cout << "about to sleep" << endl;
            usleep(1000000);
            cout << "end commands" << endl;

            // TODO n°2 find a variable to exit the while(1). If you put the variable
            //  to false, the AI will play versus herself infinitely.
        }
    });

    while (window.isOpen()){
        // TODO n°1 : try to handleInputs (if you uncomment the two lines, any event
        //  from mouse or keyboard will create a signal 6: SIGABRT). We Should try to
        //  block AI when new events are coming on TO DO n°2
<<<<<<< Updated upstream
        handleInputs(window, scene, engine);
=======
        input_lock->lock();
        handleInputs(window, scene, engine);
        input_lock->unlock();
>>>>>>> Stashed changes
        //engine->runCommands();
    }
    eng.join();
}

void Client::connectNetwork(){
    this->addPlayer();
    int id = 1;
    int lock = 1;
    while(lock == 1){
        lock = this->getPlayer(id);
        id+=1;
    }
    this->addPlayer();
}

void Client::deletePlayer(int id){
    sf::Http http("http://localhost", 8080);
    sf::Http::Response response;
    sf::Http::Request req3;
    req3.setMethod(sf::Http::Request::Delete);
    req3.setHttpVersion(1, 1);
    req3.setUri("/user/" + std::to_string(id));
    req3.setField("Content-Type", "application/json");
    req3.setBody("");
    response = http.sendRequest(req3);
}

int Client::addPlayer(){
    sf::Http http("http://localhost", 8080);
    sf::Http::Response response;
    sf::Http::Request req3;
    Json::Value User;
    User["orientation"] = 4;
    User["currentLife"] = 3;
    User["x"] = 1;
    User["y"] = 12;
    req3.setUri("/user");
    req3.setMethod(sf::Http::Request::Put);
    req3.setBody(User.toStyledString());
    req3.setField("Content-Type", "application/json");
    req3.setHttpVersion(1, 1);
    response = http.sendRequest(req3);
    if (response.getStatus() == sf::Http::Response::Created)
    {
        //cout<< response.getBody()<<endl;
        //Get This player Id
        Json::Value root;
        Json::Value players;
        Json::Reader reader;
        if (!reader.parse(response.getBody(), root, false))
        {
            cout << reader.getFormattedErrorMessages() << endl;
        }
        int id = root["id"].asInt();
        if (id == -1)
        {
            cout << "No more connections possible" << endl;
            return -1;
        }
        else
        {
            return id;
        }
    }
    else
    {
        cout << "Put Request Failed: " << response.getStatus() << endl;
        return -1;
    }
}

int Client::getPlayer(int id){
    sf::Http http("http://localhost", 8080);
    sf::Http::Response response;
    sf::Http::Request req("/user/"+to_string(id), sf::Http::Request::Get);
    response = http.sendRequest(req);

    if (response.getStatus() == sf::Http::Response::Ok)
    {
        cout << response.getBody() << endl;
        return 1;
    }
    else
    {
        return 0;
    }
}

unsigned int Client::findEnemy(std::map<unsigned int, std::shared_ptr<state::Player>> &MPlayers, unsigned int &player){
    for (auto p : MPlayers)
    {
        if (p.first != player)
        {
            return p.first;
        }
    }
    new runtime_error("no enemy found");
    return -1;
}


/******************************/
/** Mouse and keyboard EVENT **/
/******************************/

void Client::handleInputs(sf::RenderWindow& window, unique_ptr<Scene>& scene, shared_ptr<Engine> e){

    /****************************/
    /***** GLOBAL VARIABLES *****/
    /****************************/



    int oldMouseEventX = 0;
    int oldMouseEventY = 0;
    int mouseEventX = 0;
    int mouseEventY = 0;
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
                    // iaTurn = true;
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


