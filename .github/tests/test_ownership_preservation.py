"""Read-only immutable-history checks; standard library only."""

import json
from pathlib import Path
import subprocess
import unittest


ROOT = Path(__file__).resolve().parents[2]
BASE = "21899b389e1e30c8b7c52dbab50081a5a3c3a141"
CHANGED = {"4DPlugin-Apple-file-promises.cpp", "tests/test_native_workflow.py"}
ADDED = {
    ".github/tests/ownership-audit.md",
    ".github/tests/ownership_host.mm",
    ".github/tests/ownership_platform.h",
    ".github/tests/test_ownership.py",
    ".github/tests/test_ownership_baseline.py",
    ".github/tests/test_ownership_preservation.py",
    ".github/workflows/ownership.yml",
}


def git(*args):
    return subprocess.check_output(["git", "-C", str(ROOT), *args])


def original(path):
    return git("show", f"{BASE}:{path}")


class PreservationTests(unittest.TestCase):
    def test_all_other_baseline_files_unchanged(self):
        paths = git("ls-tree", "-r", "--name-only", "-z", BASE).decode().split("\0")
        for path in filter(None, paths):
            if path not in CHANGED:
                with self.subTest(path=path):
                    self.assertEqual((ROOT / path).read_bytes(), original(path))

    def test_only_expected_paths_added_or_changed(self):
        base = set(filter(None, git("ls-tree", "-r", "--name-only", "-z", BASE).decode().split("\0")))
        head = set(filter(None, git("ls-tree", "-r", "--name-only", "-z", "HEAD").decode().split("\0")))
        self.assertTrue(base <= head)
        self.assertTrue(head - base <= ADDED, sorted(head - base - ADDED))

    def test_complete_native_inventory(self):
        paths = git("ls-tree", "-r", "--name-only", BASE).decode().splitlines()
        actual = {p for p in paths if Path(p).suffix in (".c", ".cpp", ".mm", ".m")}
        self.assertEqual(actual, {
            "4D Plugin API/4DPluginAPI.c", "4DPlugin-Apple-file-promises.cpp",
            "support/4DPlugin-JSON.cpp",
            *("4D Plugin API/Classes/" + n + ".cpp"
              for n in ("ARRAY_TEXT", "C_BLOB", "C_LONGINT", "C_TEXT")),
        })

    def test_both_manifests_identical_to_baseline(self):
        for path in ("manifest.json",
                     "test/Plugins/Apple file promises.bundle/Contents/Resources/manifest.json"):
            manifest = json.loads((ROOT / path).read_bytes().decode("utf-8-sig"))
            self.assertEqual(manifest, json.loads(original(path).decode("utf-8-sig")))
            self.assertEqual([c["syntax"] for c in manifest["commands"]],
                             ["ACCEPT FILE PROMISES(&L;&T;&T)"])

    def test_narrow_unsigned_workflow(self):
        self.assertEqual((ROOT / ".github/workflows/ownership.yml").read_text(), "name: Ownership\n\non:\n  push:\n    branches:\n      - miyako-apple-file-promises-ownership-audit\n      - master\n    paths:\n      - '4D Plugin API/**'\n      - '4DPlugin-Apple-file-promises.*'\n      - 'Mail.h'\n      - 'Outlook.h'\n      - 'Photos.h'\n      - '.github/tests/**'\n      - '.github/workflows/ownership.yml'\n      - 'tests/test_native_workflow.py'\n  pull_request:\n    branches:\n      - master\n    paths:\n      - '4D Plugin API/**'\n      - '4DPlugin-Apple-file-promises.*'\n      - 'Mail.h'\n      - 'Outlook.h'\n      - 'Photos.h'\n      - '.github/tests/**'\n      - '.github/workflows/ownership.yml'\n      - 'tests/test_native_workflow.py'\n\npermissions:\n  contents: read\n\njobs:\n  synthetic-ownership:\n    runs-on: macos-latest\n    env:\n      PYTHONDONTWRITEBYTECODE: '1'\n    steps:\n      - uses: actions/checkout@v4\n        with:\n          ref: ${{ github.sha }}\n          fetch-depth: 0\n          persist-credentials: false\n      - name: Actual sources, unchanged SDK, synthetic boundaries, ASan and UBSan\n        run: python3 .github/tests/test_ownership.py -v\n      - name: Immutable baseline with identical final harness\n        run: python3 .github/tests/test_ownership_baseline.py -v\n      - name: Protect SDK, release automation, manifests and build memberships\n        run: python3 .github/tests/test_ownership_preservation.py -v\n")

    def test_existing_workflows_scripts_and_hooks_unchanged(self):
        for directory in (".github/workflows", ".github/scripts", ".github/hooks"):
            for path in git("ls-tree", "-r", "--name-only", BASE, directory).decode().splitlines():
                with self.subTest(path=path):
                    self.assertEqual((ROOT / path).read_bytes(), original(path))

    def test_sdk_and_build_memberships_unchanged(self):
        paths = git("ls-tree", "-r", "--name-only", BASE, "4D Plugin API").decode().splitlines()
        paths += ["Apple file promises.vcxproj", "Apple file promises.vcxproj.filters",
                  "Apple file promises.sln", "Apple file promises.xcodeproj/project.pbxproj",
                  "Apple file promises.xcodeproj/xcshareddata/xcschemes/Apple file promises.xcscheme"]
        self.assertGreater(len(paths), 20)
        for path in paths:
            with self.subTest(path=path):
                self.assertEqual((ROOT / path).read_bytes(), original(path))


if __name__ == "__main__":
    unittest.main()
