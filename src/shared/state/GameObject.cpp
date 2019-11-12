#include "GameObject.h"


using namespace state;

GameObject::GameObject(int x, int y) {
    this->x = x;
    this->y = y;
}

GameObject::GameObject() {
    this->x = 0;
    this->y = 0;
}

int GameObject::getX() const {
    return this->x;
}

int GameObject::getY() const {
    return this->y;
}

void GameObject::setX(int x) {
    this->x = x;
}

void GameObject::setY(int y) {
    this->y = y;
}


