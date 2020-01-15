#include "CommandService.h"
#include <iostream>
using namespace server;
using namespace std;


CommandService::CommandService (engine::Engine& engine): AbstractService("/command"), engine(engine){

}

HttpStatus CommandService::get (Json::Value& out, int id) const{
    out=listCommand;
    return HttpStatus::OK;

}

HttpStatus CommandService::put(Json::Value& out,const Json::Value& in){
    listCommand = in;
    /*if(!(listCommand["commands"].empty())) {
        listCommand["commands"][listCommand["commands"].size()] = in;
    }
    else {
        listCommand["commands"][0] = in;
    }*/
    return HttpStatus::CREATED;
}

bool CommandService::isVersion(){
    return false;
}

