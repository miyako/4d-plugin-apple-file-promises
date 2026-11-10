"""The final publication exemption is smaller than the bundle or build roots."""

import json
import unittest

from helper_support import ROOT, source
import native


class GeneratedOutputTests(unittest.TestCase):
    def test_exact_hosted_evidence_roots_and_binding_not_inferred_from_stubs(self):
        self.assertEqual(str(native.XCODE_DATA), "test/Plugins/XCBuildData")
        self.assertEqual(tuple(map(str, native.DIRECTORIES)), (
            "test/Plugins/Apple file promises.build",
            "test/Plugins/SwiftExplicitPrecompiledModules",
            "test/Plugins/ExplicitPrecompiledModules",
            "test/Plugins/EagerLinkingTBDs/Default",
        ))
        evidence = json.loads((ROOT / ".github/native-evidence.json").read_text())
        self.assertEqual((evidence["run_id"], evidence["job_id"], evidence["head_sha"]),
                         (36969739358, 110721037879,
                          "a3934bf886188e4d08cab3936df59ad0ae3657ed"))
        exact, interval, binding = evidence["artifacts"]
        self.assertEqual(exact["lines"], 928)
        self.assertEqual(exact["sha256"],
                         "79433992ae865d21c8544eefcae0eee2b20271652bc1967d601fb4d7c7a7fe6c")
        self.assertEqual(interval["lines"], 950)
        self.assertNotEqual(exact["sha256"], interval["sha256"])
        self.assertIn("job-step.json", binding["session_path"])

    def test_exact_native_files_and_controlled_directory_descendants(self):
        expected = (
            "test/Plugins/Apple file promises.bundle/Contents/MacOS/Apple file promises",
            "test/Plugins/Apple file promises.bundle/Contents/Info.plist",
            "test/Plugins/Apple file promises.bundle/Contents/Resources/English.lproj/InfoPlist.strings",
            "test/Plugins/Apple file promises.bundle/Contents/_CodeSignature/CodeResources",
            "test/Plugins/Apple file promises.bundle/Contents/Resources/constants.xlf",
            "test/Plugins/Apple file promises.bundle/Contents/Resources/manifest.json",
            "test/Plugins/XCBuildData/build.db",
            "test/Plugins/XCBuildData/build.db-wal",
            "test/Plugins/XCBuildData/build.db-shm",
            "test/Plugins/XCBuildData/8438e7a5fde6e4f98039a780576f6382.xcbuilddata/build-request.json",
            "test/Plugins/Apple file promises.build/Default/Apple file promises.build/Objects-normal/arm64/main.o",
            "test/Plugins/SwiftExplicitPrecompiledModules/module.pcm",
            "test/Plugins/ExplicitPrecompiledModules/module.pcm",
            "test/Plugins/EagerLinkingTBDs/Default/module.tbd",
        )
        for path in expected:
            with self.subTest(path=path):
                self.assertTrue(native.generated(path))

    def test_every_resource_source_or_unproven_output_stays_protected(self):
        excluded = (
            "build/Default/Apple file promises.build/build.db",
            "build/Apple file promises.build/Objects/file.o",
            "test/Plugins/Default/Apple file promises.build/Objects/file.o",
            "test/Plugins/Apple file promises.bundle/Contents/Windows/Apple file promises.4DX",
            "test/Plugins/Apple file promises.bundle/Contents/Windows64/Apple file promises.4DX",
            "test/Plugins/Apple file promises.bundle/Contents/MacOS/runtime.dylib",
            "test/Plugins/Apple file promises.bundle/Contents/QuickLook/Thumbnail.png",
            "test/Plugins/Default/Apple file promises.bundle/Contents/Info.plist",
            "test/Plugins/Default/keep", "build/XCBuildData-evil/build.db",
            "build/unexpected-source.cpp",
            "build/XCBuildData/unexpected-source.cpp",
            "build/XCBuildData/build.db-evil",
            "build/XCBuildData/evil.xcbuilddata/build-request.json",
            "build/Apple file promises.build-evil/injected.cpp",
            "test/Plugins/XCBuildData-evil/build.db",
            "test/Plugins/XCBuildData/unexpected-source.cpp",
            "test/Plugins/XCBuildData/build.db-evil",
            "test/Plugins/XCBuildData/evil.xcbuilddata/build-request.json",
            "test/Plugins/XCBuildData/8438E7A5FDE6E4F98039A780576F6382.xcbuilddata/request",
            "test/Plugins/Apple file promises.build-evil/injected.cpp",
            "test/Plugins/SwiftExplicitPrecompiledModules-evil/keep",
            "test/Plugins/ExplicitPrecompiledModules-evil/keep",
            "test/Plugins/EagerLinkingTBDs/Debug/keep",
            "test/Plugins/EagerLinkingTBDs/Default-evil/keep",
            "test/Plugins/EagerLinkingTBDs/keep",
            "test/Plugins/unexpected-source.cpp",
            "test/Plugins/Apple file promises.bundle/Contents/_CodeSignature-evil/file",
            "test/Plugins/Apple file promises.bundle/Contents/Info.plist.backup",
            *map(str, source.SOURCE_PATHS),
        )
        for path in excluded:
            with self.subTest(path=path):
                self.assertFalse(native.generated(path))
