#include "UserService.h"
#include "ServiceException.h"

using namespace server;
using namespace std;

UserService::UserService (UserDB& userDB) : AbstractService("/user"),
                                            userDB(userDB) {

}

HttpStatus UserService::get (Json::Value& out, int id) const {
    const User* user = userDB.getUser(id);
    if (!user)
        throw ServiceException(HttpStatus::NOT_FOUND,"Invalid user id");
    out["orientation"] = user->orientation;
    out["currentLife"] = user->currentLife;
    out["x"] = user->x;
    out["y"] = user->y;
    return HttpStatus::OK;
}

HttpStatus UserService::post (const Json::Value& in, int id) {
    const User* user = userDB.getUser(id);
    if (!user)
        throw ServiceException(HttpStatus::NOT_FOUND,"Invalid user id");
    unique_ptr<User> usermod (new User(*user));
    if (in.isMember("orientation")) {
        usermod->orientation = in["orientation"].asInt();
    }
    if (in.isMember("currentLife")) {
        usermod->currentLife = in["currentLife"].asInt();
    }
    if (in.isMember("x")) {
        usermod->x = in["x"].asInt();
    }
    if (in.isMember("y")) {
        usermod->x = in["y"].asInt();
    }
    userDB.setUser(id,std::move(usermod));
    return HttpStatus::NO_CONTENT;
}

HttpStatus UserService::put (Json::Value& out,const Json::Value& in) {
    int x = in["x"].asInt();
    int y = in["y"].asInt();
    int orientation = in["orientation"].asInt();
    int currentLife = in["currentLife"].asInt();
    out["id"] = userDB.addUser(make_unique<User>(orientation,currentLife,x,y));
    return HttpStatus::CREATED;
}

HttpStatus UserService::remove (int id) {
    const User* user = userDB.getUser(id);
    if (!user)
        throw ServiceException(HttpStatus::NOT_FOUND,"Invalid user id");
    userDB.removeUser(id);
    return HttpStatus::NO_CONTENT;
}



