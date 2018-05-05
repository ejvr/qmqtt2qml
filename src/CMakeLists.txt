set(libname qmqtt2qml)

find_package(Qt5 COMPONENTS Core REQUIRED)

set(CMAKE_AUTOMOC ON)

file(GLOB src *.cpp *.hpp)

add_library(${libname} ${src})

if (BUILD_SHARED_LIBS)
    target_compile_definitions(${libname} PUBLIC -DQMQTT2QML_SHARED)
    target_compile_definitions(${libname} PRIVATE -DQMQTT2QML_SHARED_EXPORT)
endif (BUILD_SHARED_LIBS)

target_include_directories(${libname} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${libname} PUBLIC qmqtt Qt5::Core)
