import os
from sys import platform
import subprocess
import errno
import ctypes
import shutil

def isAdmin():

    try:
        is_admin = os.getuid() == 0
    except AttributeError:
        is_admin = ctypes.windll.shell32.IsUserAnAdmin() != 0

    return is_admin


#am i a super user?
if not isAdmin():
    print("Installation requires root")
    quit()

print("Creating build dir")
if not os.path.exists("build"):
    os.mkdir("build")
else:
    shutil.rmtree("build")
    os.mkdir("build")

#build with cmake
try:
    if platform == "linux" or platform == "linux2" or "darwin":
        subprocess.call(["cmake", "-S", ".", "-B", "./build", "-DBUILD_SHARED_LIBS=ON"])
    elif platform == "win32":
        subprocess.call(["cmake", "-S", ".", "-B", ".\\build", "-DBUILD_SHARED_LIBS=ON"])

except OSError as e:
    if e.errno == errno.ENOENT:
        #program was not found
        print("cmake was not found in PATH or not installed")
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
        subprocess.call("cpack")
        subprocess.call(["sh", "ByteStream-1.0.0-Linux.sh"])
    elif platform == "win32":
        subprocess.call(["MSBuild.exe","/p:DebugType=None","/p:Configuration=Release","ByteStream.vcxproj"])
        subprocess.call("cpack")

    subprocess.call("cpack")

except OSError as e:
    if e.errno == errno.ENOENT:
        #program was not found
        print("failed to build lib either make or MSBuild was not found in PATH or it was not installed")
        quit()
    else:
        #program output
        raise


#install to system

#build
# try:
#     if platform == "linux" or platform == "linux2" or "darwin":
#         subprocess.call(["python", "scripts/build.py"])
#     elif platform == "win32":
#         print(" ");    
    
#     if os.path.exists("build"):
#         os.chdir("build")

#     subprocess.call("cpack")

#     if platform == "linux" or platform == "linux2" or "darwin":
#         subprocess.call(["sh", "ByteStream-1.0.0-Linux.sh"])
#     elif platform == "win32":
#         print(" ");    
    

# except OSError as e:
#     if e.errno == errno.ENOENT:
#         #program was not found
#         print("unable to install")
#         quit()
#     else:
#         #program output
#         raise