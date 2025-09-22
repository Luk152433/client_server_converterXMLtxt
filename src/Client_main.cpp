#include "header.h"
#include <iostream>
#include <stdexcept> 

int main(int argc, char *argv[])
{
 #ifdef _WIN32
    
    WSAData wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed: " << WSAGetLastError() << std::endl;
        return -1;
    }
#endif

    
   cli::client ObjC("127.0.0.1", 8070);
         while (true) {
        std::string filename;
        std::cout << "Enter file name to send ( 'exit' to quit): ";
        std::cin >> filename;

        if (filename == "exit") {
            break;
        }
         

   
        ObjC.connectToServ(); 
        ObjC.sendFile(filename); 
        ObjC.receiveFile("converted_client"); 
         }
      

      #ifdef _WIN32
        closesocket(ObjC.socket_var);
        WSACleanup(); 
    #else
        close(ObjC.socket_var);
    #endif    

    return 0; 
}