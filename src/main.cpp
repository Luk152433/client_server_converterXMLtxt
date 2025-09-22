#include "header.h"
#include <csignal>
#include <cstdlib>

bool running = true;



int main()
{
  #ifdef _WIN32
    
    WSAData wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed: " << WSAGetLastError() << std::endl;
        return -1; 
    }
#endif

   
    serv::server servObj(8070);

 
        servObj.process();

    return 0;
}