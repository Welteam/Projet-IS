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


bool clientAITurn = false;
bool forceTo = false;


Client::Client(){

}

void Client::runNetwork(sf::RenderWindow& window, sf::View& view){

    /// 1. Intancier GameState
    GameState gameState{};
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
    int enemyId = 0;
    if(clientId == 1){
        enemyId = 2;
    } else {
        enemyId = 1;
    }

    while (window.isOpen()){
        if(engine->getGameState().getActivePlayer().getId() == clientId || !forceTo){
            handleInputs(window, scene, engine);
            engine->runCommands();
        } else {
            sf::Event event{};
            forceTo = false;
            this->getServerCommands(enemyId);
            cout << "about to sleep" << endl;
            usleep(1000000);
            cout << "end reading commands" << endl;
            while (window.pollEvent(event)) {
                switch (event.type) {
                    case sf::Event::Closed:
                        window.close();
                        break;default:break;
                }
            }
        }
    }
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
    shared_ptr<Engine> enginePtr = engine;
    shared_ptr<AI> aiPtr = ai;
    mutex input_lock{};

    thread eng([this, &input_lock] {
        clientAITurn = true;
        while(1){
            if(!clientAITurn)
                return 0;
            input_lock.lock();
            ai->run_thread(*engine, &input_lock);
            input_lock.unlock();

            cout << "about to sleep" << endl;
            usleep(1000000);
            cout << "end commands" << endl;
        }
    });

    while (window.isOpen()){
        input_lock.lock();
        handleInputs(window, scene, engine);
        input_lock.unlock();
    }
    eng.join();
}

int Client::connectNetwork(){
    int id = 1;
    int lock = 1;
    while(lock == 1){
        lock = this->getPlayer(id);
        id+=1;
    }
    this->clientId =this->addPlayer();
    cout << "Your player ID is " << this->clientId << endl;
    return this->clientId;
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

    if (response.getStatus() == sf::Http::Response::Created) {
        //Get This player Id
        Json::Value root;
        Json::Value players;
        Json::Reader reader;
        if (!reader.parse(response.getBody(), root, false)){
            cout << reader.getFormattedErrorMessages() << endl;
        }
        int id = root["id"].asInt();
        if (id == -1){
            cout << "No more connections possible" << endl;
            return -1;
        } else {
            return id;
        }
    } else {
        cout << "Put Request Failed: " << response.getStatus() << endl;
        return -1;
    }
}

int Client::getPlayer(int id){
    sf::Http http("http://localhost", 8080);
    sf::Http::Response response;
    sf::Http::Request req("/user/"+to_string(id), sf::Http::Request::Get);
    response = http.sendRequest(req);

    if (response.getStatus() == sf::Http::Response::Ok){
        cout << response.getBody() << endl;
        return 1;
    } else {
        return 0;
    }
}

bool Client::getServerCommands(int enemyId){

    sf::Http http("http://localhost", 8080);
    sf::Http::Response response;

    sf::Http::Request req("/command/"+to_string(enemyId), sf::Http::Request::Get);
    response = http.sendRequest(req);
    Json::Reader jsonReader;
    Json::Value cmds;
    if (response.getStatus() == sf::Http::Response::Ok){
        cout << "Commands recu avec succés" << endl;
        cout << response.getBody() << endl;
        Json::Value obj;
        jsonReader.parse(response.getBody(),obj);
        const Json::Value &cmds = obj["commands"];

        for (unsigned int i = 0; i < cmds.size(); i++) {
            if (engine->getGameState().getPlayer1().getUnits().size() > 0 &&
                engine->getGameState().getPlayer2().getUnits().size() > 0) {
                switch ((CommandTypeId) (cmds[i]["CommandTypeId"].asInt())) {
                    case engine::CommandTypeId::ATTACK_CMD: {
                        cout << "received attack cmd" << endl;
                        for (const auto &unit : engine->getGameState().getActivePlayer().getUnits()) {
                            if ((unit.getX() == cmds[i]["selectedUnitX"].asInt() &&
                                 unit.getY() == cmds[i]["selectedUnitY"].asInt())) {
                                engine->getGameState().setSelectedUnit(make_shared<Character>(unit));
                                engine->addCommand(make_shared<AttackCommand>(engine->getGameState().getSelectedUnit(),
                                                                              cmds[i]["targetX"].asInt(),
                                                                              cmds[i]["targetY"].asInt()), 0);
                                engine->runCommands();
                                engine->getGameState().unselectedUnit();
                            }
                        }
                        break;
                    }
                    case engine::CommandTypeId::NEW_TURN_CMD: {
                        cout << "received new turn cmd" << endl;
                        engine->addCommand(make_shared<NewTurnCommand>(), 0);
                        engine->runCommands();
                        engine->getGameState().unselectedUnit();
                        break;
                    }
                    case engine::CommandTypeId::MOVE_CMD: {
                        cout << "received move cmd" << endl;
                        for (const auto &unit : engine->getGameState().getActivePlayer().getUnits()) {
                            if ((unit.getX() == cmds[i]["selectedUnitX"].asInt() &&
                                 unit.getY() == cmds[i]["selectedUnitY"].asInt())) {
                                engine->getGameState().setSelectedUnit(make_shared<Character>(unit));
                                engine->addCommand(make_shared<MoveCommand>(engine->getGameState().getSelectedUnit(),
                                                                            cmds[i]["destinationX"].asInt(),
                                                                            cmds[i]["destinationY"].asInt()), 0);
                                engine->runCommands();
                                engine->getGameState().unselectedUnit();
                            }
                        }
                        break;
                    }
                    default:
                        cout << "La commande " << i << " est inconnue" << endl;
                        break;
                }
            }
        }
        cout << "finish read commands" << endl;
        return 1;
    } else {
        cout << "cannot read commands" << endl;
        return 0;
    }
}

void Client::putServerCommand (shared_ptr<Command> command){
    sf::Http http("http://localhost", 8080);
    sf::Http::Response response;
    sf::Http::Request request1;
    request1.setUri("/command");
    request1.setMethod(sf::Http::Request::Put);
    Json::Value root;
    request1.setBody(command->serialize(root).toStyledString());
    request1.setField("Content-Type", "application/json");
    request1.setHttpVersion(1, 1);
    response = http.sendRequest(request1);
    if (response.getStatus() == sf::Http::Response::Created) {
        cout << "Put Request Succed: " << response.getStatus() << endl;
    } else {
        cout << "Put Request Failed: " << response.getStatus() << endl;
    }
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
                clientAITurn = false;
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
                    putServerCommand(newTurnCommand);
                    forceTo = true;
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
                                    putServerCommand(attack);
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
                                    putServerCommand(move);
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


