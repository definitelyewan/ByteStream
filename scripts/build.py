import os
from sys import platform
import subprocess
import errno

buildDir = "build"

#does the folder exist?
if not os.path.exists(buildDir):
    os.mkdir(buildDir)


#build with cmake
try:
    if platform == "linux" or platform == "linux2" or "darwin":
        subprocess.call(["cmake", "-S", ".", "-B", "./" + buildDir, "-DBUILD_SHARED_LIBS=ON", "-DBUILD_TESTS=ON", "-DBUILD_BENCHMARKS=ON"])
    elif platform == "win32":
        subprocess.call(["cmake", "-S", ".", "-B", ".\\" + buildDir, "-DBUILD_SHARED_LIBS=ON", "-DBUILD_TESTS=ON", "-DBUILD_BENCHMARKS=ON"])

except OSError as e:
    if e.errno == errno.ENOENT:
        #program was not found
        print("cmake was not found or not installed")
        quit()
    else:
        #program output
        raise


#run make 
os.chdir(buildDir)
try:
    subprocess.call("make")
except OSError as e:
    if e.errno == errno.ENOENT:
        #program was not found
        print("make was not found or not installed")
        quit()
    else:
        #program output
        raise
