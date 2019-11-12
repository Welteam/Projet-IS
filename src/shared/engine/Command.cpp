#include "Command.h"

using namespace engine;

unsigned int Command::getIdPlayer() {
    return 0;
}

engine::CommandTypeId Command::getTypeId() {
    return ATTACK_CMD;
}

void Command::setIdPlayer(unsigned int idPlayer) {

}
