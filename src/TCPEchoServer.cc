#include "YASL.h"      // For Socket, ServerSocket, and SocketException
#include <iostream>    // For cerr and cout
#include <cstdlib>     // For atoi()
#include "json.hpp"
#include <string>
#include <fstream>

const uint32_t RCVBUFSIZE = 32;    // Size of receive buffer
using json = nlohmann::json;//para usar la libreria de nlohmann/json
// TCP client handling function
std::string root_dir="", notFoundFile="";

std::string leer(std::string pagina);//funcion que se usara para leer las paginas solicitadas

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

  //variables de control para obtener el get que envia el cliente
  int cont= 0,control =1,i=0;
  //variables paraalmacenar la pagina el la respuesta al cliente
  std::string pagina="",respuesta ="";

  uint32_t recvMsgSize;
while((recvMsgSize = sock->recv(echoBuffer, RCVBUFSIZE))>=RCVBUFSIZE){    
      //obtencion del GET enviado por el cliente
      i =0;
      while(control == 1){//aqui obtengo la pagina
        i++;
        pagina="";
        if(echoBuffer[i]=='/'){
          cont=i+1;
          while(1){
            if(echoBuffer[cont]==' '){//ya obteniendo la pagina
              respuesta ="";
              respuesta = leer(pagina);//una vez obtenida la pagina se envia a la funcion leer() que comprueba si existe la pagina y retorna la pagina html
              sock->send(respuesta.c_str(),respuesta.size());//se envia la pagina al socket
              control = 0;//se finalizan los while
              break;
            } 
            pagina+=echoBuffer[cont];//se va almacenando la pagina en esta variable
            cont++;
          }      
        }
    }
  }if(recvMsgSize<0){//si el tamaño del msg es <0 quiere decir que fallo el envio de datos
    std::cout << "Error al recibir los datos"<<std::endl;
  }
  delete sock;// se elimina el socket para esperar la siguiente peticion
}

json leer_json(){//funcion para leer el archivo json
  json j;
  std::ifstream ifs("bin/config.json");//se lee el archivo
  j = json::parse(ifs);//se parsea
  return j;// se retorna
}
std::string leer(std::string pagina){//funcion para leer los html que solicite el cliente
    std::ifstream bafer(root_dir+pagina);//se abre el archivo
    std::string a,n="", response="",b;//variables para leer los archivos
    if(bafer.good()){//se comprueba que el archivo exista
      while(!bafer.eof()){//se lee el archivo y se guarda en la variable response que se retornara como respuesta
        getline(bafer,a); 
        response+=a;
      }
      //response = "HTTP/1.1 200 OK\r\nConnection: keep-alive\r\nContent-Type: text/html\r\nContent-Length:"+std::to_string(n.size())+"\r\n"+n;
    }else{//si no existe el archivo solicitado se envia la pagina 404
      std::ifstream error(notFoundFile+"404.html");
      while(!error.eof()){
        getline(error,b); 
        response+=b;
      }
      //response = "HTTP/1.1 200 OK\r\nConnection: keep-alive\r\n Content-Type: text/html\r\nContent-Length:138\r\n \r\n"+m;
    } 
    return response;//retorna la variable con la pagina solicitada o con la pagina 404.html

}
int main(int argc, char *argv[]) {
  json cfg = leer_json();
  uint16_t echoServPort;   
  std::string ip = cfg["ip"];
  echoServPort  = cfg["puerto"];
  root_dir = cfg["root_dir"];
  notFoundFile = cfg["notFoundFile"];
  std::cout <<"Server a la escucha en: "+ip+":"+std::to_string(echoServPort)<< std::endl;
  try {
    TCPServerSocket servSock(ip,echoServPort);     // Server Socket object
    for(;;){
      HandleTCPClient(servSock.accept());
    }

    
  } catch (SocketException &e) {
    std::cerr << e.what() << std::endl;
    exit(EXIT_FAILURE);
  }
  // NOT REACHED

  return EXIT_SUCCESS;
}
