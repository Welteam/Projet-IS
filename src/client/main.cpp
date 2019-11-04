#include <iostream>
#include <SFML/Graphics.hpp>
#include <state.h>
#include "render.h"
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
            // 1. Intancier GameState
            GameState gameState{};

            // 2. Create Window SFML
            sf::RenderWindow window(sf::VideoMode(640, 640), "ZCOM");
            // on crée une vue
            //sf::View view(window.getView().getCenter(), sf::Vector2f((float)320, (float)320));
            sf::View view(sf::FloatRect(0, 0, 320, 320));

            // 3. Intanciate Scene
            shared_ptr<Scene> scene = make_shared<Scene>(window, view);

            // 4. Register Scene -> GameState
            gameState.registerObserver(scene.get());

            // 5. Charger la carte World dans GameState
            gameState.setWorld(World{"../res/map.txt"});

            // 6. Charger players dans GameState
            gameState.setPlayer1(Player{1, gameState.getWorld().getSpawnUnits1(), gameState.getWorld().getSpawnTowers1(), gameState.getWorld().getSpawnApparitionAreas1()});
            gameState.setPlayer2(Player{2, gameState.getWorld().getSpawnUnits2(), gameState.getWorld().getSpawnTowers2(), gameState.getWorld().getSpawnApparitionAreas2()});

            while (window.isOpen()) {
                // TODO 1.0 : Gestion des événements
                handleInputs(window, scene, 0);
            }
        }
        else if (strcmp(argv[1], "engine") == 0) {
            cout << "engine" << endl;

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
            // Create our engine
            shared_ptr<Engine> engine = make_shared<Engine>(gameState);

            while (window.isOpen()) {
                // TODO 1.0 : Gestion des événements
                handleInputs(window, scene, engine);
            }
        }
    } else {
        cout << "I don't understand" << endl;
        cout << "you can only say hello" << endl;
    }
    return 0;
}



void handleInputs(sf::RenderWindow &window, shared_ptr<Scene> scene, shared_ptr<Engine> engine){
    sf::Event event;
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

                    std::cout << "pos x: " << x << std::endl;
                    std::cout << "pos y: " << y << std::endl;
                }
                break;
            case sf::Event::MouseButtonReleased:
                break;
            case sf::Event::MouseMoved:
                //std::cout << "new mouse x: " << event.mouseMove.x << std::endl;
                //std::cout << "new mouse y: " << event.mouseMove.y << std::endl;
                break;
            case sf::Event::MouseEntered:
                std::cout << "the mouse cursor has entered the window" << std::endl;
                break;
            case sf::Event::MouseLeft:
                std::cout << "the mouse cursor has left the window" << std::endl;
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


