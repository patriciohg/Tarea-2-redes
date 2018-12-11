#include "YASL.h"      // For Socket, ServerSocket, and SocketException
#include <iostream>    // For cerr and cout
#include <cstdlib>     // For atoi()
#include "json.hpp"
#include <string>
#include "boost.hpp"
#include <fstream>

const uint32_t RCVBUFSIZE = 32;    // Size of receive buffer
using json = nlohmann::json;
// TCP client handling function
std::string root_dir="", notFoundFile="";

std::string leer(std::string pagina);

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

  // Send received string and receive again until the end of transmission
  char echoBuffer[RCVBUFSIZE];  
  int cont= 0,control =1,i=0;
  std::string pagina="",respuesta ="";

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
          if(echoBuffer[cont]==' '){
            std::cout <<pagina<<std::endl;
            control = 0;
            respuesta = leer(pagina);
            std::cout <<respuesta<< std::endl;
            recvMsgSize = 0;
            break;
          } 
          pagina+=echoBuffer[cont];
          cont++;
        }       
      }
    }    
    sock->send(respuesta.c_str(), respuesta.size()); 
  }
} 
json leer_json(){
  json j;
  std::ifstream ifs("bin/config.json");
  j = json::parse(ifs);
  return j;
}
std::string leer(std::string pagina){
    std::ifstream bafer("WWW-data/"+pagina);
    std::string a,n="", response="",b,m="";
    if(bafer.good()){
      std::cout <<"existe"<< std::endl;
      while(!bafer.eof()){
        getline(bafer,a); 
        n+=a;
      }
      
      response = "HTTP/1.1 200 OK\r\nConnection: keep-alive\r\n Content-Type: text/html\r\ncharset=ISO-8859-1\r\nContent-Length:"+std::to_string(n.size())+"\r\n"+n;
    }else{
      std::ifstream error("WWW-error/404.html");
      while(!error.eof()){
        getline(error,b); 
        m+=b;
      }
      response = "HTTP/1.1 200 OK\r\nConnection: keep-alive\r\n Content-Type: text/html\r\nContent-Length:138\r\n \r\n"+m;
    } 
    return response;

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
