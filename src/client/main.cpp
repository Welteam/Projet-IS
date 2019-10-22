#include <iostream>
#include <SFML/Graphics.hpp>
#include <state.h>
#include <sstream>
#include "render.h"
// #include "Cordinate.h"

using namespace std;
using namespace state;
using namespace render;

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
                      "1, 5, 3, 9, 4, 17, 1, 12,/ 9, 10,/ 0, 19,:";

    // 1. Intancier GameState
    GameState gameState{};

    // 2. & 3. Open Window SFML & instancier Scene
    // Set up the render
    sf::RenderWindow window(sf::VideoMode(640, 640), "Tilemap");
    shared_ptr<Scene> scene;
    scene.reset(new Scene(window));
    cout << scene.get() << endl;

    // 4. Register Scene -> GameState
    gameState.registerObserver(scene.get());

    // 5. Charger la carte World dans GameState
    gameState.setWorld(World{mapWorld});

    // 6. Appel GameState::notify(EventID event, void* tileSet);
    //gameState.notifyObservers(StateEvent{ALLCHANGED}, gameState);

    // on crée la fenêtre
    /*sf::RenderWindow window(sf::VideoMode(640, 640), "Tilemap");

    // on crée la tilemap avec le niveau précédemment défini
    World world{mapWorld};
    vector<TileType> input = world.getTiles();
    int arr[input.size()];
    std::copy(input.begin(), input.end(), arr);

    TileMap map;
    if (!map.load("res/tiles2.png", sf::Vector2u(16, 16), arr, 20, 20))
        return -1;

    sf::Texture texture;
    if (!texture.loadFromFile("res/unity_right.png"))
    {
        // erreur...
    }
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setPosition(sf::Vector2f(0.f, 0.f)); // position absolue


    // on fait tourner la boucle principale
    while (window.isOpen())
    {
        // on gère les évènements
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                switch(event.key.code){
                    case sf::Keyboard::Left:
                        std::cout << "the left key was pressed" << std::endl;
                        sprite.move(sf::Vector2f(-16.f, 0.f)); // décalage relatif à la position actuelle
                        break;
                    case sf::Keyboard::Right:
                        std::cout << "the right key was pressed" << std::endl;
                        sprite.move(sf::Vector2f(16.f, 0.f)); // décalage relatif à la position actuelle
                        break;
                    case sf::Keyboard::Up:
                        std::cout << "the up key was pressed" << std::endl;
                        sprite.move(sf::Vector2f(0.f, -16.f)); // décalage relatif à la position actuelle
                        break;
                    case sf::Keyboard::Down:
                        std::cout << "the down key was pressed" << std::endl;
                        sprite.move(sf::Vector2f(0.f, 16.f)); // décalage relatif à la position actuelle
                        break;
                    default:
                        std::cout << "Other key was pressed" << std::endl;
                        break;
                }
            }
        }

        // on dessine le niveau
        window.clear();
        window.draw(map);
        window.draw(sprite);
        window.display();



    }*/


    return 0;
}


