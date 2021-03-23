from conans import ConanFile, CMake, tools

class ProtobufPlayground(ConanFile):
    name        = "protobuf-playground"
    version     = "0.0.1"
    generators  = "cmake_find_package", "cmake_paths"
    settings = "os", "compiler", "build_type", "arch"

    def requirements(self):
        self.requires("protobuf/3.15.5")

    def configure(self):
        self.options["protobuf" ].with_zlib = False
        self.options["protobuf" ].with_rtti = False
        self.options["protobuf" ].lite = True
        self.options["protobuf" ].shared = False

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
