cmake_minimum_required(VERSION 3.5)
project(BattleForDawnridge)
set(CMAKE_CXX_STANDARD 23)
add_definitions(-Werror)
add_definitions(-O3)
#add_definitions(-flto) # Project does not work with -flto for unknown reason
file(GLOB_RECURSE BattleForDawnridge_src ${PROJECT_SOURCE_DIR}/src/*)

set(CMAKE_PREFIX_PATH ${PROJECT_SOURCE_DIR}/extlibs/lib/cmake/SFML)
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.5 COMPONENTS system window graphics audio network REQUIRED)
include_directories(${SFML_INCLUDE_DIR})

file(GLOB bzip2_SRCS "${PROJECT_SOURCE_DIR}/bzip2-1.0.8/*.cpp" "${PROJECT_SOURCE_DIR}/bzip2-1.0.8/*.c" "${PROJECT_SOURCE_DIR}/bzip2-1.0.8/*.h")
add_library(Bzip2Lib STATIC ${bzip2_SRCS})

set(CMAKE_PREFIX_PATH ${PROJECT_SOURCE_DIR}/boost_1_85_0/tools/cmake/include)
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS iostreams serialization)
include_directories(${Boost_INCLUDE_DIRS})
add_library(boost_bzip2 SHARED IMPORTED)
add_library(boost_iostreams SHARED IMPORTED)
add_library(boost_serialization SHARED IMPORTED)
set_target_properties(boost_bzip2 PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/boost_1_85_0/stage/lib/libboost_bzip2.a)
set_target_properties(boost_iostreams PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/boost_1_85_0/stage/lib/libboost_iostreams.a)
set_target_properties(boost_serialization PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/boost_1_85_0/stage/lib/libboost_serialization.a)

add_executable(BattleForDawnridge ${BattleForDawnridge_src})

target_link_libraries(BattleForDawnridge sfml-system sfml-window sfml-graphics sfml-audio sfml-network boost_bzip2 boost_iostreams boost_serialization Bzip2Lib)