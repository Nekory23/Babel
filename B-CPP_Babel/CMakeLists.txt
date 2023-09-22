cmake_minimum_required(VERSION 3.17)
project(CPP_BABEL)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "./bin")

set(CMAKE_CXX_STANDARD 14)

if (UNIX)
    add_definitions("-fPIC")
endif()
if(WIN32)
    add_compile_options("/std:c++latest")
    add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
endif()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(client/include)

add_executable(babel_client
            client/include/Babel.hpp
            client/include/LibAudio.hpp
            client/include/Error.hpp
            client/include/Network.hpp
            
            client/src/Main.cpp
            client/src/PortAudio.cpp
            client/src/Opus.cpp
            client/src/Error.cpp
            client/src/Network.cpp
)

if (!WIN32)
    target_compile_options(babel_client PRIVATE -g3)
endif()

target_link_libraries(babel_client PRIVATE ${CONAN_LIBS})
target_include_directories(babel_client PRIVATE ${CONAN_INCLUDE_DIRS})
get_target_property(BABEL_EXE_PATH babel_client CMAKE_RUNTIME_OUTPUT_DIRECTORY)

include_directories(server/include)

add_executable(babel_server
            server/include/Babel.hpp
            server/include/Error.hpp

            server/src/Main.cpp
            server/src/Error.cpp
)

if (!WIN32)
    target_compile_options(babel_server PRIVATE -g3)
endif()

get_target_property(BABEL_EXE_PATH babel_server CMAKE_RUNTIME_OUTPUT_DIRECTORY)