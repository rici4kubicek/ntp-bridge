#!/usr/bin/env python3

import subprocess

def extract_version():
    version_parts = subprocess.check_output(["git", "describe", "--tags", "--dirty", "--long", "--always"]).decode().strip().split('-')
    branch = subprocess.check_output(["git", "rev-parse", "--abbrev-ref", "HEAD"]).decode().strip()

    dirty = False
    if version_parts[-1] == "dirty":
        dirty = True
        version_parts.pop()

    if len(version_parts) >= 3:
        # discard abbrev
        version_parts.pop()
        build = version_parts.pop()
        version = '-'.join(version_parts)
    else:
        # no tag at all
        version = '0.0.0'
        build = subprocess.check_output(["git", "rev-list", "--all", "--count"]).decode().strip()

    # print("TAG=\"{}\"".format(tag))
    # print("AFTER={}".format(after))
    # print("DIRTY={}".format(1 if dirty else 0))
    # print("BRANCH={}".format(branch))

    dirty_text = "" if not dirty else "m"
    branch_text = "" if branch in ("master", "HEAD") else "({})".format(branch)

    return "{}.{}{}{}".format(version, build, dirty_text, branch_text)
