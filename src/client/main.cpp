#include <iostream>
#include <SFML/Graphics.hpp>
#include <state.h>
#include "render.h"
#include "engine.h"
#include "engine/Cordinate.cpp"
#include "engine/DisplayAttack.cpp"
#include <cstring>

using namespace std;
using namespace state;
using namespace render;
using namespace engine;

void handleInputs(sf::RenderWindow &window, shared_ptr<Scene> scene, shared_ptr<Engine> engine);

int main(int argc,char* argv[])
{
    if (argc == 2) {
        if (strcmp(argv[1], "hello") == 0) {
            cout << "Bonjour le monde !" << endl;
        }
        else if (strcmp(argv[1], "render") == 0) {
            cout << "Bienvenue sur render" << endl;
            // 1. Intancier GameState
            GameState gameState{};

            // 2. Create Window SFML
            sf::RenderWindow window(sf::VideoMode(640, 640), "ZCOM");
            sf::View view(sf::FloatRect(0, 0, 320, 320));

            // 3. Intanciate Scene
            shared_ptr<Scene> scene = make_shared<Scene>(window, view);

            // 4. Register Scene -> GameState
            gameState.registerObserver(scene.get());

            // 5. Charger la carte World et Players dans GameState
            gameState.setWorld(World{"../res/map.txt"});
            gameState.setPlayer1(Player{1, gameState.getWorld().getSpawnUnits1(), gameState.getWorld().getSpawnTowers1(), gameState.getWorld().getSpawnApparitionAreas1()});
            gameState.setPlayer2(Player{2, gameState.getWorld().getSpawnUnits2(), gameState.getWorld().getSpawnTowers2(), gameState.getWorld().getSpawnApparitionAreas2()});

            while (window.isOpen()) {
                sf::Event event;
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
        else if (strcmp(argv[1], "engine") == 0) {
            cout << "Bienvenue sur engine" << endl;
            // 1. Intancier GameState
            GameState gameState{};

            // 2. Create Window SFML
            sf::RenderWindow window(sf::VideoMode(640, 640), "ZCOM");
            // on crée une vue
            //sf::View view(window.getView().getCenter(), sf::Vector2f((float)320, (float)320));
            sf::View view(sf::FloatRect(0, 0, 320, 320));

            // 3. Intanciate Scene
            shared_ptr<Scene> scene = make_shared<Scene>(Scene(window, view));

            // 4. Register Scene -> GameState
            gameState.registerObserver(scene.get());

            // 5. Charger la carte World dans GameState
            gameState.setWorld(World{"../res/map.txt"});

            // 6. Charger players dans GameState
            gameState.setPlayer1(Player{1, gameState.getWorld().getSpawnUnits1(), gameState.getWorld().getSpawnTowers1(), gameState.getWorld().getSpawnApparitionAreas1()});
            gameState.setPlayer2(Player{2, gameState.getWorld().getSpawnUnits2(), gameState.getWorld().getSpawnTowers2(), gameState.getWorld().getSpawnApparitionAreas2()});
            gameState.setActivePlayer(gameState.getPlayer2());

            // Create our engine
            shared_ptr<Engine> engine = make_shared<Engine>(gameState);

            while (window.isOpen()) {
                handleInputs(window, scene, engine);
            }
        }
    } else {
        cout << "I don't understand" << endl;
        cout << "you can say hello, render, engine..." << endl;
    }
    return 0;
}



void handleInputs(sf::RenderWindow &window, shared_ptr<Scene> scene, shared_ptr<Engine> engine){
    sf::Event event;
    int oldMouseEventX;
    int oldMouseEventY;
    int mouseEventX;
    int mouseEventY;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::LostFocus:
                break;
            case sf::Event::GainedFocus:
                break;
            case sf::Event::TextEntered:
                break;
            case sf::Event::KeyPressed:
                break;
            case sf::Event::KeyReleased:
                break;
            case sf::Event::MouseWheelMoved:
                break;
            case sf::Event::MouseWheelScrolled:
                break;
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    int x = (event.mouseButton.x - (((1 - window.getView().getViewport().width)*window.getSize().x)/2))/(window.getView().getViewport().width*window.getSize().x)*20;
                    int y = (event.mouseButton.y - (((1 - window.getView().getViewport().height)*window.getSize().y)/2))/(window.getView().getViewport().height*window.getSize().y)*20;
                    bool foundUnit = false;
                    if(engine->getSelectedUnit() != nullptr){
                        cout << "unité déjà sélectionée" << endl;
                        for(auto unit : engine->getGameState().getActivePlayer().getUnits()){
                            if((unit.getX() == x && unit.getY() == y)){
                                cout << "Même unité séléctionnée" << endl;
                                if(x != engine->getSelectedUnit().get()->getX() || y != engine->getSelectedUnit().get()->getY()){
                                    cout << "Nouvelle unité séléctionnée" << endl;
                                    foundUnit = true;
                                    engine->setSelectedUnit(make_shared<Character>(unit));
                                    engine::Node posUnit;
                                    posUnit.x = unit.getX();
                                    posUnit.y = unit.getY();
                                    vector<Node> nodes;
                                    nodes.push_back(posUnit);
                                    scene->updateTrajectory(nodes);
                                    scene->updateAttackField(vector<int>(400,0));
                                }
                            }
                        }
                        if(!foundUnit){
                            if(engine->getAttackMode()){
                                cout << "unité déjà en mode attaque : attaque !!!!" << endl;
                                vector<int> attackField = DisplayAttack::createField(engine->getSelectedUnit().get(), engine->getGameState().getWorld());
                                if(attackField[x+y*engine->getGameState().getWorld().getYMax()] == 1){
                                    if(engine->getGameState().getActivePlayer().getId() == 2){
                                        Player player = engine->getGameState().getPlayer1();
                                        vector<Character> newUnits;
                                        for(auto unit : engine->getGameState().getPlayer1().getUnits()){
                                            if((unit.getX() == x && unit.getY() == y)){
                                                unit.setHp(unit.getHp()- engine->getSelectedUnit().get()->getWeapon().getDamage());
                                            }
                                            if(unit.getHp() > 0){
                                                newUnits.push_back(unit);
                                            } else {
                                                Player playerAttacking = engine->getGameState().getPlayer2();
                                                shared_ptr<Character> unitEvolve = engine->getSelectedUnit();
                                                vector<Character> newAttackingUnits;
                                                for(auto unitAttacking : engine->getGameState().getPlayer2().getUnits()) {
                                                    if ((unitAttacking.getX() == unitEvolve.get()->getX() && unitAttacking.getY() == unitEvolve.get()->getY())) {
                                                        unitAttacking.levelUp();
                                                        if(unit.getX() < unitAttacking.getX()){
                                                            unitAttacking.setOrientation(Orientation::WEST);
                                                        } else {
                                                            unitAttacking.setOrientation(Orientation::EST);
                                                        }
                                                    }
                                                    newAttackingUnits.push_back(unitAttacking);
                                                }
                                                playerAttacking.setUnits(newAttackingUnits);
                                                engine->getGameState().setPlayer2(playerAttacking);
                                                engine->getGameState().setActivePlayer(engine->getGameState().getPlayer2());
                                            }
                                        player.setUnits(newUnits);
                                        engine->getGameState().setPlayer1(player);
                                        }
                                    } else {
                                        Player player = engine->getGameState().getPlayer2();
                                        Player playerAttacking = engine->getGameState().getPlayer1();
                                        vector<Character> newUnits;
                                        for(auto unit : engine->getGameState().getPlayer2().getUnits()){
                                            if((unit.getX() == x && unit.getY() == y)){
                                                unit.setHp(unit.getHp()- engine->getSelectedUnit().get()->getWeapon().getDamage());
                                            }
                                            if(unit.getHp() > 0){
                                                newUnits.push_back(unit);
                                            } else {
                                                Player playerAttacking = engine->getGameState().getPlayer1();
                                                shared_ptr<Character> unitEvolve = engine->getSelectedUnit();
                                                vector<Character> newAttackingUnits;
                                                for(auto unitAttacking : engine->getGameState().getPlayer1().getUnits()) {
                                                    if ((unitAttacking.getX() == unitEvolve.get()->getX() && unitAttacking.getY() == unitEvolve.get()->getY())) {
                                                        unitAttacking.levelUp();
                                                        if(unitAttacking.getX() < unitAttacking.getX()){
                                                            unitAttacking.setOrientation(Orientation::WEST);
                                                        } else {
                                                            unitAttacking.setOrientation(Orientation::EST);
                                                        }
                                                    }
                                                    newAttackingUnits.push_back(unitAttacking);
                                                }
                                                playerAttacking.setUnits(newAttackingUnits);
                                                engine->getGameState().setPlayer1(playerAttacking);
                                                engine->getGameState().setActivePlayer(engine->getGameState().getPlayer1());
                                            }
                                        }
                                        player.setUnits(newUnits);
                                        engine->getGameState().setPlayer2(player);
                                    }
                                }
                                engine->unselectedUnit();
                                scene->updateTrajectory(vector<Node>{});
                                scene->updateAttackField(vector<int>(400,0));
                            } else{
                                cout << "unité déjà en mode déplacement" << endl;
                                if(engine->getSelectedUnit().get()->getX() == x && engine->getSelectedUnit().get()->getY() == y){
                                    engine->setAttackMode(true);
                                    scene->updateAttackField(DisplayAttack::createField(engine->getSelectedUnit().get(), engine->getGameState().getWorld()));
                                    scene->updateTrajectory(vector<Node>{});
                                    cout << "unité mis en mode attaque" << endl;
                                } else {
                                    cout << "unité se déplace" << endl;
                                    engine::Node depart, destination;
                                    depart.x = engine->getSelectedUnit().get()->getX();
                                    depart.y = engine->getSelectedUnit().get()->getY();
                                    destination.x = x;
                                    destination.y = y;
                                    vector<GameObject> gameObjects;
                                    for(auto gameObject : engine->getGameState().getPlayer1().getUnits())
                                        gameObjects.push_back(gameObject);
                                    for(auto gameObject : engine->getGameState().getPlayer1().getTowers())
                                        gameObjects.push_back(gameObject);
                                    for(auto gameObject : engine->getGameState().getPlayer1().getApparitionAreas())
                                        gameObjects.push_back(gameObject);
                                    for(auto gameObject : engine->getGameState().getPlayer2().getUnits())
                                        gameObjects.push_back(gameObject);
                                    for(auto gameObject : engine->getGameState().getPlayer2().getTowers())
                                        gameObjects.push_back(gameObject);
                                    for(auto gameObject : engine->getGameState().getPlayer2().getApparitionAreas())
                                        gameObjects.push_back(gameObject);
                                    vector<Node> nodes = Cordinate::aStar(depart, destination, engine->getGameState().getWorld(), gameObjects, engine->getSelectedUnit()->getWeapon().getPm());
                                    if(nodes.at(nodes.size()-1).x == x && nodes.at(nodes.size()-1).y == y){
                                        shared_ptr<Character> unitSelected = engine->getSelectedUnit();
                                        Player player = engine->getGameState().getActivePlayer();
                                        vector<Character> newUnits;
                                        for(auto unit : player.getUnits()){
                                            if(unit.getX() == unitSelected->getX() && unit.getY() == unitSelected->getY()){
                                                unit.setX(nodes.at(nodes.size()-1).x);
                                                unit.setY(nodes.at(nodes.size()-1).y);
                                            }
                                            newUnits.push_back(unit);
                                        }
                                        player.setUnits(newUnits);
                                        if(player.getId() == 1){
                                            engine->getGameState().setPlayer1(player);
                                            engine->getGameState().setActivePlayer(engine->getGameState().getPlayer1());
                                        } else {
                                            engine->getGameState().setPlayer2(player);
                                            engine->getGameState().setActivePlayer(engine->getGameState().getPlayer2());
                                        }
                                    }
                                    engine->unselectedUnit();
                                    scene->updateTrajectory(vector<Node>{});
                                }
                            }
                        }
                    } else {
                        cout << "Aucune unité sélectionée" << endl;
                        for(auto unit : engine->getGameState().getActivePlayer().getUnits()){
                            if(unit.getX() == x && unit.getY() == y){
                                engine->setSelectedUnit(make_shared<Character>(unit));
                                engine::Node posUnit;
                                posUnit.x = unit.getX();
                                posUnit.y = unit.getY();
                                vector<Node> nodes;
                                nodes.push_back(posUnit);
                                scene->updateTrajectory(nodes);
                                scene->updateAttackField(vector<int>(400,0));
                            }
                        }
                    }
                }
                if (event.mouseButton.button == sf::Mouse::Right) {
                    engine->unselectedUnit();
                    scene->updateAttackField(vector<int>(400,0));
                    scene->updateTrajectory(vector<Node>{});
                }
                break;
            case sf::Event::MouseButtonReleased:
                break;
            case sf::Event::MouseMoved:
                if(engine->getSelectedUnit() != nullptr){
                    // Récupère la position de la souris en pixel et la convertie en "case"
                    mouseEventX = (event.mouseMove.x - (((1 - window.getView().getViewport().width)*window.getSize().x)/2))/(window.getView().getViewport().width*window.getSize().x)*20;
                    mouseEventY = (event.mouseMove.y - (((1 - window.getView().getViewport().height)*window.getSize().y)/2))/(window.getView().getViewport().height*window.getSize().y)*20;
                    // Calcul un chemin uniquement s'il s'agit d'une nouvelle case
                    if(oldMouseEventX != mouseEventX || oldMouseEventY != mouseEventY){
                        if(!engine->getAttackMode()){
                            engine::Node depart, destination;
                            depart.x = engine->getSelectedUnit().get()->getX();
                            depart.y = engine->getSelectedUnit().get()->getY();
                            destination.x = mouseEventX;
                            destination.y = mouseEventY;
                            vector<GameObject> gameObjects;
                            for(auto gameObject : engine->getGameState().getPlayer1().getUnits())
                                gameObjects.push_back(gameObject);
                            for(auto gameObject : engine->getGameState().getPlayer1().getTowers())
                                gameObjects.push_back(gameObject);
                            for(auto gameObject : engine->getGameState().getPlayer1().getApparitionAreas())
                                gameObjects.push_back(gameObject);
                            for(auto gameObject : engine->getGameState().getPlayer2().getUnits())
                                gameObjects.push_back(gameObject);
                            for(auto gameObject : engine->getGameState().getPlayer2().getTowers())
                                gameObjects.push_back(gameObject);
                            for(auto gameObject : engine->getGameState().getPlayer2().getApparitionAreas())
                                gameObjects.push_back(gameObject);
                            scene->updateTrajectory(Cordinate::aStar(depart, destination, engine->getGameState().getWorld(), gameObjects, engine->getSelectedUnit()->getWeapon().getPm()));
                        } else {
                            vector<int> attackField = DisplayAttack::createField(engine->getSelectedUnit().get(), engine->getGameState().getWorld());
                            if(attackField[mouseEventX+mouseEventY*engine->getGameState().getWorld().getYMax()] == 1){
                                engine::Node posUnit;
                                posUnit.x = mouseEventX;
                                posUnit.y = mouseEventY;
                                vector<Node> nodes;
                                nodes.push_back(posUnit);
                                scene->updateTrajectory(nodes);
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
                if(engine->getSelectedUnit() == nullptr){
                    scene->updateTrajectory(vector<Node>{});
                } else {
                    if(!engine->getAttackMode()){
                        engine::Node posUnit;
                        posUnit.x = engine->getSelectedUnit().get()->getX();
                        posUnit.y = engine->getSelectedUnit().get()->getY();
                        vector<Node> nodes;
                        nodes.push_back(posUnit);
                        scene->updateTrajectory(nodes);
                    } else {
                        scene->updateTrajectory(vector<Node>{});
                    }
                }
                break;
            case sf::Event::SensorChanged:
                break;
            case sf::Event::Count:
                break;
            default:
                scene.get()->updateAll();
                break;
        }
    }

  }


