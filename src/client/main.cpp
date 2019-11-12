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
    gameState.setActivePlayer(gameState.getPlayer1());

    if (argc == 2) {
        if (strcmp(argv[1], "hello") == 0) {
            cout << "Bonjour le monde !" << endl;
        }
        else if (strcmp(argv[1], "render") == 0) {
            cout << "Bienvenue sur render" << endl;

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
                if (event.key.code == sf::Keyboard::T)
                {
                    std::cout << "the T key was pressed, new turn" << std::endl;
                    engine->unselectedUnit();
                    scene->updateTrajectory(vector<Node>{});
                    scene->updateAttackField(vector<int>(400,0));
                    shared_ptr<Command> newTurnCommand = make_shared<NewTurnCommand>();
                    engine->addCommand(newTurnCommand, 1);
                    engine->runCommands(true);

                }
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
                    bool foundNewUnit = false;
                    if(engine->getSelectedUnit() != nullptr){
                        for(auto unit : engine->getGameState().getActivePlayer().getUnits()){
                            if((unit.getX() == x && unit.getY() == y)){
                                if(x != engine->getSelectedUnit().get()->getX() || y != engine->getSelectedUnit().get()->getY()){
                                    foundNewUnit = true;
                                    engine->setSelectedUnit(make_shared<Character>(unit));
                                    scene->updateTrajectory(vector<Node>{Node{.x = unit.getX(), .y = unit.getY()}});
                                    scene->updateAttackField(vector<int>(400,0));
                                }
                            }
                        }
                        if(!foundNewUnit){
                            if(engine->getAttackMode()){
                                shared_ptr<Command> attack = make_shared<AttackCommand>(engine->getSelectedUnit(), x, y);
                                engine->addCommand(attack, 1);
                                engine->runCommands(true);
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
                                    shared_ptr<Command> move = make_shared<MoveCommand>(engine->getSelectedUnit(), x, y);
                                    engine->addCommand(move, 1);
                                    engine->runCommands(true);
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
                                scene->updateTrajectory(vector<Node>{Node{.x = unit.getX(), .y = unit.getY()}});
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
                    // S'il s'agit d'une nouvelle case
                    if(oldMouseEventX != mouseEventX || oldMouseEventY != mouseEventY){
                        if(!engine->getAttackMode()){
                            // Calcul et affiche un chemin possible
                            Node depart = {.x =  engine->getSelectedUnit().get()->getX(), .y = engine->getSelectedUnit().get()->getY()};
                            Node destination = {.x = mouseEventX, .y = mouseEventY};
                            scene->updateTrajectory(Cordinate::aStar(depart, destination, engine->getGameState().getWorld(), engine->getGameState().getGameObjects(), engine->getSelectedUnit().get()->getPm()));
                        } else if(!engine->getSelectedUnit().get()->hasAttacked) {
                            // Affiche les cases pouvant être affectées par l'attaque
                            vector<int> attackField = DisplayAttack::createField(engine->getSelectedUnit().get(), engine->getGameState().getWorld());
                            if(attackField[mouseEventX+mouseEventY*engine->getGameState().getWorld().getYMax()] == 1){
                                scene->updateTrajectory(DisplayAttack::createDamageArea(mouseEventX, mouseEventY, engine->getSelectedUnit().get(), engine->getGameState().getWorld()));
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
                        scene->updateTrajectory(vector<Node>{Node{.x = engine->getSelectedUnit().get()->getX(), .y = engine->getSelectedUnit().get()->getY()}});
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


