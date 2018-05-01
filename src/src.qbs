import qbs

StaticLibrary {
    name: "qmqtt2qml"

    Depends { name: "cpp" }
    Depends { name: "qmqtt" }
    Depends {
        name: "Qt"
        property var submodules: ["core"]
    }

    files: [
        "*.cpp",
        "*.hpp",
        "*.qrc",
    ]

    Export {
        Depends { name: "cpp" }
        Depends { name: "qmqtt" }
        cpp.includePaths: [product.sourceDirectory]
    }
}
