#include "YASL.h"      // For Socket, ServerSocket, and SocketException
#include <iostream>    // For cerr and cout
#include <cstdlib>     // For atoi()
#include "json.hpp"
#include <string>
#include "regex.h"
#include <fstream>

const uint32_t RCVBUFSIZE = 32;    // Size of receive buffer
using json = nlohmann::json;
// TCP client handling function
std::string root_dir="", notFoundFile="";
void leer(std::string pagina);
void HandleTCPClient(TCPSocket *sock) {
	std::cout << "Handling client ";
	try {
		std::cout << sock->getForeignAddress() << ":";
	} catch (SocketException e) {
		std::cerr << "Unable to get foreign address" << std::endl;
	}
	try {
		std::cout << sock->getForeignPort();
	} catch (SocketException e) {
		std::cerr << "Unable to get foreign port" << std::endl;
	}
	std::cout << std::endl;

	// Send received string and receive again until the end of transmission
	char echoBuffer[RCVBUFSIZE];	
	int cont= 0,control =1,i=0;
	std::string pagina="";

	uint32_t recvMsgSize;
	
	while ((recvMsgSize = sock->recv(echoBuffer, RCVBUFSIZE)) > 0) { // Zero means
		//obtencion del GET enviado por el cliente
		i =0;
		while(control == 1){
			i++;
			pagina="";
			if(echoBuffer[i]=='/'){
				cont=i+1;
				while(1){
					pagina+=echoBuffer[cont];
					if(echoBuffer[cont]==' '){
						std::cout << "1"+pagina+"1"<<std::endl;
						control = 0;
						leer(pagina);
						break;
					}	
					cont++;
				}				
			}
		}
		//leer la pagina html pedida por el cliente

		
		
		sock->send(echoBuffer, recvMsgSize);	
	}
	delete sock;
} 
json leer_json(){
	json j;
    std::ifstream ifs("bin/config.json");
    j = json::parse(ifs);
    return j;
}
void leer(std::string pagina){
		std::ifstream bafer("WWW-data/"+pagina);
		std::string a;
		if(bafer.good()){
			std::cout <<"existe"<< std::endl;
			while(!bafer.eof()){
				bafer>>a;	
			}
		}else{
			std::cout <<"no existe"<< std::endl;
		}	

}
int main(int argc, char *argv[]) {
	json cfg = leer_json();
	uint16_t echoServPort;   
	std::string ip = cfg["ip"];
	echoServPort  = cfg["puerto"];
	root_dir = cfg["root_dir"];
	notFoundFile = cfg["notFoundFile"];

	try {
		TCPServerSocket servSock(ip,echoServPort);     // Server Socket object
		for (;;) {   // Run forever
			HandleTCPClient(servSock.accept());
			
		}
	} catch (SocketException &e) {
		std::cerr << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	// NOT REACHED

	return EXIT_SUCCESS;
}
