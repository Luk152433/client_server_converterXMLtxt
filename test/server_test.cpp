#include "server.h"  
#include "gtest/gtest.h"
#include <fstream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>




TEST(ServerTests, ServerConstructorTest) {
    serv::server testServer(8070);
    EXPECT_NO_THROW(serv::server(8070));  // check constructor
}


int main(int argc, char **argv)
{
   

    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}

