set(exename qmqtt2qml_example)

find_package(Qt6 COMPONENTS Core Qml Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

file(GLOB src *.cpp *.hpp *.qrc *.qml)

add_executable(${exename} ${src})

target_include_directories(${exename} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${exename} PUBLIC qmqtt2qml Qt6::Core Qt6::Qml Qt6::Widgets)
