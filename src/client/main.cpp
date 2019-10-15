#include <iostream>
#include <SFML/Graphics.hpp>
#include <state.h>
//#include "Cordinate.h"

using namespace std;
using namespace state;

int main(int argc,char* argv[])
{
    cout << "It works !" << endl;
    
    Player player;
    cout << player.getUnits().empty() << endl;

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

