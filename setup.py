import os
import sys
import platform
import subprocess
from setuptools import setup, Extension, find_packages
from setuptools.command.build_ext import build_ext


class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=""):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):
    def run(self):
        try:
            subprocess.check_output(["cmake", "--version"])
        except OSError:
            raise RuntimeError(
                "CMake must be installed to build the extension"
            )

        for ext in self.extensions:
            self.build_extension(ext)

    def build_extension(self, ext):
        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
        
        # Required for auto-detection of auxiliary "native" libs
        if not extdir.endswith(os.path.sep):
            extdir += os.path.sep
        
        cmake_args = [
            f"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={extdir}",
            f"-DPYTHON_EXECUTABLE={sys.executable}"
        ]
        
        cfg = "Debug" if self.debug else "Release"
        build_args = ["--config", cfg]
        
        if platform.system() == "Windows":
            cmake_args += [f"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{cfg.upper()}={extdir}"]
            # Assuming you use MSVC
            cmake_args += ["-DCMAKE_GENERATOR=Visual Studio 16 2019"]
            build_args += ["--", "/m"]
        else:
            cmake_args += [f"-DCMAKE_BUILD_TYPE={cfg}"]
            build_args += ["--", "-j4"]
        
        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)
            
        # Build in cpp directory
        sourcedir = os.path.join(ext.sourcedir, "cpp")
        
        subprocess.check_call(
            ["cmake", sourcedir] + cmake_args, cwd=self.build_temp
        )
        subprocess.check_call(
            ["cmake", "--build", "."] + build_args, cwd=self.build_temp
        )


setup(
    name="pydungeon",
    version="0.1.0",
    author="Eugene Leontiev",
    author_email="eleonti.it@gmail.com",
    description="A Python-based procedural dungeon generator with C++ core",
    long_description=open("README.md").read(),
    long_description_content_type="text/markdown",
    packages=find_packages(),
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
    ],
    python_requires=">=3.6",
    install_requires=[
        "pygame>=2.0.0",
        "numpy>=1.19.0",
        "pybind11>=2.6.0",
    ],
    ext_modules=[CMakeExtension("pydungeon_cpp")],
    cmdclass={"build_ext": CMakeBuild},
)
