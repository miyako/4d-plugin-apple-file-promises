"""Narrow macOS cleanup and final-only output retention."""

import os
import shutil
from unittest import mock

from helper_support import Fixture, GitFixture, cwd
import native
import repository


class NativePreparationTests(Fixture):
    def test_selective_cleanup_preserves_resources_dependencies_and_default_siblings(self):
        protected = (
            native.BUNDLE / "Contents/QuickLook/Thumbnail.png",
            native.BUNDLE / "Contents/MacOS/runtime.dylib",
            native.BUNDLE / "Contents/Resources/custom.txt",
            native.BASE / "Default/Apple file promises.bundle/Contents/Info.plist",
            native.BASE / "Default/keep", native.BASE / "Other.bundle/keep",
            "build/unrelated/keep", "a/libjsoncpp.a", "lib/windows64/jsoncpp.lib",
        )
        for path in protected:
            self.write(path, b"preserved")
        for path in native.FILES:
            self.write(path, b"stale")
        for path in native.DIRECTORIES:
            self.write(path / "intermediate", b"stale")
        with cwd(self.root):
            native.prepare()
            native.prepare()
        for path in protected:
            self.assertEqual((self.root / path).read_bytes(), b"preserved")
        for path in (*native.FILES, *native.DIRECTORIES):
            self.assertFalse((self.root / path).exists(), path)

    def test_symlink_anywhere_prevalidated_before_first_deletion(self):
        roots = (*native.DIRECTORIES, native.XCODE_DATA)
        for where in (native.FILES[-1], native.BUNDLE, *roots,
                      *(directory / "nested/link" for directory in roots)):
            with self.subTest(where=where):
                first = self.write(native.FILES[0], b"must remain")
                destination = self.write("outside/keep", b"untouched")
                link = self.root / where
                if link.is_dir():
                    shutil.rmtree(link)
                link.parent.mkdir(parents=True, exist_ok=True)
                link.symlink_to(destination)
                with cwd(self.root), self.assertRaisesRegex(ValueError, "symlink"):
                    native.prepare()
                if where != native.BUNDLE:
                    self.assertEqual(first.read_bytes(), b"must remain")
                self.assertEqual(destination.read_bytes(), b"untouched")
                link.unlink()

    def test_cleanup_io_error_is_fatal(self):
        self.write(native.FILES[0], b"stale")
        with cwd(self.root), mock.patch("pathlib.Path.unlink", side_effect=PermissionError("denied")):
            with self.assertRaises(PermissionError):
                native.prepare()
        self.write(native.DIRECTORIES[0] / "stale", b"stale")
        with cwd(self.root), mock.patch("shutil.rmtree", side_effect=PermissionError("denied")):
            with self.assertRaises(PermissionError):
                native.prepare()

    def test_all_late_cleanup_target_types_prevalidated_before_any_deletion(self):
        cases = (
            (native.FILES[-1], "directory"),
            (native.FILES[-1], "fifo"),
            *((root, kind) for root in (*native.DIRECTORIES, native.XCODE_DATA)
              for kind in ("file", "fifo")),
            *((root / relative, kind)
              for root in (*native.DIRECTORIES, native.XCODE_DATA)
              for relative, kind in (("nested/special", "fifo"),
                                     ("nested/.git", "file"), ("nested/.git", "directory"))),
        )
        for relative, kind in cases:
            with self.subTest(path=relative, kind=kind):
                first = self.write(native.FILES[0], b"earlier leaf must remain byte-identical")
                late = self.root / relative
                late.parent.mkdir(parents=True, exist_ok=True)
                if kind == "directory":
                    late.mkdir()
                elif kind == "file":
                    late.write_bytes(b"wrong type or nested Git marker")
                else:
                    os.mkfifo(late)
                try:
                    with cwd(self.root), self.assertRaises(ValueError):
                        native.prepare()
                    self.assertEqual(first.read_bytes(), b"earlier leaf must remain byte-identical")
                    self.assertTrue(late.exists())
                finally:
                    if late.is_dir():
                        late.rmdir()
                    else:
                        late.unlink(missing_ok=True)

    def test_resources_must_be_fresh_exact_source_copies(self):
        for name in ("constants.xlf", "manifest.json"):
            self.write(name, b"source")
            self.write(native.BUNDLE / "Contents/Resources" / name, b"source")
        with cwd(self.root):
            native.verify_resources()
            (native.BUNDLE / "Contents/Resources/constants.xlf").write_bytes(b"stale")
            with self.assertRaisesRegex(ValueError, "differs"):
                native.verify_resources()

    def test_xcode_data_is_not_recursively_cleaned(self):
        paths = (
            "test/Plugins/XCBuildData/build.db",
            "test/Plugins/XCBuildData/unexpected-source.cpp",
            "test/Plugins/XCBuildData/evil.xcbuilddata/keep",
        )
        for path in paths:
            self.write(path, b"preserve original bytes")
        with cwd(self.root):
            native.prepare()
        for path in paths:
            self.assertEqual((self.root / path).read_bytes(), b"preserve original bytes")


class GeneratedRetentionTests(GitFixture):
    def test_actual_xcode_database_wal_and_outputs_are_final_only(self):
        for relative in (
            *native.FILES,
            "test/Plugins/XCBuildData/build.db", "test/Plugins/XCBuildData/build.db-wal",
            "test/Plugins/XCBuildData/build.db-shm",
            "test/Plugins/XCBuildData/8438e7a5fde6e4f98039a780576f6382.xcbuilddata/build-request.json",
            "test/Plugins/Apple file promises.build/Default/Apple file promises.build/Objects-normal/arm64/main.o",
            "test/Plugins/SwiftExplicitPrecompiledModules/module.pcm",
            "test/Plugins/ExplicitPrecompiledModules/module.pcm",
            "test/Plugins/EagerLinkingTBDs/Default/module.tbd",
        ):
            target = self.local / relative
            target.parent.mkdir(parents=True, exist_ok=True)
            target.write_bytes(b"fresh native output")
        with cwd(self.local):
            with self.assertRaises(ValueError):
                repository.verify_checkout(self.sha, generated=native.generated)
            repository.verify_checkout(self.sha, generated=native.generated, final=True)

    def test_evil_siblings_and_protected_resource_never_gain_final_exemption(self):
        for relative in (
            "build/XCBuildData-evil/build.db",
            "build/XCBuildData/unexpected-source.cpp",
            "build/XCBuildData/build.db-evil",
            "build/XCBuildData/evil.xcbuilddata/build-request.json",
            "build/Apple file promises.build-evil/main.o",
            "build/unexpected-source.cpp",
            "build/Apple file promises.build/main.o",
            "build/XCBuildData/build.db",
            "test/Plugins/XCBuildData-evil/build.db",
            "test/Plugins/XCBuildData/unexpected-source.cpp",
            "test/Plugins/XCBuildData/build.db-evil",
            "test/Plugins/XCBuildData/evil.xcbuilddata/build-request.json",
            "test/Plugins/Apple file promises.build-evil/keep",
            "test/Plugins/SwiftExplicitPrecompiledModules-evil/keep",
            "test/Plugins/ExplicitPrecompiledModules-evil/keep",
            "test/Plugins/EagerLinkingTBDs/Debug/keep",
            "test/Plugins/EagerLinkingTBDs/Default-evil/keep",
            "Objs/Release-evil/main.obj",
            "Objs/Debug/main.obj",
            "winbuild/nested/Apple file promises.4DX",
            "test/Plugins/Default/Apple file promises.bundle/Contents/Info.plist",
            str(native.BUNDLE / "Contents/QuickLook/Thumbnail.png"),
            str(native.BUNDLE / "Contents/MacOS/runtime.dylib"),
            str(native.BUNDLE / "Contents/Resources/custom.txt"),
            str(native.BUNDLE / "Contents/_CodeSignature/evil"),
        ):
            with self.subTest(relative=relative):
                target = self.local / relative
                target.parent.mkdir(parents=True, exist_ok=True)
                target.write_bytes(b"unexpected")
                with cwd(self.local), self.assertRaises(ValueError):
                    repository.verify_checkout(self.sha, generated=native.generated, final=True)
                target.unlink()
