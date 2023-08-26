import os
from sys import platform
import subprocess
import errno

#where are the docs
docDir = ""

if platform == "linux" or platform == "linux2" or "darwin":
    docDir = "build/benchmark"
elif platform == "win32":
    docDir = ".\\build\\benchmark"
else:
    quit()

if os.path.exists(docDir):
    os.chdir(docDir)

#call test execs
try:
    if platform == "linux" or platform == "linux2" or "darwin":
        subprocess.call("./stream_bench")
        subprocess.call("./unicode_bench")
        subprocess.call("./endian_bench")
        subprocess.call("./int_bench")

except OSError as e:
    if e.errno == errno.ENOENT:
        #program was not found
        print("failed to run a benchmark")
        quit()
    else:
        #program output
        raise
