#include "websocketserver.h"

void WebsocketServer::on_message(websocketpp::connection_hdl hdl, message_ptr msg){
    std::cout << "on_message called with hdl: " << hdl.lock().get()
              << " and message: " << msg->get_payload()
              << std::endl;

    json response = { "response", "Unknown command " + msg->get_payload() };

    try{
        json jmsg = json::parse(msg->get_payload());
        std::string request = jmsg["request"];

        if(!request.compare("frame")){
            //Send frame
            double frameNumber = jmsg["frame"];
            m_OCT.getLoader()->readFrame(static_cast<int>(frameNumber));
            vector<unsigned char>* ptr = m_OCT.getDisplay()->getLastBScan8Bit();
            response = { { "response" , "frame"},
                         { "frame" , frameNumber },
                         { "width" , m_OCT.getProcessor()->getWidth() },
                         { "height" , m_OCT.getProcessor()->getHeight()} };
            m_WebsocketServer.send(hdl, ptr->data(), ptr->size()*sizeof(unsigned char), websocketpp::frame::opcode::binary);
        }else if(!request.compare("save")){

        }else if(!request.compare("run")){

        }else if(!request.compare("stop")){

        }else if(!request.compare("load")){
            //request["path"];
            m_octcf.readOCTConfig(m_CurrentPath + "parameters.oct_scan", &m_octc);
            m_OCT.getLoader()->configureOCTData(m_CurrentPath + "data.bin", &m_octc);
            m_OCT.getProcessor()->setEnfaceRange(1, 10);
            m_OCT.start(m_octc);
            response = { { "response" , "load" } ,
                         { "name" , m_CurrentPath } ,
                         { "frames" , m_OCT.getLoader()->getNumberOfFrames() }};
        }else if(!request.compare("galvos")){

        }else if(!request.compare("scan_parameters")){

        }else if(!request.compare("system_status")){

        }else if(!request.compare("benchmarks")){

        }else if(!request.compare("init")){

        }else if(!request.compare("options")){

        }else if(!request.compare("enface")){

        }else if(!request.compare("scaling")){
            double max = jmsg["max"];
            double min = jmsg["min"];
            m_OCT.getDisplay()->setMax(static_cast<float>(max));
            m_OCT.getDisplay()->setMin(static_cast<float>(min));
            response = { "response", "Updated min / max to " + to_string(min) + " / " + to_string(max) };
        }else if(!request.compare("filter")){

        }else{
            //Send frame
            m_WebsocketServer.send(hdl, response, websocketpp::frame::opcode::TEXT);
        }
    }catch(std::exception e){
        response = { "response", "Error, bad parsing of " + msg->get_payload() };
    }

    m_WebsocketServer.send(hdl, response.dump(), websocketpp::frame::opcode::TEXT);
}

void WebsocketServer::on_open(websocketpp::connection_hdl hdl){
    int x = 0;
    x += 1;
}

void WebsocketServer::on_close(websocketpp::connection_hdl hdl){
    int y = 0;
}

WebsocketServer::WebsocketServer()
{
    try {
        m_WebsocketServer.set_access_channels(websocketpp::log::alevel::all);
        m_WebsocketServer.clear_access_channels(websocketpp::log::alevel::frame_payload);
        m_WebsocketServer.init_asio();
        m_WebsocketServer.set_message_handler(bind(&WebsocketServer::on_message, this, ::_1, ::_2));
        m_WebsocketServer.set_open_handler(bind(&WebsocketServer::on_open, this, ::_1));
        m_WebsocketServer.set_close_handler(bind(&WebsocketServer::on_close, this, ::_1));

        m_CurrentPath = "/Users/amcelroy/code/OSIP/test_data/";

        m_OCT.init();

        m_OCT.getLoader()->setBufferData(true);
        m_OCT.getDisplay()->setMin(-20);
        m_OCT.getDisplay()->setMax(40);
        m_OCT.getLoader()->setLoop(false);
    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "other exception" << std::endl;
    }
}

void WebsocketServer::run() {
    try {
        m_WebsocketServer.listen(9090);
        m_WebsocketServer.start_accept();
        m_WebsocketServer.run();
    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "other exception" << std::endl;
    }
}

