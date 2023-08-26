import os
from sys import platform
import subprocess
import errno
import webbrowser


#where are the docs
docDir = ""

if platform == "linux" or platform == "linux2" or "darwin":
    docDir = "build/docs"
elif platform == "win32":
    docDir = ".\\build\\doc"
else:
    quit()

if os.path.exists(docDir):
    os.chdir(docDir)

#call doxygen
try:
    subprocess.call(["doxygen", "Doxyfile.docs"])
except OSError as e:
    if e.errno == errno.ENOENT:
        #program was not found
        print("cannot find doxygen or doxygen is not installed")
        quit()
    else:
        #program output
        raise

#open the doc
if platform == "linux" or platform == "linux2" or platform == "darwin":
    webbrowser.open_new_tab("html/index.html") 
elif platform == "win32":
    webbrowser.open_new_tab(".\\html\\index.html")