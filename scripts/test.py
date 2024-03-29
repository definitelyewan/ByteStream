from common.buildHelpers import *
from common.InformUser import *

update = Inform(1)

# mk build
update.message("Creating build dir ..")
replace_folder("build")

# build with cmake
update.message("Running Cmake")
cmake_build(".", "build", ["-DBUILD_TESTS=ON", "-DBUILD_SHARED_LIBS=ON","-DBUILD_STREAM_DEBUG=ON"])

# move to build
update.message("Moving to build dir")
if os.path.exists("build"):
    os.chdir("build")
else:
    quit()

# compile the code
update.message("Compiling libraries")
compile_code("ByteStream")

# move to tests
update.message("Moving to tests")
if os.path.exists("test"):
    os.chdir("test")
else:
    quit()

# compile code
update.message("Compiling byteEndian_test program")
compile_code("endian_test")

update.message("Compiling int_test program")
compile_code("int_test")

update.message("Compiling unicode_test program")
compile_code("unicode_test")

update.message("Compiling stream_test program")
compile_code("stream_test")



# call test execs
try:
    if platform == "linux" or platform == "linux2":
        
        if(is_command("valgrind")):
            subprocess.call(["valgrind", "--leak-check=full", "--show-leak-kinds=all", "./endian_test"])
            subprocess.call(["valgrind", "--leak-check=full", "--show-leak-kinds=all", "./int_test"])
            subprocess.call(["valgrind", "--leak-check=full", "--show-leak-kinds=all", "./unicode_test"])
            subprocess.call(["valgrind", "--leak-check=full", "--show-leak-kinds=all", "./stream_test"])
        else:
            subprocess.call(["./endian_test"])
            subprocess.call(["./int_test"])
            subprocess.call(["./unicode_test"])
            subprocess.call(["./stream_test"])
        
    elif platform == "darwin":

        if(is_command("leaks")):
            
            malloc_stack_logging = False

            if(os.environ.get("MallocStackLogging") != "0"):
                malloc_stack_logging = True

            # will get changed back but just in case it needs to be set
            os.environ["MallocStackLogging"] = "1"

            subprocess.call(["leaks", "--atExit", "--list", "--", "./endian_test"])
            subprocess.call(["leaks", "--atExit", "--list", "--", "./int_test"])
            subprocess.call(["leaks", "--atExit", "--list", "--", "./unicode_test"])
            subprocess.call(["leaks", "--atExit", "--list", "--", "./stream_test"])

            if(malloc_stack_logging == False):
                os.environ["MallocStackLogging"] = "0"
        else:
            subprocess.call(["./endian_test"])
            subprocess.call(["./int_test"])
            subprocess.call(["./unicode_test"])
            subprocess.call(["./stream_test"])



    elif platform == "win32":
        subprocess.call("endian_test.exe")
        subprocess.call("int_test.exe")
        subprocess.call("unicode_test.exe")
        subprocess.call("stream_test.exe")
        
except OSError as e:
    if e.errno == errno.ENOENT:
        # program was not found
        print("failed to run a test")
        quit()
    else:
        # program output
        raise

update.message("Done!")
