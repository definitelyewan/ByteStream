import os
from sys import platform
import subprocess
import errno
import shutil

#mk build
print("Creating build dir")
if not os.path.exists("build"):
    os.mkdir("build")
else:
    shutil.rmtree("build")
    os.mkdir("build")

#build with cmake
try:
    if platform == "linux" or platform == "linux2" or "darwin":
        subprocess.call(["cmake", "-S", ".", "-B", "./build", "-DBUILD_TESTS=ON"])
    elif platform == "win32":
        subprocess.call(["cmake", "-S", ".", "-B", ".\\build", "-DBUILD_TESTS=ON"])

except OSError as e:
    if e.errno == errno.ENOENT:
        #program was not found
        print("cmake was not found or not installed")
        quit()
    else:
        #program output
        raise


#build lib
if os.path.exists("build"):
    os.chdir("build")
else:
    quit()

try:
    if platform == "linux" or platform == "linux2" or platform == "darwin":
        subprocess.call("make")
    elif platform == "win32":
        subprocess.call(["MSBuild.exe","ByteStream.vcxproj"])

except OSError as e:
    if e.errno == errno.ENOENT:
        #program was not found
        print("failed to build benchmarks either make or MSBuild was not found in PATH or it was not installed")
        quit()
    else:
        #program output
        raise

#build tests
if os.path.exists("test"):
    os.chdir("test")
else:
    quit()

try:
    if platform == "linux" or platform == "linux2" or platform == "darwin":
        subprocess.call("make")
    elif platform == "win32":
        subprocess.call(["MSBuild.exe","/p:DebugType=None","/p:Configuration=Release","endian_test.vcxproj"])
        subprocess.call(["MSBuild.exe","/p:DebugType=None","/p:Configuration=Release","int_test.vcxproj"])
        subprocess.call(["MSBuild.exe","/p:DebugType=None","/p:Configuration=Release","unicode_test.vcxproj"])
        subprocess.call(["MSBuild.exe","/p:DebugType=None","/p:Configuration=Release","stream_test.vcxproj"])
except OSError as e:
    if e.errno == errno.ENOENT:
        #program was not found
        print("failed to build tests either make or MSbuild was not found in PATH or it was not installed")
        quit()
    else:
        #program output
        raise


#call test execs
try:
    if platform == "linux" or platform == "linux2" or "darwin":
        subprocess.call("./stream_test")
        subprocess.call("./unicode_test")
        subprocess.call("./endian_test")
        subprocess.call("./int_test")
    elif platform == "win32":
        if os.path.exists("Release"):
            os.chdir("Release")

        subprocess.call(".\\endian_bench.exe")
        subprocess.call(".\\int_bench.exe")
        subprocess.call(".\\unicode_bench.exe")
        subprocess.call(".\\stream_bench.exe")
        
except OSError as e:
    if e.errno == errno.ENOENT:
        #program was not found
        print("failed to run a test")
        quit()
    else:
        #program output
        raise
