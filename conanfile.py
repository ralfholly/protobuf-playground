from conans import ConanFile, CMake, tools

class ProtobufPlayground(ConanFile):
    name        = "protobuf-playground"
    version     = "0.0.1"
    generators  = "cmake_find_package", "cmake_paths"
    def requirements(self):
        self.requires("protobuf/3.15.5")
        settings = "os", "compiler", "build_type", "arch"

    def configure(self):
        pass

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        pass

    def package_info(self):
        pass

    def imports(self):
        pass

    def build_directory(self):
        pass
