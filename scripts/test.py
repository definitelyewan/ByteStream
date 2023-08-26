import os
from sys import platform
import subprocess
import errno

#where are the docs
testDir = ""

if platform == "linux" or platform == "linux2" or "darwin":
    testDir = "build/test"
elif platform == "win32":
    testDir = ".\\build\\test"
else:
    quit()

if os.path.exists(testDir):
    os.chdir(testDir)
else:
    quit()

#call test execs
try:
    if platform == "linux" or platform == "linux2" or "darwin":
        subprocess.call("./stream_test")
        subprocess.call("./unicode_test")
        subprocess.call("./endian_test")
        subprocess.call("./int_test")

except OSError as e:
    if e.errno == errno.ENOENT:
        #program was not found
        print("failed to run a test")
        quit()
    else:
        #program output
        raise
