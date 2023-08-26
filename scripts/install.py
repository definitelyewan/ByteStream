import os
from sys import platform
import subprocess
import errno

#am i a super user?
if os.geteuid() != 0:
    print("Installation requires root")
    quit()

#build
try:
    if platform == "linux" or platform == "linux2" or "darwin":
        subprocess.call(["python", "scripts/build.py"])
    elif platform == "win32":
        print(" ");    
    
    if os.path.exists("build"):
        os.chdir("build")

    subprocess.call("cpack")

    if platform == "linux" or platform == "linux2" or "darwin":
        subprocess.call(["sh", "ByteStream-1.0.0-Linux.sh"])
    elif platform == "win32":
        print(" ");    
    

except OSError as e:
    if e.errno == errno.ENOENT:
        #program was not found
        print("unable to install")
        quit()
    else:
        #program output
        raise