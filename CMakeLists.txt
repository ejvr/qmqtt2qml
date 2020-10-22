cmake_minimum_required(VERSION 3.7)

project(qmqtt2qml VERSION 1.0 LANGUAGES CXX)

include(CPack)

set(CMAKE_CXX_STANDARD 14)

find_package(Qt6 COMPONENTS Core Qml Widgets REQUIRED)
find_package(qmqtt COMPONENTS qmqtt REQUIRED)

add_subdirectory(src)
add_subdirectory(example)

install(EXPORT ${CMAKE_PROJECT_NAME}
    NAMESPACE ${CMAKE_PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME}
    FILE ${CMAKE_PROJECT_NAME}-config.cmake)
