/* --------------------------------------------------------------------------------
 #
 #	4DPlugin.cpp
 #	source generated by 4D Plugin Wizard
 #	Project : Apple file promises
 #	author : miyako
 #	2018/05/14
 #
 # --------------------------------------------------------------------------------*/

#include "4DPluginAPI.h"
#include "4DPlugin.h"

Swizzle_XMacNSView_saisierec_Class *swizzle_XMacNSView_saisierec = nil;

static IMP __orig_imp_draggingEntered;
static IMP __orig_imp_draggingUpdated;
static IMP __orig_imp_prepareForDragOperation;
static IMP __orig_imp_performDragOperation;
static IMP __orig_imp_concludeDragOperation;

std::mutex globalMutex;
std::mutex globalMutex0;

namespace FilePromise
{
	Listener *listener = nil;
	
	process_number_t METHOD_PROCESS_ID = 0;
	process_stack_size_t STACK_SIZE = 0;
	const process_name_t PROCESS_NAME = (PA_Unichar *)"$\0A\0F\0P\0\0\0";
	
	C_TEXT LISTENER_METHOD;
	std::vector<CUTF16String>PATHS;
	C_TEXT LISTENER_CONTEXT;
	
	bool PROCESS_SHOULD_TERMINATE = false;
	bool PROCESS_SHOULD_RESUME = false;
}

NSString *copyDecodedDynUTI(NSString *uti)
{
	/* 
	 * from: https://gist.github.com/jtbandes/19646e7457208ae9b1ad
	 */
	
	NSString *result;
	
	NSUInteger decoded = 0;
	NSUInteger decodedBits = 0;
	NSMutableData *dat = [[NSMutableData alloc]init];
	
	@autoreleasepool
	{
		if(uti)
		{
			if([uti hasPrefix:@"dyn.a"])
			{
				NSArray *arr =
				@[@"a",@"b",@"c",@"d",@"e",
					@"f",@"g",@"h"          ,
					@"k",     @"m",@"n"     ,
					@"p",@"q",@"r",@"s",@"t",
					@"u",@"v",@"w",@"x",@"y",
					@"z",@"0",@"1",@"2",@"3",
					@"4",@"5",@"6",@"7",@"8",
					@"9"];
				
				NSString *encodedString = [uti substringFromIndex:5];
				
				for (NSUInteger counter = 0; counter < encodedString.length; ++counter)
				{
					NSString *substring = [encodedString substringWithRange:NSMakeRange(counter, 1)];
					
					if (![arr containsObject:substring])
					{
						break;
					}
					
					NSUInteger pos = [arr indexOfObject:substring];
					
					decoded = (decoded << 5) | pos;
					decodedBits += 5;
					
					if (decodedBits >= 8)
					{
						NSUInteger extra = decodedBits - 8;
						UInt8 data = (UInt8)(decoded >> extra);
						decoded &= (1 << extra) - 1;
						decodedBits = extra;
						[dat appendBytes:&data length:sizeof(UInt8)];
					}
				}
			}
		}
	}
	
	result = !decoded ? [[NSString alloc]initWithData:dat encoding:NSUTF8StringEncoding] : nil;
	
	[dat release];
	
	return result;
}

NSURL *temporaryDirectory()
{
	NSURL *url = nil;
	
	NSArray *URLs = [[NSFileManager defaultManager]
									 URLsForDirectory:NSDesktopDirectory
									 inDomains:NSUserDomainMask];
	
	if(URLs && [URLs count])
	{
		
		url = [[NSFileManager defaultManager]
					 URLForDirectory:NSItemReplacementDirectory
					 inDomain:NSUserDomainMask
					 appropriateForURL:[URLs objectAtIndex:0]
					 create:YES
					 error:nil];
	}
	
	return url;
}

void generateUuid(C_TEXT &returnValue)
{
	returnValue.setUTF16String([[[NSUUID UUID]UUIDString]stringByReplacingOccurrencesOfString:@"-" withString:@""]);
}

#pragma mark AppleScript

void sb_tell_photos_to_export(NSURL *url)
{
	@autoreleasepool
	{
		PhotosApplication *application = [SBApplication applicationWithBundleIdentifier:@"com.apple.Photos"];
		
		if(application)
		{
			[application
			 export:[application selection]
			 to:url
			 usingOriginals:YES];
		}
	}
}

void sb_tell_mail_to_export(NSURL *url)
{
	@autoreleasepool
	{
		MailApplication *application = [SBApplication applicationWithBundleIdentifier:@"com.apple.mail"];
		
		if(application)
		{
			NSArray *mails = [application selection];
			NSArray *sources = [mails arrayByApplyingSelector:@selector(source)];
            
			NSUInteger i = 0;
			
			NSString *path = (NSString *)CFURLCopyFileSystemPath((CFURLRef)url, kCFURLPOSIXPathStyle);
			
			for (id source in sources) {
				i++;
				NSString *dst = [path stringByAppendingFormat:@"/%d.%@", i, @"eml"];
        
				[(NSString *)source writeToFile:dst
														 atomically:NO /* avoid catching the atomic write files */
															 encoding:NSWindowsCP1252StringEncoding /* NSUTF8StringEncoding */
																	error:nil];
			}
		}
	}
	
}

void sb_tell_outlook_to_export(NSURL *url)
{
	@autoreleasepool
	{
		NSArray *runningApplications = [NSRunningApplication runningApplicationsWithBundleIdentifier:@"com.microsoft.Outlook"];
		
		if([runningApplications count])
		{
			OutlookApplication *application = [SBApplication applicationWithProcessIdentifier:[[runningApplications objectAtIndex:0]processIdentifier]];
			
			if(application)
			{
				NSArray *mails = [application selectedObjects];
				NSArray *sources = [mails arrayByApplyingSelector:@selector(source)];
				NSUInteger i = 0;
				
				NSString *path = (NSString *)CFURLCopyFileSystemPath((CFURLRef)url, kCFURLPOSIXPathStyle);
				
				for (id source in sources) {
					i++;
					NSString *dst = [path stringByAppendingFormat:@"/%d.%@", i, @"eml"];
					NSLog(@"%@", dst);
					[(NSString *)source writeToFile:dst
															 atomically:NO /* avoid catching the atomic write files */
																 encoding:NSWindowsCP1252StringEncoding /* NSUTF8StringEncoding */
																		error:nil];
				}
			}
		}
	}	
}

NSDragOperation __swiz_draggingEntered(id self, SEL _cmd, id sender)
{
	NSDragOperation returnValue = ((NSDragOperation(*)(id,SEL, id))__orig_imp_draggingEntered)(self, _cmd, sender);
	
	if(returnValue & NSDragOperationNone)
	{
		/* can we change the drag icon here? */
	}
	
	NSPasteboard *pboard = [sender draggingPasteboard];
	
	if(true)
	{
		[sender enumerateDraggingItemsWithOptions:
		 NSDraggingItemEnumerationConcurrent forView:self
																			classes:[NSArray arrayWithObject:[NSPasteboardItem class]]
																searchOptions:nil
																	 usingBlock:^(NSDraggingItem *draggingItem, NSInteger idx, BOOL *stop)
		 {
			 /* for each item (file, folder...) being dragged over */
			 NSArray *types = [[draggingItem item]types];
			
			 /* to check content types (image or not, etc.) */
			 if(false)
			 {
				 if([types containsObject:(NSString *)kPasteboardTypeFilePromiseContent])
				 {
					 NSLog(@"content types:%@", [pboard stringForType:(NSString *)kPasteboardTypeFilePromiseContent]);
				 }
			 }

			 /* evidently no need to register */
			 if([types containsObject:(NSString *)kPasteboardTypeFileURLPromise])
			 {
				 if(false)
				 {
					 NSWindow *window = [sender draggingDestinationWindow];
					 if(window)
					 {
						 NSView *contentView = [window contentView];
						 if(contentView)
						 {
							 const char *name = class_getName([contentView class]);//NSView
							 NSArray *subviews = [contentView subviews];
							 if([subviews count])
							 {
								 NSView *mainView = [subviews objectAtIndex:0];
								 [mainView registerForDraggedTypes:@[
																										 NSFilenamesPboardType,
																										 (NSString *)kPasteboardTypeFileURLPromise,
                                                                                                         @"com.apple.pasteboard.promised-file-name", @"com.apple.pasteboard.NSFilePromiseID"]];
								 NSArray *registeredDraggedTypes = [mainView registeredDraggedTypes];
								 NSLog(@"registered types:%@", registeredDraggedTypes);//com.apple.pasteboard.promised-file-url
							 }
						 }
					 }
				 }
			 }
	
		 }];
	}
	
	return returnValue;
}

NSDragOperation __swiz_draggingUpdated(id self, SEL _cmd, id sender)
{
	NSDragOperation returnValue = ((NSDragOperation(*)(id,SEL, id))__orig_imp_draggingUpdated)(self, _cmd, sender);
	
	if(returnValue & NSDragOperationNone)
	{
		/* can we change the drag icon here? */
	}
	
	if(false)
	{
		[sender enumerateDraggingItemsWithOptions:
		 NSDraggingItemEnumerationConcurrent forView:self
																			classes:[NSArray arrayWithObject:[NSPasteboardItem class]]
																searchOptions:nil
																	 usingBlock:^(NSDraggingItem *draggingItem, NSInteger idx, BOOL *stop)
		 {
			 /* for each item (file, folder...) being dragged over */
			 NSArray *types = [[draggingItem item]types];
		 }];
	}
	
	return returnValue;
}

BOOL __swiz_prepareForDragOperation(id self, SEL _cmd, id sender)
{
	BOOL returnValue = ((BOOL(*)(id,SEL, id))__orig_imp_prepareForDragOperation)(self, _cmd, sender);
	
	if(false)
	{
		[sender enumerateDraggingItemsWithOptions:
		 NSDraggingItemEnumerationConcurrent forView:self
																			classes:[NSArray arrayWithObject:[NSPasteboardItem class]]
																searchOptions:nil
																	 usingBlock:^(NSDraggingItem *draggingItem, NSInteger idx, BOOL *stop)
		 {
			 /* for each item (file, folder...) being dragged over */
			 NSLog(@"draggingEntered:draggingItemTypes:%@", [[draggingItem item]types]);
		 }];
	}
	
	return returnValue;
}

BOOL __swiz_performDragOperation(id self, SEL _cmd, id sender)
{
	BOOL returnValue = ((BOOL(*)(id,SEL, id))__orig_imp_performDragOperation)(self, _cmd, sender);
	
	NSPasteboard *pboard = [sender draggingPasteboard];
	NSArray *types = [pboard types];
	
	if(true)
	{
		/* resolves as "?0=6:4=ERMessagePasteboardType" */
		if ([types containsObject:(NSString *)@"dyn.ah62d4rv4gu8ynywrqz31g2phqzkgc65yqzvg82pwqvnhw6df"])
		{
			goto exit;
		}
	}
	
	if(true)
	{
		if ([types containsObject:(NSString *)@"com.apple.mail.PasteboardTypeAutomator"])
		{
			NSArray *plist = [pboard propertyListForType:@"com.apple.mail.PasteboardTypeAutomator"];
			
			if(plist)
			{
				if([plist count])
				{
					/* I guess the idea is to run automator/applescript based on this information, when multiple messages are dropped/
					 * evidently mail.app uses file promise for a single message but automator for multiple messages (1 of which is transferred via promise in El Capitan)
					 * plist is an array of dictionaries [{account:string, id:integer, mailbox:string, subject:string}]
					 * we will skip the standard kPasteboardTypeFileURLPromise in this case
					 */
					goto exit;
				}
			}
		}
	}

	if(false)
	{
		if ([types containsObject:(NSString *)@"com.apple.mail.PasteboardTypeMessageTransfer"])
		{
			/* contains the path to .mbox of the mailbox */
			NSLog(@"photoUUID:%@", [pboard stringForType:@"com.apple.mail.PasteboardTypeMessageTransfer"]);
		}
	}
	
    if(false)
    {
        if ([types containsObject:(NSString *)@"com.apple.pasteboard.promised-file-name"])
        {
            NSLog(@"%@", [pboard stringForType:@"com.apple.pasteboard.promised-file-name"]);
        }
        
        if ([types containsObject:(NSString *)@"com.apple.pasteboard.NSFilePromiseID"])
        {
            NSLog(@"%@", [pboard stringForType:@"com.apple.pasteboard.NSFilePromiseID"]);
        }
    }

	if (([types containsObject:(NSString *)kPasteboardTypeFileURLPromise]) || ([types containsObject:@"NSPromiseContentsPboardType"]))
	{
		NSURL *url = temporaryDirectory();
		
		/*
		 * Drag destinations should invoke this method within their performDragOperation: method.
		 * https://developer.apple.com/documentation/appkit/nsdragginginfo/1415980-namesofpromisedfilesdroppedatdes
		 */
		NSArray *filenames = [sender namesOfPromisedFilesDroppedAtDestination:url];
		
		std::lock_guard<std::mutex> lock(globalMutex);
		
		/* prepare listener for fiel copy */
		[FilePromise::listener setURL:url];
	}
	
	exit:
	
	return returnValue;
}

void __swiz_concludeDragOperation(id self, SEL _cmd, id sender)
{
	((void(*)(id,SEL, id))__orig_imp_concludeDragOperation)(self, _cmd, sender);
	
	NSPasteboard *pboard = [sender draggingPasteboard];
	NSArray *types = [pboard types];

	if(1)
	{
		/* resolves as "?0=6:4=ERMessagePasteboardType" */
		if ([types containsObject:(NSString *)@"dyn.ah62d4rv4gu8ynywrqz31g2phqzkgc65yqzvg82pwqvnhw6df"])
		{
			NSURL *url = temporaryDirectory();
			
			std::lock_guard<std::mutex> lock(globalMutex);
			
			/* prepare listener for file copy */
			[FilePromise::listener setURL:url];
			
			[swizzle_XMacNSView_saisierec performSelectorInBackground:@selector(askOutlookToExport:) withObject:url];

			goto exit;
		}
	}
	
	if(1)
	{
		if ([types containsObject:(NSString *)@"com.apple.mail.PasteboardTypeAutomator"])
		{
			NSURL *url = temporaryDirectory();
			
			std::lock_guard<std::mutex> lock(globalMutex);
			
			/* prepare listener for file copy */
			[FilePromise::listener setURL:url];
			
			[swizzle_XMacNSView_saisierec performSelectorInBackground:@selector(askMailToExport:) withObject:url];
			
			goto exit;
		}
	}
	
	if(true)
	{
		if ([types containsObject:(NSString *)@"com.apple.PhotoPrintProduct.photoUUID"])
		{
			NSURL *url = temporaryDirectory();
			
			std::lock_guard<std::mutex> lock(globalMutex);
			
			/* prepare listener for file copy */
			[FilePromise::listener setURL:url];
			
			[swizzle_XMacNSView_saisierec performSelectorInBackground:@selector(askPhotosToExport:) withObject:url];
			
			goto exit;
		}
	}
	
	if (([types containsObject:(NSString *)kPasteboardTypeFileURLPromise]) || ([types containsObject:@"NSPromiseContentsPboardType"]))
	{
		/* the promised files may not be available at this point 
		 * does not seem to be working after El Capitain (maybe due to namesOfPromisedFilesDroppedAtDestination deprecation)
		 */
		
		goto exit;
	}

	if ([types containsObject:NSFilenamesPboardType])
	{
		NSArray *plist = [pboard propertyListForType:NSFilenamesPboardType];
		
		[plist enumerateObjectsUsingBlock:^(id obj, NSUInteger idx, BOOL *stop) {
			
			NSURL *url = (NSURL *)CFURLCreateWithFileSystemPath(kCFAllocatorDefault, (CFStringRef)obj, kCFURLPOSIXPathStyle, false);
			if(url)
			{
				NSString *path = (NSString *)CFURLCopyFileSystemPath((CFURLRef)url, kCFURLHFSPathStyle);
				C_TEXT t;
				t.setUTF16String(path);
				CUTF16String u16;
				t.copyUTF16String(&u16);
				
				std::lock_guard<std::mutex> lock(globalMutex);
				
				FilePromise::PATHS.push_back(u16);
				
				[path release];
			}
			
		}];
	}
	
	exit:
	
	listenerLoopExecute();
}

#pragma mark -

void gotEvent(FSEventStreamRef stream,
							void *callbackInfo,
							size_t numEvents,
							void *eventPaths,
							const FSEventStreamEventFlags eventFlags[],
							const FSEventStreamEventId eventIds[]
							)
{
	NSArray *paths = (NSArray *)(CFArrayRef)eventPaths;
	for(NSUInteger i = 0; i < [paths count] ; ++i)
	{
		
		FSEventStreamEventFlags flags = eventFlags[i];
		
		if(flags & kFSEventStreamEventFlagItemIsFile)
		{
			if(((!(flags & kFSEventStreamEventFlagItemCreated))
				 && (flags & kFSEventStreamEventFlagItemRenamed)
				 || (flags & kFSEventStreamEventFlagItemModified)
				 )||(flags == 0x0041C500))
			{
				NSURL *url = (NSURL *)CFURLCreateWithFileSystemPath(kCFAllocatorDefault, (CFStringRef)[paths objectAtIndex:i], kCFURLPOSIXPathStyle, false);
				if(url)
				{
					NSString *path = (NSString *)CFURLCopyFileSystemPath((CFURLRef)url, kCFURLHFSPathStyle);
					C_TEXT t;
					t.setUTF16String(path);
					CUTF16String u16;
					t.copyUTF16String(&u16);
					
					if(1)
					{
						std::lock_guard<std::mutex> lock(globalMutex);
						
						FilePromise::PATHS.push_back(u16);
					}
					
					[path release];
					
					listenerLoopExecute();
				}
			}
		}
	}
}

IMP __swiz_imp_draggingEntered = (IMP)__swiz_draggingEntered;
IMP __swiz_imp_draggingUpdated = (IMP)__swiz_draggingUpdated;
IMP __swiz_imp_prepareForDragOperation = (IMP)__swiz_prepareForDragOperation;
IMP __swiz_imp_performDragOperation = (IMP)__swiz_performDragOperation;
IMP __swiz_imp_concludeDragOperation = (IMP)__swiz_concludeDragOperation;

#pragma mark -

@implementation Swizzle_XMacNSView_saisierec_Class

- (id)init
{
	if(!(self = [super init])) return self;
	
#if CGFLOAT_IS_DOUBLE
	Class MainViewClass = NSClassFromString(@"XMacNSView_saisierec");// XMacNSView_saisierec < NSView < V4DNSWindow
	
	if(MainViewClass)
	{
		__orig_imp_draggingEntered =
		method_setImplementation(
														 class_getInstanceMethod(MainViewClass, @selector(draggingEntered:)),
														 __swiz_imp_draggingEntered); // Q24@0:8@16
		
		__orig_imp_draggingUpdated =
		method_setImplementation(
														 class_getInstanceMethod(MainViewClass, @selector(draggindUpdated:)),
														 __swiz_imp_draggingUpdated); // Q24@0:8@16
		
		__orig_imp_prepareForDragOperation =
		method_setImplementation(
														 class_getInstanceMethod(MainViewClass, @selector(prepareForDragOperation:)),
														 __swiz_imp_prepareForDragOperation); // c24@0:8@16
		
		__orig_imp_performDragOperation =
		method_setImplementation(
														 class_getInstanceMethod(MainViewClass, @selector(performDragOperation:)),
														 __swiz_imp_performDragOperation); // c24@0:8@16
		
		__orig_imp_concludeDragOperation =
		method_setImplementation(
														 class_getInstanceMethod(MainViewClass, @selector(concludeDragOperation:)),
														 __swiz_imp_concludeDragOperation); // v24@0:8@16
	}
#endif
	
	return self;
}

- (void)dealloc
{
#if CGFLOAT_IS_DOUBLE
	Class MainViewClass = NSClassFromString(@"XMacNSView_saisierec");
	
	if(MainViewClass)
	{
		method_setImplementation(
														 class_getInstanceMethod(MainViewClass, @selector(draggingEntered:)),
														 __orig_imp_draggingEntered);

		method_setImplementation(
														 class_getInstanceMethod(MainViewClass, @selector(draggingUpdated:)),
														 __orig_imp_draggingUpdated);
		
		method_setImplementation(
														 class_getInstanceMethod(MainViewClass, @selector(prepareForDragOperation:)),
														 __orig_imp_prepareForDragOperation);
		
		method_setImplementation(
														 class_getInstanceMethod(MainViewClass, @selector(performDragOperation:)),
														 __orig_imp_performDragOperation);
		
		method_setImplementation(
														 class_getInstanceMethod(MainViewClass, @selector(concludeDragOperation:)),
														 __orig_imp_concludeDragOperation);
	}
#endif
}

- (void)askPhotosToExport:(NSURL *)url
{
	sb_tell_photos_to_export(url);
}

- (void)askOutlookToExport:(NSURL *)url
{
	sb_tell_outlook_to_export(url);
}

- (void)askMailToExport:(NSURL *)url
{
	sb_tell_mail_to_export(url);
}

@end

void swizzle_on()
{
#if CGFLOAT_IS_DOUBLE
	if(!swizzle_XMacNSView_saisierec)
	{
		swizzle_XMacNSView_saisierec = [[Swizzle_XMacNSView_saisierec_Class alloc]init];
	}
	listenerLoopStart();
#endif
}

void swizzle_off()
{
#if CGFLOAT_IS_DOUBLE
	if(swizzle_XMacNSView_saisierec)
	{
		[swizzle_XMacNSView_saisierec release];
		swizzle_XMacNSView_saisierec = nil;
	}
	listenerLoopFinish();
#endif
}

#pragma mark -

void PluginMain(PA_long32 selector, PA_PluginParameters params)
{
	try
	{
		PA_long32 pProcNum = selector;
		sLONG_PTR *pResult = (sLONG_PTR *)params->fResult;
		PackagePtr pParams = (PackagePtr)params->fParameters;
		
		CommandDispatcher(pProcNum, pResult, pParams);
	}
	catch(...)
	{
		
	}
}

#pragma mark -

bool IsProcessOnExit()
{
	C_TEXT name;
	PA_long32 state, time;
	PA_GetProcessInfo(PA_GetCurrentProcessNumber(), name, &state, &time);
	CUTF16String procName(name.getUTF16StringPtr());
	CUTF16String exitProcName((PA_Unichar *)"$\0x\0x\0\0\0");
	return (!procName.compare(exitProcName));
}

void OnStartup()
{

}


void OnExit()
{
    swizzle_off();
}

/*
void OnCloseProcess()
{
	if(IsProcessOnExit())
	{
		swizzle_off();
	}
}
 */

#pragma mark -

void CommandDispatcher (PA_long32 pProcNum, sLONG_PTR *pResult, PackagePtr pParams)
{
	switch(pProcNum)
	{
		case kInitPlugin :
		case kServerInitPlugin :
			OnStartup();
			break;

        case kDeinitPlugin :
            OnExit();
            break;
            
            /*
		case kCloseProcess :
			OnCloseProcess();
			break;
             */
			
			// --- Apple file promises
			
		case 1 :
			ACCEPT_FILE_PROMISES(pResult, pParams);
			break;
			
	}
}

// ------------------------------ Apple file promises -----------------------------

@implementation Listener

- (id)init
{
	if(!(self = [super init])) return self;
	
	/* setup object instance here */
	
	stream = 0;
	
	return self;
}

- (void)dealloc
{
	if(stream)
	{
		FSEventStreamStop(stream);
		FSEventStreamUnscheduleFromRunLoop (stream, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
		FSEventStreamInvalidate(stream);
		FSEventStreamRelease(stream);
		stream = 0;
	}
	
	[super dealloc];
}

- (void)setURL:(NSURL *)url
{
	if(stream)
	{
		FSEventStreamStop(stream);
		FSEventStreamUnscheduleFromRunLoop (stream, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
		FSEventStreamInvalidate(stream);
		FSEventStreamRelease(stream);
		stream = 0;
	}
	
	NSString *path = (NSString *)CFURLCopyFileSystemPath((CFURLRef)url, kCFURLPOSIXPathStyle);
	
	FSEventStreamContext context = {0, NULL, NULL, NULL, NULL};
	NSTimeInterval latency = 1.0;
	
	stream = FSEventStreamCreate(NULL,
															 (FSEventStreamCallback)gotEvent,
															 &context,
															 (CFArrayRef)@[path],
															 kFSEventStreamEventIdSinceNow,
															 (CFAbsoluteTime)latency,
															 kFSEventStreamCreateFlagUseCFTypes
															 | kFSEventStreamCreateFlagFileEvents
															 | kFSEventStreamCreateFlagNoDefer
//															 | kFSEventStreamCreateFlagIgnoreSelf
															 );
	
	FSEventStreamScheduleWithRunLoop(stream, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
	FSEventStreamStart(stream);
	[path release];
}

@end

#pragma mark -

void listenerLoop()
{
	if(1)
	{
		std::lock_guard<std::mutex> lock(globalMutex);
		
		FilePromise::listener = [[Listener alloc]init];
		FilePromise::PROCESS_SHOULD_TERMINATE = false;
	}

	while(!PA_IsProcessDying())
	{
		PA_YieldAbsolute();
		
        std::lock_guard<std::mutex> lock(globalMutex0);
		
		if(FilePromise::PROCESS_SHOULD_RESUME)
		{
            
			while(FilePromise::PATHS.size())
			{
				PA_YieldAbsolute();
				
				if(CALLBACK_IN_NEW_PROCESS)
				{
					C_TEXT processName;
					generateUuid(processName);
					PA_NewProcess((void *)listenerLoopExecuteMethod,
												FilePromise::STACK_SIZE,
												(PA_Unichar *)processName.getUTF16StringPtr());
				}else
				{
					listenerLoopExecuteMethod();
				}
				
				if (FilePromise::PROCESS_SHOULD_TERMINATE)
					break;
			}
			
			FilePromise::PROCESS_SHOULD_RESUME = false;
			
		}else
		{
			PA_PutProcessToSleep(PA_GetCurrentProcessNumber(), CALLBACK_SLEEP_TIME);
		}
		
		if (FilePromise::PROCESS_SHOULD_TERMINATE)
			break;
	}
	
	if(1)
	{
		std::lock_guard<std::mutex> lock(globalMutex);
		
		[FilePromise::listener release];
		FilePromise::listener = nil;
		FilePromise::PATHS.clear();
		FilePromise::LISTENER_METHOD.setUTF16String((PA_Unichar *)"\0\0", 0);
		FilePromise::METHOD_PROCESS_ID = 0;
	}
	
	PA_KillProcess();
}


void listenerLoopStart()
{
    std::lock_guard<std::mutex> lock(globalMutex0);
	
	if(!FilePromise::METHOD_PROCESS_ID)
	{
		FilePromise::METHOD_PROCESS_ID = PA_NewProcess((void *)listenerLoop,
																					FilePromise::STACK_SIZE,
																					FilePromise::PROCESS_NAME);
	}
}

void listenerLoopFinish()
{
	std::lock_guard<std::mutex> lock(globalMutex);
	
	if(FilePromise::METHOD_PROCESS_ID)
	{
		FilePromise::PROCESS_SHOULD_TERMINATE = true;
		
		PA_YieldAbsolute();
		
		FilePromise::PROCESS_SHOULD_RESUME = true;
	}
}

void listenerLoopExecute()
{
	std::lock_guard<std::mutex> lock(globalMutex);
	
	FilePromise::PROCESS_SHOULD_TERMINATE = false;
	FilePromise::PROCESS_SHOULD_RESUME = true;
}

void listenerLoopExecuteMethod()
{
	std::lock_guard<std::mutex> lock(globalMutex);
	
	std::vector<CUTF16String>::iterator p = FilePromise::PATHS.begin();
	
	method_id_t methodId = PA_GetMethodID((PA_Unichar *)FilePromise::LISTENER_METHOD.getUTF16StringPtr());
	
	if(methodId)
	{
		PA_Variable	params[2];
		params[0] = PA_CreateVariable(eVK_Unistring);
		params[1] = PA_CreateVariable(eVK_Unistring);
		
		PA_Unistring command = PA_CreateUnistring((PA_Unichar *)p->c_str());
		PA_Unistring context = PA_CreateUnistring((PA_Unichar *)FilePromise::LISTENER_CONTEXT.getUTF16StringPtr());
	
		PA_SetStringVariable(&params[0], &command);
		PA_SetStringVariable(&params[1], &context);
		
		FilePromise::PATHS.erase(p);
		
		PA_ExecuteMethodByID(methodId, params, 2);
		
		PA_ClearVariable(&params[0]);
		PA_ClearVariable(&params[1]);
		
	}else
	{
		PA_Variable	params[3];
		params[1] = PA_CreateVariable(eVK_Unistring);
		params[2] = PA_CreateVariable(eVK_Unistring);
		
		PA_Unistring command = PA_CreateUnistring((PA_Unichar *)p->c_str());
		PA_Unistring context = PA_CreateUnistring((PA_Unichar *)FilePromise::LISTENER_CONTEXT.getUTF16StringPtr());
		
		PA_SetStringVariable(&params[1], &command);
		PA_SetStringVariable(&params[2], &context);
		
		params[0] = PA_CreateVariable(eVK_Unistring);
		PA_Unistring method = PA_CreateUnistring((PA_Unichar *)FilePromise::LISTENER_METHOD.getUTF16StringPtr());
		PA_SetStringVariable(&params[0], &method);
		
		FilePromise::PATHS.erase(p);
		
		PA_ExecuteCommandByID(1007, params, 3);

		PA_ClearVariable(&params[0]);
		PA_ClearVariable(&params[1]);
		PA_ClearVariable(&params[2]);
	}
}

#pragma mark -

void ACCEPT_FILE_PROMISES(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_LONGINT Param1;
	C_TEXT Param2_method;
	C_TEXT Param3_context;

	Param1.fromParamAtIndex(pParams, 1);
	
	if(!IsProcessOnExit())
	{
		if(1)
		{
			std::lock_guard<std::mutex> lock(globalMutex);
			
			FilePromise::LISTENER_METHOD.fromParamAtIndex(pParams, 2);
			FilePromise::LISTENER_CONTEXT.fromParamAtIndex(pParams, 3);
		}
		
		if(Param1.getIntValue())
		{
			swizzle_on();
		}else
		{
			swizzle_off();
		}
	}
}
