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

#run cmake with bench on
try:
    if platform == "linux" or platform == "linux2" or platform == "darwin":
        subprocess.call(["cmake", "-S", ".", "-B", "./build", "-DBUILD_BENCHMARKS=ON"])
    elif platform == "win32":
        subprocess.call(["cmake", "-S", ".", "-B", ".\\build", "-DBUILD_BENCHMARKS=ON"])

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


#build benchmarks
if os.path.exists("benchmark"):
    os.chdir("benchmark")
else:
    quit()

try:
    if platform == "linux" or platform == "linux2" or platform == "darwin":
        subprocess.call("make")
    elif platform == "win32":
        subprocess.call(["MSBuild.exe","/p:DebugType=None","/p:Configuration=Release","endian_bench.vcxproj"])
        subprocess.call(["MSBuild.exe","/p:DebugType=None","/p:Configuration=Release","int_bench.vcxproj"])
        subprocess.call(["MSBuild.exe","/p:DebugType=None","/p:Configuration=Release","unicode_bench.vcxproj"])
        subprocess.call(["MSBuild.exe","/p:DebugType=None","/p:Configuration=Release","stream_bench.vcxproj"])
except OSError as e:
    if e.errno == errno.ENOENT:
        #program was not found
        print("failed to build benchmarks either make or MSbuild was not found in PATH or it was not installed")
        quit()
    else:
        #program output
        raise




#call test execs
try:
    if platform == "linux" or platform == "linux2" or platform == "darwin":
        subprocess.call("./stream_bench")
        subprocess.call("./unicode_bench")
        subprocess.call("./endian_bench")
        subprocess.call("./int_bench")
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
        print("failed to run a benchmark")
        quit()
    else:
        #program output
        raise
