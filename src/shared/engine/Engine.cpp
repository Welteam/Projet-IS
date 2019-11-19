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

void engine::Engine::addCommand(std::shared_ptr<Command> command, unsigned int priority) {
    if (!command) {
        throw new std::runtime_error("empty command error");
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

void engine::Engine::runCommands(bool prod) {
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

std::stack<std::shared_ptr<engine::Command>> engine::Engine::update() {
    return std::stack<std::shared_ptr<engine::Command>>();
}

const std::map<int, std::shared_ptr<Command>> &Engine::getCommands() {
    return commands;
}

void Engine::setCommands(const std::map<int, std::shared_ptr<Command>> &commands) {
    this->commands = commands;
}
