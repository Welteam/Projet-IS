#include "ServiceException.h"

using namespace std;
using namespace server;

ServiceException::ServiceException (HttpStatus status, string msg)
        : httpStatus(status),msg(msg) {
}

HttpStatus ServiceException::status () const {
    return httpStatus;
}

const char* ServiceException::what () const noexcept(false) {
    return msg.c_str();
}

