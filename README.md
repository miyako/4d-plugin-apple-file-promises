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

``draggingEntered:``: nothing special  
``draggingUpdated:``: nothing special  
``prepareForDragOperation:``: nothing special  

``performDragOperation:``:  

Inspect ``[[sender draggingPasteboard]types]``.  

If ``"com.apple.mail.PasteboardTypeAutomator`` is found, run code to get ``eml`` files out of Mail.

**Note**: Evidently, Mail uses [file promises](https://developer.apple.com/documentation/uikit/drag_and_drop/understanding_a_drag_item_as_a_promise) to export a single message (short operation) in El Capitan and Automator to export multiple messages. ``com.apple.mail.PasteboardTypeAutomator`` is property list, an ``NSArray`` of ``NSDictionary``, which maps to objects with the structure ``[{account:string, id:integer, mailbox:string, subject:string}]``. Given the 3 identifiers (``account``, ``mailbox``, ``id``) it is possible to use AppleScript like this:

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



## Syntax

```
ACCEPT FILE PROMISES (accept;method;context)
```

Parameter|Type|Description
------------|------------|----
accept|LONGINT|``1`` or ``0``
method|TEXT|
context|TEXT|optional
