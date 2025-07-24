cmake_minimum_required(VERSION 3.31)
project(untitled)

set(CMAKE_CXX_STANDARD 20)

add_executable(untitled main.cpp
        App.cpp
        App.h
        proto/Image.cpp
        proto/Image.h
        proto/proto.h
        proto/Types.cpp
        proto/Types.h
        proto/Transform.h)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(untitled
        PRIVATE
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)