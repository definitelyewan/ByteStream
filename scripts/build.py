from common.buildHelpers import *
from common.InformUser import Inform

update = Inform(1)

# mk build
update.message("Creating build dir ...")
replace_folder("build")

# build with cmake
update.message("Running Cmake")
cmake_build(".", "build", ["-DBUILD_SHARED_LIBS=ON", "-DBUILD_STREAM_DEBUG=OFF"])
    
# move to the build folder
update.message("moving to build dir")
if os.path.exists("build"):
    os.chdir("build")
else:
    quit()

update.message("Compiling")
compile_code("ByteStream")

update.message("Done!")
