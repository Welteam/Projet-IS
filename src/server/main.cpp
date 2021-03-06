#include <iostream>
#include "server.h"
#include <string.h>
#include <microhttpd.h>
#include "engine.h"
#include "state.h"


using namespace std;
using namespace engine;
using namespace state;
using namespace server;

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

class Request {
public:
    struct MHD_PostProcessor *pp = nullptr;
    string data;
    ~Request() {
        if (pp) MHD_destroy_post_processor (pp);
    }
};



// Détruit les données d'une requête
static void request_completed (void *cls, struct MHD_Connection *connection,
                   void **con_cls, enum MHD_RequestTerminationCode toe)
{
    Request *request = (Request*)*con_cls;
    if (request) {
        delete request;
        *con_cls = nullptr;
    }
}

// Fonction pour gérer les données imporantes en upload (non implanté ici)
static int post_iterator(void *cls,
                         enum MHD_ValueKind kind,
                         const char *key,
                         const char *filename,
                         const char *content_type,
                         const char *transfer_encoding,
                         const char *data, uint64_t off, size_t size)
{
    return MHD_NO;
}

// Gestionnaire principal
static int main_handler (void *cls,
              struct MHD_Connection *connection,
              const char *url, //
              const char *method,
              const char *version,
              const char *upload_data, size_t *upload_data_size, void **ptr)
{
    // Données pour une requête (en plusieurs appels à cette fonction)
    Request *request = (Request*)*ptr;

    // Premier appel pour cette requête
    if (!request) {
        request = new Request();
        if (!request) {
            return MHD_NO;
        }
        *ptr = request;

        return MHD_YES;
    }

    // Cas où il faut récupérer les données envoyés par l'utilisateur
    if (strcmp(method, MHD_HTTP_METHOD_POST) == 0
        || strcmp(method, MHD_HTTP_METHOD_PUT) == 0) {
        if (*upload_data_size != 0) {
            request->data = upload_data;
            *upload_data_size = 0;
            return MHD_YES;
        }
    }

    HttpStatus status;
    string response;
    try {
        ServicesManager *manager = (ServicesManager*) cls;
        status = manager->queryService(response,request->data,url,method);
    }
    catch(ServiceException& e) {
        status = e.status();
        response = e.what();
        response += "\n";
    }
    catch(exception& e) {
        status = HttpStatus::SERVER_ERROR;
        response = e.what();
        response += "\n";
    }
    catch(...) {
        status = HttpStatus::SERVER_ERROR;
        response = "Unknown exception\n";
    }

    struct MHD_Response *mhd_response;
    mhd_response = MHD_create_response_from_buffer(response.size(),(void *)response.c_str(),MHD_RESPMEM_MUST_COPY);
    if (strcmp(method,MHD_HTTP_METHOD_GET) == 0) {
        MHD_add_response_header(mhd_response,"Content-Type","application/json; charset=utf-8");
    }
    int ret = MHD_queue_response(connection, status, mhd_response);
    MHD_destroy_response(mhd_response);
    return ret;
}

int main(int argc, char *argv[]) {


    /****************************/
    /********** RECORD **********/
    /****************************/
    if(!strcmp(argv[1], "record")) {
        cout << "record" << endl;
        Server s;
        s.run();
    }

    /****************************/
    /********** LISTEN **********/
    /****************************/
    else if(!strcmp(argv[1], "listen")){
        cout << "starting" << endl;
        try {
            ServicesManager servicesManager;
            servicesManager.registerService(make_unique<VersionService>());

            UserDB userDB;
            servicesManager.registerService(make_unique<UserService>(std::ref(userDB)));

            Engine engine(GameState{});
            servicesManager.registerService(make_unique<CommandService>(engine));

            struct MHD_Daemon *d;
            d = MHD_start_daemon(// MHD_USE_SELECT_INTERNALLY | MHD_USE_DEBUG | MHD_USE_POLL,
                    MHD_USE_SELECT_INTERNALLY | MHD_USE_DEBUG,
                    // MHD_USE_THREAD_PER_CONNECTION | MHD_USE_DEBUG | MHD_USE_POLL,
                    // MHD_USE_THREAD_PER_CONNECTION | MHD_USE_DEBUG,
                    8080,
                    NULL, NULL,
                    &main_handler, (void*) &servicesManager,
                    MHD_OPTION_NOTIFY_COMPLETED, request_completed, NULL,
                    MHD_OPTION_END);
            if (d == NULL)
                return 1;
            cout << "Pressez <entrée> pour arrêter le serveur" << endl;
            (void) getc(stdin);
            MHD_stop_daemon(d);
        }
        catch(exception& e) {
            cerr << "Exception: " << e.what() << endl;
        }
    }

    else{
        cout << "I don't understand" << endl << "you can say record or listen" << endl;
    }

    return 0;
}

//Connection to Host
// sf::Http http("http://localhost",8080);
// //Test de connection avec requete sur la version
// sf::Http::Request req ("/version",sf::Http::Request::Get);
// sf::Http::Response response = http.sendRequest(req);


// if (response.getStatus()==sf::Http::Response::Ok){
//   cout << response.getBody()<<endl;
// } else {
//   cout << "Request Failed: "<<response.getStatus()<<endl;
// }
