add_library(src_Lib Client_main.cpp main.cpp client.cpp server.cpp converter.cpp)


target_include_directories(src_Lib 
                        PUBLIC ${PROJECT_SOURCE_DIR}/include/
                        
                        )