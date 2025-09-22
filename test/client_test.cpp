#include <gtest/gtest.h>
#include <fstream>
#include "client.h"

//mock
class ClientTest : public ::testing::Test {
protected:
    cli::client* testClient;
    void SetUp() override {
        testClient = new cli::client("127.0.0.1", 8070);
    }

    void TearDown() override {
        delete testClient;
    }
};



TEST_F(ClientTest, SendFile) {
   
    std::ofstream testFile("test_file.txt");
    testFile << "This is a test file." << std::endl;
    testFile.close();

   
    testClient->socket_var = 1; 

    
    testClient->sendFile("test_file.txt");

    
    std::ifstream sentFile("test_file.txt");
    ASSERT_TRUE(sentFile.is_open()) << "plik nie został poprawnie otwarty podczas wysyłania";

 
    sentFile.close();

    
   // std::remove("test_file.txt");
}

