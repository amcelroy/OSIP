#ifndef WEBSOCKETSERVER_H
#define WEBSOCKETSERVER_H

#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <octpipeline.h>
#include <octconfigfile.h>

typedef websocketpp::server<websocketpp::config::asio> server;
typedef server::message_ptr message_ptr;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

using json = nlohmann::json;

using namespace std;

class WebsocketServer
{
    server m_WebsocketServer;

    void on_message(websocketpp::connection_hdl hdl, message_ptr msg);

    void on_open(websocketpp::connection_hdl hdl);

    void on_close(websocketpp::connection_hdl hdl);

    void on_http(websocketpp::connection_hdl hdl);

    OCTPipeline m_OCT;

    OCTConfigFile m_octcf;

    OCTConfig m_octc;

    string m_CurrentPath;

    websocketpp::connection_hdl m_ConnectionHandle;

    bool m_NoConnection = true;
public:
    WebsocketServer();

    void run();

    void frameFinished();

    void datasetFinished();
};

#endif // WEBSOCKETSERVER_H