#ifndef __WEBSOCKETSERVER_H
#define __WEBSOCKETSERVER_H

#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <octpipeline.h>
#include <octconfigfile.h>
#include <boost/filesystem.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;
typedef server::message_ptr message_ptr;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

using json = nlohmann::json;

using namespace std;
using namespace boost::filesystem;

class WebsocketServer
{
	json _getDAQ();
	json _startDAQ();

    server m_WebsocketServer;

    void on_message(websocketpp::connection_hdl hdl, message_ptr msg);

    void on_open(websocketpp::connection_hdl hdl);

    void on_close(websocketpp::connection_hdl hdl);

    void on_http(websocketpp::connection_hdl hdl);

    OCTPipeline m_OCT;

    OCTConfigFile m_octcf;

    OCTConfig m_octc;

    string m_CurrentPath;

	Galvos::GalvoParameters m_GalvoParameters;

    websocketpp::connection_hdl m_ConnectionHandle;

    bool m_NoConnection = true;

	void _simulateUserDAQChange();

	void _simulateModeChange();

public:
    WebsocketServer();

    void run();

    void sendFrame();

    void datasetFinished();

};

#endif // WEBSOCKETSERVER_H
