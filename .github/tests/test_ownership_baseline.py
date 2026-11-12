"""Run the same final ownership harness against an immutable archive of the baseline."""

import io
import json
import os
from pathlib import Path
import subprocess
import sys
import tarfile
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[2]
BASELINE = "21899b389e1e30c8b7c52dbab50081a5a3c3a141"
PATHS = ("4D Plugin API", "4DPlugin-Apple-file-promises.cpp", "4DPlugin-Apple-file-promises.h",
         "Mail.h", "Outlook.h", "Photos.h")


def expected_leaks(name, branch, empty, kind):
    attempts = 1 if name == "stop" else 2
    arguments = 2 if branch == "method" else 3
    delivered = 0 if kind == "handle" else attempts
    strings = (attempts * arguments if empty == "allocated" else 0)
    strings += delivered if kind == "string" else 0
    pictures = delivered if kind == "picture" else 0
    return strings, pictures


@unittest.skipUnless(sys.platform == "darwin", "macOS synthetic host")
class BaselineOwnershipTests(unittest.TestCase):
    def test_baseline_fails_exactly_the_named_leaks(self):
        with tempfile.TemporaryDirectory(prefix="apple-file-promises-baseline-") as temp:
            archive = subprocess.run(["git", "-C", str(ROOT), "archive", BASELINE, *PATHS],
                                     capture_output=True, check=True).stdout
            with tarfile.open(fileobj=io.BytesIO(archive)) as tar:
                tar.extractall(temp, filter="data")
            results = Path(temp) / "results.json"
            done = subprocess.run(
                [sys.executable, str(ROOT / ".github/tests/test_ownership.py")],
                env={**os.environ, "OWNERSHIP_SOURCE_ROOT": temp,
                     "OWNERSHIP_RESULTS": str(results), "PYTHONDONTWRITEBYTECODE": "1"},
                capture_output=True, text=True, timeout=1800,
            )
            self.assertEqual(done.returncode, 1, done.stdout + done.stderr)
            rows = json.loads(results.read_text())
        self.assertEqual(len(rows), 192)
        failures = controls = 0
        for row in rows:
            key = {k: row[k] for k in ("name", "branch", "empty", "kind", "text")}
            strings, pictures = expected_leaks(row["name"], row["branch"], row["empty"], row["kind"])
            with self.subTest(**key):
                self.assertNotIn("Sanitizer", row["output"])
                self.assertNotIn("FAIL unexpected engine selector", row["output"])
                if strings or pictures:
                    failures += 1
                    self.assertEqual(row["returncode"], 1, row["output"])
                    self.assertIn("FAIL unbalanced", row["output"])
                    self.assertIn(f"COUNTS strings={strings} pictures={pictures} handles=0",
                                  row["output"])
                else:
                    controls += 1
                    self.assertEqual(row["returncode"], 0, row["output"])
                    self.assertIn("strings=0 pictures=0 handles=0", row["output"])
        self.assertEqual((failures, controls), (128, 64))
        print(f"BASELINE runs={len(rows)} failures={failures} controls={controls}",
              file=sys.stderr)


if __name__ == "__main__":
    unittest.main()
