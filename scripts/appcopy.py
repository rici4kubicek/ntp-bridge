Import("env")

from zipfile import ZipFile, ZIP_DEFLATED
from os.path import join, basename, exists
from os import mkdir
import shutil
from git_version import extract_version

TARGETDIR = "dist/"

def after_build(source, target, env):

    version_str = extract_version()
    print(version_str)

    if not exists(TARGETDIR):
        mkdir(TARGETDIR)

    target_file = env.File(join("$BUILD_DIR", "${PROGNAME}.bin")).rstr()

    file_name = target_file.removesuffix('.bin').split('/')[-1]

    shutil.copy(target_file, TARGETDIR + f"{file_name}" + f"-{version_str}.bin")

env.AddPostAction(
    "buildprog",
    env.VerboseAction(
        after_build,
        "Copy binary"
    )
)