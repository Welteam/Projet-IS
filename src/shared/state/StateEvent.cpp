#include "StateEvent.h"

namespace state {

    StateEvent::StateEvent(StateEventID se) {
        this->stateEventID = se;
    }

    void StateEvent::setStateEventID(StateEventID newID) {
        this->stateEventID = newID;
    }

    int StateEvent::getStateEventID() const {
        return this->stateEventID;
    }
}

