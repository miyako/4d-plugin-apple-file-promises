# Manual source maintenance and publication

The two workflows are independent:

* **Bump version** takes one required `patch` / `minor` / `major` choice
  (default `patch`). It validates the immutable selected branch SHA, creates
  one direct source-only child with Actions bot author and committer, and
  performs one nonforce, no-follow-tags branch update. It never builds, tags,
  releases or dispatches another workflow. Detached checkouts need no ambient
  Git identity. A committed executable `.github/hooks/pre-push` is mandatory;
  its exact raw blob and mode are checked before editing and before pushing.
  The hook checks the actual advertised old SHA, not only an earlier read.
* **Build** has no custom inputs. Select a ref with GitHub's built-in selector.
  All checkouts use immutable `github.sha`. Source determines canonical
  `X.Y.Z` and the new `vX.Y.Z` tag. Publication is serialized across refs by a
  constant concurrency group with cancellation disabled (not a FIFO guarantee).
  There is no default-branch-only restriction.

## Authority and intake repair

The introducing Bump header at
`bc48174dbceaa54f4a90ce3f317e1a4ac3d1055e` explicitly coordinates
`MARKETING_VERSION`, `CURRENT_PROJECT_VERSION` and plist versions; its
marketing-only body was inconsistent with that contract. At immutable intake
`a3934bf886188e4d08cab3936df59ad0ae3657ed`, all six marketing values were
**4.1.3**. The migration does **not** increment it. It narrowly aligns the six
stale current values and the two copied source English display tokens to
4.1.3. The plist retains `$(MARKETING_VERSION)` and its absent build key;
the source strings retain their CR-only encoding. No prebuilt metadata is
normalized. Future bumps validate and co-bump the copied source localization.

The helper validates the six exact Development/Deployment/Default target and
project configuration identities, lexical duplicate assignments, canonical
versions, references/cycles and XML/localized metadata before any writes.
Edits preserve original token spans, encoding, individual line endings,
file modes and unrelated values.

## Native policy and retained outputs

Build retains the original Ubuntu version/macOS native topology. Despite the
old Windows header, there was no Windows build job. The tracked Windows64
payload and QuickLook thumbnail remain exact selected-source bytes.
The original Default universal Xcode invocation, deployment target 11.0,
project output settings and Deployment-only shell phase are preserved.
No configuration-output override, signing-disable flag, dependency clone,
runtime library rule or legacy-phase bypass is introduced.

Cleanup removes only six exact generated bundle leaves and the project's
`build/Apple file promises.build` intermediates, after checking every cleanup
path and descendant for symlinks. It never removes whole bundles, resources
siblings, prebuilt Windows inputs or Default siblings.
All existing cleanup leaves must be regular files, and intermediate/XCBuildData
roots must be directories. Every target's type is checked, and special-file
descendants and nested `.git` markers are rejected before the first deletion, so a late invalid
target cannot destroy earlier valid outputs.
Unsigned validation precedes signing; final publication separately requires
all eight packaged leaves to be nonempty regular nonsymlink files, exact
source resources, matching built XML/binary/localized metadata, and unchanged
Windows/QuickLook bytes.
Universal architecture validation requires exact arm64 and x86_64 tokens,
without introducing an extra-architecture ban. Signing is strict; both
notarytool exit success and strict JSON `Accepted` are required for both
artifacts, followed by stapling and strict signature verification.

Initial checkout validation is globally clean, including ignored files.
Final validation retains real native-produced tracked bytes, checks immutable
HEAD, exact index and all nongenerated raw bytes independently, and rejects
both `ls-files -v` and `-f` hidden/stat flags. Final exemptions cover only the
exact generated leaves, project intermediates, exact `build/XCBuildData`
database/WAL/SHM names and lowercase 32-hex `.xcbuilddata` descendants.
Unexpected ignored source, database-like siblings and source/config changes
fail before API access. Generated files are never reset to fake cleanliness.

## Create-only publication and manual recovery

Explicit nonempty `GH_TOKEN`, exact canonical
`miyako/4d-plugin-apple-file-promises`, and matching `GITHUB_SHA` are required.
Availability is checked before native work and after all native operations:
draft-inclusive release pages are read until an empty page, and exact API
and remote tag absence must agree. Authentication, transport, later-page,
duplicate-key, nonfinite-number and malformed-schema errors fail closed.

Publication separately creates a tag ref at the selected SHA, verifies it
through API and Git, creates a **new** public nonprerelease release, then
uploads only to its new positive ID. Original notes policy remains empty
body with generated notes disabled. Local filenames retain spaces;
GitHub's remote asset names replace spaces with dots.
No update, clobber, DELETE, PATCH, reuse, retry or rollback is performed.

On failure, inspect the reported retained tag, release ID and confirmed or
possibly uploaded assets manually. Partial state is intentionally retained.
Do not rerun blindly: even a tag at the same SHA is occupied and rejected.
Any deliberate recovery is a separate operator decision, not automation.

## Offline validation

Run `PYTHONDONTWRITEBYTECODE=1 python3 -m unittest discover -s tests -v`
(Python 3 and PyYAML) and `actionlint .github/workflows/*.yml`.
Tests execute target helpers and real local bare Git fixtures only; extracted
native workflow Bash uses a closed PATH of offline native/credential stubs.
They do not certify hosted Xcode, signing, notarization, 4D or GitHub services.
Reference code is never executed. Immutable reference raw blobs, contextual
scope closure and exact normalization/history evidence are retained in the
coordinator's session artifacts.
