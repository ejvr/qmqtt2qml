import qbs

Application {
    name: "example"

    Depends { name: "cpp" }
    Depends { name: "qmqtt2qml" }
    Depends {
        name: "Qt"
        property var submodules: ["core", "qml", "widgets"]
    }

    files: [
        "*.cpp",
        "*.hpp",
        "*.qrc",
    ]
}
