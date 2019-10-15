#include <iostream>
#include <SFML/Graphics.hpp>
#include <state.h>
#include "Cordinate.h"


using namespace std;
using namespace state;




int main(int argc,char* argv[])
{
    

    cout << "It works !" << endl;

    /*Node player;
    player.x = 50 / X_STEP;
    player.y = 200 / Y_STEP;

    Node destination;
    destination.x = 600 / X_STEP;
    destination.y = 300 / Y_STEP;
    int i = 1;
    vector<int> obstacles(1250, 0);

    for (Node node : Cordinate::aStar(player, destination, obstacles)) {
        //Your code here
        cout << " noeud " << i << " : x = "<< node.x << " y = " << node.y << endl;
        i++;

    }*/

    Weapon weapon{};
    weapon.setDirection(static_cast<DirectionType>(3));
    cout << weapon.getDirection() << endl;

    Sniper sniper;
    cout << sniper.getDirection() << endl;



    sf::Window window(sf::VideoMode(800, 600), "My window");

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }

    return 0;
}

// TODO 1.0 : Create the screen title with a background image, buttons to launch/exit the game
//SFMLWindows titleScreen = new SFMLWindows ...;

/* TODO 2.0 : Game screen containing status bar(LP, nb of units, etc.. for each player),
 * TODO 2.1 : a layer of the static map,
 * TODO 2.2 : interaction bottom bar with buttons for settings (tactical render of the map, sound...). */
//SFMLWIndows gameScreen = new SFMLWindows ...;

// TODO 3.0 Read txt file create two players with initial number of character
/*initPlayers(){
    new Map(dile.txt)
    new player(Map.units1, Map.tower1, Map.apparitionArea1)
    ....
}*/

