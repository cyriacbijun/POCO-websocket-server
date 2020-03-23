#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerRequestImpl.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/WebSocket.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPMessage.h"
#include "Poco/Net/HTTPClientSession.h"
#include <iostream>
#include <string>
#include <vector>


using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerRequestImpl;
using Poco::Net::HTTPServerResponse;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPServerParams;
using Poco::Net::HTTPServer;
using Poco::Net::WebSocket;
using Poco::Net::WebSocketException;
using Poco::Net::ServerSocket;
using Poco::Util::ServerApplication;
using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPMessage;
using std::vector;
using std::string;
using std::cout;
using std::stoi;

char message[1024];
int size = 0;
int flag;
bool wait = true;

class WebSocketRequestHandler: public Poco::Net::HTTPRequestHandler
	{
	public:
		void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
		{
			try
			{
				WebSocket ws(request, response);
				char buffer[1024];
				int flags = 0;
				int n,len;
				n = ws.receiveFrame(buffer, sizeof(buffer), flags);
				if(n == 2){
						do
						{
							n = ws.receiveFrame(buffer, sizeof(buffer), flags);
							cout<<"Frame recieved"<<"\n"<<buffer;
							strcpy(message,buffer);
							size = n;
							flag = flags;
							wait = false;
						}while (n > 0 || (flags & WebSocket::FRAME_OP_BITMASK) != WebSocket::FRAME_OP_CLOSE);
				}
				else if(n == 4){
					do
						{	
							if(size != 0 && !wait){
								ws.sendFrame(message, size, flags);
								wait = true;
							}
						}while (true);
				}
				
			}
			catch (WebSocketException& exc)
			{
				switch (exc.code())
				{
				case WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION:
					response.set("Sec-WebSocket-Version", WebSocket::WEBSOCKET_VERSION);
					// fallthrough
				case WebSocket::WS_ERR_NO_HANDSHAKE:
				case WebSocket::WS_ERR_HANDSHAKE_NO_VERSION:
				case WebSocket::WS_ERR_HANDSHAKE_NO_KEY:
					response.setStatusAndReason(HTTPResponse::HTTP_BAD_REQUEST);
					response.setContentLength(0);
					response.send();
					break;
				}
			}
		}
	};
    class WebSocketRequestHandlerFactory: public Poco::Net::HTTPRequestHandlerFactory
	{
	public:
		Poco::Net::HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request)
		{
			return new WebSocketRequestHandler;
		}
	};


class MyServerApp : public ServerApplication {
protected:
    int main(const vector<string> &args) {
        ServerSocket ss(stoi(args[0]), 4000);
        HTTPServer server(new WebSocketRequestHandlerFactory, ss, new HTTPServerParams);
        cout<<"starting up the server"<<"\n";
		server.start();
        waitForTerminationRequest();
        server.stop();
        return Application::EXIT_OK;
    }
};

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " port " << std::endl;
        return 1;
    }
    MyServerApp app;
    return app.run(argc, argv);
}