/*
 * outlook.h
 */

#import <AppKit/AppKit.h>
#import <ScriptingBridge/ScriptingBridge.h>


@class outlookItem, outlookWindow, OutlookApplication, outlookMainWindow, outlookDraftWindow, outlookObject, outlookCategorizableObject, outlookTodoableObject, outlookFolder, outlookCategory, outlookContactsPanel, outlookDirectorySource, outlookMeetingRoomMru, outlookAttachment, outlookAlertsCenter, outlookAlert, outlookNavigationNode, outlookItemListNode, outlookAccount, outlookExchangeAccount, outlookPopAccount, outlookImapAccount, outlookLdapAccount, outlookDelegatedAccount, outlookOtherUsersFolderAccount, outlookRecipient, outlookMailFolder, outlookMessage, outlookIncomingMessage, outlookOutgoingMessage, outlookToRecipient, outlookCcRecipient, outlookBccRecipient, outlookMeetingMessage, outlookAddressBook, outlookContact, outlookGroup, outlookCalendar, outlookCalendarEvent, outlookAttendee, outlookRequiredAttendee, outlookOptionalAttendee, outlookResourceAttendee, outlookTaskFolder, outlookTask, outlookNoteFolder, outlookNote, outlookSchedule, outlookSignature, outlookDebugTest, outlookUnitTest, outlookImapFolder;

enum outlookSaveOptions {
	outlookSaveOptionsYes = 'yes ' /* Save the file. */,
	outlookSaveOptionsNo = 'no  ' /* Do not save the file. */,
	outlookSaveOptionsAsk = 'ask ' /* Ask the user whether or not to save the file. */
};
typedef enum outlookSaveOptions outlookSaveOptions;

// The print error reporting styles.
enum outlookPrintingErrorHandling {
	outlookPrintingErrorHandlingStandard = 'lwst' /* Standard PostScript error handling. */,
	outlookPrintingErrorHandlingDetailed = 'lwdt' /* Print a detailed report of PostScript errors. */
};
typedef enum outlookPrintingErrorHandling outlookPrintingErrorHandling;

// Indicates what tasks to print.
enum outlookTaskSelection {
	outlookTaskSelectionPrintAll = 'PAll' /* Print all tasks */,
	outlookTaskSelectionSelectedTask = 'Sele' /* Print selected task */,
	outlookTaskSelectionDueToday = 'DTod' /* print tasks due today */,
	outlookTaskSelectionDueThisWeek = 'DTWk' /* print tasks due this week */,
	outlookTaskSelectionIncomplete = 'ICom' /* print all incomplete tasks */,
	outlookTaskSelectionListView = 'LVie' /* print list view  */
};
typedef enum outlookTaskSelection outlookTaskSelection;

// Indicates the style in which contacts names will be printed.
enum outlookContactNameStyle {
	outlookContactNameStyleLastFirst = 'LaFi' /* Prints name as Last First */,
	outlookContactNameStyleFirstLast = 'FiLa' /* Prints contact name as First Last */,
	outlookContactNameStyleLastCommaFirst = 'LCFi' /* prints contact name as Last, First */
};
typedef enum outlookContactNameStyle outlookContactNameStyle;

// Indicates what contacts to print.
enum outlookContactSelection {
	outlookContactSelectionSelectedContact = 'PCSe' /* print selected contact. */,
	outlookContactSelectionAllContacts = 'AllC' /* print all contacts. */,
	outlookContactSelectionFlaggedContacts = 'FlgC' /* print flagged contacts. */
};
typedef enum outlookContactSelection outlookContactSelection;

// The search type to use when searching the contacst panel
enum outlookSearchType {
	outlookSearchTypeNameSearch = 'cpNS' /* Search for contacts by name */,
	outlookSearchTypeAllFields = 'cpAF' /* search for matches on several different fields */
};
typedef enum outlookSearchType outlookSearchType;

// The different view's that Outlook's main window can be in.
enum outlookMainWindowView {
	outlookMainWindowViewMailView = 'MaVw' /* Look at all mails. */,
	outlookMainWindowViewCalendarView = 'CaVw' /* Look at all events. */,
	outlookMainWindowViewContactView = 'CoVw' /* Look at all contacts. */,
	outlookMainWindowViewTaskView = 'TaVw' /* Look at all tasks. */,
	outlookMainWindowViewNoteView = 'NoVw' /* Look at all notes. */
};
typedef enum outlookMainWindowView outlookMainWindowView;

// The priority values.
enum outlookPriority {
	outlookPriorityPriorityHigh = 'PrHi' /* High priority. */,
	outlookPriorityPriorityNormal = 'PrNr' /* Normal priority. */,
	outlookPriorityPriorityLow = 'PrLo' /* Low priority. */
};
typedef enum outlookPriority outlookPriority;

// The flagged states.
enum outlookFlag {
	outlookFlagNotFlagged = 'FlNF' /* Not flagged. */,
	outlookFlagNotCompleted = 'FlNC' /* Flagged but not completed. */,
	outlookFlagCompleted = 'FlCo' /* Flagged and completed. */
};
typedef enum outlookFlag outlookFlag;

// The type of the email address.
enum outlookEmailAddressType {
	outlookEmailAddressTypeUnresolvedAddress = 'UnAd' /* Email address is not resolved. */,
	outlookEmailAddressTypeContactAddress = 'CoAd' /* Email address is a contact. */,
	outlookEmailAddressTypePublicGroupAddress = 'PuAd' /* Email address is a public group. */,
	outlookEmailAddressTypePrivateGroupAddress = 'PrAd' /* Email address is a private group. */
};
typedef enum outlookEmailAddressType outlookEmailAddressType;

// The options for when to mark an item as read.
enum outlookMarkAsReadBehavior {
	outlookMarkAsReadBehaviorAfterPreviewTime = 'MRPr' /* Mark an item as read after it has been previewed for a while. */,
	outlookMarkAsReadBehaviorAfterSelectionChange = 'MRSC' /* Mark an item as read when the selection changes. */,
	outlookMarkAsReadBehaviorAfterOpening = 'MROp' /* Mark an item as read when they are opened. */
};
typedef enum outlookMarkAsReadBehavior outlookMarkAsReadBehavior;

// The attribution style options.
enum outlookAttributionStyle {
	outlookAttributionStyleNone = 'none' /* No attribution. */,
	outlookAttributionStyleIncludeHeader = 'InHd' /* Use the header in attribution. */,
	outlookAttributionStyleIncludeCustom = 'InCs' /* Use a custom attribution. */
};
typedef enum outlookAttributionStyle outlookAttributionStyle;

// The downloading options for internet pictures.
enum outlookInternetPictureBehavior {
	outlookInternetPictureBehaviorAlways = 'Alws' /* Download pictures from the internet for all messages. */,
	outlookInternetPictureBehaviorOnlyFromContacts = 'FrCo' /* Download pictures from the internet only for messages from my contacts. */,
	outlookInternetPictureBehaviorNever = 'Neva' /* Never download pictures from the internet. */
};
typedef enum outlookInternetPictureBehavior outlookInternetPictureBehavior;

// Automatically add self when sending messages setting.
enum outlookSendToSelfBehavior {
	outlookSendToSelfBehaviorCc = 'CC  ' /* Add self as cc recipient. */,
	outlookSendToSelfBehaviorBcc = 'BCC ' /* Add self as bcc recipient. */
};
typedef enum outlookSendToSelfBehavior outlookSendToSelfBehavior;

// The position of the reading pane is displayed.
enum outlookReadingPanePosition {
	outlookReadingPanePositionDisplayRight = 'ilDc' /* The reading pane display is on the right. */,
	outlookReadingPanePositionDisplayBelow = 'ilDp' /* The reading pane display is on the bottom. */,
	outlookReadingPanePositionDisplayHidden = 'ilDh' /* The reading pane display is hidden. */
};
typedef enum outlookReadingPanePosition outlookReadingPanePosition;

// Fields for sorting on the main view item list
enum outlookItemListSort {
	outlookItemListSortStatusSort = 'ilSS' /* Sort by status (unread, repiled to, etc) */,
	outlookItemListSortPrioritySort = 'ilPS' /* sort by priority */,
	outlookItemListSortAttachmentSort = 'ilAS' /* sort by presence or absense of attachments */,
	outlookItemListSortFromSort = 'ilFS' /* Sort by the name and/or address of the sender */,
	outlookItemListSortToSort = 'ilTS' /* sort by the name(s) and/or address(es) of the recipient(s) */,
	outlookItemListSortSubjectSort = 'ilSU' /* sort by the subject of the email */,
	outlookItemListSortSentTimeSort = 'ilSE' /* sort by the date the message was sent */,
	outlookItemListSortReceivedTimeSort = 'ilRS' /* sort by the date the message was received */,
	outlookItemListSortFlagSort = 'ilFL' /* sort by the flag status of the record */,
	outlookItemListSortCategorySort = 'ilCS' /* sort by the categories on the records */,
	outlookItemListSortSizeSort = 'ilSI' /* sort by the size of the message */,
	outlookItemListSortFolderSort = 'ilFO' /* sort by the folder the record is in */,
	outlookItemListSortAccountSort = 'ilAC' /* sort by the account the record is in */,
	outlookItemListSortConversationSort = 'ilCO' /* sort by conversation */,
	outlookItemListSortDisplayNameSort = 'ilDS' /* sort by contact display name */,
	outlookItemListSortTitleSort = 'ilTI' /* sort by title of the note or task */,
	outlookItemListSortDueDateSort = 'ilDD' /* sort by the due date of the task */
};
typedef enum outlookItemListSort outlookItemListSort;

// The product activation state (full functionality, reduced functionality, offline demo)
enum outlookProductActivationState {
	outlookProductActivationStateFullFunctionality = 'pFFM' /* Outlook is fully functional */,
	outlookProductActivationStateReducedFunctionalityNeverActivated = 'pRFM' /* Outlook has never been activated. Online access is not allowed */,
	outlookProductActivationStateReducedFunctionalityDeactivated = 'pRFD' /* Outlook's license is expired or inactive. Online access is not allowed */,
	outlookProductActivationStateOfflineDemo = 'pOFD' /* Outlook is licensed in offline demo mode. Online access is not allowed. */
};
typedef enum outlookProductActivationState outlookProductActivationState;

// The message digest algorithm for encryption.
enum outlookSecurityMessageDigestAlgorithm {
	outlookSecurityMessageDigestAlgorithmSHA512 = 'S512' /* Secure Hash Algorithm (SHA-2 family) with digest size 512. */,
	outlookSecurityMessageDigestAlgorithmSHA384 = 'S384' /* Secure Hash Algorithm (SHA-2 family) with digest size 384. */,
	outlookSecurityMessageDigestAlgorithmSHA256 = 'S256' /* Secure Hash Algorithm (SHA-2 family) with digest size 256. */,
	outlookSecurityMessageDigestAlgorithmSHA1 = 'SHA1' /* Secure Hash Algorithm (SHA-1 family). */
};
typedef enum outlookSecurityMessageDigestAlgorithm outlookSecurityMessageDigestAlgorithm;

// The message encryption algorithm for encryption.
enum outlookSecuritySymmetricEncryptionAlgorithm {
	outlookSecuritySymmetricEncryptionAlgorithmAES256 = 'A256' /* Advanced Encryption Standard with 256-bit key. */,
	outlookSecuritySymmetricEncryptionAlgorithmAES192 = 'A192' /* Advanced Encryption Standard with 192-bit key. */,
	outlookSecuritySymmetricEncryptionAlgorithmAES128 = 'A128' /* Advanced Encryption Standard with 128-bit key. */,
	outlookSecuritySymmetricEncryptionAlgorithmTripleDES = '3DES' /* Triple Data Encryption Standard algorithm. */
};
typedef enum outlookSecuritySymmetricEncryptionAlgorithm outlookSecuritySymmetricEncryptionAlgorithm;

// The type of the exchange account (primary, delegated or other users folder).
enum outlookExchangeAccountType {
	outlookExchangeAccountTypePrimaryAccount = 'ePrm' /* This is a primary account. */,
	outlookExchangeAccountTypeDelegatedAccount = 'eDlg' /* This is a delegated account. */,
	outlookExchangeAccountTypeOtherUsersFolderAccount = 'eOth' /* This is another users folder acount. */
};
typedef enum outlookExchangeAccountType outlookExchangeAccountType;

// Type of a recipient (to, cc, bcc).
enum outlookRecipientType {
	outlookRecipientTypeToRecipientType = 'RTto' /* To recipient. */,
	outlookRecipientTypeCcRecipientType = 'RTcc' /* CC (carbon copy) recipient. */,
	outlookRecipientTypeBccRecipientType = 'RTbc' /* BCC (blind carbon copy) recipient. */
};
typedef enum outlookRecipientType outlookRecipientType;

// The type of the meeting message (request, response, etc.)
enum outlookMeetingMessageType {
	outlookMeetingMessageTypeRequestMeetingType = 'MTrq' /* Meeting request (invite, update). */,
	outlookMeetingMessageTypeResponseMeetingType = 'MTre' /* Meeting response message. */,
	outlookMeetingMessageTypeCancellationMeetingType = 'MTca' /* Meeting cancellation message */,
	outlookMeetingMessageTypeCounterProposalMeetingType = 'MTco' /* Meeting response with a proposed new time. */
};
typedef enum outlookMeetingMessageType outlookMeetingMessageType;

// The type of e-mail address.
enum outlookContactEmailAddressType {
	outlookContactEmailAddressTypeHome = 'eHme' /* Home e-mail address. */,
	outlookContactEmailAddressTypeWork = 'eWrk' /* Work e-mail address. */,
	outlookContactEmailAddressTypeOther = 'eOth' /* Other e-mail address. */
};
typedef enum outlookContactEmailAddressType outlookContactEmailAddressType;

// The type of instant messaging address.
enum outlookContactIMAddressType {
	outlookContactIMAddressTypeDotNet = 'eDNt' /* Dot net IM address. */,
	outlookContactIMAddressTypeSIP = 'eSIp' /* SIP IM address. */,
	outlookContactIMAddressTypeImOther = 'eIOt' /* Other IM address. */
};
typedef enum outlookContactIMAddressType outlookContactIMAddressType;

// Describes what sort of contact this is (person, Distribution list, meeting room, etc)
enum outlookContactDisplayType {
	outlookContactDisplayTypePerson = 'cdtP' /* This contact is a person. The default option if no other information is available. */,
	outlookContactDisplayTypeDistributionList = 'cdtD' /* This contact is a distribution list. */,
	outlookContactDisplayTypePrivateDistributionList = 'cdPD' /* This contact is a private distribution list. */,
	outlookContactDisplayTypeMeetingRoom = 'cdpR' /* This contact is a meeting room. */,
	outlookContactDisplayTypeEquipment = 'cpdE' /* This contact is a piece of equipment, like a projector. */
};
typedef enum outlookContactDisplayType outlookContactDisplayType;

// The free busy status for an event.
enum outlookFreeBusyStatus {
	outlookFreeBusyStatusBusy = 'eSBu' /* Busy. */,
	outlookFreeBusyStatusFree = 'eSFr' /* Free. */,
	outlookFreeBusyStatusTentative = 'eSTe' /* Tentative. */,
	outlookFreeBusyStatusOutOfOffice = 'eSOO' /* Out of office. */
};
typedef enum outlookFreeBusyStatus outlookFreeBusyStatus;

// Types for the end dates of recurrences. End dates can either be numbered, dated, or non-existant. 
enum outlookEndDataType {
	outlookEndDataTypeNoEndType = 'eNEt' /* Recurrence never ends */,
	outlookEndDataTypeEndDateType = 'eEDt' /* Recurrence ends after a specific date */,
	outlookEndDataTypeEndNumberedType = 'eENt' /* Recurrence ends after a number of occurrences */
};
typedef enum outlookEndDataType outlookEndDataType;

// Recurrence types
enum outlookRecurrencePatternType {
	outlookRecurrencePatternTypeDaily = 'eRdp' /* A daily recurrence pattern */,
	outlookRecurrencePatternTypeWeekly = 'eRwp' /* A weekly recurrence pattern */,
	outlookRecurrencePatternTypeRelativeMonthly = 'eRrm' /* A relative monthly recurrence pattern */,
	outlookRecurrencePatternTypeAbsoluteMonthly = 'eRam' /* An absolute monthly recurrence pattern */,
	outlookRecurrencePatternTypeRelativeYearly = 'eRry' /* A relative yearly recurrence pattern */,
	outlookRecurrencePatternTypeAbsoluteYearly = 'eRay' /* An absolute yearly recurrence pattern */
};
typedef enum outlookRecurrencePatternType outlookRecurrencePatternType;

// Type of an attendee (required, optional, resource)
enum outlookAttendeeType {
	outlookAttendeeTypeRequiredAttendeeType = 'ATrq' /* Required attendee. */,
	outlookAttendeeTypeOptionalAttendeeType = 'ATop' /* Optional attendee. */,
	outlookAttendeeTypeResourceAttendeeType = 'ATrs' /* Resource attendee. */
};
typedef enum outlookAttendeeType outlookAttendeeType;

// Attendee acceptance status.
enum outlookAcceptanceStatus {
	outlookAcceptanceStatusAccepted = 'ASac' /* Attendee accepted. */,
	outlookAcceptanceStatusTentativelyAccepted = 'ASte' /* Attendee tentatively accepted. */,
	outlookAcceptanceStatusDeclined = 'ASde' /* Attendee declined. */,
	outlookAcceptanceStatusNotResponded = 'ASnr' /* Attendee hasn't responded. */
};
typedef enum outlookAcceptanceStatus outlookAcceptanceStatus;

// Address type of an attendee (unresolved address, contact address, public group address, private group address)
enum outlookAttendeeAddressType {
	outlookAttendeeAddressTypeUnresolvedAttendee = 'unAd' /* Unresolved address type */,
	outlookAttendeeAddressTypeContactAttendee = 'cnAd' /* Contact address type */,
	outlookAttendeeAddressTypePublicGroupAttendee = 'pgAd' /* Public group address type */,
	outlookAttendeeAddressTypePrivateGroupAttendee = 'paAd' /* Private group address type */
};
typedef enum outlookAttendeeAddressType outlookAttendeeAddressType;

@protocol outlookGenericMethods

- (void) open;  // Open an object.
- (void) closeSaving:(outlookSaveOptions)saving savingIn:(NSURL *)savingIn;  // Close an object.
- (void) delete;  // Delete an object. Messages and folders are moved to the corresponding deleted items folder (See permanently delete command in the Outlook Suite).
- (void) duplicateTo:(SBObject *)to withProperties:(NSDictionary *)withProperties;  // Copy object(s) and put the copies at a new location.
- (BOOL) exists;  // Verify if an object exists.
- (void) moveTo:(SBObject *)to;  // Move object(s) to a new location.
- (void) saveIn:(NSURL *)in_ as:(NSString *)as;  // Save an object.
- (void) run;  // Open application.
- (void) printWithProperties:(NSDictionary *)withProperties;  // Print an object.

@end



/*
 * Standard Suite
 */

// A scriptable object.
@interface outlookItem : SBObject <outlookGenericMethods>

@property (copy) NSDictionary *properties;  // All of the object's properties.


@end

// A window.
@interface outlookWindow : outlookItem

@property (copy) NSString *name;  // The full title of the window.
- (NSInteger) id;  // The unique identifier of the window.
@property NSRect bounds;  // The bounding rectangle of the window.
@property (readonly) BOOL closeable;  // Whether the window has a close box.
@property (readonly) BOOL titled;  // Whether the window has a title bar.
@property NSInteger index;  // The index of the window in the back-to-front window ordering.
@property (copy, readonly) outlookObject *object;  // The record displayed in the window (if there is one).
@property (readonly) BOOL floating;  // Whether the window floats.
@property (readonly) BOOL miniaturizable;  // Whether the window can be miniaturized.
@property BOOL miniaturized;  // Whether the window is currently miniaturized.
@property (readonly) BOOL modal;  // Whether the window is the application's current modal window.
@property (readonly) BOOL resizable;  // Whether the window can be resized.
@property BOOL visible;  // Whether the window is currently visible.
@property (readonly) BOOL zoomable;  // Whether the window can be zoomed.
@property BOOL zoomed;  // Whether the window is currently zoomed.


@end



/*
 * Outlook Suite
 */

// Outlook's top level scripting object.
@interface OutlookApplication : SBApplication

- (SBElementArray *) addressBooks;
- (SBElementArray *) calendars;
- (SBElementArray *) calendarEvents;
- (SBElementArray *) categories;
- (SBElementArray *) contacts;
- (SBElementArray *) draftWindows;
- (SBElementArray *) exchangeAccounts;
- (SBElementArray *) folders;
- (SBElementArray *) groups;
- (SBElementArray *) imapAccounts;
- (SBElementArray *) incomingMessages;
- (SBElementArray *) ldapAccounts;
- (SBElementArray *) mailFolders;
- (SBElementArray *) mainWindows;
- (SBElementArray *) messages;
- (SBElementArray *) notes;
- (SBElementArray *) noteFolders;
- (SBElementArray *) outgoingMessages;
- (SBElementArray *) popAccounts;
- (SBElementArray *) tasks;
- (SBElementArray *) taskFolders;
- (SBElementArray *) schedules;
- (SBElementArray *) signatures;
- (SBElementArray *) windows;

@property (copy, readonly) NSArray *currentMessages;  // The current messages depending on the context.
@property (copy) outlookAccount *defaultAccount;  // The default account.
@property (readonly) BOOL frontmost;  // Is this the frontmost (active) application?
@property (copy) outlookContact *meContact;  // The contact that represents the user.
@property (copy, readonly) NSString *name;  // The name of the application.
@property (copy, readonly) outlookFolder *onMyComputer;  // The On My Computer folder.
@property (copy) outlookFolder *selectedFolder;  // The folder that is currently selected in the frontmost main window.
@property (copy) NSArray *selectedObjects;  // The list of objects that are currently selected in the frontmost main window.
@property (copy) NSString *selectedText;  // The text that is currently selected in the frontmost window.
@property (copy) id selection;  // The selection in the frontmost window.
@property (copy, readonly) outlookSchedule *sendAll;  // The send all schedule.
@property (copy, readonly) outlookSchedule *sendAndReceiveAll;  // The send and receive all schedule.
@property (copy, readonly) NSString *version;  // The version of the application.
@property BOOL workingOffline;  // Is Outlook working offline (not connected to the internet)?
@property (copy, readonly) outlookMailFolder *inbox;  // The inbox folder of the default account.
@property (copy, readonly) outlookMailFolder *drafts;  // The drafts folder of the default account.
@property (copy, readonly) outlookMailFolder *sentItems;  // The sent items folder of the default account.
@property (copy, readonly) outlookMailFolder *deletedItems;  // The deleted items folder of the default account.
@property (copy, readonly) outlookMailFolder *junkMail;  // The junk mail folder of the default account.
@property (copy, readonly) outlookMailFolder *outbox;  // The outbox folder of the default account.
@property (copy, readonly) NSURL *currentIdentityFolder;  // The folder of the current identity being used by OutLook.
@property BOOL groupSimilarFolders;  // Indicates whether to group similar folders (Inbox, Drafts, etc.) from different accounts.
@property BOOL hideOnMyComputerFolders;  // Indicates whether the on my computer folders are hidden.
@property BOOL displayAlerts;  // Indicates whether to display a desktop alert when new mail arrives.
@property BOOL alertShowsPreview;  // Indicates whether the alert displayed when new mail arrives includes a preview of the mail.
@property BOOL bounceDockIcon;  // Indicates whether to bounce the dock icon when new mail arrives.
@property BOOL playSoundOnNewMessage;  // Indicates whether to play sound when a new message arrives.
@property BOOL playSoundOnMessageSend;  // Indicates whether to play sound when a message is sent.
@property BOOL playSoundOnReminder;  // Indicates whether to play sound when a reminder fires.
@property BOOL playSoundOnSyncError;  // Indicates whether to play sound when an error occurs while syncing.
@property BOOL playSoundOnNoNewMessages;  // Indicates whether to play sound if there are no new messages.
@property BOOL playSoundOnStartup;  // Indicates whether to play sound when Outlook starts up.
@property BOOL categorizeMessageFromContacts;  // Indicates whether messages are automatically assigned categories based on the categores of the sender's contact.
@property BOOL showNewCategoriesInNavigationPane;  // Indicates whether to automatically show new categories in the navigation pane.
@property outlookMarkAsReadBehavior markItemAsRead;  // Indicates when to mark an item as read.
@property NSInteger markAsReadPreviewDuration;  // The number of seconds to preview an item before marking it as read (applicable only when mark item as read is set to mark as read after preview).
@property BOOL expandSingleConversation;  // Indicates whether to expand only one conversation at a time.
@property BOOL expandConversationOnSelect;  // Indicates whether to automatically expand a conversation when selected.
@property BOOL highlightMessagesFromConversation;  // Indicates whether to highlight messages from the same conversation.
@property BOOL hideImapMessagesMarkedForDeletion;  // Indicates whether to hide IMAP messages which are marked for deletion.
@property outlookInternetPictureBehavior internetPictureBehavior;  // Indicates when to download internet pictures.
@property BOOL indentLinesOfOriginalHtmlMessage;  // Indicates whether to indent each line of the original message when replying or forwarding a message in HTML.
@property BOOL placeCursorBeforeOriginalHtmlMessage;  // Indicates whether to place the cursor before the original message when replying or forwarding a message in HTML.
@property outlookAttributionStyle attributionOfHtmlMessage;  // The attribution style to use for HTML messages.
@property BOOL indentLinesOfOriginalPlainTextMessage;  // Indicates whether to indent each line of the original message when replying or forwarding a message in plain text.
@property BOOL placeCursorBeforeOriginalPlainTextMessage;  // Indicates whether to place the cursor before the original message when replying or forwarding a message in plain text.
@property outlookAttributionStyle attributionOfPlainTextMessage;  // The attribution style to use for plain text messages.
@property BOOL composeMessagesInHtmlByDefault;  // Indicates whether new messages are created in HTML by default.
@property BOOL retainFormatOnRepliesOrForwards;  // Indicates whether to use the format of the original message when replying or forwarding.
@property BOOL useDefaultAccountForRepliesAndForwards;  // Indicates whether to use the default account when replying or forwarding.
@property BOOL closeWindowAfterReplyAndForward;  // Indicates whether to close the original message window when replying or forwarding.
@property BOOL includeMyselfWhenSending;  // Indicates whether to add myself when sending messages.
@property outlookSendToSelfBehavior includeMyselfAs;  // The option to use when including myself when sending messages.
@property (copy, readonly) outlookAlertsCenter *sharedAlertsCenter;
@property (copy, readonly) outlookContactsPanel *sharedContactsPanel;  // Access to the Contacts Panel, a window that can be used to search various directory sources for contacts.
@property (copy, readonly) outlookMeetingRoomMru *sharedMeetingRoomMru;  // Global mru of your recently used meeting rooms and locations.
@property BOOL systemDefaultEverythingApplication;  // Property returns YES if Outlook is the system default for mailto: links. Setting this value to YES makes Outlook the default application for mailto:, eml, ics, and vcf files and their appropriate UTI types. Setting this value to NO throws an error
@property BOOL systemDefaultMailApplication;  // Returns whether Outlook is the default application for mailto: links. Setting it to true makes Outlook the default for mailto: and for eml files. Setting this property to false makes mail.app the defalut
@property BOOL systemDefaultCalendarApplication;  // Returns whether Outlook is the default handler for ics files. Setting it to false makes ical the default handler
@property BOOL systemDefaultAddressBookApplication;  // Returns whether Outlook is the default handler for vcf files. Setting it to false makes Address Book the default
@property outlookProductActivationState activationState;  // The current product activation state of the application
@property NSInteger defaultTextEncoding;  // The default text encoding to use when creating new draft messages. use 0 for 'automatic'.  Use 21 for UTF-8. Other values are not publicly exposed.

- (void) open:(id)x;  // Open an object.
- (void) quitSaving:(outlookSaveOptions)saving;  // Quit an application.
- (void) run;  // Open application.
- (BOOL) importOlm:(NSURL *)x;  // Import an olm archive.
- (BOOL) importPst:(NSURL *)x;  // Import a pst archive.
- (BOOL) importRge:(NSURL *)x;  // Import a rge archive.
- (BOOL) importIdentity:(NSURL *)x;  // Import a 2004/2008/EWS identity folder.
- (void) clearRecentRecipients;  // Remove all recent email recipients.
- (NSDictionary *) autodiscoverEmailAddress:(NSString *)emailAddress domain:(NSString *)domain userName:(NSString *)userName password:(NSString *)password;  // Autodiscover the settings for an exchange account.
- (outlookIncomingMessage *) importEml:(NSURL *)x to:(SBObject *)to;  // Import an eml file.
- (BOOL) openContactEmailAddress:(NSString *)emailAddress;  // Open a contact in Outlook.
- (NSArray *) expand:(id)x exchangeAccount:(outlookExchangeAccount *)exchangeAccount;  // Queries DL expansion information for a distribution list.
- (id) loadImageForceUpdate:(BOOL)forceUpdate emailAddress:(NSDictionary *)emailAddress contact:(outlookContact *)contact;  // Load an image using OLImageLoader
- (NSArray *) importVcf:(NSURL *)x to:(SBObject *)to;  // Import a vcf file.
- (void) goTo:(NSDate *)x;  // Switches the current calendar view to the date specified.
- (NSArray *) importIcs:(NSURL *)x to:(SBObject *)to;  // Import an ics file.
- (void) handleURL:(NSString *)x;  // Handle a mailto URL.

@end

// A main window.
@interface outlookMainWindow : outlookWindow

- (SBElementArray *) navigationNodes;
- (SBElementArray *) objects;
- (SBElementArray *) itemListNodes;

@property outlookMainWindowView view;  // Main window's view (mail, calendar, task, etc).
@property outlookReadingPanePosition readingPanePosition;  // The display position of the reading pane.
@property BOOL sortedInGroups;  // Whether the item list is sorted by groups
@property (copy) NSDictionary *itemListSort;  // Description of the sort order of the item list


@end

// A message draft window.
@interface outlookDraftWindow : outlookWindow


@end

// An abstract class representing an Outlook item (message, contact, etc.). It is never used directly.
@interface outlookObject : outlookItem

- (NSInteger) id;  // The unique ID of a record.


@end

// An abstract class representing an object that can be categorizable. It is never used directly.
@interface outlookCategorizableObject : outlookObject

@property (copy) NSArray *categories;  // The categories of an categorizable object.


@end

// An abstract class representing an object that can be a todo. It is never used directly.
@interface outlookTodoableObject : outlookCategorizableObject

@property outlookFlag todoFlag;  // The flagged status of a todoable object.
@property (copy) id startDate;  // The date when a todo is scheduled to begin.
@property (copy) id dueDate;  // The date when a todo is due.
@property (copy) id reminderDateTime;  // The date and time to remind about a todo.
@property (copy, readonly) NSDate *completedDate;  // The date when a todo was marked completed.


@end

// An abstract class that represents a folder. To create a new folder use mail folder, address book, calendar, task folder or note folder.
@interface outlookFolder : outlookObject

- (SBElementArray *) folders;
- (SBElementArray *) mailFolders;
- (SBElementArray *) addressBooks;
- (SBElementArray *) calendars;
- (SBElementArray *) taskFolders;
- (SBElementArray *) noteFolders;
- (SBElementArray *) objects;

@property (copy) NSString *name;  // The name of a folder.
@property (copy, readonly) outlookFolder *container;  // The containing folder of a folder.
@property (copy, readonly) outlookAccount *account;  // The account a folder belongs to.
@property (readonly) BOOL subscribed;  // Is this folder subscribed to? (applicable only to exchange public folders).
@property (copy, readonly) NSString *exchangeId;  // The exchange identifier of a folder (only applicable for folders belonging to exchange accounts).
@property (readonly) BOOL hasIdleThread;  // Does this folder have IDLE running on it? (applicable only to IMAP folders)

- (void) sync;  // Synchronize a folder or account.
- (void) repair;  // Repair message list of folder
- (void) purgeDeletedItems;  // Purge the deleted items in a folder
- (void) emptyCache;  // Empty the local cache of an exchange folder.

@end

// A category.
@interface outlookCategory : outlookObject

@property (copy) NSString *name;  // The name of a category.
@property (copy) NSColor *color;  // The color of a category.
@property BOOL showInNavigationPane;  // True if a category is displayed in navigation pane in Calendar, Contact, Task and Note Views.


@end

// The contacts panel, used for finding contacts from a variety of address books.
@interface outlookContactsPanel : outlookWindow

- (SBElementArray *) directorySources;
- (SBElementArray *) contacts;

@property (copy) NSString *searchString;  // The query to search on.
@property outlookSearchType searchType;  // the type of search to run.
@property (copy) outlookDirectorySource *currentSource;  // The current place searches will happen from.
@property (readonly) BOOL searching;  // True if the contacts panel is currently in progress of searching.
@property (copy, readonly) NSString *errorString;  // Text string for the current error condition (can be empty is all is well).


@end

@interface outlookDirectorySource : outlookItem

@property (copy, readonly) NSString *name;  // The name of a directory source.


@end

@interface outlookMeetingRoomMru : outlookItem

- (SBElementArray *) attendees;

@property (copy) NSArray *locations;  // List of recently used locations.


@end

// A file attached to a record.
@interface outlookAttachment : outlookItem

@property (copy, readonly) NSString *name;  // The name of an attachment.
@property (copy, readonly) NSString *contentType;  // The MIME type of an attachement (eg. text/plain).
@property (copy) NSURL *file;  // The associated file of an attachment (if there is one). This is writable only when making a new attachment using a file.
@property (readonly) NSInteger fileSize;  // The size of the associated file of an attachment (if there is one).


@end

// Owner of all toast alerts
@interface outlookAlertsCenter : outlookObject

- (SBElementArray *) alerts;

@property NSInteger duration;  // The length of time alerts stay on the screen before they fade out
@property (copy) outlookMessage *showAlert;  // Set this property to show an alert with the message.
@property BOOL closeAlerts;  // Closes all open alerts immediately
@property NSInteger overflowTimeout;  // The length of time the alerts center will block alerts after receiving too many all at once


@end

// A single alert being displayed on the screen
@interface outlookAlert : outlookItem

@property (copy, readonly) NSString *subject;  // The subject line of the alert
@property (copy, readonly) NSString *preview;  // The body snippet displayed in the alert window
@property (copy, readonly) outlookMessage *content;  // The message whose info is shown by the alert
@property (copy, readonly) outlookWindow *window;  // The window that displays the alert
@property (copy, readonly) NSString *sender;  // The sender string shown in the window
@property BOOL openContent;  // set this to true to open the message in its own window
@property BOOL followup;  // set to true to activate the 'followup' button in the alert
@property BOOL deleteContent;  // set to true to activate the 'delete' button in the alert


@end

// And item in the navigation pane hierarchy
@interface outlookNavigationNode : outlookItem

- (SBElementArray *) navigationNodes;

@property BOOL expanded;  // Whether the node is currently expanded
@property (copy, readonly) NSString *title;  // The displayed name of the node
@property (copy, readonly) id folder;  // The folder this node represents. Maybe missing value if this node does not represent a folder
@property (copy, readonly) id account;  // The account this node represents. May be missing value if this node does not represent an account
@property (copy, readonly) id category;  // The category this node represents. May be missing value if thisn ode does not represent a category
@property (readonly) BOOL checked;  // true if this node has a check-state and is checked. Otherwise false
@property (readonly) NSInteger badgeCount;  // The number being shown in the unread/count badge, or 0 if no badge is shown
@property (readonly) BOOL displayNameIsBold;  // YES if the display name is currently showing bold, NO otherwise
@property (readonly) NSInteger selectionPointX;  // The x-coordinate point on the screen where you can click to select the node. Only works properly if the node is visible on the screen.
@property (readonly) NSInteger selectionPointY;  // The y-coordinate point on the screen where you can click to select the node. Only works properly if the node is visible on the screen.
@property (readonly) NSInteger checkboxPointX;  // The x-coordinate point on the screen where you can click to check the checkbox. Only works properly if the node is visible on the screen
@property (readonly) NSInteger checkboxPointY;  // The y-coordinate point on the screen where you can click to check the checkbox. Only works properly if the node is visible on the screen
@property BOOL visible;  // true if this row is completely visible, false if it is at all obscured


@end

// A node in the item list hierarchy
@interface outlookItemListNode : outlookItem

- (SBElementArray *) itemListNodes;

@property (readonly) BOOL expandable;  // Whether this node can be expanded
@property BOOL expanded;  // Whether the node is currently expanded
@property BOOL visible;  // Whether the node is currently visible on the screen. Setting this property to true scrolls the item list to make it visible if possible (ie if all of this node's ancestors are expanded)
@property (copy, readonly) outlookObject *representedRecord;  // The record represented by this node, displayed in this node's row in the item list. Will be missing if this node is a header node
@property (copy, readonly) NSString *headerName;  // The label for this group header node, if this node represents a group header
@property (readonly) NSInteger unreadCount;  // The badged count displayed in this header if it is a header node. 0 if it is not
@property (readonly) NSInteger selectionPointX;  // The x coordinate of the center of the node's location on screen. Only useful if the node's row is actually visible on screen (not hidden under collapsed parents our by the scroll view)
@property (readonly) NSInteger selectionPointY;  // The y coordinate of the center of the node's location on screen. Only useful if the node's row is actually visible on screen (not hidden under collapsed parents our by the scroll view)


@end



/*
 * Account Suite
 */

// An abstract class that represents an account. To create new accounts use exchange account, pop account or imap account.
@interface outlookAccount : outlookObject

- (SBElementArray *) folders;
- (SBElementArray *) mailFolders;
- (SBElementArray *) addressBooks;
- (SBElementArray *) calendars;
- (SBElementArray *) taskFolders;
- (SBElementArray *) noteFolders;

@property (copy) NSString *name;  // The name of an account.
@property (copy) NSString *fullName;  // The users display name.
@property (copy) NSString *emailAddress;  // The e-mail address of an account.
@property (copy) NSString *userName;  // The user name used to connect to an account.
@property (copy) NSString *password;  // The password for an account (write-only).
@property (copy) NSString *server;  // The server used to connect to an account.
@property NSInteger port;  // The port used to connect to an account.
@property BOOL useSsl;  // Indicates whether a SSL connection is used for an account.
@property (copy, readonly) outlookMailFolder *inbox;  // The inbox folder of an account.
@property (copy, readonly) outlookMailFolder *drafts;  // The drafts folder of an account. Some subclasses may allow writing this property.
@property (copy, readonly) outlookMailFolder *sentItems;  // The sent items folder of an account. Some subclasses may allow writing this property.
@property (copy, readonly) outlookMailFolder *deletedItems;  // The deleted items folder of an account. Some subclasses may allow writing this property.
@property (copy, readonly) outlookMailFolder *junkMail;  // The junk mail folder of an account. Some subclasses may allow writing this property.
@property (copy, readonly) outlookMailFolder *outbox;  // The outbox folder of an account.
@property (copy, readonly) outlookCalendar *defaultCalendar;  // The default calendar for the account
@property (copy, readonly) outlookAddressBook *defaultAddressBook;  // The default address book for the account
@property (copy, readonly) outlookNoteFolder *defaultNoteFolder;  // The default note folder for the account
@property (copy, readonly) outlookTaskFolder *defaultTaskFolder;  // The default task folder for the account

- (void) sync;  // Synchronize a folder or account.

@end

// An exchange account.
@interface outlookExchangeAccount : outlookAccount

- (SBElementArray *) delegatedAccounts;
- (SBElementArray *) otherUsersFolderAccounts;

@property (copy) NSString *domain;  // The domain of an exchange account.
@property (readonly) outlookExchangeAccountType exchangeType;  // The type of an exchange account.
@property (readonly) BOOL isConnected;  // Indicates whether an exchange account is online.
@property (copy) NSString *principal;  // The GSSAPI principal name (Kerberos v5).
@property (copy) NSString *galDownloadDirectory;  // The directory from which to download Offline Address Book files
@property BOOL receivePartialMessages;  // Indicates whether an exchange account gets partial messages.
@property BOOL signMessageByDefault;  // Indicates whether outgoing messages are signed by default.
@property BOOL includeSigningCertificate;  // Indicates whether outgoing messages include your signing certificate.
@property BOOL encryptMessageByDefault;  // Indicates whether outgoing messages are encrypted by default.
@property outlookSecurityMessageDigestAlgorithm messageDigestAlgorithm;  // The message digest algorithm to use.
@property outlookSecuritySymmetricEncryptionAlgorithm symmetricEncryptionAlgorithm;  // The symmetric encryption algorithm to use.
@property BOOL useKerberosAuthentication;  // Indicates whether kerberos authentication is used for an account.
@property (copy) NSString *ldapServer;  // The LDAP server for an exchange account.
@property NSInteger ldapPort;  // The LDAP port for an exchange account.
@property BOOL ldapUseSsl;  // Indicates whether a SSL connection is used connect to the LDAP server.
@property BOOL ldapNeedsAuthentication;  // Indicates whether authentication is needed for  connecting to the LDAP server.
@property (copy) NSString *ldapSearchBase;  // The search base of the LDAP server.
@property NSInteger ldapMaxEntries;  // The maximum number of entires to get from the LDAP server.
@property BOOL backgroundAutodiscover;  // Indicates whether background autodisover is enabled or not.
@property (copy, readonly) outlookDirectorySource *oab;  // The directory source for this account's Offline Address Book, if it has one. Use with the contacts panel to search the OAB.
@property (copy, readonly) outlookDirectorySource *rooms;  // The directory source for this account's meeting room index, if it has one. Use with the contacts panel to run searches
@property (copy, readonly) NSString *serverVersion;  // The version of the Exchange server.
@property (copy, readonly) NSDate *lastPasswordExpirationCheckTime;  // The time the password expiration was last checked.
@property (readonly) BOOL isMicrosoftOnline;  // Indicates whether an account is a Microsoft Online account.

- (id) queryPasswordExpiration;  // Query the password expiration for this account.
- (NSArray *) searchForName:(NSString *)forName;  // Search for contacts using an account.
- (id) imageSearchForEmailAddress:(NSDictionary *)forEmailAddress;  // Retrieve a contact image
- (id) queryOrganizationInformationFor:(outlookContact *)for_;  // Retreive the organization information for a contact.
- (NSArray *) queryFreebusyForAttendees:(NSArray *)forAttendees rangeStartTime:(NSDate *)rangeStartTime rangeEndTime:(NSDate *)rangeEndTime interval:(NSInteger)interval;  // Queries free/busy information from an exchange server for a set of attendees.

@end

// A POP account.
@interface outlookPopAccount : outlookAccount

@property BOOL useSecurePassword;  // Indicates whether a POP account always sends the password securely.
@property BOOL deleteMessagesFromServer;  // Indicates whether a POP account deletes messages on the server after downloading.
@property NSInteger deleteMessagesFromServerAfter;  // Number of days after which the downloaded messages will be deleted from the server.
@property BOOL deleteMessagesFromServerWhenDeletedFromComputer;  // Indicates whether messages will be deleted from the server when they are deleted from the computer.
@property (copy) NSString *smtpServer;  // The SMTP server used to send mail.
@property NSInteger smtpPort;  // The port to use when connecting to the SMTP server.
@property BOOL smtpUseSsl;  // Indicates whether a SSL connection is used connect to the SMTP server.
@property BOOL smtpNeedsAuthentication;  // Indicates whether authentication is needed for sending mail using the SMTP server.
@property BOOL smtpUseAccountAuthentication;  // Indicates whether to use the account user name and password for authentication with the SMTP server.
@property (copy) NSString *smtpUserName;  // The user name used to connect to the SMTP server.
@property (copy) NSString *smtpPassword;  // The password used to connect to the SMTP server (write-only).


@end

// An IMAP account.
@interface outlookImapAccount : outlookAccount

@property BOOL useSecurePassword;  // Indicates whether an IMAP account always sends the password securely.
@property (copy) NSString *rootFolderPath;  // The path to an IMAP account's root folder.
@property BOOL downloadMessageBodies;  // Indicates if message bodies are downloaded completely.
@property BOOL downloadCompleteMessagesInInboxOnly;  // Indicates if only messages in Inbox are downloaded completely.
@property BOOL imapIdle;  // Indicates whether to enable idle support.
@property BOOL imapIdleTimeout;  // Indicates whether IMAP idle timeout is enabled or not.
@property NSInteger imapIdleTimeoutTime;  // Number of minutes of inactivity after which the connection to the server times out.
@property NSInteger messagesExpireAfter;  // Number of days after which downloaded messages are considered expired.
@property BOOL deleteExpiredMessagesOnQuit;  // Indicates whether expired messages are deleted when quitting.
@property BOOL deleteAllMessagesOnQuit;  // Indicates wether all messages are deleted when quitting.
@property BOOL emptyDeletedItemsFolderOnQuit;  // Indicates whether the messages in the deleted items folder are deleted when quitting.
@property BOOL purgeMessagesOnQuit;  // Indicates whether messages marked as deleted are permanently erased on quit.
@property BOOL storeSentItemsOnServer;  // Indicates whether sent messages will be stored on the IMAP server.
@property BOOL storeDraftsOnServer;  // Indicates whether draft messages will be stored on the IMAP server.
@property BOOL storeDeletedItemsOnServer;  // Indicates whether deleted messages will be stored on the IMAP server.
@property BOOL storeJunkMailOnServer;  // Indicates whether junk mail will be stored on the IMAP server.
@property (copy) NSString *smtpServer;  // The SMTP server used to send mail.
@property NSInteger smtpPort;  // The port to use when connecting to the SMTP server.
@property BOOL smtpUseSsl;  // Indicates whether a SSL connection is used connect to the SMTP server.
@property BOOL smtpNeedsAuthentication;  // Indicates whether authentication is needed for sending mail using the SMTP server.
@property BOOL smtpUseAccountAuthentication;  // Indicates whether to use the account user name and password for authentication with the SMTP server.
@property (copy) NSString *smtpUserName;  // The user name used to connect to the SMTP server.
@property (copy) NSString *smtpPassword;  // The password used to connect to the SMTP server (write-only).
@property (copy) outlookMailFolder *sentItems;  // The sent items folder of an imap account.
@property (copy) outlookMailFolder *drafts;  // The drafts folder of an imap account.
@property (copy) outlookMailFolder *junkMail;  // The junk mail folder of an imap account.
@property (copy) outlookMailFolder *deletedItems;  // The deleted items folder of an imap account.
@property BOOL saveSentItems;  // Indicates whether whether we will save sent items.


@end

// A LDAP account.
@interface outlookLdapAccount : outlookObject

@property (copy) NSString *name;  // The name of a LDAP account.
@property (copy) NSString *userName;  // The user name used to connect to a LDAP account.
@property (copy) NSString *password;  // The password for a LDAP account (write-only).
@property (copy) NSString *server;  // The server used to connect to a LDAP account.
@property NSInteger port;  // The port used to connect to a LDAP account.
@property BOOL useSsl;  // Indicates whether a SSL connection is used for a LDAP account.
@property BOOL needsAuthentication;  // Indicates whether authentication is needed for  connecting to a LDAP server.
@property BOOL useKerberosAuthentication;  // Indicates whether kerberos authentication is used for a LDAP account.
@property (copy) NSString *principal;  // The GSSAPI principal name (Kerberos v5).
@property NSInteger searchTimeout;  // The number of seconds to wait before timing out.
@property (copy) NSString *searchBase;  // The search base of a LDAP account.
@property BOOL useSimpleSearch;  // Indicates whether the server should use simplied search string.
@property NSInteger maximumEntries;  // The maximum number of entires to get from the server for a LDAP account.

- (NSArray *) searchForName:(NSString *)forName;  // Search for contacts using an account.

@end

// A delegated exchange account.
@interface outlookDelegatedAccount : outlookExchangeAccount

@property (copy, readonly) outlookExchangeAccount *exchangeAccount;  // The exchange account which this account belongs to.


@end

// An other users folder account.
@interface outlookOtherUsersFolderAccount : outlookExchangeAccount

@property (copy, readonly) outlookExchangeAccount *exchangeAccount;  // The exchange account which this account belongs to.
@property BOOL inboxOpen;  // Indicates whether the other users Inbox is open.
@property BOOL addressBookOpen;  // Indicates whether the other users Address Book is open.
@property BOOL calendarOpen;  // Indicates whether the other users Calendar is open.


@end



/*
 * Mail Suite
 */

// An e-mail recipient.
@interface outlookRecipient : outlookItem

@property (copy) NSDictionary *emailAddress;  // The e-mail address of a recipient.
@property (readonly) outlookRecipientType type;  // The type of a recipient (to, cc, bcc).


@end

// A mail folder. It contains e-mails.
@interface outlookMailFolder : outlookFolder

- (SBElementArray *) messages;
- (SBElementArray *) incomingMessages;
- (SBElementArray *) outgoingMessages;
- (SBElementArray *) meetingMessages;

@property (readonly) NSInteger unreadCount;  // The number of unread messages in a folder.


@end

// An e-mail message.
@interface outlookMessage : outlookTodoableObject

- (SBElementArray *) recipients;
- (SBElementArray *) toRecipients;
- (SBElementArray *) ccRecipients;
- (SBElementArray *) bccRecipients;
- (SBElementArray *) attachments;

@property (copy) NSString *subject;  // The subject of a message.
@property (copy) NSString *content;  // The HTML content of a message.
@property (copy) NSString *plainTextContent;  // The content of a message as plain text.
@property (copy, readonly) NSDate *timeReceived;  // The time at which a message was received.
@property (copy, readonly) NSDate *timeSent;  // The time at which a message was sent.
@property (copy) NSDictionary *sender;  // The e-mail address of the sender of a message.
@property outlookPriority priority;  // The priority of a message.
@property (copy, readonly) NSString *headers;  // The header of a message.
@property (copy) NSString *source;  // The raw source text of a message.
@property (readonly) BOOL hasHtml;  // Indicates whether a message has html text?
@property (copy, readonly) NSDate *modificationDate;  // The date on which a message was last modified.
@property BOOL smimeSigned;  // Indicates whether a message is signed?
@property BOOL smimeEncrypted;  // Indicates whether a message is encrypted?
@property (readonly) BOOL isMeeting;  // Indicates whether  a message is a meeting invite/update?
@property (copy, readonly) outlookMailFolder *folder;  // The folder in which a message is filed.
@property (copy) outlookAccount *account;  // The account associated with a message.
@property BOOL isRead;  // Indicates whether a message has been read.
@property BOOL isMarkedForDelete;  // Indicates whether a message is marked for delete.
@property (readonly) BOOL repliedTo;  // Indicates whether a message has been replied to.
@property (readonly) BOOL repliedToAll;  // Indicates whether a message has been replied to all.
@property (readonly) BOOL forwarded;  // Indicates whether a message has been forwarded.
@property (readonly) BOOL redirected;  // Indicates whether a message has been redirected.
@property (readonly) BOOL edited;  // Indicates whether a message has been edited.
@property (copy, readonly) NSString *exchangeId;  // The exchange identifier of a message (only applicable for messages belonging to exchange accounts).
@property (readonly) NSInteger imapUid;  // The IMAP UID of a message (only applicable for messages belonging to IMAP accounts).
@property (readonly) BOOL isPartiallyDownloaded;  // Indicates whether a message is partially downloaded.
@property (readonly) BOOL isRightsProtected;  // Indicates whether a message is rights protected.
@property (readonly) BOOL isReadReceiptRequested;  // Indicates whether the message has a read receipt requested.
@property BOOL sendReadReceipt;  // Indicates whether a read receipt response can be sent for a message. This needs to be set before marking a message as read.

- (void) permanentlyDelete;  // Permanently delete a message or folder i.e. without moving it to the deleted items folder (See delete command in the Standard Suite).
- (void) send;  // Send e-mail messages.
- (outlookMessage *) replyToOpeningWindow:(BOOL)openingWindow replyToAll:(BOOL)replyToAll;  // Create a reply message.
- (outlookMessage *) forwardTo:(NSString *)to openingWindow:(BOOL)openingWindow;  // Creates a forwarded message.

@end

// A message recieved by the user.
@interface outlookIncomingMessage : outlookMessage


@end

// A message created by the user.
@interface outlookOutgoingMessage : outlookMessage

@property (readonly) BOOL wasSent;  // Indicates whether a outgoing message has been sent.


@end

// An e-mail recipient in the "To" field.
@interface outlookToRecipient : outlookRecipient


@end

// An e-mail recipient in the "Cc" field.
@interface outlookCcRecipient : outlookRecipient


@end

// An e-mail recipient in the "Bcc" field.
@interface outlookBccRecipient : outlookRecipient


@end

// A meeting message recieved by the user.
@interface outlookMeetingMessage : outlookIncomingMessage

@property (readonly) outlookMeetingMessageType type;  // The type of the meeting message (request, response, etc.)
@property (copy, readonly) outlookCalendarEvent *meeting;  // The meeting data from the meeting message.

- (void) acceptInviteSendingResponse:(BOOL)sendingResponse comment:(NSString *)comment;  // Accepts a given incoming meeting message. 
- (void) acceptTentativelyInviteSendingResponse:(BOOL)sendingResponse comment:(NSString *)comment;  // Accepts tentatively a given incoming meeting message. 
- (void) declineInviteSendingResponse:(BOOL)sendingResponse comment:(NSString *)comment;  // Declines a given incoming meeting message. 

@end



/*
 * Contact Suite
 */

// An address book. It contains contacts.
@interface outlookAddressBook : outlookFolder

- (SBElementArray *) contacts;
- (SBElementArray *) groups;

@property (copy, readonly) outlookDirectorySource *directorySource;  // The directory source for this folder. Use it with the Contacts Panel to run searches


@end

// A contact.
@interface outlookContact : outlookTodoableObject

@property (copy) NSString *firstName;  // The first name of a contact.
@property (copy) NSString *lastName;  // The last name of a contact.
@property (copy) NSString *middleName;  // The middle name of a contact.
@property (copy, readonly) NSString *displayName;  // The display name of a contact.
@property (copy) NSString *title;  // The title for a contact.
@property (copy) NSString *nickname;  // The nickname of a contact.
@property (copy) NSString *suffix;  // The name suffix of a contact.
@property (copy) NSString *phone;  // The default phone number of a contact.
@property (copy) NSString *homePhoneNumber;  // The home phone number of a contact.
@property (copy) NSString *otherHomePhoneNumber;  // The second home phone number of a contact.
@property (copy) NSString *homeFaxNumber;  // The home fax number of a contact.
@property (copy) NSString *businessPhoneNumber;  // The business phone number of a contact.
@property (copy) NSString *otherBusinessPhoneNumber;  // The second business phone number of a contact.
@property (copy) NSString *businessFaxNumber;  // The business fax number of a contact.
@property (copy) NSString *pagerNumber;  // The pager number of a contact.
@property (copy) NSString *mobileNumber;  // The cell phone number of a contact.
@property (copy) NSArray *emailAddresses;  // The e-mail addresses of a contact.
@property (copy) NSArray *IMAddresses;  // The instant messaging addresses of a contact.
@property (copy) NSString *office;  // The office of a contact.
@property (copy) NSString *company;  // The name of the company of a contact.
@property (copy) NSString *jobTitle;  // The job title of a contact.
@property (copy) NSString *department;  // The department of a contact.
@property (copy) NSString *assistantPhoneNumber;  // The phone number of the assistant of a contact.
@property NSInteger age;  // The age of a contact.
@property (copy) NSDate *anniversary;  // The anniversary of a contact.
@property (copy) NSString *astrologySign;  // The astrological sign of a contact.
@property (copy) NSDate *birthday;  // The birthday of a contact.
@property (copy) NSString *bloodType;  // The blood group of a contact.
@property (copy) NSString *objectDescription;  // The notes of a contact.
@property (copy) NSString *homeStreetAddress;  // The home street address of a contact.
@property (copy) NSString *homeCity;  // The home city of a contact
@property (copy) NSString *homeState;  // The home state of a contact.
@property (copy) NSString *homeCountry;  // The home country of a contact.
@property (copy) NSString *homeZip;  // The home zip code of a contact.
@property (copy) NSString *businessStreetAddress;  // The business street address of a contact.
@property (copy) NSString *businessCity;  // The business city of a contact
@property (copy) NSString *businessState;  // The business state of a contact.
@property (copy) NSString *businessCountry;  // The business country of a contact.
@property (copy) NSString *businessZip;  // The business zip code of a contact.
@property (copy) NSString *homeWebPage;  // The home web page of a contact.
@property (copy) NSString *businessWebPage;  // The business web page of a contact.
@property (copy) NSString *spouse;  // The name of the spouse of a contact.
@property (copy) NSArray *children;  // The list of children of a contact.
@property (copy) NSString *interests;  // The interests of a contact.
@property (copy) id image;  // The image of a contact.
@property BOOL isJapaneseContact;  // Indicates whether a contact is in Japanese format?
@property (copy) NSString *firstNameFurigana;  // The furigana for a contact's first name (Japanese format contact only).
@property (copy) NSString *lastNameFurigana;  // The furigana for a contact's last name (Japanese format contact only).
@property (copy) NSString *middleNameFurigana;  // The furigana for a contact's middle name (Japanese format contact only).
@property (copy) NSArray *childrenFurigana;  // The furigana name of the children of a contact (Japanese format contact only).
@property (copy) NSString *spouseFurigana;  // The furigana name of the spouse of a contact (Japanese format contact only).
@property (copy) NSString *companyFurigana;  // The furigana for the company of a contact (Japanese format contact only).
@property (copy) NSString *customFieldOne;  // The first custom field of a contact.
@property (copy) NSString *customFieldTwo;  // The first custom field of a contact.
@property (copy) NSString *customFieldThree;  // The third custom field of a contact.
@property (copy) NSString *customFieldFour;  // The fourth custom field of a contact.
@property (copy) NSString *customFieldFive;  // The fifth custom field of a contact.
@property (copy) NSString *customFieldSix;  // The sixth custom field of a contact.
@property (copy) NSString *customFieldSeven;  // The seventh custom field of a contact.
@property (copy) NSString *customFieldEight;  // The eigth custom field of a contact.
@property (copy) NSString *customPhone1;  // The first custom phone field of a contact.
@property (copy) NSString *customPhone2;  // The second custom phone field of a contact.
@property (copy) NSString *customPhone3;  // The third custom phone field of a contact.
@property (copy) NSString *customPhone4;  // The fourth custom phone field of a contact.
@property (copy) NSDate *customDateFieldOne;  // The first custom date field of a contact.
@property (copy) NSDate *customDateFieldTwo;  // The second custom date field of a contact.
@property (copy, readonly) outlookAddressBook *addressBook;  // The address book containing a contact.
@property (copy, readonly) NSString *exchangeId;  // The exchange identifier of a contact (only applicable for contacts belonging to exchange accounts).
@property (copy, readonly) NSDate *modificationDate;  // The date on which a contact was last modified.
@property (copy) NSString *note;  // The HTML notes for a contact.
@property (copy) NSString *plainTextNote;  // The notes for a contact as plain text.
@property outlookContactDisplayType displayType;  // The type of contact
@property (copy, readonly) NSString *vcardData;  // The information of a contact in vCard format. Use the 'import vcf' command to create a contact from vCard information.


@end

// A contact group.
@interface outlookGroup : outlookTodoableObject

@property (copy) NSString *name;  // The name of a group.
@property (copy) NSArray *members;  // The member of a group.
@property (copy, readonly) outlookAddressBook *addressBook;  // The address book containing a group.
@property (copy) NSString *note;  // The notes for a group.
@property (copy) NSString *plainTextNote;  // The notes for a group as plain text.
@property BOOL suppressMemberNames;  // Indicates whether the members names are suppressed when messages are to a group.
@property (copy, readonly) NSString *exchangeId;  // The exchange identifier of a group (only applicable for groups belonging to exchange accounts).
@property (copy, readonly) NSDate *modificationDate;  // The date on which a group was last modified.

- (NSArray *) expandExchangeAccount:(outlookExchangeAccount *)exchangeAccount;  // Queries DL expansion information for a distribution list.

@end



/*
 * Calendar Suite
 */

// A calendar folder. It contains events.
@interface outlookCalendar : outlookFolder

- (SBElementArray *) calendarEvents;


@end

// A calendar event.
@interface outlookCalendarEvent : outlookCategorizableObject

- (SBElementArray *) attendees;
- (SBElementArray *) requiredAttendees;
- (SBElementArray *) optionalAttendees;
- (SBElementArray *) resourceAttendees;
- (SBElementArray *) attachments;

@property (copy) NSString *subject;  // The subject of an event.
@property (copy) NSDate *startTime;  // The time at which an event begins.
@property (copy) NSDate *endTime;  // The time at which an event ends.
@property (copy) NSString *location;  // The location of an event.
@property (copy) NSString *content;  // The HTML description of the event.
@property (copy) NSString *plainTextContent;  // The description of the event as plain text.
@property (readonly) BOOL hasHtml;  // Indicates whether the description of the event has html text?
@property BOOL allDayFlag;  // A flag for whether or not an event is an all day event.
@property BOOL hasReminder;  // Indicates whether or not an event has a reminder.
@property NSInteger reminderTime;  // The number of minutes from the start time of an event when a reminder will fire (if it has one).
@property outlookFreeBusyStatus freeBusyStatus;  // The free/busy status for an event.
@property (copy, readonly) NSString *exchangeId;  // The exchange identifier of an event (only applicable for events belonging to exchange accounts).
@property (copy, readonly) NSDate *modificationDate;  // The date an event was last modified on.
@property (copy) id recurrence;  // The calendar recurrence.
@property (readonly) BOOL isRecurring;  // Indicates whehter an event is part of a recurring series of events.
@property (readonly) BOOL isOccurrence;  // Indicates whether an event is an occurrence of a recurring series.
@property (copy, readonly) NSDate *recurrenceId;  // The recurrence id of an event (the date at which the recurring event occurs).
@property (copy, readonly) outlookCalendarEvent *master;  // The master event for an event that is an exception from a recurring series.
@property BOOL isPrivate;  // Indicates whether an event is private.
@property (copy, readonly) outlookCalendar *calendar;  // The calendar folder in which an event is filed.
@property (copy, readonly) outlookAccount *account;  // The account associated with an event.
@property (copy, readonly) NSString *organizer;  // The organizer of the meeting.
@property (copy) NSDictionary *timezone;  // The timezone of an event.
@property BOOL requestResponses;  // Indicates whether a response is requested from the attendees of the meeting.
@property (copy, readonly) NSString *icalendarData;  // The information of an event in iCalendar format. Use the 'import ics' command to create an event from iCalendar information.

- (id) getOccurrenceOfAt:(NSDate *)at;  // Gets an occurrence of a calendar event specified by date.
- (void) sendMeetingFromAccount:(outlookAccount *)fromAccount;  // Send meeting invites/updates.
- (void) cancelMeeting;  // Cancel meeting and send out cancellations.
- (void) acceptMeetingComment:(NSString *)comment sendingResponse:(BOOL)sendingResponse;  // Accept meeting and send meeting response when needed.
- (void) acceptTentativelyMeetingComment:(NSString *)comment sendingResponse:(BOOL)sendingResponse;  // Accept tentatively meeting and send meeting response when needed.
- (void) declineMeetingComment:(NSString *)comment sendingResponse:(BOOL)sendingResponse;  // Decline meeting and send meeting response when needed.
- (outlookMessage *) replyToOpeningWindow:(BOOL)openingWindow replyToAll:(BOOL)replyToAll;  // Create a reply message to a meeting.
- (outlookMessage *) forwardTo:(NSString *)to openingWindow:(BOOL)openingWindow;  // Creates a forwarded meeting message.

@end

// A meeting attendee.
@interface outlookAttendee : outlookItem

@property (copy) NSDictionary *emailAddress;  // The e-mail address of an attendee.
@property (readonly) outlookAttendeeType type;  // The type of a recipient (required, optional, resource)
@property (readonly) outlookAcceptanceStatus status;  // Attendee acceptance status.
@property (readonly) outlookAttendeeAddressType addressType;  // The address type of a recipient (unresolved address, contact address, public group address, private group address)


@end

// A required meeting attendee.
@interface outlookRequiredAttendee : outlookAttendee


@end

// An optional meeting attendee.
@interface outlookOptionalAttendee : outlookAttendee


@end

// A resource meeting attendee.
@interface outlookResourceAttendee : outlookAttendee


@end



/*
 * Task Suite
 */

// A task folder. It contains tasks.
@interface outlookTaskFolder : outlookFolder

- (SBElementArray *) tasks;


@end

// A task.
@interface outlookTask : outlookTodoableObject

@property (copy) NSString *name;  // The name of a task.
@property (copy) NSString *content;  // The HTML notes of a task.
@property (copy) NSString *plainTextContent;  // The notes of a task as plain text.
@property outlookPriority priority;  // The priority of a task.
@property (copy, readonly) outlookTaskFolder *folder;  // The folder in which a task is filed.
@property (copy, readonly) NSString *exchangeId;  // The exchange identifier of a task (only applicable for tasks belonging to exchange accounts).
@property (copy, readonly) NSDate *modificationDate;  // The date a task was last modified on.
@property (copy, readonly) NSString *icalendarData;  // The information of a task in iCalendar format. Use the 'import ics' command to create a task from iCalendar information.


@end



/*
 * Note Suite
 */

// A note folder. It contains notes.
@interface outlookNoteFolder : outlookFolder

- (SBElementArray *) notes;


@end

// A note.
@interface outlookNote : outlookCategorizableObject

@property (copy) NSString *name;  // The name of a note.
@property (copy) NSString *content;  // The HTML content of a note.
@property (copy) NSString *plainTextContent;  // The content of a note as plain text.
@property (copy, readonly) outlookNoteFolder *folder;  // The folder in which a note is filed.
@property (copy, readonly) NSDate *creationDate;  // The date a note was created on.
@property (copy, readonly) NSString *exchangeId;  // The exchange identifier of a note (only applicable for notes belonging to exchange accounts).
@property (copy, readonly) NSDate *modificationDate;  // The date a note was last modified on.
@property (copy, readonly) NSString *icalendarData;  // The information of a note in iCalendar format. Use the 'import ics' command to create a note from iCalendar information.


@end



/*
 * Schedules Suite
 */

// A schedule
@interface outlookSchedule : outlookObject

@property (copy) NSString *name;  // The name of a schedule.
@property BOOL enabled;  // Is a schedule enabled?
@property (copy, readonly) NSDate *lastRunTime;  // The last time schedule was run.


@end



/*
 * Signature Suite
 */

// A signature.
@interface outlookSignature : outlookObject

@property (copy) NSString *name;  // The name of a signature.
@property (copy) NSString *content;  // The HTML content of a signature.
@property (copy) NSString *plainTextContent;  // The content of a signature as plain text.
@property BOOL includeInRandom;  // Indicates whether the signature is included in the random list.


@end

