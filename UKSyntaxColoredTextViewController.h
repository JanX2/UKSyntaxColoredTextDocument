//
//  UKSyntaxColoredTextViewController.h
//  UKSyntaxColoredDocument
//
//  Created by Uli Kusterer on 13.03.10.
//  Copyright 2010 Uli Kusterer.
//
//	This software is provided 'as-is', without any express or implied
//	warranty. In no event will the authors be held liable for any damages
//	arising from the use of this software.
//
//	Permission is granted to anyone to use this software for any purpose,
//	including commercial applications, and to alter it and redistribute it
//	freely, subject to the following restrictions:
//
//	   1. The origin of this software must not be misrepresented; you must not
//	   claim that you wrote the original software. If you use this software
//	   in a product, an acknowledgment in the product documentation would be
//	   appreciated but is not required.
//
//	   2. Altered source versions must be plainly marked as such, and must not be
//	   misrepresented as being the original software.
//
//	   3. This notice may not be removed or altered from any source
//	   distribution.
//

// -----------------------------------------------------------------------------
//	Headers:
// -----------------------------------------------------------------------------

#import <Cocoa/Cocoa.h>


// -----------------------------------------------------------------------------
//	Constants:
// -----------------------------------------------------------------------------

#define TD_USER_DEFINED_IDENTIFIERS			@"SyntaxColoring:UserIdentifiers"		// Key in user defaults holding user-defined identifiers to colorize.
#define TD_SYNTAX_COLORING_MODE_ATTR		@"UKTextDocumentSyntaxColoringMode"		// Anything we colorize gets this attribute. The value is an NSString holding the component name.


@class UKSyntaxColoredTextViewController;

@protocol UKSyntaxColoredTextViewDelegate <NSObject,NSTextViewDelegate>

@optional
-(void)	textViewControllerWillStartSyntaxRecoloring: (UKSyntaxColoredTextViewController*)sender;		// Show your progress indicator.
-(void)	textViewControllerProgressedWhileSyntaxRecoloring: (UKSyntaxColoredTextViewController*)sender;	// Make sure it gets redrawn.
-(void)	textViewControllerDidFinishSyntaxRecoloring: (UKSyntaxColoredTextViewController*)sender;		// Hide your progress indicator.

-(void)	selectionInTextViewController: (UKSyntaxColoredTextViewController*)sender						// Update any selection status display.
			changedToStartCharacter: (NSUInteger)startCharInLine endCharacter: (NSUInteger)endCharInLine
			inLine: (NSUInteger)lineInDoc startCharacterInDocument: (NSUInteger)startCharInDoc
			endCharacterInDocument: (NSUInteger)endCharInDoc;

-(NSString*)		syntaxDefinitionFilenameForTextViewController: (UKSyntaxColoredTextViewController*)sender;	// Won't get called if you override syntaxDefinitionDictionaryForTextViewController:.
-(NSDictionary*)	syntaxDefinitionDictionaryForTextViewController: (UKSyntaxColoredTextViewController*)sender;
-(NSArray*)			userIdentifiersForKeywordComponentName: (NSString*)inModeName;	// If you can parse your code & provide a list of identifiers the user uses, you can provide this method to tell the editor about them.
-(NSDictionary*)	textAttributesForComponentName: (NSString*)inModeName color: (NSColor*)inColor;	// If you don't just want a color, provide an NSAttributedString attributes dictionary here.

@end



// -----------------------------------------------------------------------------
//	Class:
// -----------------------------------------------------------------------------

@interface UKSyntaxColoredTextViewController : NSViewController <NSTextViewDelegate>
{
	BOOL								autoSyntaxColoring;		// Automatically refresh syntax coloring when text is changed?
	BOOL								maintainIndentation;	// Keep new lines indented at same depth as their predecessor?
	id									reserved;
	BOOL								syntaxColoringBusy;		// Set while recolorRange is busy, so we don't recursively call recolorRange.
	NSRange								affectedCharRange;
	NSString*							replacementString;
	IBOutlet id<UKSyntaxColoredTextViewDelegate>	delegate;
}

+(void) 	makeSurePrefsAreInited;		// No need to call this.

-(void)		setDelegate: (id<UKSyntaxColoredTextViewDelegate>)delegate;
-(id)		delegate;

-(IBAction)	recolorCompleteFile: (id)sender;
-(IBAction)	toggleAutoSyntaxColoring: (id)sender;
-(IBAction)	toggleMaintainIndentation: (id)sender;
-(IBAction) indentSelection: (id)sender;
-(IBAction) unindentSelection: (id)sender;
-(IBAction)	toggleCommentForSelection: (id)sender;

@property (nonatomic) BOOL autoSyntaxColoring;

@property (nonatomic) BOOL maintainIndentation;

-(void)		goToLine: (NSUInteger)lineNum;
-(void)		goToCharacter: (NSUInteger)charNum;
-(void)		goToRangeFrom: (NSUInteger)startCh toChar: (NSUInteger)endCh;

// Override any of the following in one of your subclasses to customize this object further:
@property (nonatomic, readonly, copy) NSString *syntaxDefinitionFilename;   // Defaults to "SyntaxDefinition.plist" in the app bundle's "Resources" directory.
@property (nonatomic, readonly, copy) NSDictionary *syntaxDefinitionDictionary; // Defaults to loading plist from -syntaxDefinitionFilename.

-(NSDictionary*)	defaultTextAttributes;		// Style attributes dictionary for an NSAttributedString.
-(NSRange)			defaultSelectedRange;		// Selected text range when document is opened.

// Private:
-(void) turnOffWrapping;

-(void) recolorRange: (NSRange) range;

-(void)	colorOneLineComment: (NSString*) startCh inString: (NSMutableAttributedString*) s
				withColor: (NSColor*) col andMode:(NSString*)attr;
-(void)	colorCommentsFrom: (NSString*) startCh to: (NSString*) endCh inString: (NSMutableAttributedString*) s
				withColor: (NSColor*) col andMode:(NSString*)attr;
-(void)	colorIdentifier: (NSString*) ident inString: (NSMutableAttributedString*) s
				withColor: (NSColor*) col andMode:(NSString*)attr charset: (NSCharacterSet*)cset;
-(void)	colorStringsFrom: (NSString*) startCh to: (NSString*) endCh inString: (NSMutableAttributedString*) s
				withColor: (NSColor*) col andMode:(NSString*)attr andEscapeChar: (NSString*)vStringEscapeCharacter;
-(void)	colorTagFrom: (NSString*) startCh to: (NSString*)endCh inString: (NSMutableAttributedString*) s
				withColor: (NSColor*) col andMode:(NSString*)attr exceptIfMode: (NSString*)ignoreAttr;

@end
