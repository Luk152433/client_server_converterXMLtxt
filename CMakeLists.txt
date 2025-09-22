cmake_minimum_required(VERSION 3.10)
project(Zadani)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

message(STATUS "Project source directory: ${PROJECT_SOURCE_DIR}")

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")

add_subdirectory(src)

if(UNIX)
    add_subdirectory(test)
    set(Test_DIR "${PROJECT_SOURCE_DIR}/test")
endif()

if(WIN32)
   
    set(PUGIXML_INCLUDE_DIR "D:/Users/pugixml-1.14/pugixml-1.14/include")
    set(PUGIXML_LIB_DIR "D:/Users/pugixml-1.14/pugixml-1.14/build")
    set(PUGIXML_LIB "pugixml")  

    include_directories(${PUGIXML_INCLUDE_DIR})
    link_directories(${PUGIXML_LIB_DIR})
endif()


add_executable(main ${SRC_DIR}/main.cpp)
add_executable(client ${SRC_DIR}/Client_main.cpp)

if(WIN32)
    target_link_libraries(main PUBLIC src_Lib ${PUGIXML_LIB} ws2_32)
    target_link_libraries(client PUBLIC src_Lib ${PUGIXML_LIB} ws2_32)
else()
    target_link_libraries(main PUBLIC src_Lib pugixml)
    target_link_libraries(client PUBLIC src_Lib pugixml)
endif()
