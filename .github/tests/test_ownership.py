"""Unsigned synthetic host: actual dispatcher, listener and callback with the unchanged SDK.

Standard library only. Set OWNERSHIP_SOURCE_ROOT to compile another source tree
(the immutable baseline) with this same final harness, and OWNERSHIP_RESULTS to
write per-case outcomes as JSON.
"""

from concurrent.futures import ThreadPoolExecutor
import hashlib
import itertools
import json
import os
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[2]
SOURCE = Path(os.environ.get("OWNERSHIP_SOURCE_ROOT", ROOT)).resolve()
SDK = {
    "4DPluginAPI.c": "62f745e91d81ef2c3c901c656ea8e175d3b173e8",
    "4DPluginAPI.h": "528bf1d2552df08d6a8b1bc95011a9b72934ad5c",
    "4DPluginAPI.def": "5bbdb82fb724b6f8e307feb01d9f158b802de89d",
    "EntryPoints.h": "da428d5f6ca23b0a30371c7288a01d29aa3c466a",
    "Flags.h": "7a5a35539f27430a3fd9cb711d6b789051316471",
    "PrivateTypes.h": "a3098cb662cdbc43ba574fdef72a059c4c6d1cd4",
    "PublicTypes.h": "9ae20b001bff9128f690fff3e6c3f873b6238333",
    "Classes/C_TEXT.cpp": "45ea6432dff52b681a5c6a7cea58ad5fc15df66b",
    "Classes/C_TEXT.h": "6967390757db569ffcd3cfea8696bf14045cc748",
    "Classes/C_LONGINT.cpp": "005b83a4aec2c1466b8b219cb27ab845eb58de91",
    "Classes/C_LONGINT.h": "71e24df633aed7155cba1ffddc07d6a1dfc12fa9",
    "Classes/C_TYPES.h": "972cf3f597a8c21881a8c9aa69bc1699455be373",
}
# Exactly the Xcode Sources build phase of "Apple file promises".
SOURCES = ("4D Plugin API/4DPluginAPI.c", "4D Plugin API/Classes/C_TEXT.cpp",
           "4D Plugin API/Classes/C_LONGINT.cpp", "4DPlugin-Apple-file-promises.cpp")
FORBIDDEN = {
    "_system", "_popen", "_execve", "_execvp", "_posix_spawn", "_kill", "_fork",
    "_NSClassFromString", "_class_getInstanceMethod", "_method_setImplementation",
    "_CFURLCreateWithFileSystemPath", "_CFURLCopyFileSystemPath",
    "_AECreateDesc", "_AEDisposeDesc", "_AEDeterminePermissionToAutomateTarget",
    "_AESend", "_AESendMessage", "_OSAExecute", "_NSLog",
    "_OBJC_CLASS_$_SBApplication", "_OBJC_CLASS_$_NSFileManager",
    "_OBJC_CLASS_$_NSRunningApplication", "_OBJC_CLASS_$_NSFilePromiseReceiver",
    "_OBJC_CLASS_$_NSOperationQueue", "_OBJC_CLASS_$_NSWorkspace",
    "_OBJC_CLASS_$_NSAppleScript", "_OBJC_CLASS_$_NSTask",
}
CASES = ("listener", "stop")
BRANCHES = ("method", "command")
EMPTIES = ("allocated", "null")
RESULTS = ("undefined", "string", "picture", "longint", "error", "handle")
TEXTS = ("ASCII", "\u65e5", "\U0001f600", "")


def blob(path):
    contents = path.read_bytes()
    return hashlib.sha1(f"blob {len(contents)}\0".encode() + contents).hexdigest()


def matrix():
    return list(itertools.product(CASES, BRANCHES, EMPTIES, RESULTS, TEXTS))


@unittest.skipUnless(sys.platform == "darwin", "macOS synthetic host")
class OwnershipTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = tempfile.TemporaryDirectory(prefix="apple-file-promises-ownership-")
        cls.addClassCleanup(cls.temp.cleanup)
        cls.binary = Path(cls.temp.name) / "ownership"
        api = SOURCE / "4D Plugin API"
        for name, expected in SDK.items():
            actual = blob(api / name)
            if actual != expected:
                raise AssertionError(f"Re-audit changed SDK: {name}: {actual}")
        result = subprocess.run([
            "clang++", "-std=c++14", "-x", "objective-c++", "-fblocks",
            "-include", str(ROOT / ".github/tests/ownership_platform.h"),
            "-I", str(api), "-I", str(api / "Classes"), "-I", str(SOURCE),
            "-fsanitize=address,undefined", "-fno-sanitize=alignment",
            "-fno-omit-frame-pointer", "-g",
            "-Wno-deprecated-declarations", "-Wno-writable-strings",
            "-Wno-return-type-c-linkage", "-Wno-bridge-cast",
            *(str(SOURCE / name) for name in SOURCES),
            str(ROOT / ".github/tests/ownership_host.mm"),
            "-framework", "Cocoa", "-o", str(cls.binary),
        ], capture_output=True, text=True)
        if result.returncode:
            raise AssertionError(result.stdout + result.stderr)
        imports = subprocess.run(["nm", "-u", str(cls.binary)], capture_output=True, text=True)
        if imports.returncode:
            raise AssertionError(imports.stderr)
        linked = {line.split()[-1] for line in imports.stdout.splitlines() if line.split()}
        denied = FORBIDDEN & linked
        denied |= {n for n in linked if n.startswith(("_FSEventStream", "_OBJC_CLASS_$_SB"))}
        if denied:
            raise AssertionError(f"Real platform boundary linked: {sorted(denied)}")

        def run(case):
            name, branch, empty, kind, text = case
            done = subprocess.run(
                [str(cls.binary), name, text, empty, branch, kind],
                env={**os.environ, "ASAN_OPTIONS": "detect_leaks=0:abort_on_error=0",
                     "UBSAN_OPTIONS": "halt_on_error=1:print_stacktrace=1"},
                capture_output=True, text=True, timeout=120,
            )
            return {"name": name, "branch": branch, "empty": empty, "kind": kind, "text": text,
                    "returncode": done.returncode, "output": done.stdout + done.stderr}

        with ThreadPoolExecutor(max_workers=os.cpu_count() or 4) as pool:
            cls.rows = list(pool.map(run, matrix()))
        if os.environ.get("OWNERSHIP_RESULTS"):
            Path(os.environ["OWNERSHIP_RESULTS"]).write_text(
                json.dumps(cls.rows, ensure_ascii=False, indent=1))
        passed = sum(row["returncode"] == 0 for row in cls.rows)
        print(f"LEAF runs={len(cls.rows)} passed={passed} failed={len(cls.rows) - passed}",
              file=sys.stderr)

    def test_every_case_balances_owners_with_exact_text(self):
        self.assertEqual(len(self.rows), 192)
        for row in self.rows:
            with self.subTest(**{k: row[k] for k in ("name", "branch", "empty", "kind", "text")}):
                self.assertEqual(row["returncode"], 0, row["output"])
                self.assertIn(f"PASS {row['name']} strings=0 pictures=0 handles=0", row["output"])
                self.assertNotIn("Sanitizer", row["output"])
                self.assertNotIn("runtime error", row["output"])

    def test_isolated_fail_closed_probes(self):
        probes = {
            "probe-double-dispose": "FAIL duplicate/unowned string disposal",
            "probe-poisoned-clear": "FAIL unsupported clear kind",
            "probe-unexpected-selector": "FAIL unexpected engine selector",
        }
        for name, message in probes.items():
            with self.subTest(probe=name):
                done = subprocess.run(
                    [str(self.binary), name, "ASCII", "allocated", "method", "undefined"],
                    env={**os.environ, "ASAN_OPTIONS": "detect_leaks=0"},
                    capture_output=True, text=True, timeout=60,
                )
                self.assertEqual(done.returncode, 1, done.stdout + done.stderr)
                self.assertIn(message, done.stdout)
                self.assertNotIn("PASS", done.stdout)


if __name__ == "__main__":
    unittest.main()
