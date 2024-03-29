![version](https://img.shields.io/badge/version-18%2B-EB8E5F)
![platform](https://img.shields.io/static/v1?label=platform&message=mac-intel%20|%20mac-arm&color=blue)
[![license](https://img.shields.io/github/license/miyako/4d-plugin-apple-file-promises)](LICENSE)
![downloads](https://img.shields.io/github/downloads/miyako/4d-plugin-apple-file-promises/total)

See [4d-utility-sign-app](https://github.com/miyako/4d-utility-sign-app) on how to enable the plugin in 4D.

# 4d-plugin-apple-file-promises
Accept drag and drop of messages from Apple Mail and Microsoft Outlook

**A similar solution for Windows is available**: [message-file-drop](https://github.com/miyako/4d-plugin-message-file-drop)

The plugin is compatible with the new security rules of macOS Mojave, but it is your responsibility to add the "Privacy - AppleEvents Sending Usage Description" (``NSAppleEventsUsageDescription``)  key to the main app's ``Info.plist``.

See [4d-plugin-notes](https://github.com/miyako/4d-plugin-notes) for more information.

## Syntax

```
ACCEPT FILE PROMISES (accept;method{;context})
```

Parameter|Type|Description
------------|------------|----
accept|LONGINT|``1`` or ``0``
method|TEXT|callback method(``$1:text;$2:text``)
context|TEXT|any string (``$2`` passed to ``method``)

In ``method`` pass the callback project method name. It will be called with 2 parameters, where ``$1`` is the system full path of the file dropped to 4D, and ``$2`` is a copy of the ``context`` you passed earlier. You can use ``context`` to let the callback notify and update your UI worker/process, for example.

**Note**: You can disable the folder monitoring process by passing ``0`` to the command. 

**Do not abort the callback method**. If you abort the execution context, the process will keep running but the method will not longer be called from the plugin until you reopen the structure file.

---

#### Technical Details

This plugin is designed to add the following functionalities to 4D:

* Drag and drop emails from typical clients (Mail, Outlook) directly to 4D.

To customise drag and drop event handling, the plugin uses a technique known as **method swizzling**. For that reason, it is only compatible with the 64-bit Cocoa version of 4D.

When swizzling is activated, the plugin adds some extra code to the following drag and drop obj-c methods from the ``NSDraggingDestination`` protocol: ``draggingEntered:`` ``draggingUpdated:`` ``prepareForDragOperation:`` ``performDragOperation:`` ``concludeDragOperation:``.

This allows the plugin to perform some extra work when 4D processes drag and drop events (``On Drag Over``, ``On Drop``) on its form objects.

``draggingEntered:`` nothing special  
``draggingUpdated:`` nothing special  
``prepareForDragOperation:`` nothing special  

``performDragOperation:``  

Inspect ``[[sender draggingPasteboard]types]``.  

* Mail (Apple)

If ``com.apple.mail.PasteboardTypeAutomator`` is found, the plugin runs code to get ``eml`` files out of Mail.

**Note**: Evidently, Mail uses [file promises](https://developer.apple.com/documentation/uikit/drag_and_drop/understanding_a_drag_item_as_a_promise) to export a single message (short operation) and Automator to export multiple messages. ``com.apple.mail.PasteboardTypeAutomator`` is a property list, an ``NSArray`` of ``NSDictionary``, which maps to objects with the structure ``[{account:string, id:integer, mailbox:string, subject:string}]``. Given the 3 identifiers (``account``, ``mailbox``, ``id``) it is possible to use AppleScript like this:

```applescript
on run argv
	set param_mailbox to item 1 of argv
	set param_account to item 2 of argv
	set param_id to item 3 of argv
	set param_path to item 4 of argv
	tell application "Mail"
		set mm to (messages of mailbox param_mailbox of account param_account) whose id is param_id
		if (count mm) is 1 then
			set m to item 1 of mm
			set p to param_path
			set s to (source of m) as «class utf8»
			try
				set f to open for access p with write permission
				write s to f
				close access f
			end try
		end if
	end tell
end run
```

**For optimisation**, the plugin uses [``ScriptingBridge``](https://developer.apple.com/documentation/scriptingbridge) to create the ``eml`` file by itself, rather than launching ``osascript`` or ``NSAppleScript`` to run the AppleScript shown above.

```objc
MailApplication *application = [SBApplication applicationWithBundleIdentifier:@"com.apple.mail"];

NSArray *mails = [application selection];
NSArray *sources = [mails arrayByApplyingSelector:@selector(source)];
NSUInteger i = 0;

NSString *path = (NSString *)CFURLCopyFileSystemPath((CFURLRef)url, kCFURLPOSIXPathStyle);

for (id source in sources) {
	i++;
	NSString *dst = [path stringByAppendingFormat:@"/%d.%@", i, @"eml"];
	[(NSString *)source writeToFile:dst
		atomically:NO
		encoding:NSUTF8StringEncoding
		error:nil];
}
```

* Outlook (Microsoft)

Likewise, if ``dyn.ah62d4rv4gu8ynywrqz31g2phqzkgc65yqzvg82pwqvnhw6df`` is found, the plugin runs code to get ``eml`` files out of Outlook.

**Note**: The above dynamic UTI resolves as ``?0=6:4=ERMessagePasteboardType`` using the method published [here](https://gist.github.com/jtbandes/19646e7457208ae9b1ad). Outlook used to deliver a type named ``ERMessagePasteboardType`` in version 14 but evidently removed it in version 15.

There is no list of selected messages (as was the case with Mail) but we could assume in AppleScript that the current selection should be exported like this:

```applescript
on run argv
	set param_path to item 1 of argv
	tell application "Microsoft Outlook"
		set p to param_path
		set mm to selection as list
		repeat with m in mm
			set s to (source of m) as «class utf8»
			try
				set f to open for access (p & (id of m) & ".eml") with write permission
				write s to f
				close access f
			end try
		end repeat
	end tell
end run
``` 

**For optimisation**, the plugin uses [``ScriptingBridge``](https://developer.apple.com/documentation/scriptingbridge) to create the ``eml`` file by itself, rather than launching ``osascript`` or ``NSAppleScript`` to run the AppleScript shown above. Notice the property is ``selectedObjects``, not ``selection`` as in Mail.

```objc
MailApplication *application = [SBApplication applicationWithBundleIdentifier:@"com.apple.mail"];
		
NSArray *mails = [application selection];
NSArray *sources = [mails arrayByApplyingSelector:@selector(source)];
NSUInteger i = 0;

NSString *path = (NSString *)CFURLCopyFileSystemPath((CFURLRef)url, kCFURLPOSIXPathStyle);

for (id source in sources) {
	i++;
	NSString *dst = [path stringByAppendingFormat:@"/%d.%@", i, @"eml"];
	[(NSString *)source writeToFile:dst
		atomically:NO
		encoding:NSUTF8StringEncoding
		error:nil];
}
```

* Fallback

When neither of the app specific types are found, but either ``kPasteboardTypeFileURLPromise`` or ``NSPromiseContentsPboardType`` is found, the plugin falls back to generic file promise handling. In particular, it calls ``namesOfPromisedFilesDroppedAtDestination:`` to request the source to created the promised files. **However, this does not seem to work on High Sierra** (no issues on El Capitan). 

Maybe it is because [``namesOfPromisedFilesDroppedAtDestination:``](https://developer.apple.com/documentation/appkit/nsdragginginfo/1415980-namesofpromisedfilesdroppedatdes) has been deprecated in 10.13. 

* Folder Watching

Whether the designated apps (Mail, Outlook) are invoked by scripting, or file promises are used, the plugin starts monitoring the destination folder (which is created in the temporary folder, ``NSItemReplacementDirectory`` in ``NSUserDomainMask`` appropriate for ``NSDesktopDirectory``) using [``FSEventStream``](https://developer.apple.com/library/content/documentation/Darwin/Conceptual/FSEvents_ProgGuide/UsingtheFSEventsFramework/UsingtheFSEventsFramework.html).

The callback 4D method is executed every time a file is added to the destination folder. Meanwhile, **for optimisation**, the code to extract and export ``eml`` files from Mail or Outlook are executed in a background thread, with [``performSelectorInBackground:``](https://developer.apple.com/documentation/objectivec/nsobject/1412390-performselectorinbackground?language=objc).  

* Simple API

If neither the app specific types or the file promise types are found, but ``NSFilenamesPboardType`` is found, in other words, a file (not a promise) has been dropped, the same callback 4D method is invoked instantly. 4D does not have to know if the file existed already or was created via scripting or via promises being kept. It just receives a path.
