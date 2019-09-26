#include "websocketserver.h"

//#define SIMULATE_UI

json WebsocketServer::_getDAQ(){
    json response = { { "response" , "get_daq" },
               { "frames" , m_OCT.getConfig().TotalBScans },
               { "width" , m_OCT.getProcessor()->getWidth() } ,
               { "height" , m_OCT.getProcessor()->getHeight() } ,
               { "ascan_length", m_OCT.getProcessor()->getRawLength() },
               { "fft_length" , m_OCT.getProcessor()->getFFTLength() } ,
			   { "mode", "daq" } };
    return response;
}

void WebsocketServer::on_message(websocketpp::connection_hdl hdl, message_ptr msg){
    json response = { "response", "Unknown command " + msg->get_payload() };

    try{
        json jmsg = json::parse(msg->get_payload());
        std::string request = jmsg["request"];

        if(!request.compare("frame")){
            //Send frame, see frameProcessed()
        	if(m_OCT.getLoader()->typeName() == "LoadOCTPipeline"){
				LoadOCTPipeline* loctp = dynamic_cast<LoadOCTPipeline*>(m_OCT.getLoader());
				double frameNumber = jmsg["frame"];
				loctp->readFrame(static_cast<int>(frameNumber));
        	}

        	sendFrame();

            return;
        }else if(!request.compare("save")){
			m_OCT.stop();
			m_OCT.start(m_octc, m_GalvoParameters, true);
        }else if(!request.compare("list")){
			//TODO: Fetch pullbacks from the oct folder
			string test[3] = { "1", "2", "3" };

			path p = "c:\\oct_data";
			directory_iterator it{ p };
			vector<string> directory_list;
			while (it != directory_iterator{}) {
				directory_list.push_back((*it++).path().filename().string());
			}

			response = {
				{"response", "pullbacks" },
				{"pullback", directory_list }
			};
			m_WebsocketServer.send(hdl, response.dump(), websocketpp::frame::opcode::TEXT);
        }else if(!request.compare("run")){
            //Re-run the processing with the new values and wait for processing to finish
            //see datasetFinished()
        	if(typeid(m_OCT.getLoader()) == typeid(LoadOCTPipeline)){
				LoadOCTPipeline* loctp = dynamic_cast<LoadOCTPipeline*>(m_OCT.getLoader());
				loctp->reload();
				response = { { "response" , "load" } ,
							 { "name" , m_CurrentPath } ,
							 { "frames" , m_OCT.getConfig().TotalBScans } };
				m_WebsocketServer.send(hdl, response.dump(), websocketpp::frame::opcode::TEXT);
        	}

        	//if(m_OCT.getLoader()->typeName() == "DaqStageAlazar660"){
        	//	_getDAQ();
        	//	m_WebsocketServer.send(hdl, _getDAQ().dump(), websocketpp::frame::opcode::TEXT);
        	//	m_OCT.startDAQ(m_octc, false);
        	//}

            return;
		}
		else if (!request.compare("stop")) {
			//update the 

			m_OCT.stop();
        }else if(!request.compare("load")){
            //request["path"];
            response = { { "response" , "load" } ,
                         { "name" , m_CurrentPath } ,
                         { "frames" , m_OCT.getConfig().TotalBScans }};
            m_WebsocketServer.send(hdl, response.dump(), websocketpp::frame::opcode::TEXT);
            return;
        }else if(!request.compare("set_daq")){
            double points_per_a = jmsg["points_per_ascan"];
            double a_per_b = jmsg["a_per_b"];
            double bscans = jmsg["bscans"];
            double start_trim = jmsg["start_trim"];
            double stop_trim = jmsg["stop_trim"];
            double voltage = jmsg["voltage"];
            double bits = jmsg["bits"];
            double faa = jmsg["fast_axis_amp"];
            double fao = jmsg["fast_axis_offset"];
            double saa = jmsg["slow_axis_amp"];
            double sao = jmsg["slow_axis_offset"];

            OCTConfig o;
            o.PointsPerAScan = static_cast<unsigned long>(points_per_a);
            o.AScansPerBScan = static_cast<unsigned long>(a_per_b);
            o.TotalBScans = static_cast<unsigned long>(bscans);
            o.StartTrim = static_cast<unsigned long>(start_trim);
            o.StopTrim = static_cast<unsigned long>(stop_trim);
            o.Range = static_cast<float>(voltage);
            o.Bits = static_cast<unsigned long>(bits);

			if (o.TotalBScans == 1) {
				o.BScansPerTransfer = static_cast <unsigned long>(1);
			}
			else {
				o.BScansPerTransfer = static_cast <unsigned long>(64);
			}
			o.Gain = 4.0f / 65535.0f;
			o.Bias = 2;

			m_octc = o;

			Galvos::GalvoParameters gp;
			gp.FastAxisAmplitude = faa;
			gp.FastAxisOffset = fao;
			gp.SlowAxisAmplitude = saa;
			gp.SlowAxisOffset = sao;

			m_GalvoParameters = gp;

			m_OCT.stop();
  			m_OCT.start(o, gp, false);
        }else if(!request.compare("get_daq")){
            m_WebsocketServer.send(hdl, _getDAQ().dump(), websocketpp::frame::opcode::TEXT);
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

void WebsocketServer::sendFrame(){
	try{
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
						 { "height" , m_OCT.getConfig().TotalBScans } };
			m_WebsocketServer.send(m_ConnectionHandle, response.dump(), websocketpp::frame::opcode::TEXT);
			m_WebsocketServer.send(m_ConnectionHandle, ptr.data(), ptr.size()*sizeof(unsigned char), websocketpp::frame::opcode::binary);
		}
	}catch(exception e){
		cout << (string(e.what()));
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
void WebsocketServer::_simulateUserDAQChange() {
	std::this_thread::sleep_for(std::chrono::seconds(5));

	OCTConfig o;
	o.PointsPerAScan = static_cast<unsigned long>(2048);
	o.AScansPerBScan = static_cast<unsigned long>(512);
	o.TotalBScans = static_cast<unsigned long>(1);
	o.StartTrim = static_cast<unsigned long>(0);
	o.StopTrim = static_cast<unsigned long>(0);
	o.Range = static_cast<float>(2.0f/65535.0f);
	o.Bits = static_cast<unsigned long>(16);
	o.BScansPerTransfer = static_cast <unsigned long>(1);
	o.Gain = 2 / (65535);
	o.Bias = 2;

	Galvos::GalvoParameters gp;
	gp.FastAxisAmplitude = 5;
	gp.FastAxisOffset = 0;
	gp.SlowAxisAmplitude = 5;
	gp.SlowAxisOffset = 0;

	m_OCT.stop();
	m_OCT.start(o, gp, false);
}

void WebsocketServer::_simulateModeChange() {
	std::this_thread::sleep_for(std::chrono::seconds(5));

	OCTConfig o;
	o.PointsPerAScan = static_cast<unsigned long>(512);
	o.AScansPerBScan = static_cast<unsigned long>(128);
	o.TotalBScans = static_cast<unsigned long>(128);
	o.StartTrim = static_cast<unsigned long>(0);
	o.StopTrim = static_cast<unsigned long>(0);
	o.Range = static_cast<float>(2.0f / 65535.0f);
	o.Bits = static_cast<unsigned long>(16);
	o.BScansPerTransfer = static_cast <unsigned long>(1);
	o.Gain = 2 / (65535);
	o.Bias = 2;

	Galvos::GalvoParameters gp;
	gp.FastAxisAmplitude = 5;
	gp.FastAxisOffset = 0;
	gp.SlowAxisAmplitude = 5;
	gp.SlowAxisOffset = 0;

	m_OCT.changeMode(OCTPipeline::PLAYBACK);
	m_OCT.start(o, gp, false);

	//m_OCT.stop();
	//m_OCT.start(o, gp);
}

WebsocketServer::WebsocketServer()
{
    try {
        //m_WebsocketServer.set_access_channels(websocketpp::log::alevel::all);
        //m_WebsocketServer.clear_access_channels(websocketpp::log::alevel::frame_payload);
    	m_WebsocketServer.clear_access_channels(websocketpp::log::alevel::all);
        m_WebsocketServer.init_asio();
        m_WebsocketServer.set_message_handler(bind(&WebsocketServer::on_message, this, ::_1, ::_2));
        m_WebsocketServer.set_open_handler(bind(&WebsocketServer::on_open, this, ::_1));
        m_WebsocketServer.set_close_handler(bind(&WebsocketServer::on_close, this, ::_1));

        m_CurrentPath = "C:\\Users\\Austin\\Documents\\OSIP\\test_data\\";

        m_octcf.readOCTConfig(m_CurrentPath, &m_octc);

        m_octc.TotalBScans = 128;
        m_octc.PointsPerAScan = 1376;
        m_octc.AScansPerBScan = 128;
		m_octc.BScansPerTransfer = 64;
        m_octc.StartTrim = 0;
        m_octc.StopTrim = 0;
        m_OCT.setConfig(m_octc);
        m_OCT.changeMode(OCTPipeline::OCT_PIPELINE_STATES::DAQ);

		m_GalvoParameters.FastAxisAmplitude = 5;
		m_GalvoParameters.FastAxisOffset = -2.5;
		m_GalvoParameters.SlowAxisAmplitude = 5;
		m_GalvoParameters.SlowAxisOffset = -2.5;

		m_OCT.setDataFolder("oct_data");
        m_OCT.getDisplay()->setMin(-20);
        m_OCT.getDisplay()->setMax(40);
        m_OCT.getLoader()->setLoop(false);
        m_OCT.getProcessor()->subscribeProcessingFinished(std::bind(&WebsocketServer::datasetFinished, this));
        m_OCT.getProcessor()->setAScanSplits(1);
        m_OCT.getProcessor()->setEnfaceRange(1, 10);
        m_OCT.start(m_octc, m_GalvoParameters, false);

        m_NoConnection = true;

#ifdef SIMULATE_UI
		//std::thread simulateUserEvent(&WebsocketServer::_simulateUserDAQChange, this);
		std::thread simulateUserEvent(&WebsocketServer::_simulateModeChange, this);
		simulateUserEvent.detach();
#endif // SIMULATE_UI

    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "other exception" << std::endl;
    }
}

void WebsocketServer::run() {
    try {
        m_WebsocketServer.listen(8081);
        m_WebsocketServer.start_accept();
        m_WebsocketServer.run();
    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "other exception" << std::endl;
    }
}

