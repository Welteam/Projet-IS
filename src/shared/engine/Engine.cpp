#include <iostream>
#include "Engine.h"
#include "../../../extern/jsoncpp-1.8.0/jsoncpp.cpp"
#include <fstream>

using namespace engine;
using namespace std;

Engine::Engine(state::GameState gameState) {
    this->gameState = gameState;
}

Engine::Engine(state::GameState gameState, bool dev) {
    this->gameState = gameState;
    this->dev = dev;
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

    if (dev) {
        ifstream readF;
        readF.open("replay.txt", ios_base::in);
        Json::Reader reader;
        Json::Value root;
        vector<Command> prevCmds;
        reader.parse(readF, root);
        readF.close();
        command->serialize(root);
        ofstream file;
        file.open("replay.txt", ios_base::out | ios_base::trunc);
        Json::StyledWriter writer;
        file << writer.write(root);
        file.close();
    }

    if (commands.find(priority) == commands.cend())
        commands[priority] = command;
    else {
        priority = static_cast<unsigned int>(commands.size());
        commands[priority] = command;
    }
    commands_mutex->unlock();
}

void engine::Engine::undoCommands() {
    if (!previous_commands.empty()) {
        auto prevState = this->previous_commands.top();
        this->gameState = std::move(*prevState);
        this->previous_commands.pop();
        this->gameState.notifyObservers(state::StateEvent{state::PLAYER1}, this->gameState);
        this->gameState.notifyObservers(state::StateEvent{state::PLAYER2}, this->gameState);
    }
}

bool engine::Engine::runCommands() {
    if(!commands.empty()){
        bool validAction = true;
        cout << "runCommand" << endl;
        commands_mutex->lock();
        // Copy commands in the buffer
        std::unique_ptr<std::map<int, std::shared_ptr<Command>>> commands_buffer;
        commands_buffer.reset(new std::map<int, std::shared_ptr<Command>>(commands));
        commands.clear();
        commands_mutex->unlock();
        state::GameState& currentState = gameState;
        previous_commands.push(make_shared<state::GameState>(currentState));
        auto it = commands_buffer->begin();
        while (it != commands_buffer->cend()) {
            validAction = it->second->execute(gameState);
            it++;
        }
        return validAction;
    }
    return true;
}

const std::map<int, std::shared_ptr<Command>> &Engine::getCommands() {
    return commands;
}

void Engine::setCommands(const std::map<int, std::shared_ptr<Command>> &newCommands) {
    this->commands = newCommands;
}

const std::stack<std::shared_ptr<state::GameState>> &Engine::getPreviousCommands() {
    return previous_commands;
}
