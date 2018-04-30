import qbs

Application {
    name: "app"

    Depends { name: "cpp" }
    Depends { name: "qmqtt" }
    Depends {
        name: "Qt"
        property var submodules: ["core", "network", "qml", "widgets"]
    }

    files: [
        "*.cpp",
        "*.h",
        "qml.qrc",
    ]
}
