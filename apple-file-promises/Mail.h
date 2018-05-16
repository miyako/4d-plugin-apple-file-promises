/*
 * mail.h
 */

#import <AppKit/AppKit.h>
#import <ScriptingBridge/ScriptingBridge.h>


@class MailApplication, mailDocument, mailWindow, mailRichText, mailAttachment, mailParagraph, mailWord, mailCharacter, mailAttributeRun, mailOutgoingMessage, mailLdapServer, mailOLDMessageEditor, mailMessageViewer, mailSignature, mailMessage, mailAccount, mailImapAccount, mailICloudAccount, mailPopAccount, mailSmtpServer, mailMailbox, mailRule, mailRuleCondition, mailRecipient, mailBccRecipient, mailCcRecipient, mailToRecipient, mailContainer, mailHeader, mailMailAttachment;

enum mailSaveOptions {
	mailSaveOptionsYes = 'yes ' /* Save the file. */,
	mailSaveOptionsNo = 'no  ' /* Do not save the file. */,
	mailSaveOptionsAsk = 'ask ' /* Ask the user whether or not to save the file. */
};
typedef enum mailSaveOptions mailSaveOptions;

enum mailPrintingErrorHandling {
	mailPrintingErrorHandlingStandard = 'lwst' /* Standard PostScript error handling */,
	mailPrintingErrorHandlingDetailed = 'lwdt' /* print a detailed report of PostScript errors */
};
typedef enum mailPrintingErrorHandling mailPrintingErrorHandling;

enum mailSaveableFileFormat {
	mailSaveableFileFormatNativeFormat = 'item' /* Native format */
};
typedef enum mailSaveableFileFormat mailSaveableFileFormat;

enum mailDefaultMessageFormat {
	mailDefaultMessageFormatPlainFormat = 'dmpt' /* Plain Text */,
	mailDefaultMessageFormatRichFormat = 'dmrt' /* Rich Text */
};
typedef enum mailDefaultMessageFormat mailDefaultMessageFormat;

enum mailQuotingColor {
	mailQuotingColorBlue = 'ccbl' /* Blue */,
	mailQuotingColorGreen = 'ccgr' /* Green */,
	mailQuotingColorOrange = 'ccor' /* Orange */,
	mailQuotingColorOther = 'ccot' /* Other */,
	mailQuotingColorPurple = 'ccpu' /* Purple */,
	mailQuotingColorRed = 'ccre' /* Red */,
	mailQuotingColorYellow = 'ccye' /* Yellow */
};
typedef enum mailQuotingColor mailQuotingColor;

enum mailViewerColumns {
	mailViewerColumnsAttachmentsColumn = 'ecat' /* Column containing the number of attachments a message contains */,
	mailViewerColumnsMessageColor = 'eccl' /* Used to indicate sorting should be done by color */,
	mailViewerColumnsDateReceivedColumn = 'ecdr' /* Column containing the date a message was received */,
	mailViewerColumnsDateSentColumn = 'ecds' /* Column containing the date a message was sent */,
	mailViewerColumnsFlagsColumn = 'ecfl' /* Column containing the flags of a message */,
	mailViewerColumnsFromColumn = 'ecfr' /* Column containing the sender's name */,
	mailViewerColumnsMailboxColumn = 'ecmb' /* Column containing the name of the mailbox or account a message is in */,
	mailViewerColumnsMessageStatusColumn = 'ecms' /* Column indicating a messages status (read, unread, replied to, forwarded, etc) */,
	mailViewerColumnsNumberColumn = 'ecnm' /* Column containing the number of a message in a mailbox */,
	mailViewerColumnsSizeColumn = 'ecsz' /* Column containing the size of a message */,
	mailViewerColumnsSubjectColumn = 'ecsu' /* Column containing the subject of a message */,
	mailViewerColumnsToColumn = 'ecto' /* Column containing the recipients of a message */,
	mailViewerColumnsDateLastSavedColumn = 'ecls' /* Column containing the date a draft message was saved */
};
typedef enum mailViewerColumns mailViewerColumns;

enum mailAuthentication {
	mailAuthenticationPassword = 'axct' /* Clear text password */,
	mailAuthenticationApop = 'aapo' /* APOP */,
	mailAuthenticationKerberos5 = 'axk5' /* Kerberos V5 (GSSAPI) */,
	mailAuthenticationNtlm = 'axnt' /* NTLM */,
	mailAuthenticationMd5 = 'axmd' /* CRAM-MD5 */,
	mailAuthenticationExternal = 'aext' /* External authentication (TLS client certificate) */,
	mailAuthenticationAppleToken = 'atok' /* Apple token */,
	mailAuthenticationNone = 'ccno' /* None */
};
typedef enum mailAuthentication mailAuthentication;

enum mailHighlightColors {
	mailHighlightColorsBlue = 'ccbl' /* Blue */,
	mailHighlightColorsGray = 'ccgy' /* Gray */,
	mailHighlightColorsGreen = 'ccgr' /* Green */,
	mailHighlightColorsNone = 'ccno' /* None */,
	mailHighlightColorsOrange = 'ccor' /* Orange */,
	mailHighlightColorsOther = 'ccot' /* Other */,
	mailHighlightColorsPurple = 'ccpu' /* Purple */,
	mailHighlightColorsRed = 'ccre' /* Red */,
	mailHighlightColorsYellow = 'ccye' /* Yellow */
};
typedef enum mailHighlightColors mailHighlightColors;

enum mailMessageCachingPolicy {
	mailMessageCachingPolicyDoNotKeepCopiesOfAnyMessages = 'x9no' /* Do not use this option (deprecated). If you do, Mail will use the 'all messages but omit attachments' policy */,
	mailMessageCachingPolicyOnlyMessagesIHaveRead = 'x9wr' /* Do not use this option (deprecated). If you do, Mail will use the 'all messages but omit attachments' policy */,
	mailMessageCachingPolicyAllMessagesButOmitAttachments = 'x9bo' /* All messages but omit attachments */,
	mailMessageCachingPolicyAllMessagesAndTheirAttachments = 'x9al' /* All messages and their attachments */
};
typedef enum mailMessageCachingPolicy mailMessageCachingPolicy;

enum mailRuleQualifier {
	mailRuleQualifierBeginsWithValue = 'rqbw' /* Begins with value */,
	mailRuleQualifierDoesContainValue = 'rqco' /* Does contain value */,
	mailRuleQualifierDoesNotContainValue = 'rqdn' /* Does not contain value */,
	mailRuleQualifierEndsWithValue = 'rqew' /* Ends with value */,
	mailRuleQualifierEqualToValue = 'rqie' /* Equal to value */,
	mailRuleQualifierLessThanValue = 'rqlt' /* Less than value */,
	mailRuleQualifierGreaterThanValue = 'rqgt' /* Greater than value */,
	mailRuleQualifierNone = 'rqno' /* Indicates no qualifier is applicable */
};
typedef enum mailRuleQualifier mailRuleQualifier;

enum mailRuleType {
	mailRuleTypeAccount = 'tacc' /* Account */,
	mailRuleTypeAnyRecipient = 'tanr' /* Any recipient */,
	mailRuleTypeCcHeader = 'tccc' /* Cc header */,
	mailRuleTypeMatchesEveryMessage = 'tevm' /* Every message */,
	mailRuleTypeFromHeader = 'tfro' /* From header */,
	mailRuleTypeHeaderKey = 'thdk' /* An arbitrary header key */,
	mailRuleTypeMessageContent = 'tmec' /* Message content */,
	mailRuleTypeMessageIsJunkMail = 'tmij' /* Message is junk mail */,
	mailRuleTypeSenderIsInMyContacts = 'tsii' /* Sender is in my contacts */,
	mailRuleTypeSenderIsInMyPreviousRecipients = 'tsah' /* Sender is in my previous recipients */,
	mailRuleTypeSenderIsMemberOfGroup = 'tsim' /* Sender is member of group */,
	mailRuleTypeSenderIsNotInMyContacts = 'tsin' /* Sender is not in my contacts */,
	mailRuleTypeSenderIsNotInMyPreviousRecipients = 'tnah' /* sender is not in my previous recipients */,
	mailRuleTypeSenderIsNotMemberOfGroup = 'tsig' /* Sender is not member of group */,
	mailRuleTypeSenderIsVIP = 'tsig' /* Sender is VIP */,
	mailRuleTypeSubjectHeader = 'tsub' /* Subject header */,
	mailRuleTypeToHeader = 'ttoo' /* To header */,
	mailRuleTypeToOrCcHeader = 'ttoc' /* To or Cc header */,
	mailRuleTypeAttachmentType = 'tatt' /* Attachment Type */
};
typedef enum mailRuleType mailRuleType;

enum mailTypeOfAccount {
	mailTypeOfAccountPop = 'etpo' /* POP */,
	mailTypeOfAccountSmtp = 'etsm' /* SMTP */,
	mailTypeOfAccountImap = 'etim' /* IMAP */,
	mailTypeOfAccountICloud = 'etit' /* iCloud */,
	mailTypeOfAccountUnknown = 'etun' /* Unknown */
};
typedef enum mailTypeOfAccount mailTypeOfAccount;

@protocol mailGenericMethods

- (void) closeSaving:(mailSaveOptions)saving savingIn:(NSURL *)savingIn;  // Close a document.
- (void) saveIn:(NSURL *)in_ as:(mailSaveableFileFormat)as;  // Save a document.
- (void) printWithProperties:(NSDictionary *)withProperties printDialog:(BOOL)printDialog;  // Print a document.
- (void) delete;  // Delete an object.
- (void) duplicateTo:(SBObject *)to withProperties:(NSDictionary *)withProperties;  // Copy an object.
- (void) moveTo:(SBObject *)to;  // Move an object to a new location.

@end



/*
 * Standard Suite
 */

// The application's top-level scripting object.
@interface MailApplication : SBApplication

- (SBElementArray *) documents;
- (SBElementArray *) windows;

@property (copy, readonly) NSString *name;  // The name of the application.
@property (readonly) BOOL frontmost;  // Is this the active application?
@property (copy, readonly) NSString *version;  // The version number of the application.

- (id) open:(id)x;  // Open a document.
- (void) print:(id)x withProperties:(NSDictionary *)withProperties printDialog:(BOOL)printDialog;  // Print a document.
- (void) quitSaving:(mailSaveOptions)saving;  // Quit the application.
- (BOOL) exists:(id)x;  // Verify that an object exists.
- (void) checkForNewMailFor:(mailAccount *)for_;  // Triggers a check for email.
- (NSString *) extractNameFrom:(NSString *)x;  // Command to get the full name out of a fully specified email address. E.g. Calling this with "John Doe <jdoe@example.com>" as the direct object would return "John Doe"
- (NSString *) extractAddressFrom:(NSString *)x;  // Command to get just the email address of a fully specified email address. E.g. Calling this with "John Doe <jdoe@example.com>" as the direct object would return "jdoe@example.com"
- (void) GetURL:(NSString *)x;  // Opens a mailto URL.
- (void) importMailMailboxAt:(NSURL *)at;  // Imports a mailbox created by Mail.
- (void) mailto:(NSString *)x;  // Opens a mailto URL.
- (void) performMailActionWithMessages:(NSArray *)x inMailboxes:(mailMailbox *)inMailboxes forRule:(mailRule *)forRule;  // Script handler invoked by rules and menus that execute AppleScripts. The direct parameter of this handler is a list of messages being acted upon.
- (void) synchronizeWith:(mailAccount *)with;  // Command to trigger synchronizing of an IMAP account with the server.

@end

// A document.
@interface mailDocument : SBObject <mailGenericMethods>

@property (copy, readonly) NSString *name;  // Its name.
@property (readonly) BOOL modified;  // Has it been modified since the last save?
@property (copy, readonly) NSURL *file;  // Its location on disk, if it has one.


@end

// A window.
@interface mailWindow : SBObject <mailGenericMethods>

@property (copy, readonly) NSString *name;  // The title of the window.
- (NSInteger) id;  // The unique identifier of the window.
@property NSInteger index;  // The index of the window, ordered front to back.
@property NSRect bounds;  // The bounding rectangle of the window.
@property (readonly) BOOL closeable;  // Does the window have a close button?
@property (readonly) BOOL miniaturizable;  // Does the window have a minimize button?
@property BOOL miniaturized;  // Is the window minimized right now?
@property (readonly) BOOL resizable;  // Can the window be resized?
@property BOOL visible;  // Is the window visible right now?
@property (readonly) BOOL zoomable;  // Does the window have a zoom button?
@property BOOL zoomed;  // Is the window zoomed right now?
@property (copy, readonly) mailDocument *document;  // The document whose contents are displayed in the window.


@end



/*
 * Text Suite
 */

// Rich (styled) text
@interface mailRichText : SBObject <mailGenericMethods>

- (SBElementArray *) paragraphs;
- (SBElementArray *) words;
- (SBElementArray *) characters;
- (SBElementArray *) attributeRuns;
- (SBElementArray *) attachments;

@property (copy) NSColor *color;  // The color of the first character.
@property (copy) NSString *font;  // The name of the font of the first character.
@property (copy) NSNumber *size;  // The size in points of the first character.


@end

// Represents an inline text attachment. This class is used mainly for make commands.
@interface mailAttachment : mailRichText

@property (copy) NSURL *fileName;  // The file for the attachment


@end

// This subdivides the text into paragraphs.
@interface mailParagraph : SBObject <mailGenericMethods>

- (SBElementArray *) words;
- (SBElementArray *) characters;
- (SBElementArray *) attributeRuns;
- (SBElementArray *) attachments;

@property (copy) NSColor *color;  // The color of the first character.
@property (copy) NSString *font;  // The name of the font of the first character.
@property (copy) NSNumber *size;  // The size in points of the first character.


@end

// This subdivides the text into words.
@interface mailWord : SBObject <mailGenericMethods>

- (SBElementArray *) characters;
- (SBElementArray *) attributeRuns;
- (SBElementArray *) attachments;

@property (copy) NSColor *color;  // The color of the first character.
@property (copy) NSString *font;  // The name of the font of the first character.
@property (copy) NSNumber *size;  // The size in points of the first character.


@end

// This subdivides the text into characters.
@interface mailCharacter : SBObject <mailGenericMethods>

- (SBElementArray *) attributeRuns;
- (SBElementArray *) attachments;

@property (copy) NSColor *color;  // The color of the character.
@property (copy) NSString *font;  // The name of the font of the character.
@property (copy) NSNumber *size;  // The size in points of the character.


@end

// This subdivides the text into chunks that all have the same attributes.
@interface mailAttributeRun : SBObject <mailGenericMethods>

- (SBElementArray *) paragraphs;
- (SBElementArray *) words;
- (SBElementArray *) characters;
- (SBElementArray *) attachments;

@property (copy) NSColor *color;  // The color of the first character.
@property (copy) NSString *font;  // The name of the font of the first character.
@property (copy) NSNumber *size;  // The size in points of the first character.


@end



/*
 * Mail
 */

// A new email message
@interface mailOutgoingMessage : SBObject <mailGenericMethods>

- (SBElementArray *) bccRecipients;
- (SBElementArray *) ccRecipients;
- (SBElementArray *) recipients;
- (SBElementArray *) toRecipients;

@property (copy) NSString *sender;  // The sender of the message
@property (copy) NSString *subject;  // The subject of the message
@property (copy) mailRichText *content;  // The contents of the message
@property BOOL visible;  // Controls whether the message window is shown on the screen. The default is false
@property (copy) id messageSignature;  // The signature of the message
- (NSInteger) id;  // The unique identifier of the message

- (BOOL) send;  // Sends a message.

@end

// Mail's top level scripting object.
@interface MailApplication (Mail)

- (SBElementArray *) accounts;
- (SBElementArray *) popAccounts;
- (SBElementArray *) imapAccounts;
- (SBElementArray *) iCloudAccounts;
- (SBElementArray *) smtpServers;
- (SBElementArray *) outgoingMessages;
- (SBElementArray *) mailboxes;
- (SBElementArray *) messageViewers;
- (SBElementArray *) rules;
- (SBElementArray *) signatures;

@property BOOL alwaysBccMyself;  // Indicates whether you will be included in the Bcc: field of messages which you are composing
@property BOOL alwaysCcMyself;  // Indicates whether you will be included in the Cc: field of messages which you are composing
@property (copy, readonly) NSArray *selection;  // List of messages that the user has selected
@property (copy, readonly) NSString *applicationVersion;  // The build number of the application
@property NSInteger fetchInterval;  // The interval (in minutes) between automatic fetches of new mail, -1 means to use an automatically determined interval
@property (readonly) NSInteger backgroundActivityCount;  // Number of background activities currently running in Mail, according to the Activity window
@property BOOL chooseSignatureWhenComposing;  // Indicates whether user can choose a signature directly in a new compose window
@property BOOL colorQuotedText;  // Indicates whether quoted text should be colored
@property mailDefaultMessageFormat defaultMessageFormat;  // Default format for messages being composed or message replies
@property BOOL downloadHtmlAttachments;  // Indicates whether images and attachments in HTML messages should be downloaded and displayed
@property (copy, readonly) mailMailbox *draftsMailbox;  // The top level Drafts mailbox
@property BOOL expandGroupAddresses;  // Indicates whether group addresses will be expanded when entered into the address fields of a new compose message
@property (copy) NSString *fixedWidthFont;  // Font for plain text messages, only used if 'use fixed width font' is set to true
@property double fixedWidthFontSize;  // Font size for plain text messages, only used if 'use fixed width font' is set to true
@property (copy, readonly) mailMailbox *inbox;  // The top level In mailbox
@property BOOL includeAllOriginalMessageText;  // Indicates whether all of the original message will be quoted or only the text you have selected (if any)
@property BOOL quoteOriginalMessage;  // Indicates whether the text of the original message will be included in replies
@property BOOL checkSpellingWhileTyping;  // Indicates whether spelling will be checked automatically in messages being composed
@property (copy, readonly) mailMailbox *junkMailbox;  // The top level Junk mailbox
@property mailQuotingColor levelOneQuotingColor;  // Color for quoted text with one level of indentation
@property mailQuotingColor levelTwoQuotingColor;  // Color for quoted text with two levels of indentation
@property mailQuotingColor levelThreeQuotingColor;  // Color for quoted text with three levels of indentation
@property (copy) NSString *messageFont;  // Font for messages (proportional font)
@property double messageFontSize;  // Font size for messages (proportional font)
@property (copy) NSString *messageListFont;  // Font for message list
@property double messageListFontSize;  // Font size for message list
@property (copy) NSString *newMailSound;  // Name of new mail sound or 'None' if no sound is selected
@property (copy, readonly) mailMailbox *outbox;  // The top level Out mailbox
@property BOOL shouldPlayOtherMailSounds;  // Indicates whether sounds will be played for various things such as when a messages is sent or if no mail is found when manually checking for new mail or if there is a fetch error
@property BOOL sameReplyFormat;  // Indicates whether replies will be in the same text format as the message to which you are replying
@property (copy) NSString *selectedSignature;  // Name of current selected signature (or 'randomly', 'sequentially', or 'none')
@property (copy, readonly) mailMailbox *sentMailbox;  // The top level Sent mailbox
@property BOOL fetchesAutomatically;  // Indicates whether mail will automatically be fetched at a specific interval
@property BOOL highlightSelectedConversation;  // Indicates whether messages in conversations should be highlighted in the Mail viewer window when not grouped
@property (copy, readonly) mailMailbox *trashMailbox;  // The top level Trash mailbox
@property BOOL useAddressCompletion;  // This always returns true, and setting it doesn't do anything (deprecated)
@property BOOL useFixedWidthFont;  // Should fixed-width font be used for plain text messages?
@property (copy, readonly) NSString *primaryEmail;  // The user's primary email address

@end

// Represents the object responsible for managing a viewer window
@interface mailMessageViewer : SBObject <mailGenericMethods>

- (SBElementArray *) messages;

@property (copy, readonly) mailMailbox *draftsMailbox;  // The top level Drafts mailbox
@property (copy, readonly) mailMailbox *inbox;  // The top level In mailbox
@property (copy, readonly) mailMailbox *junkMailbox;  // The top level Junk mailbox
@property (copy, readonly) mailMailbox *outbox;  // The top level Out mailbox
@property (copy, readonly) mailMailbox *sentMailbox;  // The top level Sent mailbox
@property (copy, readonly) mailMailbox *trashMailbox;  // The top level Trash mailbox
@property mailViewerColumns sortColumn;  // The column that is currently sorted in the viewer
@property BOOL sortedAscending;  // Whether the viewer is sorted ascending or not
@property BOOL mailboxListVisible;  // Controls whether the list of mailboxes is visible or not
@property BOOL previewPaneIsVisible;  // Controls whether the preview pane of the message viewer window is visible or not
@property (copy) NSArray *visibleColumns;  // List of columns that are visible. The subject column and the message status column will always be visible
- (NSInteger) id;  // The unique identifier of the message viewer
@property (copy) NSArray *visibleMessages;  // List of messages currently being displayed in the viewer
@property (copy) NSArray *selectedMessages;  // List of messages currently selected
@property (copy) NSArray *selectedMailboxes;  // List of mailboxes currently selected in the list of mailboxes
@property (copy) mailWindow *window;  // The window for the message viewer


@end

// Email signatures
@interface mailSignature : SBObject <mailGenericMethods>

@property (copy) NSString *content;  // Contents of email signature. If there is a version with fonts and/or styles, that will be returned over the plain text version
@property (copy) NSString *name;  // Name of the signature


@end



/*
 * Mail Framework
 */

// An email message
@interface mailMessage : SBObject <mailGenericMethods>

- (SBElementArray *) bccRecipients;
- (SBElementArray *) ccRecipients;
- (SBElementArray *) recipients;
- (SBElementArray *) toRecipients;
- (SBElementArray *) headers;
- (SBElementArray *) mailAttachments;

- (NSInteger) id;  // The unique identifier of the message.
@property (copy, readonly) NSString *allHeaders;  // All the headers of the message
@property mailHighlightColors backgroundColor;  // The background color of the message
@property (copy) mailMailbox *mailbox;  // The mailbox in which this message is filed
@property (copy, readonly) mailRichText *content;  // Contents of an email message
@property (copy, readonly) NSDate *dateReceived;  // The date a message was received
@property (copy, readonly) NSDate *dateSent;  // The date a message was sent
@property BOOL deletedStatus;  // Indicates whether the message is deleted or not
@property BOOL flaggedStatus;  // Indicates whether the message is flagged or not
@property NSInteger flagIndex;  // The flag on the message, or -1 if the message is not flagged
@property BOOL junkMailStatus;  // Indicates whether the message has been marked junk or evaluated to be junk by the junk mail filter.
@property BOOL readStatus;  // Indicates whether the message is read or not
@property (copy, readonly) NSString *messageId;  // The unique message ID string
@property (copy, readonly) NSString *source;  // Raw source of the message
@property (copy, readonly) NSString *replyTo;  // The address that replies should be sent to
@property (readonly) NSInteger messageSize;  // The size (in bytes) of a message
@property (copy, readonly) NSString *sender;  // The sender of the message
@property (copy, readonly) NSString *subject;  // The subject of the message
@property (readonly) BOOL wasForwarded;  // Indicates whether the message was forwarded or not
@property (readonly) BOOL wasRedirected;  // Indicates whether the message was redirected or not
@property (readonly) BOOL wasRepliedTo;  // Indicates whether the message was replied to or not

- (mailOutgoingMessage *) forwardOpeningWindow:(BOOL)openingWindow;  // Creates a forwarded message.
- (mailOutgoingMessage *) redirectOpeningWindow:(BOOL)openingWindow;  // Creates a redirected message.
- (mailOutgoingMessage *) replyOpeningWindow:(BOOL)openingWindow replyToAll:(BOOL)replyToAll;  // Creates a reply message.

@end

// A Mail account for receiving messages (POP/IMAP). To create a new receiving account, use the 'pop account', 'imap account', and 'iCloud account' objects
@interface mailAccount : SBObject <mailGenericMethods>

- (SBElementArray *) mailboxes;

@property (copy) id deliveryAccount;  // The delivery account used when sending mail from this account
@property (copy) NSString *name;  // The name of an account
@property (copy) NSString *password;  // Password for this account. Can be set, but not read via scripting
@property mailAuthentication authentication;  // Preferred authentication scheme for account
@property (readonly) mailTypeOfAccount accountType;  // The type of an account
@property (copy) NSArray *emailAddresses;  // The list of email addresses configured for an account
@property (copy) NSString *fullName;  // The users full name configured for an account
@property NSInteger emptyJunkMessagesFrequency;  // Number of days before junk messages are deleted (0 = delete on quit, -1 = never delete)
@property NSInteger emptySentMessagesFrequency;  // Number of days before archived sent messages are deleted (0 = delete on quit, -1 = never delete)
@property NSInteger emptyTrashFrequency;  // Number of days before messages in the trash are permanently deleted (0 = delete on quit, -1 = never delete)
@property BOOL emptyJunkMessagesOnQuit;  // Indicates whether the messages in the junk messages mailboxes will be deleted on quit
@property BOOL emptySentMessagesOnQuit;  // Indicates whether the messages in the sent messages mailboxes will be deleted on quit
@property BOOL emptyTrashOnQuit;  // Indicates whether the messages in deleted messages mailboxes will be permanently deleted on quit
@property BOOL enabled;  // Indicates whether the account is enabled or not
@property (copy) NSString *userName;  // The user name used to connect to an account
@property (copy, readonly) NSURL *accountDirectory;  // The directory where the account stores things on disk
@property NSInteger port;  // The port used to connect to an account
@property (copy) NSString *serverName;  // The host name used to connect to an account
@property BOOL includeWhenGettingNewMail;  // Indicates whether the account will be included when getting new mail
@property BOOL moveDeletedMessagesToTrash;  // Indicates whether messages that are deleted will be moved to the trash mailbox
@property BOOL usesSsl;  // Indicates whether SSL is enabled for this receiving account


@end

// An IMAP email account
@interface mailImapAccount : mailAccount

@property BOOL compactMailboxesWhenClosing;  // Indicates whether an IMAP mailbox is automatically compacted when you quit Mail or switch to another mailbox
@property mailMessageCachingPolicy messageCaching;  // Message caching setting for this account
@property BOOL storeDraftsOnServer;  // Indicates whether drafts will be stored on the IMAP server
@property BOOL storeJunkMailOnServer;  // Indicates whether junk mail will be stored on the IMAP server
@property BOOL storeSentMessagesOnServer;  // Indicates whether sent messages will be stored on the IMAP server
@property BOOL storeDeletedMessagesOnServer;  // Indicates whether deleted messages will be stored on the IMAP server


@end

// An iCloud or MobileMe email account
@interface mailICloudAccount : mailImapAccount


@end

// A POP email account
@interface mailPopAccount : mailAccount

@property NSInteger bigMessageWarningSize;  // If message size (in bytes) is over this amount, Mail will prompt you asking whether you want to download the message (-1 = do not prompt)
@property NSInteger delayedMessageDeletionInterval;  // Number of days before messages that have been downloaded will be deleted from the server (0 = delete immediately after downloading)
@property BOOL deleteMailOnServer;  // Indicates whether POP account deletes messages on the server after downloading
@property BOOL deleteMessagesWhenMovedFromInbox;  // Indicates whether messages will be deleted from the server when moved from your POP inbox


@end

// An SMTP account (for sending email)
@interface mailSmtpServer : SBObject <mailGenericMethods>

@property (copy, readonly) NSString *name;  // The name of an account
@property (copy) NSString *password;  // Password for this account. Can be set, but not read via scripting
@property (readonly) mailTypeOfAccount accountType;  // The type of an account
@property mailAuthentication authentication;  // Preferred authentication scheme for account
@property BOOL enabled;  // Indicates whether the account is enabled or not
@property (copy) NSString *userName;  // The user name used to connect to an account
@property NSInteger port;  // The port used to connect to an account
@property (copy) NSString *serverName;  // The host name used to connect to an account
@property BOOL usesSsl;  // Indicates whether SSL is enabled for this receiving account


@end

// A mailbox that holds messages
@interface mailMailbox : SBObject <mailGenericMethods>

- (SBElementArray *) mailboxes;
- (SBElementArray *) messages;

@property (copy) NSString *name;  // The name of a mailbox
@property (readonly) NSInteger unreadCount;  // The number of unread messages in the mailbox
@property (copy, readonly) mailAccount *account;
@property (copy, readonly) mailMailbox *container;


@end

// Class for message rules
@interface mailRule : SBObject <mailGenericMethods>

- (SBElementArray *) ruleConditions;

@property mailHighlightColors colorMessage;  // If rule matches, apply this color
@property BOOL deleteMessage;  // If rule matches, delete message
@property (copy) NSString *forwardText;  // If rule matches, prepend this text to the forwarded message. Set to empty string to include no prepended text
@property (copy) NSString *forwardMessage;  // If rule matches, forward message to this address, or multiple addresses, separated by commas. Set to empty string to disable this action
@property BOOL markFlagged;  // If rule matches, mark message as flagged
@property NSInteger markFlagIndex;  // If rule matches, mark message with the specified flag. Set to -1 to disable this action
@property BOOL markRead;  // If rule matches, mark message as read
@property (copy) NSString *playSound;  // If rule matches, play this sound (specify name of sound or path to sound)
@property (copy) NSString *redirectMessage;  // If rule matches, redirect message to this address or multiple addresses, separate by commas. Set to empty string to disable this action
@property (copy) NSString *replyText;  // If rule matches, reply to message and prepend with this text. Set to empty string to disable this action
@property (copy) id runScript;  // If rule matches, run this compiled AppleScript file. Set to empty string to disable this action
@property BOOL allConditionsMustBeMet;  // Indicates whether all conditions must be met for rule to execute
@property (copy) mailMailbox *copyMessage;  // If rule matches, copy to this mailbox
@property (copy) mailMailbox *moveMessage;  // If rule matches, move to this mailbox
@property BOOL highlightTextUsingColor;  // Indicates whether the color will be used to highlight the text or background of a message in the message list
@property BOOL enabled;  // Indicates whether the rule is enabled
@property (copy) NSString *name;  // Name of rule
@property BOOL shouldCopyMessage;  // Indicates whether the rule has a copy action
@property BOOL shouldMoveMessage;  // Indicates whether the rule has a move action
@property BOOL stopEvaluatingRules;  // If rule matches, stop rule evaluation for this message


@end

// Class for conditions that can be attached to a single rule
@interface mailRuleCondition : SBObject <mailGenericMethods>

@property (copy) NSString *expression;  // Rule expression field
@property (copy) NSString *header;  // Rule header key
@property mailRuleQualifier qualifier;  // Rule qualifier
@property mailRuleType ruleType;  // Rule type


@end

// An email recipient
@interface mailRecipient : SBObject <mailGenericMethods>

@property (copy) NSString *address;  // The recipients email address
@property (copy) NSString *name;  // The name used for display


@end

// An email recipient in the Bcc: field
@interface mailBccRecipient : mailRecipient


@end

// An email recipient in the Cc: field
@interface mailCcRecipient : mailRecipient


@end

// An email recipient in the To: field
@interface mailToRecipient : mailRecipient


@end

// A mailbox that contains other mailboxes.
@interface mailContainer : mailMailbox


@end

// A header value for a message. E.g. To, Subject, From.
@interface mailHeader : SBObject <mailGenericMethods>

@property (copy) NSString *content;  // Contents of the header
@property (copy) NSString *name;  // Name of the header value


@end

// A file attached to a received message.
@interface mailMailAttachment : SBObject <mailGenericMethods>

@property (copy, readonly) NSString *name;  // Name of the attachment
@property (copy, readonly) NSString *MIMEType;  // MIME type of the attachment E.g. text/plain.
@property (readonly) NSInteger fileSize;  // Approximate size in bytes.
@property (readonly) BOOL downloaded;  // Indicates whether the attachment has been downloaded.
- (NSString *) id;  // The unique identifier of the attachment.


@end

