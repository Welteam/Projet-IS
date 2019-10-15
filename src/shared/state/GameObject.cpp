#include "GameObject.h"


namespace state {

    int GameObject::getX() {
        return this->x;
    }

    int GameObject::getY() {
        return this->y;
    }

    void GameObject::setX(int x) {
        this->x = x;
    }

    void GameObject::setY(int y) {
        this->y = y;
    }

    GameObject::GameObject(int x, int y) {
        this->x = x;
        this->y = y;
    }

    GameObject::GameObject() {
        this->x = 0;
        this->y = 0;
    }
}

