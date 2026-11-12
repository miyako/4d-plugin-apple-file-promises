// Force-included before every translation unit of the synthetic ownership host.
// Real frameworks are imported first for types; every operation that could reach
// the pasteboard, drag sessions, Apple Events, Scripting Bridge, FSEvents, the
// filesystem, other applications or the Objective-C runtime swizzle is then
// redirected to an in-memory stub defined in ownership_host.mm.
#ifndef OWNERSHIP_PLATFORM_H
#define OWNERSHIP_PLATFORM_H
#ifdef __OBJC__
#import <Cocoa/Cocoa.h>
#import <CoreServices/CoreServices.h>
#import <ScriptingBridge/ScriptingBridge.h>
#import <objc/runtime.h>

@interface OwnershipSBApplication : NSObject
+ (id)applicationWithBundleIdentifier:(NSString *)identifier;
+ (id)applicationWithProcessIdentifier:(pid_t)pid;
@end
@interface OwnershipRunningApplication : NSObject
+ (NSArray *)runningApplicationsWithBundleIdentifier:(NSString *)identifier;
- (pid_t)processIdentifier;
@end
@interface OwnershipFileManager : NSObject
+ (OwnershipFileManager *)defaultManager;
- (NSArray *)URLsForDirectory:(NSSearchPathDirectory)d inDomains:(NSSearchPathDomainMask)m;
- (NSURL *)URLForDirectory:(NSSearchPathDirectory)d inDomain:(NSSearchPathDomainMask)m
         appropriateForURL:(NSURL *)u create:(BOOL)c error:(NSError **)e;
@end
@interface OwnershipOperationQueue : NSObject
+ (OwnershipOperationQueue *)mainQueue;
@end
@interface OwnershipPromiseReceiver : NSObject
+ (NSArray *)readableDraggedTypes;
- (void)receivePromisedFilesAtDestination:(NSURL *)d options:(NSDictionary *)o
                           operationQueue:(OwnershipOperationQueue *)q
                                   reader:(void (^)(NSURL *, NSError *))r;
@end

CFURLRef OwnershipCreateURL(CFAllocatorRef, CFStringRef, CFURLPathStyle, Boolean);
CFStringRef OwnershipCopyPath(CFURLRef, CFURLPathStyle);
OSErr OwnershipAECreateDesc(DescType, const void *, Size, AEDesc *);
OSErr OwnershipAEDisposeDesc(AEDesc *);
OSStatus OwnershipAEPermission(const AEAddressDesc *, AEEventClass, AEEventID, Boolean);
FSEventStreamRef OwnershipFSCreate(CFAllocatorRef, FSEventStreamCallback, FSEventStreamContext *,
                                   CFArrayRef, FSEventStreamEventId, CFTimeInterval,
                                   FSEventStreamCreateFlags);
void OwnershipFSSchedule(FSEventStreamRef, CFRunLoopRef, CFStringRef);
Boolean OwnershipFSStart(FSEventStreamRef);
void OwnershipFSStream(FSEventStreamRef);
Class OwnershipClassFromString(NSString *);
Method OwnershipGetInstanceMethod(Class, SEL);
IMP OwnershipSetImplementation(Method, IMP);
void OwnershipLog(NSString *, ...);

#define SBApplication OwnershipSBApplication
#define NSRunningApplication OwnershipRunningApplication
#define NSFileManager OwnershipFileManager
#define NSOperationQueue OwnershipOperationQueue
#define NSFilePromiseReceiver OwnershipPromiseReceiver
#define CFURLCreateWithFileSystemPath OwnershipCreateURL
#define CFURLCopyFileSystemPath OwnershipCopyPath
#define AECreateDesc OwnershipAECreateDesc
#define AEDisposeDesc OwnershipAEDisposeDesc
#define AEDeterminePermissionToAutomateTarget OwnershipAEPermission
#define FSEventStreamCreate OwnershipFSCreate
#define FSEventStreamScheduleWithRunLoop OwnershipFSSchedule
#define FSEventStreamUnscheduleFromRunLoop OwnershipFSSchedule
#define FSEventStreamStart OwnershipFSStart
#define FSEventStreamStop OwnershipFSStream
#define FSEventStreamInvalidate OwnershipFSStream
#define FSEventStreamRelease OwnershipFSStream
#define NSClassFromString OwnershipClassFromString
#define class_getInstanceMethod OwnershipGetInstanceMethod
#define method_setImplementation OwnershipSetImplementation
#define NSLog OwnershipLog
#endif
#endif
