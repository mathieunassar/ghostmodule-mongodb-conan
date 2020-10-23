from conans import ConanFile, CMake, tools


class GhostmoduleMongoDbConan(ConanFile):
    name = "ghostmodule-mongodb"
    version = "1.0"
    license = "Apache License 2.0"
    author = "Mathieu Nassar mathieu.nassar@gmail.com"
    url = "https://github.com/mathieunassar/ghostmodule-mongodb"
    description = "ghostmodule extension to integrate MongoDB."
    topics = ("framework", "microservice", "command-line", "database", "mongodb")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = "shared=False"
    generators = "cmake"

    requires = (
        "mongo-cxx-driver/3.3.0@bincrafters/stable",
        "ghostmodule/1.4@mathieunassar/stable",
        "grpc/1.20.0@inexorgame/stable",
        "protobuf/3.6.1@bincrafters/stable",
        "protoc_installer/3.6.1@bincrafters/stable",
        "gtest/1.8.1@bincrafters/stable"
    )

    def source(self):
        git = tools.Git(folder="ghostmodule-mongodb")
        git.clone("https://github.com/mathieunassar/ghostmodule-mongodb.git", "master")

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder="ghostmodule-mongodb", args=["-DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=TRUE"])
        cmake.build()

    def package(self):
        self.copy(pattern="LICENSE", dst="licenses")
        self.copy('*', dst='include', src='ghostmodule-mongodb/include')
        self.copy("*.lib", dst="lib", src="", keep_path=False)
        self.copy("*.a", dst="lib", src="", keep_path=False)
        self.copy("*", dst="bin", src="bin")
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)

    def package_info(self):
        if self.settings.build_type == "Debug":
            self.cpp_info.libs = ["ghost_persistence_mongodbd"]
        else:
            self.cpp_info.libs = ["ghost_persistence_mongodb"]
