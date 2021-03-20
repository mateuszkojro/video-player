cmake_minimum_required(VERSION 3.17)
project(video_player_sdl)

set(CMAKE_CXX_STANDARD 14)

# add SDL lib
add_subdirectory(SDL/)
include_directories("./SDL/include")

add_executable(video_player_sdl main.cpp)

target_link_libraries(video_player_sdl SDL2 SDL2::SDL2main ) #SDL2_timer)
