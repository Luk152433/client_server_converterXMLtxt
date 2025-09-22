#ifndef SERVER_H
#define SERVER_H
#include <string>
namespace serv{

    class server
    {
    private:
        /* data */
       
        int clientSock;
        int port;
        std::string convertedContent;

    public:
     int socket_var;
        void fun();
        void process();
        void handleClient(int clientSock);
        void sendToClient(int clientSock, const std::string& filename);
        std::string saveConvertedFile(const std::string &filename, bool isXmlFormat);
        server(int arg_port);
        ~server();
    };
    

}



#endif 
