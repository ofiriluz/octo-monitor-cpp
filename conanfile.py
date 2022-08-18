from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout
from conan.tools.build import check_min_cppstd
from conan.errors import ConanInvalidConfiguration
from conan.tools.scm import Version

class OctoPreloaderCPPConan(ConanFile):
    name = "octo-preloader-cpp"
    version = "1.0.0"
    url = "https://github.com/ofiriluz/octo-preloader-cpp"
    author = "Ofir Iluz"
    settings = "os", "compiler", "build_type", "arch"

    @property
    def _compilers_minimum_version(self):
        return {
            "gcc": "8",
            "clang": "9",
            "apple-clang": "11",
            "Visual Studio": "16",
        }

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()
        cd = CMakeDeps(self)
        cd.generate()

    def layout(self):
        cmake_layout(self)

    def validate(self):
        if self.info.settings.os not in ["Linux", "Macos"]:
            raise ConanInvalidConfiguration(
                f"{self.name} is only supported in Linux / Macos distributions."
            )
        if self.info.settings.compiler.cppstd:
            check_min_cppstd(self, "17")

        minimum_version = self._compilers_minimum_version.get(str(self.info.settings.compiler), False)
        if minimum_version and Version(self.info.settings.compiler.version) < minimum_version:
            raise ConanInvalidConfiguration(
                f"{self.name} requires C++17, which your compiler does not support."
            )
        else:
            self.output.warn(f"{self.name} requires C++17. Your compiler is unknown. Assuming it supports C++17.")

    def requirements(self):
        self.requires("octo-logger-cpp/1.1.0")
        self.requires("nlohmann_json/3.11.2")
        self.requires("stduuid/1.2.2")

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.set_property("cmake_file_name", "octo-preloader-cpp")
        self.cpp_info.set_property("cmake_target_name", "octo::octo-preloader-cpp")
        self.cpp_info.set_property("pkg_config_name", "octo-preloader-cpp")
        self.cpp_info.components["libocto-preloader-cpp"].libs = ["octo-preloader-cpp"]
        self.cpp_info.components["libocto-preloader-cpp"].requires = ["octo::octo-logger-cpp", "nlohmann_json::nlohmann_json", "stduuid::stduuid"]
        self.cpp_info.filenames["cmake_find_package"] = "octo-preloader-cpp"
        self.cpp_info.filenames["cmake_find_package_multi"] = "octo-preloader-cpp"
        self.cpp_info.names["cmake_find_package"] = "octo-preloader-cpp"
        self.cpp_info.names["cmake_find_package_multi"] = "octo-preloader-cpp"
        self.cpp_info.names["pkg_config"] = "octo-preloader-cpp"
        self.cpp_info.components["libocto-preloader-cpp"].names["cmake_find_package"] = "octo-preloader-cpp"
        self.cpp_info.components["libocto-preloader-cpp"].names["cmake_find_package_multi"] = "octo-preloader-cpp"
        self.cpp_info.components["libocto-preloader-cpp"].set_property("cmake_target_name", "octo::octo-preloader-cpp")
        self.cpp_info.components["libocto-preloader-cpp"].set_property("pkg_config_name", "octo-preloader-cpp")
