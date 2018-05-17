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

To process [file promises](https://developer.apple.com/documentation/uikit/drag_and_drop/understanding_a_drag_item_as_a_promise) in 4D, the plugin uses a technique known as **method swizzling**. For that reason, it is only compatible with the 64-bit version of 4D, which is **Cocoa**. 




## Syntax

```
ACCEPT FILE PROMISES (accept;method;context)
```

Parameter|Type|Description
------------|------------|----
accept|LONGINT|``1`` or ``0``
method|TEXT|
context|TEXT|optional
