Import("env")

from zipfile import ZipFile, ZIP_DEFLATED
from os.path import join, basename, exists
from os import mkdir
import shutil

TARGETDIR = "dist/"

def after_build(source, target, env):

    if not exists(TARGETDIR):
        mkdir(TARGETDIR)

    target_file = env.File(join("$BUILD_DIR", f"firmware.bin"))

    shutil.copy(target_file.rstr(), TARGETDIR)

env.AddPostAction(
    "buildprog",
    env.VerboseAction(
        after_build,
        "Copy binary"
    )
)