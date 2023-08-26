import os
from sys import platform
import subprocess
import errno

#where are the docs
docDir = ""

if platform == "linux" or platform == "linux2" or "darwin":
    docDir = "build/test"
elif platform == "win32":
    docDir = ".\\build\\test"
else:
    quit()

if os.path.exists(docDir):
    os.chdir(docDir)

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
