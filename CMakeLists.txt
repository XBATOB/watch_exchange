cmake_minimum_required(VERSION 2.6)
project(watch_exchange)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(watch_exchange_SRCS watch_exchange.cpp main.cpp)

qt4_automoc(${watch_exchange_SRCS})
add_executable(watch_exchange ${watch_exchange_SRCS})
target_link_libraries(watch_exchange ${QT_QTCORE_LIBRARY})

install(TARGETS watch_exchange RUNTIME DESTINATION bin)
