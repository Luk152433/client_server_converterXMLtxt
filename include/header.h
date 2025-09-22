#include <iostream>
#include <fstream>
#include <string>
#include <sstream>



#ifdef _WIN32
#include <winsock2.h>  
#include <ws2tcpip.h>  
#include "D:/Users/pugixml-1.14/pugixml-1.14/src/pugixml.hpp"
#pragma comment(lib, "ws2_32.lib") 
#else
#include <arpa/inet.h> 
#include <unistd.h>    
#include <sys/socket.h> 
#include <pugixml.hpp>
#endif

#include "server.h"
#include "client.h"
#include "converter.h"


