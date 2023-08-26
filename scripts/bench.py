import os
from sys import platform
import subprocess
import errno

#where are the benchs
benchDir = ""

if platform == "linux" or platform == "linux2" or "darwin":
    benchDir = "build/benchmark"
elif platform == "win32":
    benchDir = ".\\build\\benchmark"
else:
    quit()

if os.path.exists(benchDir):
    os.chdir(benchDir)
else:
    quit()

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
