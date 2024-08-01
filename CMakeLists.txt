cmake_minimum_required(VERSION 3.28)
project(BattleForDawnridge)
set(CMAKE_CXX_STANDARD 23)
add_definitions(-Werror)
add_definitions(-O3)
add_definitions(-flto)
file(GLOB BattleForDawnridge_src CONFIGURE_DEPENDS "*.hpp" "*.cpp")
add_executable(BattleForDawnridge ${BattleForDawnridge_src}
        CompByNewMovePriority.cpp
        CompByClickPriority.cpp
        CompByDrawPriority.cpp)

find_package(Boost REQUIRED COMPONENTS serialization)
find_package(Boost REQUIRED COMPONENTS iostreams)
add_library(boost::serialization INTERFACE IMPORTED)
add_library(boost::iostreams INTERFACE IMPORTED)
set_target_properties(boost::serialization PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${Boost_INCLUDE_DIRS}"
        INTERFACE_LINK_LIBRARIES "${Boost_SERIALIZATION_LIBRARIES}")
set_target_properties(boost::iostreams PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${Boost_INCLUDE_DIRS}"
        INTERFACE_LINK_LIBRARIES "${Boost_IOSTREAMS_LIBRARIES}")

target_link_libraries(BattleForDawnridge sfml-system sfml-window sfml-graphics sfml-audio boost::serialization boost::iostreams)