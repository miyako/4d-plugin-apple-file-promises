![version](https://img.shields.io/badge/version-18%2B-EB8E5F)
![platform](https://img.shields.io/static/v1?label=platform&message=mac-intel%20|%20mac-arm&color=blue)
[![license](https://img.shields.io/github/license/miyako/4d-plugin-apple-file-promises)](LICENSE)
![downloads](https://img.shields.io/github/downloads/miyako/4d-plugin-apple-file-promises/total)

See [4d-utility-sign-app](https://github.com/miyako/4d-utility-sign-app) on how to enable the plugin in 4D.

# 4d-plugin-apple-file-promises

**Apple file promises** is a 4D plugin (macOS only, 64-bit Cocoa builds) that adds one command, `ACCEPT FILE PROMISES`, to your 4D application. Once enabled, users can drag a message directly out of **Apple Mail** or **Microsoft Outlook** — or a photo out of the **Photos** app, or any ordinary file from the Finder — and drop it onto a 4D form. The plugin does the work of turning that drag into a real file on disk (exporting the message as an `.eml` file, or the photo as an image file), and then calls a 4D method you write, passing it the full path of the file.

Requirements:

- macOS, 64-bit Cocoa version of 4D (v18 or later)
- Your app's `Info.plist` must include the **Privacy – AppleEvents Sending Usage Description** key (`NSAppleEventsUsageDescription`), because the plugin uses Apple Events / scripting to ask Mail, Outlook and Photos to export the dropped item
- The command works with 4D's own drag-and-drop events (`On Drag Over`, `On Drop`) on form objects — you don't need to change your existing drop handling, the plugin adds to it

## Command syntax

```
ACCEPT FILE PROMISES (accept ; method {; context})
```

| Parameter | Type | Description |
| --- | --- | --- |
| `accept` | Longint | `1` to turn drag-and-drop capture **on**, `0` to turn it **off** |
| `method` | Text | The name of your project method to call back for every file dropped |
| `context` | Text | *(optional)* Any string you want passed straight through to `method` unchanged |

This command returns nothing. It has no error-reporting mechanism of its own — if something goes wrong internally (e.g. Mail refuses automation permission), it fails silently and your callback method simply won't be called for that item.

Calling it with `accept = 1` a second time (with the same or a different method name) replaces the previous callback and context. Calling it with `accept = 0` stops the background folder-monitoring process entirely — no more drops will be reported until you call it again with `accept = 1`.

## The callback method

Your `method` project method must accept exactly two parameters:

| Parameter | Type | Contents |
| --- | --- | --- |
| `$1` | Text | The full system path of the file that was just dropped/exported (e.g. `/Users/name/Library/.../3.eml`) |
| `$2` | Text | A copy of whatever `context` string you passed to `ACCEPT FILE PROMISES` |

```4d
// Example signature
C_TEXT($1;$path)
C_TEXT($2;$context)
```

**One call per file.** If several messages are dropped at once, the method is called once per resulting file — not once for the whole drop.

**Do not abort the method.** If your method calls `ABORT` (or otherwise aborts its own execution context), the plugin's background process keeps running, but it stops invoking your method for any further drops — until the structure file is reopened. Always let the method run to completion; use a normal `If`/`Else` to skip work instead of aborting.

**Using `$2` (context) for UI updates.** Because the drop is detected and processed outside your form's own event loop, `context` is commonly used to tell the callback which process/window/object to notify — for example, packing a small JSON object like `{"window":<process number>;"method":"UpdateDropZone"}` and having the callback method `CALL FORM` (or otherwise signal) that process once the file is ready.

## Usage

### 1. Turn it on when the form opens

```4d
// Form method, On Load
ACCEPT FILE PROMISES(1;"AFP_Callback";JSON Stringify({"window":Current form window;"method":"AFP_UpdateDropZone"}))
```

### 2. Turn it off when you no longer need it

```4d
// Form method, On Close, or On Unload
ACCEPT FILE PROMISES(0)
```

### 3. Write the callback method (`AFP_Callback`)

A typical callback records the path somewhere the rest of your app can see it, then pings the right process/window so the UI can react — the context string is where you tell it who to ping.

```4d
//AFP_Callback
//%attributes = {}
C_TEXT($1;$path)
C_TEXT($2)
C_OBJECT($context)

$path:=$1
$context:=JSON Parse($2;Is object)

// keep a running list of everything dropped so far
APPEND TO ARRAY(OBJECT Get pointer(Object named;"Paths")->;$path)

// notify the window/process that requested the drop
CALL FORM(OB Get($context;"window";Is longint);OB Get($context;"method";Is text);$path)
```

### 4. Handle the notification in the target form (`AFP_UpdateDropZone`)

```4d
//AFP_UpdateDropZone
//%attributes = {}
C_TEXT($1;$path)

$path:=$1

// e.g. reveal the exported file to the user
SHOW ON DISK(Temporary folder)

// ...or add it to a form array, refresh a list box, etc.
```

This mirrors the three-method pattern used internally by this plugin's own test project: one method receives the drop and records it, a second appends the path to a shared array, and `SHOW ON DISK(Temporary folder)` is a quick way to confirm a file really landed during testing.

## What gets dropped, and from where

| Source | What arrives | Notes |
| --- | --- | --- |
| Apple Mail (one message) | An `.eml` file | Delivered via a file promise |
| Apple Mail (multiple messages) | One `.eml` file per message | Mail switches to an Automator-driven export for multi-select; each resulting file still triggers your callback once |
| Microsoft Outlook | One `.eml` file per selected message | Requires Outlook to be running |
| Photos | The original image/video file | Only one asset can be exported this way at a time |
| Finder / any other app | The file as-is | No export step — the plugin just reports the path 4D received |

Other things worth knowing:

- **First-run permission prompt.** The first time a user drags from Mail or Photos, macOS will ask them to approve your app controlling that application (System Settings ▸ Privacy & Security ▸ Automation). This is normal and only happens once per app/user.
- **A short delay is normal.** Exported files are detected by watching the destination folder, so there can be a brief (roughly one second) delay between the drop finishing and your callback firing. Don't build UI that assumes it's instantaneous.
- **Disabling stops everything.** `ACCEPT FILE PROMISES(0)` stops the whole background monitor, not just your form's drop zone — no drops anywhere in the app will be reported until you re-enable it.
- **The callback runs outside your form's normal event.** Treat it like an asynchronous notification: don't assume the current form/window is the one the user dropped onto — that's exactly what the `context` parameter is for.

## Troubleshooting

**My callback method never fires.**

- Confirm `Info.plist` has `NSAppleEventsUsageDescription` set — without it, macOS blocks the automation calls to Mail/Outlook/Photos outright.
- Check System Settings ▸ Privacy & Security ▸ Automation and make sure your app is allowed to control Mail/Photos.
- Make sure the method name passed to `ACCEPT FILE PROMISES` exactly matches an existing project method name.

**It worked once, then stopped.**

- Check whether the callback method (or something it calls) executed `ABORT`. That silently kills future callbacks until the structure file is reopened — see *Do not abort the method* above.

**Dropping multiple Mail messages only exports one.**

- This is expected for a single message (file promise). For multiple messages, Mail exports each one individually and your callback fires once per message — if you only see one, check that your method isn't returning early after the first call.

**Nothing happens when dropping from Outlook.**

- Outlook must be running (not just installed) for AppleScript/ScriptingBridge export to work.

**I need to know when *all* files from one drop have arrived, not just each one individually.**

- The plugin reports files one at a time as they're ready; it doesn't send a "batch complete" signal. If you need that, have your callback method count expected vs. received files (e.g. using the `context` value to pass an expected count) and decide completion yourself.
