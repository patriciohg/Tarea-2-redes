#include "YASL.h"      // For Socket, ServerSocket, and SocketException
#include "checkArgs.h"
#include <iostream>    // For cerr and cout
#include <cstdlib>     // For atoi()
#include "Json.hpp"
#include <string>
#include "regex.h"


const uint32_t RCVBUFSIZE = 32;    // Size of receive buffer

// TCP client handling function
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
		i =0;
		while(control == 1){
			i++;
			pagina="";
			if(echoBuffer[i]=='/'){
				cont=i+1;
				while(1){
					pagina+=echoBuffer[cont];
					if(echoBuffer[cont]==' '){
						std::cout << pagina<<std::endl;
						control = 0;
						break;
					}	
					cont++;
				}				
			}
		}
		sock->send(echoBuffer, recvMsgSize);	
	}
	delete sock;
} 
int main(int argc, char *argv[]) {
	
	checkArgs* argumentos = new checkArgs(argc, argv);
	
	uint16_t echoServPort;   
	echoServPort  = argumentos->getArgs().PORT;

	try {
		TCPServerSocket servSock(echoServPort);     // Server Socket object
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
