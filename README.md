# 4d-plugin-apple-file-promises
Receive file promises from Apple (and other) apps

### Platform

| carbon | cocoa | win32 | win64 |
|:------:|:-----:|:---------:|:---------:|
||<img src="https://cloud.githubusercontent.com/assets/1725068/22371562/1b091f0a-e4db-11e6-8458-8653954a7cce.png" width="24" height="24" />|||

### Version

<img src="https://cloud.githubusercontent.com/assets/1725068/18940648/2192ddba-8645-11e6-864d-6d5692d55717.png" width="32" height="32" />

### Releases 

[1.0](https://github.com/miyako/4d-plugin-apple-file-promises/releases/tag/1.0)

### Discussion

This plugin is design to add the following functionalities to 4D:

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

**Note**: Evidently, Mail uses [file promises](https://developer.apple.com/documentation/uikit/drag_and_drop/understanding_a_drag_item_as_a_promise) to export a single message (short operation) and Automator to export multiple messages. ``com.apple.mail.PasteboardTypeAutomator`` is property list, an ``NSArray`` of ``NSDictionary``, which maps to objects with the structure ``[{account:string, id:integer, mailbox:string, subject:string}]``. Given the 3 identifiers (``account``, ``mailbox``, ``id``) it is possible to use AppleScript like this:

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

When neither of the app specific types are found, but either ``kPasteboardTypeFileURLPromise`` or ``NSPromiseContentsPboardType`` is found, the plugin falls back to generic file promise handling. In particular, it calls ``namesOfPromisedFilesDroppedAtDestination:`` to request the source to created the promised files. **However, this does not seem to work on High Sierra** (no issues on El Capitan). Maybe it is because [``namesOfPromisedFilesDroppedAtDestination:``](https://developer.apple.com/documentation/appkit/nsdragginginfo/1415980-namesofpromisedfilesdroppedatdes) has been deprecated in 10.13. It seems like ``performDragOperation:`` is no longer invoked just by calling this method in the destination's [``performDragOperation:``](https://developer.apple.com/documentation/appkit/nsdraggingdestination/1415970-performdragoperation?language=objc).

## Syntax

```
ACCEPT FILE PROMISES (accept;method;context)
```

Parameter|Type|Description
------------|------------|----
accept|LONGINT|``1`` or ``0``
method|TEXT|
context|TEXT|optional
