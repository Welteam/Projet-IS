//
// Created by xsims on 10/11/2019.
//

#include "Command.h"

unsigned int engine::Command::getIdPlayer() {
    return 0;
}

engine::CommandTypeId engine::Command::getTypeId() {
    return ATTACK_CMD;
}

void engine::Command::setIdPlayer(unsigned int idPlayer) {

}
