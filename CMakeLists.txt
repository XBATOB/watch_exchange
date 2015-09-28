cmake_minimum_required(VERSION 2.6)
project(watch_exchange)
find_package(Qt4 REQUIRED)
include(FindPkgConfig)
pkg_check_modules(M REQUIRED qamqp)

set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

include_directories(
    ${M_INCLUDE_DIRS}
    ${QT_INCLUDES}
    ${CMAKE_CURRENT_BINARY_DIR})

set(watch_exchange_SRCS watch_exchange.cpp main.cpp)

qt4_automoc(${watch_exchange_SRCS})
add_executable(watch_exchange ${watch_exchange_SRCS})
target_link_libraries(watch_exchange
    ${M_LIBRARIES}
    ${QT_QTCORE_LIBRARY})

install(TARGETS watch_exchange RUNTIME DESTINATION bin)
