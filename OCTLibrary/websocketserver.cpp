#include "websocketserver.h"

void WebsocketServer::on_message(websocketpp::connection_hdl hdl, message_ptr msg){
    json response = { "response", "Unknown command " + msg->get_payload() };

    try{
        json jmsg = json::parse(msg->get_payload());
        std::string request = jmsg["request"];

        if(!request.compare("frame")){
            //Send frame, see frameProcessed()
            double frameNumber = jmsg["frame"];
            m_OCT.getLoader()->readFrame(static_cast<int>(frameNumber));
            return;
        }else if(!request.compare("save")){

        }else if(!request.compare("list")){

        }else if(!request.compare("run")){
            //Re-run the processing with the new values and wait for processing to finish
            //see datasetFinished()
            m_OCT.getLoader()->reload();
            response = { { "response" , "load" } ,
                         { "name" , m_CurrentPath } ,
                         { "frames" , m_OCT.getLoader()->getNumberOfFrames() }};
            m_WebsocketServer.send(hdl, response.dump(), websocketpp::frame::opcode::TEXT);
            return;
        }else if(!request.compare("stop")){

        }else if(!request.compare("load")){
            //request["path"];
            response = { { "response" , "load" } ,
                         { "name" , m_CurrentPath } ,
                         { "frames" , m_OCT.getLoader()->getNumberOfFrames() }};
            m_WebsocketServer.send(hdl, response.dump(), websocketpp::frame::opcode::TEXT);
            return;
        }else if(!request.compare("set_galvos")){
            double faa = jmsg["fast_axis_amp"];
            double fao = jmsg["fast_axis_offset"];
            double saa = jmsg["slow_axis_amp"];
            double sao = jmsg["slow_axis_offset"];

        }else if(!request.compare("scan_parameters")){
            response = { { "response" , "scan_parameters" },
                       { "frames" , m_OCT.getLoader()->getNumberOfFrames() },
                       { "width" , m_OCT.getProcessor()->getWidth() } ,
                       { "height" , m_OCT.getProcessor()->getHeight() } ,
                       { "ascan_length", m_OCT.getProcessor()->getRawLength() },
                       { "fft_length" , m_OCT.getProcessor()->getFFTLength() } };
            m_WebsocketServer.send(hdl, response.dump(), websocketpp::frame::opcode::TEXT);
        }else if(!request.compare("system_status")){

        }else if(!request.compare("benchmarks")){

        }else if(!request.compare("init")){

        }else if(!request.compare("options")){

        }else if(!request.compare("update_ascan")){
            unsigned int ascan = static_cast<unsigned int>(jmsg["ascan"]);
            m_OCT.getProcessor()->setAScanToDisplay(ascan);
        }else if(!request.compare("enface")){
            unsigned int start = static_cast<unsigned int>(jmsg["start"]);
            unsigned int stop = static_cast<unsigned int>(jmsg["stop"]);
            m_OCT.getProcessor()->setEnfaceRange(start, stop);
        }else if(!request.compare("scaling")){
            double max = jmsg["max"];
            double min = jmsg["min"];
            m_OCT.getDisplay()->setMax(static_cast<float>(max));
            m_OCT.getDisplay()->setMin(static_cast<float>(min));
            response = { "response", "Updated min / max to " + to_string(min) + " / " + to_string(max) };
            m_WebsocketServer.send(hdl, response.dump(), websocketpp::frame::opcode::TEXT);
            return;
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
    m_ConnectionHandle = hdl;
    m_NoConnection = false;
}

void WebsocketServer::on_close(websocketpp::connection_hdl hdl){
    m_NoConnection = true;
}

void WebsocketServer::frameFinished(){
    if(!m_NoConnection){
        json response = { { "response" , "ascans" } };
        m_WebsocketServer.send(m_ConnectionHandle, response.dump(), websocketpp::frame::opcode::TEXT);
        vector<double> raw = m_OCT.getProcessor()->getRawAScan();
        vector<double> inten = m_OCT.getProcessor()->getIntAScan();
        raw.insert(raw.end(),
                      inten.begin(),
                      inten.end());
        m_WebsocketServer.send(m_ConnectionHandle, raw.data(), raw.size()*sizeof(double), websocketpp::frame::opcode::binary);

        vector<unsigned char> ptr = m_OCT.getDisplay()->getLastBScan8Bit();
        response = { { "response" , "frame"},
                     { "frame" , m_OCT.getProcessor()->getCurrentFrame() },
                     { "width" , m_OCT.getProcessor()->getWidth() },
                     { "height" , m_OCT.getProcessor()->getHeight()} };
        m_WebsocketServer.send(m_ConnectionHandle, response.dump(), websocketpp::frame::opcode::TEXT);
        m_WebsocketServer.send(m_ConnectionHandle, ptr.data(), ptr.size()*sizeof(unsigned char), websocketpp::frame::opcode::binary);

        ptr = m_OCT.getDisplay()->getLastEnFace8Bit();
        response = { { "response" , "enface"},
                     { "width" , m_OCT.getProcessor()->getWidth() },
                     { "height" , m_OCT.getLoader()->getNumberOfFrames() } };
        m_WebsocketServer.send(m_ConnectionHandle, response.dump(), websocketpp::frame::opcode::TEXT);
        m_WebsocketServer.send(m_ConnectionHandle, ptr.data(), ptr.size()*sizeof(unsigned char), websocketpp::frame::opcode::binary);
    }
    return;
}

void WebsocketServer::datasetFinished(){
//    if(!m_NoConnection){
//        vector<unsigned char> ptr = m_OCT.getDisplay()->getLastEnFace8Bit();
//        json response = { { "response" , "enface"},
//                     { "width" , m_OCT.getProcessor()->getWidth() },
//                     { "height" , m_OCT.getLoader()->getNumberOfFrames() } };
//        m_WebsocketServer.send(m_ConnectionHandle, response.dump(), websocketpp::frame::opcode::TEXT);
//        m_WebsocketServer.send(m_ConnectionHandle, ptr->data(), ptr->size()*sizeof(unsigned char), websocketpp::frame::opcode::binary);
//    }
    return;
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
        m_OCT.getProcessor()->subscribeFrameProcessed(std::bind(&WebsocketServer::frameFinished, this));
        m_OCT.getProcessor()->subscribeProcessingFinished(std::bind(&WebsocketServer::datasetFinished, this));

        m_octcf.readOCTConfig(m_CurrentPath + "parameters.oct_scan", &m_octc);
        m_OCT.getLoader()->configureOCTData(m_CurrentPath + "data.bin", &m_octc);
        m_OCT.getProcessor()->setEnfaceRange(1, 10);
        m_OCT.start(m_octc);

        m_NoConnection = true;
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

