"""Rebuild only proven native outputs; never discard packaged siblings."""

import argparse
from pathlib import Path
import re
import shutil
import stat


PRODUCT = "Apple file promises"
BASE = Path("test/Plugins")
BUNDLE = BASE / (PRODUCT + ".bundle")
FILES = tuple(BUNDLE / relative for relative in (
    f"Contents/MacOS/{PRODUCT}", "Contents/Info.plist",
    "Contents/Resources/English.lproj/InfoPlist.strings",
    "Contents/Resources/constants.xlf", "Contents/Resources/manifest.json",
    "Contents/_CodeSignature/CodeResources",
))
XCODE_DATA = BASE / "XCBuildData"
DIRECTORIES = (
    BASE / (PRODUCT + ".build"),
    BASE / "SwiftExplicitPrecompiledModules",
    BASE / "ExplicitPrecompiledModules",
    BASE / "EagerLinkingTBDs/Default",
)
PRESERVED = tuple(BUNDLE / relative for relative in (
    "Contents/QuickLook/Thumbnail.png",
    f"Contents/Windows64/{PRODUCT}.4DX",
))
PACKAGED = (*FILES, *PRESERVED)


def generated(path):
    return (path in set(map(str, FILES))
            or bool(re.fullmatch(
                r"test/Plugins/XCBuildData/(?:build\.db(?:-wal|-shm)?|"
                r"[0-9a-f]{32}\.xcbuilddata/.+)", path))
            or any(path.startswith(str(directory) + "/")
                   for directory in DIRECTORIES))


def validate_paths(paths):
    root = Path.cwd().resolve()
    for path in paths:
        for parent in (path, *path.parents):
            if parent == Path("."):
                break
            if parent.is_symlink():
                raise ValueError(f"Refusing symlink output: {parent}")
        if not path.resolve().is_relative_to(root):
            raise ValueError(f"Output escapes checkout: {path}")
        if path.is_dir():
            for descendant in path.rglob("*"):
                if descendant.is_symlink():
                    raise ValueError(f"Refusing symlink output: {descendant}")


def validate_cleanup_targets():
    files = FILES
    directories = DIRECTORIES
    validate_paths((*files, *directories, XCODE_DATA))
    for path in (*files, *directories, XCODE_DATA):
        try:
            mode = path.lstat().st_mode
        except FileNotFoundError:
            continue
        expected = stat.S_ISREG if path in files else stat.S_ISDIR
        if not expected(mode):
            raise ValueError(f"Unexpected cleanup target type: {path}")
    for directory in (*directories, XCODE_DATA):
        for descendant in directory.rglob("*"):
            mode = descendant.lstat().st_mode
            if descendant.name == ".git" or not (stat.S_ISREG(mode) or stat.S_ISDIR(mode)):
                raise ValueError(f"Unsupported cleanup descendant: {descendant}")


def verify_intermediate_sources(sha):
    import repository
    roots = tuple(map(str, (*DIRECTORIES, XCODE_DATA)))
    if repository.run("git", "ls-tree", "-r", "-z", sha, "--", *roots) \
            or repository.run("git", "ls-files", "--stage", "-z", "--", *roots):
        raise ValueError("Tracked source/index must not occupy cleanup intermediate roots")


def prepare():
    files = FILES
    directories = DIRECTORIES
    validate_cleanup_targets()
    for path in files:
        path.unlink(missing_ok=True)
    for path in directories:
        if path.exists():
            shutil.rmtree(path)


def require_file(path):
    validate_paths((path,))
    if not path.is_file() or path.stat().st_size == 0:
        raise ValueError(f"Missing or empty native output: {path}")


def verify_resources():
    for name in ("manifest.json", "constants.xlf"):
        copied = BUNDLE / "Contents/Resources" / name
        require_file(copied)
        if copied.read_bytes() != Path(name).read_bytes():
            raise ValueError(f"Copied resource differs from source: {copied}")


def validate_unsigned():
    for path in FILES[:-1]:
        require_file(path)
    verify_resources()


def validate_final(sha):
    import repository
    validate_cleanup_targets()
    for path in PACKAGED:
        require_file(path)
    selected = repository.entries(repository.run("git", "ls-tree", "-r", "-z", sha, "--"))
    for path in PRESERVED:
        if not repository.matches_blob(path, selected[str(path)]):
            raise ValueError(f"Preserved packaged input differs from selected source: {path}")
    verify_resources()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("phase", choices=("clean-macos", "verify-resources"))
    phase = parser.parse_args().phase
    if phase == "clean-macos":
        prepare()
    else:
        validate_unsigned()
