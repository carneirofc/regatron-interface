from conans import ConanFile


class CppStarterProject(ConanFile):
    # Note: options are copied from CMake boolean options.
    # When turned off, CMake sometimes passes them as empty strings.
    options = {
        "use_imgui": ["ON", "OFF", ""],
        "use_sdl": ["ON", "OFF", ""],
    }
    name = "RegatronInterface"
    version = "0.1"
    requires = (
        "asio/1.19.2",
        "catch2/2.13.7",
        "docopt.cpp/0.6.3",
        "fmt/8.0.1",
        "spdlog/1.9.2",
    )
    generators = ("cmake", "gcc", "txt", "cmake_find_package", "ycm")

    def requirements(self):
        if self.options.use_imgui == "ON":
            self.requires("imgui-sfml/2.1@bincrafters/stable")
        if self.options.use_sdl == "ON":
            self.requires("sdl2/2.0.10@bincrafters/stable")
