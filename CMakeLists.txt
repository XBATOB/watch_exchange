cmake_minimum_required(VERSION 3.0)
project(watch_exchange VERSION 1.0)
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

set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_GENERATOR "RPM")
set(CPACK_PACKAGE_NAME "watch-exchange")
set(CPACK_PACKAGE_RELEASE 1)
set(CPACK_PACKAGE_CONTACT "Serguei Khvatov")
set(CPACK_PACKAGE_VENDOR "RUTOLL")
set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_RELEASE}.${CMAKE_SYSTEM_PROCESSOR}")
include(CPack)
