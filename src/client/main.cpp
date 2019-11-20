#include <iostream>
#include <SFML/Graphics.hpp>
#include <state.h>
#include "render.h"
#include "engine.h"
#include "ai.h"
#include "engine/Cordinate.cpp"
#include "engine/DisplayAttack.cpp"
#include <cstring>
#include <thread>

using namespace std;
using namespace state;
using namespace render;
using namespace engine;
using namespace ai;

bool iaTurn = false;
void handleInputs(sf::RenderWindow &window, const shared_ptr<Scene>& scene, const shared_ptr<Engine>& e);

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
    gameState.setWorld(World{"../res/map2.txt"});

    // 6. Charger players dans GameState
    gameState.setPlayer1(Player{1, gameState.getWorld().getSpawnUnits1(), gameState.getWorld().getSpawnTowers1(), gameState.getWorld().getSpawnApparitionAreas1()});
    gameState.setPlayer2(Player{2, gameState.getWorld().getSpawnUnits2(), gameState.getWorld().getSpawnTowers2(), gameState.getWorld().getSpawnApparitionAreas2()});
    gameState.setActivePlayer(gameState.getPlayer1());

    if (argc == 2) {
        if (strcmp(argv[1], "hello") == 0) {
            cout << "Bonjour le monde !" << endl;
        } else if (strcmp(argv[1], "render") == 0) {
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
        } else if (strcmp(argv[1], "engine") == 0) {
            cout << "Bienvenue sur engine !" << endl;
            cout << "" << endl;
            cout << "Clic gauche pour selectionner un soldat rouge." << endl;
            cout << "Déplacer la souris pour voir ces déplacements, puis cliquer sur une des cases pour le déplacer."
                 << endl;
            cout << "" << endl;
            cout << "Double clic pour passer en mode attaque." << endl;
            cout << "Appuyer sur T pour changer de tour et laisser votre adversaire jouer." << endl;
            cout << "" << endl;
            cout << "Sinon appuyer sur D pour la demo (mais cela ruinerai votre expérience)." << endl;
            // Create our engine
            shared_ptr<Engine> engine = make_shared<Engine>(gameState);

            while (window.isOpen()) {
                handleInputs(window, scene, engine);
                engine->runCommands();
            }
        } else if (strcmp(argv[1], "random_ai") == 0) {
            cout << "Bienvenue sur random_ai !" << endl;

            cout << "Appuyer sur T pour changer de tour et laisser votre l'IA jouer." << endl;
            unique_ptr<AI> ai;
            ai.reset(new RandomAI);
            // Create our engine
            shared_ptr<Engine> engine = make_shared<Engine>(gameState);
            while (window.isOpen()) {
                if(!iaTurn){
                    // Manage user inputs
                    handleInputs(window, scene, engine);
                    engine->runCommands();
                } else {
                    cout << "run ai" << endl;
                    ai->run(*engine);
                    iaTurn = false;
                }
            }
        }
    } else {
        cout << "I don't understand" << endl;
        cout << "you can say hello, render, engine..." << endl;
    }
    return 0;
}



void handleInputs(sf::RenderWindow &window, const shared_ptr<Scene>& scene, const shared_ptr<Engine>& e){
    sf::Event event{};
    int oldMouseEventX = 0;
    int oldMouseEventY = 0;
    int mouseEventX;
    int mouseEventY;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::T)
                {
                    std::cout << "the T key was pressed, new turn" << std::endl;
                    e->getGameState().unselectedUnit();
                    shared_ptr<Command> newTurnCommand = make_shared<NewTurnCommand>();
                    e->addCommand(newTurnCommand, 1);
                    iaTurn = true;
                }
                if (event.key.code == sf::Keyboard::D)
                {
                    std::cout << "Lancement du mode démo ne pas bouger votre souris..." << std::endl;
                    for(const auto& unit : e->getGameState().getActivePlayer().getUnits()){
                        if(unit.getX() == 1 && unit.getY() == 4){
                            // cout << "Une unité est maintenant sélectionée" << endl;
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
                        cout << "L'unité démo ne peut pas être sélectionée." << endl;
                        cout << "Relancer le jeu s'il vous plaît" << endl;
                    }
                    for(const auto& unit : e->getGameState().getActivePlayer().getUnits()){
                        if(unit.getX() == 3 && unit.getY() == 6){
                            // cout << "Une unité est maintenant sélectionée" << endl;
                            e->getGameState().setSelectedUnit(make_shared<Character>(unit));
                        }
                    }
                    if(e->getGameState().getSelectedUnit() != nullptr){
                        for(int i = 0; i < 4; i++){
                            this_thread::sleep_for(std::chrono::milliseconds(400));
                            e->getGameState().setAttackMode(true);
                            vector<int> attackField = DisplayAttack::createField(e->getGameState().getSelectedUnit().get(), e->getGameState().getWorld());
                            if(attackField[9+ 5 * e->getGameState().getWorld().getYMax()] == 1){
                                scene->updateTrajectory(DisplayAttack::createDamageArea(9, 5, e->getGameState().getSelectedUnit().get(), e->getGameState().getWorld()));
                            } else{
                                scene->updateTrajectory(vector<Node>{});
                            }
                            this_thread::sleep_for(std::chrono::milliseconds(600));
                            shared_ptr<Command> attack = make_shared<AttackCommand>(e->getGameState().getSelectedUnit(), 9, 5);
                            e->addCommand(attack, 1);
                            e->runCommands();
                        }
                    } else {
                        cout << "L'unité démo ne peut pas être sélectionée." << endl;
                        cout << "Relancer le jeu s'il vous plaît" << endl;
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
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    int x = (event.mouseButton.x - (((1 - window.getView().getViewport().width)*window.getSize().x)/2))/(window.getView().getViewport().width*window.getSize().x)*20; // NOLINT(bugprone-narrowing-conversions)
                    int y = (event.mouseButton.y - (((1 - window.getView().getViewport().height)*window.getSize().y)/2))/(window.getView().getViewport().height*window.getSize().y)*20;
                    bool foundNewUnit = false;
                    if(e->getGameState().getSelectedUnit() != nullptr){
                        for(const auto& unit : e->getGameState().getActivePlayer().getUnits()){
                            if((unit.getX() == x && unit.getY() == y)){
                                if(x != e->getGameState().getSelectedUnit().get()->getX() || y != e->getGameState().getSelectedUnit().get()->getY()){
                                    foundNewUnit = true;
                                    e->getGameState().setSelectedUnit(make_shared<Character>(unit));
                                }
                            }
                        }
                        if(!foundNewUnit){
                            if(e->getGameState().getAttackMode()){
                                shared_ptr<Command> attack = make_shared<AttackCommand>(e->getGameState().getSelectedUnit(), x, y);
                                e->addCommand(attack, 1);
                                e->getGameState().unselectedUnit();
                            } else{
                                // cout << "unité déjà en mode déplacement" << endl;
                                if(e->getGameState().getSelectedUnit().get()->getX() == x && e->getGameState().getSelectedUnit().get()->getY() == y){
                                    e->getGameState().setAttackMode(true);
                                    // cout << "unité mis en mode attaque" << endl;
                                } else {
                                    shared_ptr<Command> move = make_shared<MoveCommand>(e->getGameState().getSelectedUnit(), x, y);
                                    e->addCommand(move, 1);
                                    e->getGameState().unselectedUnit();
                                }
                            }
                        }
                    } else {
                        // cout << "Aucune unité sélectionée précédent" << endl;
                        for(const auto& unit : e->getGameState().getActivePlayer().getUnits()){
                            if(unit.getX() == x && unit.getY() == y){
                                // cout << "Une unité est maintenant sélectionée" << endl;
                                e->getGameState().setSelectedUnit(make_shared<Character>(unit));
                            }
                        }
                    }
                }
                if (event.mouseButton.button == sf::Mouse::Right) {
                    e->getGameState().unselectedUnit();
                }
                break;
            case sf::Event::MouseButtonReleased:
                break;
            case sf::Event::MouseMoved:
                if(e->getGameState().getSelectedUnit() != nullptr){
                    // Récupère la position de la souris en pixel et la convertie en "case"
                    mouseEventX = (event.mouseMove.x - (((1 - window.getView().getViewport().width)*window.getSize().x)/2))/(window.getView().getViewport().width*window.getSize().x)*20;
                    mouseEventY = (event.mouseMove.y - (((1 - window.getView().getViewport().height)*window.getSize().y)/2))/(window.getView().getViewport().height*window.getSize().y)*20;
                    // S'il s'agit d'une nouvelle case
                    if(oldMouseEventX != mouseEventX || oldMouseEventY != mouseEventY){
                        if(!e->getGameState().getAttackMode()){
                            // Calcul et affiche un chemin possible
                            Node depart = {.x =  e->getGameState().getSelectedUnit().get()->getX(), .y = e->getGameState().getSelectedUnit().get()->getY()};
                            Node destination = {.x = mouseEventX, .y = mouseEventY};
                            scene->updateTrajectory(Cordinate::aStar(depart, destination, e->getGameState().getWorld(), e->getGameState().getGameObjects(), e->getGameState().getSelectedUnit().get()->getPm()));
                        } else if(!e->getGameState().getSelectedUnit().get()->getHasAttacked()) {
                            // Affiche les cases pouvant être affectées par l'attaque
                            vector<int> attackField = DisplayAttack::createField(e->getGameState().getSelectedUnit().get(), e->getGameState().getWorld());
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
