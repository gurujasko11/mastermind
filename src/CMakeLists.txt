cmake_minimum_required(VERSION 2.8)

FIND_PACKAGE(Boost 1.71 COMPONENTS program_options REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

file(GLOB mastermind_SRC
     "*.h"
     "*.cpp"
)

add_library (Game Game.cpp Game.h)
add_executable(mastermind ${mastermind_SRC})
target_link_libraries (mastermind LINK_PUBLIC ${Boost_LIBRARIES} Game)

