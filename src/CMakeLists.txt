
cmake_minimum_required(VERSION 3.16)

project(CPingPong)
    
## compilation/linkage settings
INCLUDE_DIRECTORIES(
    .
    )

ADD_EXECUTABLE(paho_pingpong paho_pingpong.c)


TARGET_LINK_LIBRARIES(paho_pingpong paho-mqtt3as json-c)

