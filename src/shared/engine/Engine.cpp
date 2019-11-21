#include <iostream>
#include "Engine.h"

using namespace engine;
using namespace std;

Engine::Engine(state::GameState gameState) {
    this->gameState = std::move(gameState);
}

state::GameState& Engine::getGameState() {
    return this->gameState;
}

// TODO: store currently selected unit in command. Currently need to run commands every time we change unit.
void engine::Engine::addCommand(const std::shared_ptr<Command>& command, unsigned int priority) {
    if (!command) {
        throw std::runtime_error("empty command error");
    }
    // Block addCommand during commands copy
    commands_mutex->lock();
    if (commands.find(priority) == commands.cend())
        commands[priority] = command;
    else {
        priority = static_cast<unsigned int>(commands.size());
        commands[priority] = command;
    }
    commands_mutex->unlock();
}

void engine::Engine::undoCommands() {
    this->commands.clear(); 
}

void engine::Engine::runCommands() {
    commands_mutex->lock();
    // Copy commands in the buffer
    std::unique_ptr<std::map<int, std::shared_ptr<Command>>> commands_buffer;
    commands_buffer.reset(new std::map<int, std::shared_ptr<Command>>(commands));
    commands.clear();
    commands_mutex->unlock();
    auto it = commands_buffer->begin();
    while (it != commands_buffer->cend()) {
        it->second->execute(gameState);
        it++;
    }
}

const std::map<int, std::shared_ptr<Command>> &Engine::getCommands() {
    return commands;
}

void Engine::setCommands(const std::map<int, std::shared_ptr<Command>> &newCommands) {
    this->commands = newCommands;
}
