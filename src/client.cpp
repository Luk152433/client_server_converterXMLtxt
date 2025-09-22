#include "header.h"

#include <cstring>


cli::client::client(const std::string &arg_address, int arg_port) 
    : address(arg_address), port(arg_port), socket_var(-1) {}

cli::client::~client() {
  
}

void cli::client::connectToServ() {
    socket_var = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_var < 0) {
        std::cerr << "Error creating socket." << std::endl;
        return;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    if (inet_pton(AF_INET, address.c_str(), &serverAddr.sin_addr) <= 0) {
        std::cerr << "Invalid address" << std::endl;
        #ifdef _WIN32
            closesocket(socket_var);
            WSACleanup(); 
        #else
            close(socket_var);
        #endif  
       
        return;
    }

    if (connect(socket_var, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "connection failed" << std::endl;
        #ifdef _WIN32
            closesocket(socket_var);
            WSACleanup(); 
        #else
            close(socket_var);
        #endif  
        return;
    }
    std::cout << "Connected server" << std::endl;
}

void cli::client::sendFile(const std::string &fileName) {
    std::ifstream file(fileName, std::ios::binary);
    if (!file.is_open()) {
        return;
    }

    char buffer[1024];
    while (!file.eof()) {
        file.read(buffer, sizeof(buffer));
        int bytesToSend = file.gcount();  
        if (bytesToSend > 0) {
            if (send(socket_var, buffer, bytesToSend, 0) < 0) {
                std::cerr << "error sending file" << std::endl;
                break;
            }
        }
    }

    std::string doneMsg = "DONE";
    if (send(socket_var, doneMsg.c_str(), doneMsg.size(), 0) < 0) {
        std::cerr << "error send DONE " << std::endl;
    }
    file.close();
    std::cout << "File sent " << std::endl;
}



void cli::client::receiveFile(const std::string &base_FileName) {
    char buffer[1024];
    int bytesRead;
    std::string fileName = base_FileName; 

   
    bytesRead = recv(socket_var, buffer, sizeof(buffer), 0);
    if (bytesRead <= 0) {
        std::cerr << "Error receiving file data." << std::endl;
        return;
    }

    
    std::string receivedData(buffer, bytesRead);
    if (receivedData.find("<?xml") != std::string::npos) {
        fileName += ".xml"; 
    } else {
        fileName += ".txt"; 
    }

  
    std::ofstream file(fileName, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file " << fileName << std::endl;
        return;
    }

    
    file.write(buffer, bytesRead);

    
    while ((bytesRead = recv(socket_var, buffer, sizeof(buffer), 0)) > 0) {
        std::string receivedData(buffer, bytesRead);

        
        size_t donePosition = receivedData.find("DONE");
        if (donePosition != std::string::npos) {
            std::cout << "Received DONE signal from server." << std::endl;

            if (donePosition > 0) {
                file.write(buffer, donePosition);  
            }
            break; 
        }

      
        file.write(buffer, bytesRead);
    }

    file.close();
    std::cout << "File received successfully as: " << fileName << std::endl;
}
