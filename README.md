# Projet-IS : ZCOM

ZCOM is a game project for our last year in engineering school who is related to the game "XCOM", with the sprites of "Advance War" and the atmosphere of "Dofus".

![Alt text](res/screenshot.png?raw=true "Screenshot of the game")

## Installation

```bash
git clone https://github.com/Welteam/Projet-IS.git
cd Projet-IS/cmake-build-debug
cmake ..
make
```

From the same folder, you can then run the client with `../bin/client` :

* You can run the client with `../bin/client hello`. It will display an "hello world" in the terminal.

* You can run the client with `../bin/client render`. It will show you a render of the game (map, units, towers...).

* You can run the client with `../bin/client engine`. You can now play with your mouse or launch the demo.

	Left-click to select a unit, and move your mouse to see the possibles moves, and left-click to confirm the move.

	Double left-click to put a unit in attack mode, move your mouse and left-click to confirm your target.

	Right-click to unselect a unit (or left-click on a empty area).

	Press T for next turn.

	Press D for the demo.

* You can run the client with `../bin/client random_ai`. You can now play with your mouse first.
	
	Then Press T for next turn and let the AI play versus you.


* You can run the client with `../bin/client heuristic_ai`. You can now play with your mouse first.
	
	Then Press T for next turn and let the AI play versus you. Be careful, she's SAVAGE !!!
