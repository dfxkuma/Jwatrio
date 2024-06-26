cmake_minimum_required(VERSION 3.28)
project(Jwatrio C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_FLAGS "-fexec-charset=CP949")

add_executable(Jwatrio main.c
        render/menu.h
        render/menu.c
        render/text.c
        screen/network.c
        screen/home.c
        render/prompt.h
        render/prompt.c
        screen/solo.c
        game/render.c
        server/server.c
        utils/color.h
        game/draw.c
        game/draw.h
        utils/keyboard.h
        game/engine.c
        server/server.h
        server/server.c
        game/render.h
)

target_link_libraries(Jwatrio ws2_32)