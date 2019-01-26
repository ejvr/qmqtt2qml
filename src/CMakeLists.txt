set(target_name qmqtt2qml)

include(GNUInstallDirs)

find_package(Qt5 COMPONENTS Core Network REQUIRED)

set(CMAKE_AUTOMOC ON)

file(GLOB src *.cpp)
file(GLOB public_headers *.hpp)
add_library(${target_name} SHARED ${src} ${public_headers})

set(export_header ${target_name}_export.hpp)

include(GenerateExportHeader)
generate_export_header(${target_name} EXPORT_FILE_NAME ${export_header})

target_include_directories(${target_name} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(${target_name} PUBLIC 
    $<$<STREQUAL:${target_name},${CMAKE_PROJECT_NAME}>:qmqtt::>qmqtt
    Qt5::Core)

set_target_properties(${target_name} PROPERTIES
    FRAMEWORK TRUE
    PUBLIC_HEADER "${public_headers};${CMAKE_CURRENT_BINARY_DIR}/${export_header}")

install(TARGETS ${target_name}
    EXPORT ${CMAKE_PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
