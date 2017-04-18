cmake_minimum_required(VERSION 3.0)
project(watch_exchange VERSION 1.1)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 REQUIRED
    COMPONENTS Core Network)

include(FindPkgConfig)
pkg_check_modules(M REQUIRED qamqp-qt5)

set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
set(CMAKE_C_FLAGS_DEBUG
    "${CMAKE_C_FLAGS_DEBUG}   -Wall -DDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG
    "${CMAKE_CXX_FLAGS_DEBUG} -Wall -DDEBUG")

include_directories(
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Network_INCLUDE_DIRS}
    ${M_INCLUDE_DIRS})

set(watch_exchange_SRCS watch_exchange.cpp main.cpp)

add_executable(watch_exchange ${watch_exchange_SRCS})
target_link_libraries(watch_exchange
    Qt5::Core Qt5::Network
    ${M_LIBRARIES})

install(TARGETS watch_exchange RUNTIME DESTINATION bin)

set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_GENERATOR "RPM")
set(CPACK_PACKAGE_NAME "watch-exchange")
set(CPACK_PACKAGE_RELEASE 1)
set(CPACK_PACKAGE_CONTACT "Serguei Khvatov")
set(CPACK_PACKAGE_VENDOR "RUTOLL")
set(CPACK_PACKAGING_INSTALL_PREFIX /usr)
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_RELEASE}.${CMAKE_SYSTEM_PROCESSOR}")
include(CPack)
