cmake_minimum_required(VERSION 3.0)
project(HASS_AliGenie)

IF (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug)
ENDIF (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)

set(WARNING_FLAGS "-Wall -Winline -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wno-inline -Wno-reorder -Wno-unused-function -Wno-switch")

set(PREDEFINE "-DCINATRA_ENABLE_SSL -DCINATRA_ENABLE_GZIP")

# Select flags.
set(CMAKE_CXX_FLAGS "-std=c++2a ${WARNING_FLAGS} ${PREDEFINE} -mcrc32 -mavx2 -march=native -static-libgcc -static-libstdc++")
set(CMAKE_CXX_FLAGS_DEBUG "-g -ggdb -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -ggdb -O3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

if (DEFINED ENABLE_COVERAGE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DENABLE_COVERAGE --coverage")
  set(CMAKE_EXE_LINKER_FLAGS "--coverage")
  set(CMAKE_SHARED_LINKER_FLAGS "--coverage")
endif ()

include_directories("/usr/local/include")
include_directories("${PROJECT_SOURCE_DIR}/cinatra/include")
link_directories("/usr/local/lib")
link_directories("/usr/lib64")
link_directories("/usr/lib")

find_package(Boost REQUIRED)

if (NOT Boost_FOUND)
  message("Not found Boost")
endif ()

add_library(objs OBJECT main.cpp)

set(DEP_LIBS ssl.a crypto.a z dl uuid pthread)

add_executable(HASS_AliGenie $<TARGET_OBJECTS:objs>)

target_link_libraries(HASS_AliGenie ${DEP_LIBS})
