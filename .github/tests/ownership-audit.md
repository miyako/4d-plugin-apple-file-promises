# Apple file promises ownership audit

## Provenance

Baseline: `21899b389e1e30c8b7c52dbab50081a5a3c3a141` on `master`.
Guidance: `miyako/skills@e6b492fcb81e5e1a7f7db723594b420e63bf6eb9`,
`4d-skills/skills/4dplugin/SKILL.md`. It was read through the GitHub API, and
the current HEAD is the same commit. The harness design is adapted from
earlier campaign audits; their results are not reused.

## Inventory and reachability

Both tracked manifests declare exactly one command:
`ACCEPT FILE PROMISES(&L;&T;&T)` (selector 1). It has no array or picture
parameters.

| Implementation | Baseline blob | Membership |
|---|---|---|
| `4DPlugin-Apple-file-promises.cpp` | `6b64e4cff6e00725d638efd44b56a0b887827ee0` | Xcode (Objective-C++, MRC). Visual Studio lists it, but the project cannot build on Windows. |
| `4D Plugin API/4DPluginAPI.c` | `62f745e91d81ef2c3c901c656ea8e175d3b173e8` | Xcode and Visual Studio |
| `Classes/C_TEXT.cpp` | `45ea6432dff52b681a5c6a7cea58ad5fc15df66b` | Xcode |
| `Classes/C_LONGINT.cpp` | `005b83a4aec2c1466b8b219cb27ab845eb58de91` | Xcode |
| `Classes/ARRAY_TEXT.cpp` | `534855e0959020ce9a66cef08f8434799c174495` | Not compiled (dormant) |
| `Classes/C_BLOB.cpp` | `27bea3172ce90e4da339991c9b318a71ea8609b2` | Not compiled (dormant) |
| `support/4DPlugin-JSON.cpp` | `a5122d68e881476b97332a6a4f40a7b3e3adeb3f` | Not compiled (dormant) |

Headers:
- Plugin header `8e34a1636593cf064f2a46ca3e9b1f21582c6fcd`
- `Mail.h` `f554e46d53ee1c311cd475699ea398d4bb76a87b`
- `Outlook.h` `660228081843d405923ef854ed6e0975f4d369dc`
- `Photos.h` `9af19e22402de60b12b0430df1cb610e10c3bbff`

The SDK blobs that are protected and unchanged are listed in `SDK` in
`test_ownership.py`. The harness verifies them before compiling.

Reachable 4D ownership paths:
1. `ACCEPT FILE PROMISES` reads its arguments through `C_LONGINT`/`C_TEXT`
   (borrowed `PA_Unistring*`, copied).
2. File-system events are queued.
3. `listenerLoop` calls `listenerLoopExecuteMethod`, which runs
   `PA_ExecuteMethodByID` when the method resolves. Otherwise it runs command
   1007 (`EXECUTE METHOD`).

`generateUuid` is dormant because `CALLBACK_IN_NEW_PROCESS` is 0. The JSON
helper is not compiled. Its `PA_CreateVariable(eVK_Unistring)` /
`eVK_Picture` then shallow-setter pattern (lines 28, 89, 106, 154, 183) has
the same shape as the bug, but it is unreachable. It is deferred and left
unchanged.

## Defect (issue #4) and fix

Baseline line numbers in `4DPluginAPI.c`:
- `PA_CreateVariable` is at line 3920. With `eVK_Unistring` it allocates an
  empty string.
- `PA_SetStringVariable` (line 4254) is a shallow overwrite, so each empty
  string leaked: 2 per method callback, 3 per `EXECUTE METHOD` fallback.
- `PA_ExecuteMethodByID` (5710) and `PA_ExecuteCommandByID` (5778) return an
  owned result. The baseline discarded it, which leaked a text or picture
  returned by the callback.

The fix:
- Starts every argument as `eVK_Undefined`. Each argument then owns exactly
  one string, which the existing `PA_ClearVariable` releases.
- Captures `PA_GetLastError()` immediately after each call.
- Clears the result only when there is no error. A failed result is
  unspecified and is not cleared.

There are no other source changes. This fix does not address issue #1
(missing Outlook pasteboard type).

## Synthetic harness

`ownership_host.mm` implements the engine selectors that the actual plugin
reaches. Any other selector fails with "unexpected engine selector".

Platform boundaries are force-included stubs (`ownership_platform.h`):
- Scripting Bridge, AppleEvents
- `NSFileManager`, `NSRunningApplication`, `NSOperationQueue`,
  `NSFilePromiseReceiver`
- CFURL paths, FSEvents
- Objective-C swizzling functions, `NSLog`

`nm -u` must show none of them. There is no real pasteboard, drag session,
Mail/Outlook/Photos, UI, network or file access.

Scenarios:
- `listener`: initialization, then `ACCEPT FILE PROMISES`, then four synthetic
  events (two delivered), then 2 callbacks.
- `stop`: the command is turned off from inside a yield, giving exactly 1
  callback, with the remaining paths cleared.

The matrix (192 cases, each in its own process, ASan+UBSan) covers:
- scenario: listener, stop
- branch: method, `EXECUTE METHOD`
- allocated vs null empties
- results: undefined, string, picture, longint, error (poisoned output),
  handle allocation failure
- texts: ASCII, BMP, supplementary, empty

Callbacks assert live, exact and distinct argument owners.

Results:
- Fixed tree: 192/192 pass with strings=0, pictures=0, handles=0.
- Baseline (`git archive` of the baseline, same harness): exactly 128 named
  "unbalanced" failures matching the formula in `test_ownership_baseline.py`,
  plus 64 controls.

Isolated probes must fail closed:
- `probe-double-dispose`: disposing after `PA_SetStringVariable`
- `probe-poisoned-clear`: clearing a failed result
- `probe-unexpected-selector`

## Gaps

- Native Windows: the project cannot build there.
- No real 4D host, Mail, Outlook, Photos or pasteboard.
- OOM and allocator contracts beyond the single handle-allocation failure.
- Packed alignment: the UBSan alignment check is disabled.
- LeakSanitizer is disabled; the harness's own owner maps are authoritative.
- Not exercised: the file-promise and Scripting Bridge export paths, and the
  `NSPasteboardItem` drag swizzles.
- Noted but not changed:
  - The `EXECUTE METHOD` fallback passes the path in its result slot.
  - The swizzle class's `dealloc` does not call `[super dealloc]`.
