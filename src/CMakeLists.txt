set(libname qmqtt2qml)

find_package(Qt5 COMPONENTS Core Network REQUIRED)
find_package(qmqtt COMPONENTS qmqtt REQUIRED)

set(CMAKE_AUTOMOC ON)

file(GLOB src *.cpp)
file(GLOB public_headers *.hpp)
add_library(${CMAKE_PROJECT_NAME} SHARED ${src} ${public_headers})

set(export_header ${CMAKE_PROJECT_NAME}_export.hpp)

include(GenerateExportHeader)
generate_export_header(${CMAKE_PROJECT_NAME} EXPORT_FILE_NAME ${export_header})

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries(${libname} PUBLIC qmqtt::qmqtt Qt5::Core)

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
    FRAMEWORK TRUE
    PUBLIC_HEADER "${public_headers};${CMAKE_CURRENT_BINARY_DIR}/${export_header}")

install(TARGETS ${CMAKE_PROJECT_NAME}
    EXPORT ${CMAKE_PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
