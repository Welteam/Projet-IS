#include <iostream>
#include <SFML/Graphics.hpp>
#include <state.h>
#include "render.h"

using namespace std;
using namespace state;
using namespace render;

shared_ptr<Scene> scene;
void handleInputs(sf::RenderWindow &window);

int main(int argc,char* argv[])
{
    // Example of a Map in a string
    string mapWorld = "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2,"
                      "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2,"
                      "0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2,"
                      "0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2,"
                      "0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2,"
                      "0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 1,"
                      "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4,"
                      "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 4, 4, 4, 4,"
                      "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 4, 4, 4, 4, 4,"
                      "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 4, 4, 4, 4, 4,"
                      "3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4,"
                      "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4,"
                      "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4,"
                      "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4,"
                      "0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 1,"
                      "0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2,"
                      "0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2,"
                      "0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2,"
                      "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2,"
                      "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2,:"
                      "1, 2, 3, 7, 1, 17, 3, 12,/ 7, 10,/ 0, 0,:"
                      "1, 5, 3, 9, 4, 17, 1, 12,/ 9, 10,/ 0, 19,:"
                      "20, 20,:";

    // 1. Intancier GameState
    GameState gameState{};

    // 2. Create Window SFML
    sf::RenderWindow window(sf::VideoMode(640, 640), "ZCOM");
    // on crée une vue
    //sf::View view(window.getView().getCenter(), sf::Vector2f((float)320, (float)320));
    sf::View view(sf::FloatRect(0,0,320,320));

    // 3. Intanciate Scene
    scene.reset(new Scene(window, view));

    // 4. Register Scene -> GameState
    gameState.registerObserver(scene.get());

    // 5. Charger la carte World dans GameState
    World world{mapWorld};
    gameState.setWorld(world);

    // 6. Charger players dans GameState
    Player player1{1, world.getSpawnUnits1(), world.getSpawnTowers1(), world.getSpawnApparitionAreas1()};
    Player player2{2, world.getSpawnUnits2(), world.getSpawnTowers2(), world.getSpawnApparitionAreas2()};
    gameState.setPlayer1(player1);
    gameState.setPlayer2(player2);

    while (window.isOpen()) {
        // TODO 1.0 : Gestion des événements
        handleInputs(window);
    }
    return 0;
}



void handleInputs(sf::RenderWindow &window){
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
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    std::cout << "the right button was pressed" << std::endl;
                    std::cout << "mouse x: " << event.mouseButton.x << std::endl;
                    std::cout << "mouse y: " << event.mouseButton.y << std::endl;
                }
                break;
            case sf::Event::MouseButtonReleased:
                break;
            case sf::Event::MouseMoved:
                std::cout << "new mouse x: " << event.mouseMove.x << std::endl;
                std::cout << "new mouse y: " << event.mouseMove.y << std::endl;
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


