#include "Observable.h"
#include "GameState.h"
#include <algorithm>
#include <iostream>


namespace state {


    Observable::~Observable() = default;

    void Observable::registerObserver(Observer *o) {
        observers.push_back(o);
    }

    void Observable::unregisterObserver(Observer *o) {
        observers.erase(find(observers.cbegin(),observers.cend(),o));
    }

    void Observable::unregisterObservers() {
        observers.clear();
    }

    void Observable::notifyObservers(const StateEvent &e, GameState &gameState) {
        for (auto o : observers)
        {
            if(o!=nullptr){
                o->stateChanged(e, gameState);
            } else {
                std::cout << "pointeur nul" << std::endl;

                if(observers.empty()){
                    std::cout << "liste vide" << std::endl;
                } else {
                    std::cout << observers.size()<< std::endl;
                    std::cout << observers.at(0) << std::endl;

                }
            }
        }
    }
}
