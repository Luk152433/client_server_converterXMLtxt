#include "header.h"


#include <cstring>


serv::server::server(int arg_port) : port(arg_port), socket_var(-1), clientSock(-1) {}

serv::server::~server() {
    if (clientSock != -1){ 
        #ifdef _WIN32
            closesocket(clientSock);
            WSACleanup(); 
        #else
            close(clientSock);
        #endif
        }  
    if (socket_var != -1){ 
        #ifdef _WIN32
            closesocket(socket_var);
            WSACleanup(); 
        #else
            close(socket_var);
        #endif
        }   
}

void serv::server::process() {
    socket_var = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_var < 0) {
        std::cerr << "Error creating socket." << std::endl;
        return;
    }

    int opt = 1;

    #ifdef _WIN32
        if (setsockopt(socket_var, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt)) < 0) {
            std::cerr << "setsockopt failed: " << WSAGetLastError() << std::endl;
        
            closesocket(socket_var);
            WSACleanup(); 
          return;  
            }   
    #else
        if (setsockopt(socket_var, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
            std::cerr << "Setsockopt error." << std::endl;
            close(socket_var);
            return;
        }
    #endif

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(socket_var, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Error binding socket." << std::endl;
       
        #ifdef _WIN32
            closesocket(socket_var);
            WSACleanup(); 
        #else
            close(socket_var);
        #endif
      
        return;
    }

    if (listen(socket_var, 5) < 0) {
        std::cerr << "Error listening on socket." << std::endl;
        #ifdef _WIN32
            closesocket(socket_var);
            WSACleanup(); 
        #else
            close(socket_var);
        #endif
        return;
    }

    std::cout << "Server listening on port " << port << "..." << std::endl;

    while (true) {
        clientSock = accept(socket_var, nullptr, nullptr);
        if (clientSock < 0) {
            std::cerr << "Error accepting connection." << std::endl;
            continue;
        }

        std::cout << "Client connected." << std::endl;
        handleClient(clientSock);
        //sendToClient(clientSock);
        #ifdef _WIN32
            closesocket(clientSock);
           // WSACleanup(); 
        #else
            close(clientSock);
        #endif

        clientSock = -1;
    }
}

void serv::server::handleClient(int clientSock) {
    char buffer[1024];
    std::ostringstream fileStream;
    std::string dataForFormatDetection;
    int bytesRead;
    bool done = false;

    std::cout << "Receiving file..." << std::endl;

    
    while (!done) {
        bytesRead = recv(clientSock, buffer, sizeof(buffer), 0);

        if (bytesRead > 0) {
            std::string receivedData(buffer, bytesRead);
            dataForFormatDetection.append(receivedData);

        
            size_t donePosition = receivedData.find("DONE");
            if (donePosition != std::string::npos) {
                done = true;

            
                fileStream.write(buffer, donePosition);
                break; 
            }

            fileStream.write(buffer, bytesRead); 
        } else if (bytesRead < 0) {
            std::cerr << "Error receiving data." << std::endl;
            done = true; 
        }
    }

    
    std::string fileContent = fileStream.str();
    std::cout << "Received data size: " << dataForFormatDetection.size() << " bytes" << std::endl;

  
    bool isXml = dataForFormatDetection.find("<?xml") != std::string::npos; 

    if (isXml) {
        std::cout << "File format detected as XML." << std::endl;
        convertedContent = conv::FileConverter().xmlToTxt(fileContent);  // Konwersja XML -> TXT
    } else {
        std::cout << "File format detected as TXT." << std::endl;
        convertedContent = conv::FileConverter().txtToXml(fileContent);  // Konwersja TXT -> XML
    }

   
    std::string outputFilename = saveConvertedFile("converted_file", !isXml);

   
    sendToClient(clientSock, outputFilename);
}




std::string serv::server::saveConvertedFile(const std::string &filename, bool isXmlFormat) {
    
    std::string finalFilename;
    if (isXmlFormat) {
        finalFilename = filename + ".xml";  
    } else {
        finalFilename = filename + ".txt"; 
    }

    std::ofstream outFile(finalFilename);
    if (!outFile.is_open()) {
        std::cerr << "Error opening file: " << finalFilename << std::endl;
        return "";
    }

    outFile << convertedContent;  
    outFile.close();
    std::cout << "Converted file saved as " << finalFilename << std::endl;

    return finalFilename;  
}


void serv::server::sendToClient(int clientSock, const std::string& filename) {
    char buffer[1024];
    std::ifstream inFile(filename, std::ios::binary);  
    if (!inFile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::cout << "Sending converted file: " << filename << std::endl;

    while (!inFile.eof()) {
        inFile.read(buffer, sizeof(buffer));
        int bytesRead = inFile.gcount();
        if (bytesRead > 0) {
            if (send(clientSock, buffer, bytesRead, 0) < 0) {
                std::cerr << "Error sending data to client." << std::endl;
                return;
            }
        }
    }

    std::string doneMsg = "DONE";
    send(clientSock, doneMsg.c_str(), doneMsg.size(), 0);

    inFile.close();
    std::cout << "File sent successfully." << std::endl;
}
