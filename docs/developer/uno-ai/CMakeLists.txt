CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

PROJECT(sample_ai)
INCLUDE_DIRECTORIES(include)
AUX_SOURCE_DIRECTORY(src DIR_SRCS)
# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -DDEBUG")
# SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
# SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage -lgcov")
set(CMAKE_CXX_STANDARD 11)
#set environment variable
SET(SAMPLE_AI ${DIR_SRCS}
        src/Datastream.cpp
        include/Datastream.h
        src/Player.cpp
        include/Player.h
        src/Card.cpp
        include/Card.h
        include/jsoncpp/json/json-forwards.h
        include/jsoncpp/json/json.h
        include/jsoncpp/jsoncpp.cpp)
SET(ORGANIZATION_NAME OneMorePigeon)
#add executable file
ADD_EXECUTABLE(sample_ai ${SAMPLE_AI} src/main.cpp)
