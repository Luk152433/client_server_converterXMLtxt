#ifndef CLIENT_H
#define CLIENT_H

namespace cli{

    class client
    {
    private:
        /* data */
        
        std::string address;
        int port;

    public:
        int socket_var;
        void fun();
        client(const std::string &arg_address, int arg_port);
        void connectToServ();
        void sendFile(const std::string &fileName);
        void receiveFile(const std::string &base_FileName);
        ~client();
    };
    

}    




#endif
