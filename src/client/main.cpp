#include <iostream>
#include <SFML/Graphics.hpp>
#include <state.h>
#include "render.h"

using namespace std;
using namespace state;
using namespace render;

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
    sf::RenderWindow window(sf::VideoMode(320, 320), "ZCOM");

    // 3. Intanciate Scene
    shared_ptr<Scene> scene;
    scene.reset(new Scene(window));

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
    window.clear();
    window.display();
    return 0;
}



void handleInputs(sf::RenderWindow &window){
    sf::Event event;
    // TODO 1.1 : Gestion deplacement souris

    // TODO 1.2 : Gestion clic gauche souris

    // TODO 1.3 : Gestion clic droit souris

    // TODO 1.4 : Gestion clic molette

    // TODO 1.5 : Gestion déplacement molette

    // TODO 2.0 : Gestion touches clavier
    while (window.pollEvent(event)) {
        if(event.type == sf::Event::Closed)
            window.close();
    }

  }


