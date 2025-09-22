
cmake_minimum_required(VERSION 3.10)
project(Zadani)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)


find_package(GTest REQUIRED)


set(TEST_SOURCES
    server_test.cpp
    client_test.cpp
    converter_test.cpp
)


if(WIN32)
    
    include_directories(D:/Users/pugixml-1.14/pugixml-1.14/src)   
    set(PUGIXML_SRC D:/Users/pugixml-1.14/pugixml-1.14/src/pugixml.cpp) 
elseif(UNIX)

    include_directories(/usr/include)                    
    link_directories(/usr/lib/x86_64-linux-gnu)          
endif()


add_executable(runTests ${TEST_SOURCES})


if(WIN32)
    target_sources(runTests PRIVATE ${PUGIXML_SRC}) 
endif()


target_link_libraries(runTests PUBLIC
    src_Lib          
    GTest::gtest     
    GTest::gtest_main
    pugixml           
)


target_include_directories(runTests PUBLIC
    ${PROJECT_SOURCE_DIR}/include 
)


include(GoogleTest)
gtest_discover_tests(runTests)
