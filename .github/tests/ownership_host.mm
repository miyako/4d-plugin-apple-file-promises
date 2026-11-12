// Synthetic, unsigned 4D host for the actual plugin dispatcher, listener loop and
// callback with the complete unchanged bundled SDK. Nothing here touches a real
// pasteboard, drag session, Mail, Outlook, Photos, FSEvents stream, Apple Event
// target, 4D process or file: every boundary is an in-memory stub.
#include "4DPluginAPI.h"
#include "PrivateTypes.h"
#include "EntryPoints.h"
#include "4DPlugin-Apple-file-promises.h"
#include <codecvt>
#include <cstring>
#include <iostream>
#include <locale>
#include <map>
#include <set>
#include <string>
#include <vector>

void gotEvent(FSEventStreamRef, void *, size_t, void *,
              const FSEventStreamEventFlags[], const FSEventStreamEventId[]);
namespace FilePromise {
    extern std::vector<CUTF16String> PATHS;
    extern process_number_t METHOD_PROCESS_ID;
}

static std::map<PA_Unichar *, size_t> strings;
static std::map<PA_Picture, int> pictures;
static std::map<PA_Handle, size_t> handles;
static bool nullEmpty, failHandle, stopDuringLoop;
static std::string branch, result;
static CUTF16String methodName, contextText;
static std::vector<CUTF16String> expectedPaths, receivedPaths;
static int methodCalls, commandCalls, dyingCalls, sleeps, killed, newProcesses, mainCalls;
static int attempts, yields, platformCalls, logs, swizzles;
static void *loopProc;

static void check(bool valid, const char *message)
{
    if (!valid) { std::cout << "FAIL " << message << "\n"; std::cout.flush(); std::exit(1); }
}
static CUTF16String utf16(const std::string &text)
{
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> codec;
    auto converted = codec.from_bytes(text);
    return CUTF16String(reinterpret_cast<const PA_Unichar *>(converted.data()), converted.size());
}
static PA_Unistring makeString(const PA_Unichar *text)
{
    size_t units = 0;
    while (text && text[units]) ++units;
    PA_Unistring value = {};
    if (units || !nullEmpty) {
        value.fString = new PA_Unichar[units + 1];
        if (units) std::memcpy(value.fString, text, units * sizeof(PA_Unichar));
        value.fString[units] = 0;
        value.fLength = static_cast<PA_long32>(units);
        strings[value.fString] = units;
    }
    return value;
}
static void disposeString(PA_Unistring value)
{
    if (!value.fString) { check(value.fLength == 0, "null nonempty string"); return; }
    check(strings.count(value.fString), "duplicate/unowned string disposal");
    check(strings.at(value.fString) == static_cast<size_t>(value.fLength), "allocation length");
    strings.erase(value.fString);
    delete[] value.fString;
}
static void textEquals(const PA_Variable &v, const CUTF16String &wanted, const char *what)
{
    check(v.fType == eVK_Unistring, what);
    const PA_Unistring &s = v.uValue.fString;
    check(s.fLength == static_cast<PA_long32>(wanted.size()), "exact UTF-16 length");
    check(s.fString || wanted.empty(), "missing text allocation");
    if (s.fString) {
        check(strings.count(s.fString), "argument read after release");
        check(s.fString[s.fLength] == 0, "text terminator");
        check(CUTF16String(s.fString, s.fLength) == wanted, what);
    }
}
static void distinctOwners(const std::vector<const PA_Variable *> &args)
{
    std::set<PA_Unichar *> seen;
    for (auto a : args) {
        if (!a->uValue.fString.fString) continue;
        check(seen.insert(a->uValue.fString.fString).second, "arguments share one owner");
    }
}
static PA_Picture makePicture()
{
    PA_Picture p = new unsigned char(42);
    pictures[p] = 1;
    return p;
}
static void releasePicture(PA_Picture p)
{
    if (!p) return;
    check(pictures.count(p), "duplicate/unowned picture disposal");
    if (--pictures[p] == 0) { pictures.erase(p); delete static_cast<unsigned char *>(p); }
}
static void clear(PA_Variable *v)
{
    if (v->fType == eVK_Unistring) disposeString(v->uValue.fString);
    else if (v->fType == eVK_Picture) releasePicture(v->uValue.fPicture);
    else check(v->fType == eVK_Undefined || v->fType == eVK_Longint,
               "unsupported clear kind (including poisoned failure output)");
    v->fType = eVK_Undefined;
    v->fFiller = 0;
}
static void setResult(PA_Variable *returned, short *error)
{
    if (result == "error") {
        // Failed calls leave the output unspecified: poison it and report an error.
        std::memset(returned, 0xa5, sizeof(*returned));
        *error = eER_IncompatibleTypes;
        return;
    }
    if (result == "string") {
        auto owned = makeString(utf16("result \xe6\x97\xa5\xf0\x9f\x98\x80").c_str());
        PA_SetStringVariable(returned, &owned);
    } else if (result == "picture") PA_SetPictureVariable(returned, makePicture());
    else if (result == "longint") PA_SetLongintVariable(returned, 7);
    else check(result == "undefined" || result == "handle", "unknown result kind");
    *error = eER_NoErr;
}
static void host(short selector, EngineBlock *eb)
{
    switch (selector) {
    case EX_CREATE_UNISTRING:
        eb->fUniString1 = makeString(reinterpret_cast<PA_Unichar *>(eb->fHandle));
        break;
    case EX_DISPOSE_UNISTRING: disposeString(eb->fUniString1); break;
    case EX_CLEAR_VARIABLE: clear(static_cast<PA_Variable *>(eb->fPtr1)); break;
    case EX_YIELD_ABSOLUTE:
        ++yields;
        if (stopDuringLoop && yields == 1) {
            // Cooperative scheduling: the 4D method stops the listener while
            // the listener process yields with two paths still queued.
            PA_long32 off = 0;
            PA_Unistring m = PA_CreateUnistring(const_cast<PA_Unichar *>(methodName.c_str()));
            PA_Unistring c = PA_CreateUnistring(const_cast<PA_Unichar *>(contextText.c_str()));
            void *params[] = {&off, &m, &c};
            PluginBlock block = {};
            block.fParameters = params;
            PluginMain(1, &block);
            PA_DisposeUnistring(&m);
            PA_DisposeUnistring(&c);
        }
        break;
    case EX_GET_METHOD_ID:
        ++attempts;
        // The next handle allocation is the callback's argument block.
        failHandle = result == "handle";
        eb->fParam1 = CUTF16String(eb->fUName) == utf16("cb") ? 321 : 0;
        eb->fError = eER_NoErr;
        break;
    case EX_CALL_BY_PROCID: {
        ++methodCalls;
        check(branch == "method", "method callback on command branch");
        check(eb->fParam1 == 321 && eb->fParam2 == 2, "callback identity and arity");
        check(handles.count(eb->fHandle), "callback handle");
        auto values = reinterpret_cast<PA_Variable *>(*eb->fHandle);
        check(values[0].fType == eVK_Undefined, "callback result slot initialized");
        check(receivedPaths.size() < expectedPaths.size(), "too many callbacks");
        textEquals(values[1], expectedPaths[receivedPaths.size()], "callback path text");
        textEquals(values[2], contextText, "callback context text");
        distinctOwners({&values[1], &values[2]});
        receivedPaths.push_back(expectedPaths[receivedPaths.size()]);
        setResult(&values[0], &eb->fError);
        break;
    }
    case EX_EXECUTE_COMMAND_BY_ID: {
        ++commandCalls;
        check(branch == "command", "command fallback on method branch");
        check(eb->fParam1 == 1007 && eb->fParam2 == 3, "EXECUTE METHOD identity and arity");
        auto args = static_cast<PA_Variable **>(eb->fPtr1);
        auto returned = static_cast<PA_Variable *>(eb->fPtr2);
        check(returned->fType == eVK_Undefined, "command result slot initialized");
        check(receivedPaths.size() < expectedPaths.size(), "too many command callbacks");
        textEquals(*args[0], methodName, "command method name");
        textEquals(*args[1], expectedPaths[receivedPaths.size()], "command path text");
        textEquals(*args[2], contextText, "command context text");
        distinctOwners({args[0], args[1], args[2]});
        receivedPaths.push_back(expectedPaths[receivedPaths.size()]);
        setResult(returned, &eb->fError);
        break;
    }
    case EX_RUN_IN_MAIN_PROCESS:
        ++mainCalls;
        check(eb->fPtr1 == reinterpret_cast<void *>(listener_start) ||
              eb->fPtr1 == reinterpret_cast<void *>(listener_end), "unexpected main-process function");
        reinterpret_cast<PA_RunInMainProcessProcPtr>(eb->fPtr1)(eb->fPtr2);
        break;
    case EX_NEW_PROCESS:
        // Record only: never launch a real 4D or OS process.
        check(eb->fHandle == reinterpret_cast<PA_Handle>(listenerLoop), "unexpected process function");
        ++newProcesses;
        loopProc = eb->fHandle;
        eb->fParam1 = 5;
        eb->fError = eER_NoErr;
        break;
    case EX_CURRENT_PROCESS_NUMBER: eb->fParam1 = 5; break;
    case EX_IS_PROCESS_DYING: eb->fManyToOne = ++dyingCalls >= 2; break;
    case EX_PUT_PROCESS_TO_SLEEP:
        check(++sleeps < 4, "listener loop did not terminate");
        break;
    case EX_KILL_PROCESS: ++killed; break;
    case EX_HANDLE_MANAGER: {
        auto h = eb->fHandle;
        switch (eb->fParam1) {
        case 1:
            if (failHandle) {
                failHandle = false;
                eb->fHandle = nullptr;
                eb->fError = eER_IncompatibleTypes;
                break;
            }
            h = new char *;
            *h = static_cast<char *>(std::calloc(1, std::max<sLONG_PTR>(1, eb->fParam2)));
            check(*h, "test handle allocation failed");
            handles[h] = eb->fParam2;
            eb->fHandle = h;
            break;
        case 2:
            check(handles.count(h), "dispose dead handle");
            std::free(*h); delete h; handles.erase(h);
            break;
        case 5: check(handles.count(h), "lock dead handle"); eb->fParam3 = reinterpret_cast<sLONG_PTR>(*h); break;
        case 6: check(handles.count(h), "unlock dead handle"); break;
        default: check(false, "unexpected handle operation");
        }
        break;
    }
    default:
        std::cout << "FAIL unexpected engine selector " << selector << "\n";
        std::exit(1);
    }
}

// ---- platform stubs (see ownership_platform.h) ----
@interface OwnershipURL : NSObject { @public NSString *path; }
@end
@implementation OwnershipURL
- (void)dealloc { [path release]; [super dealloc]; }
@end
@implementation OwnershipSBApplication
+ (id)applicationWithBundleIdentifier:(NSString *)identifier { check(false, "Scripting Bridge reached"); return nil; }
+ (id)applicationWithProcessIdentifier:(pid_t)pid { check(false, "Scripting Bridge reached"); return nil; }
@end
@implementation OwnershipRunningApplication
+ (NSArray *)runningApplicationsWithBundleIdentifier:(NSString *)identifier { check(false, "running apps reached"); return nil; }
- (pid_t)processIdentifier { return 0; }
@end
@implementation OwnershipFileManager
+ (OwnershipFileManager *)defaultManager { check(false, "file manager reached"); return nil; }
- (NSArray *)URLsForDirectory:(NSSearchPathDirectory)d inDomains:(NSSearchPathDomainMask)m { return nil; }
- (NSURL *)URLForDirectory:(NSSearchPathDirectory)d inDomain:(NSSearchPathDomainMask)m
         appropriateForURL:(NSURL *)u create:(BOOL)c error:(NSError **)e { return nil; }
@end
@implementation OwnershipOperationQueue
+ (OwnershipOperationQueue *)mainQueue { check(false, "operation queue reached"); return nil; }
@end
@implementation OwnershipPromiseReceiver
+ (NSArray *)readableDraggedTypes { check(false, "file promises reached"); return nil; }
- (void)receivePromisedFilesAtDestination:(NSURL *)d options:(NSDictionary *)o
                           operationQueue:(OwnershipOperationQueue *)q
                                   reader:(void (^)(NSURL *, NSError *))r { check(false, "file promises reached"); }
@end

CFURLRef OwnershipCreateURL(CFAllocatorRef, CFStringRef path, CFURLPathStyle style, Boolean)
{
    ++platformCalls;
    check(style == kCFURLPOSIXPathStyle, "POSIX input path");
    OwnershipURL *url = [[OwnershipURL alloc] init];
    url->path = [(NSString *)path copy];
    return (CFURLRef)url;
}
CFStringRef OwnershipCopyPath(CFURLRef url, CFURLPathStyle style)
{
    ++platformCalls;
    check(style == kCFURLHFSPathStyle, "HFS output path");
    NSString *posix = ((OwnershipURL *)url)->path;
    NSString *hfs = [@"Synthetic HD" stringByAppendingString:
                     [posix stringByReplacingOccurrencesOfString:@"/" withString:@":"]];
    return (CFStringRef)[hfs copy];
}
OSErr OwnershipAECreateDesc(DescType, const void *, Size, AEDesc *desc)
{
    ++platformCalls;
    desc->descriptorType = typeNull;
    desc->dataHandle = nullptr;
    return noErr;
}
OSErr OwnershipAEDisposeDesc(AEDesc *) { ++platformCalls; return noErr; }
OSStatus OwnershipAEPermission(const AEAddressDesc *, AEEventClass, AEEventID, Boolean)
{
    ++platformCalls;
    return procNotFound;
}
FSEventStreamRef OwnershipFSCreate(CFAllocatorRef, FSEventStreamCallback, FSEventStreamContext *,
                                   CFArrayRef, FSEventStreamEventId, CFTimeInterval,
                                   FSEventStreamCreateFlags)
{
    check(false, "FSEvents stream reached");
    return nullptr;
}
void OwnershipFSSchedule(FSEventStreamRef, CFRunLoopRef, CFStringRef) { check(false, "FSEvents reached"); }
Boolean OwnershipFSStart(FSEventStreamRef) { check(false, "FSEvents reached"); return false; }
void OwnershipFSStream(FSEventStreamRef) { check(false, "FSEvents reached"); }
Class OwnershipClassFromString(NSString *name)
{
    ++platformCalls;
    check([name isEqualToString:@"XMacNSView_saisierec"], "unexpected class lookup");
    return Nil; // no 4D view class: drag methods are never swizzled
}
Method OwnershipGetInstanceMethod(Class, SEL) { ++swizzles; check(false, "runtime swizzle reached"); return nullptr; }
IMP OwnershipSetImplementation(Method, IMP) { ++swizzles; check(false, "runtime swizzle reached"); return nullptr; }
void OwnershipLog(NSString *, ...) { ++logs; }

// ---- scenarios ----
static void accept(PA_long32 on)
{
    PA_Unistring m = PA_CreateUnistring(const_cast<PA_Unichar *>(methodName.c_str()));
    PA_Unistring c = PA_CreateUnistring(const_cast<PA_Unichar *>(contextText.c_str()));
    void *params[] = {&on, &m, &c};
    PluginBlock block = {};
    block.fParameters = params;
    PluginMain(1, &block);
    // Parameters are borrowed by the plugin: the caller still owns and releases them.
    PA_DisposeUnistring(&m);
    PA_DisposeUnistring(&c);
}
static void deliver(const std::vector<std::string> &posix, const std::vector<FSEventStreamEventFlags> &flags)
{
    NSMutableArray *paths = [NSMutableArray array];
    for (auto &p : posix) [paths addObject:[NSString stringWithUTF8String:p.c_str()]];
    std::vector<FSEventStreamEventId> ids(posix.size(), 0);
    gotEvent(nullptr, nullptr, posix.size(), (void *)paths, flags.data(), ids.data());
}
static void listenerCase(const std::string &text, const std::string &mode)
{
    PluginBlock init = {};
    PluginMain(kInitPlugin, &init);
    check(platformCalls == 6 && logs == 2, "startup permission probes are synthetic");
    accept(1);
    check(newProcesses == 1 && loopProc && mainCalls == 1, "listener registration");
    check(swizzles == 0, "no drag swizzle without a 4D view");

    std::string base = "/tmp/afp-" + text;
    const FSEventStreamEventFlags renamed = kFSEventStreamEventFlagItemIsFile | kFSEventStreamEventFlagItemRenamed;
    const FSEventStreamEventFlags created = kFSEventStreamEventFlagItemIsFile | kFSEventStreamEventFlagItemCreated;
    const FSEventStreamEventFlags dir = kFSEventStreamEventFlagItemIsDir | kFSEventStreamEventFlagItemRenamed;
    deliver({base + "-1.eml", base + "-created.eml", base + "-dir", base + "-2.eml"},
            {renamed, created, dir, renamed | kFSEventStreamEventFlagItemModified});
    for (auto suffix : {"-1.eml", "-2.eml"}) {
        std::string hfs = "Synthetic HD" + base + suffix;
        for (auto &ch : hfs) if (ch == '/') ch = ':';
        expectedPaths.push_back(utf16(hfs));
    }
    check(FilePromise::PATHS == expectedPaths, "queued exact UTF-16 paths");

    stopDuringLoop = mode == "stop";
    reinterpret_cast<void (*)()>(loopProc)();
    check(killed == 1 && mainCalls == 2 && FilePromise::METHOD_PROCESS_ID == 0, "listener termination");
    check(FilePromise::PATHS.empty(), "pending paths cleared");
    size_t wanted = stopDuringLoop ? 1 : 2;
    size_t delivered = result == "handle" ? 0 : wanted;
    check(attempts == static_cast<int>(wanted), "callback attempts");
    check(receivedPaths.size() == delivered, "callback count");
    check(methodCalls + commandCalls == static_cast<int>(delivered), "engine callback count");

    PluginMain(kDeinitPlugin, &init);
    check(newProcesses == 1, "no extra process after deinit");
}

int main(int argc, char **argv)
{
    @autoreleasepool {
        if (argc != 6) { std::cout << "usage: case text empty branch result\n"; return 2; }
        gCall4D = host;
        std::string name = argv[1];
        std::string text = argv[2];
        nullEmpty = std::string(argv[3]) == "null";
        branch = argv[4];
        result = argv[5];
        check(branch == "method" || branch == "command", "branch");
        methodName = utf16(branch == "method" ? "cb" : "missing \xe6\x97\xa5");
        contextText = utf16(text);
        if (name == "probe-double-dispose") {
            // Wrong fix: dispose the local descriptor after the variable took it.
            PA_Variable v = PA_CreateVariable(eVK_Undefined);
            PA_Unistring s = PA_CreateUnistring(const_cast<PA_Unichar *>(contextText.c_str()));
            PA_SetStringVariable(&v, &s);
            PA_ClearVariable(&v);
            PA_DisposeUnistring(&s);
        } else if (name == "probe-poisoned-clear") {
            // Wrong fix: clear a failed (unspecified) callback result.
            PA_Variable v;
            short error = 0;
            result = "error";
            setResult(&v, &error);
            PA_ClearVariable(&v);
        } else if (name == "probe-unexpected-selector") {
            PA_GetCommandID(const_cast<PA_Unichar *>(contextText.c_str()));
        } else {
            check(name == "listener" || name == "stop", "case");
            listenerCase(text, name);
        }
        std::cout << "COUNTS strings=" << strings.size() << " pictures=" << pictures.size()
                  << " handles=" << handles.size() << " callbacks=" << receivedPaths.size() << "\n";
        check(strings.empty(), "unbalanced string allocations");
        check(pictures.empty(), "unbalanced picture references");
        check(handles.empty(), "unbalanced handles");
        std::cout << "PASS " << name << " strings=0 pictures=0 handles=0\n";
    }
    return 0;
}
