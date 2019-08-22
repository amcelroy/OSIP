#include <httplib.h> //<--- MUST COME FIRST! Due to a preprocesser defination in windows.h
#include <websocketserver.h>
#include <thread>
#include <fstream>

void runHTTPServer() {
	using namespace httplib;

	Server httpServer;

	httpServer.set_base_dir("C:\\Users\\Austin\\Documents\\OSIP\\OSIP_Frontend\\public");

	httpServer.Get("/", [](const Request& req, Response& res) {
		int x = 0;
		//std::ifstream s("index.html");
		//std::stringstream buffer;
		//buffer << s.rdbuf();
		//res.set_content(buffer.str(), "text/plain");
	});

	httpServer.listen("localhost", 80);
}

int main(int argc, char *argv[]){
    WebsocketServer ws;

    std::thread serverThread(&WebsocketServer::run, &ws);
	std::thread httpThread(runHTTPServer);
    serverThread.join();
	httpThread.join();

    return 0;
}
