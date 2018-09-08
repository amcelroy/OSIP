#include <websocketserver.h>
#include <thread>

int main(int argc, char *argv[]){

    WebsocketServer ws;

    std::thread serverThread(&WebsocketServer::run, &ws);
    serverThread.join();

    return 0;
}
