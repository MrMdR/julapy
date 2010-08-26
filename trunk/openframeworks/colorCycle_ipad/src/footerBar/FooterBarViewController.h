//
//  FooterBarViewController.h
//  iPhoneEmptyExample
//
//  Created by Lukasz Karluk on 19/08/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"

#include "FooterBarConfigViewController.h"
#import <MessageUI/MessageUI.h>

@protocol FooterBarViewDelegate
// empty.
@end

@interface FooterBarViewController : UIViewController 
	<
		FooterBarConfigViewDelegate,
		MFMailComposeViewControllerDelegate,
		UIAlertViewDelegate
	>
{
	id<FooterBarViewDelegate>	_delegate;
	
	IBOutlet UIBarButtonItem*	_snapButton;
	IBOutlet UIBarButtonItem*	_colorButton;
	IBOutlet UIBarButtonItem*	_addButton;
	IBOutlet UIBarButtonItem*	_removeButton;
	IBOutlet UIBarButtonItem*	_shuffleButton;
	IBOutlet UIBarButtonItem*	_pirateButton;
	IBOutlet UIBarButtonItem*	_emailButton;
	IBOutlet UIBarButtonItem*	_infoButton;
	
	FooterBarConfigViewController*	_config;
	UIPopoverController*			_configPopover;
	NSTimer*						_hideTimer;
	BOOL							_showing;
	BOOL							_shuffleSelected;
	BOOL							_colorSelected;
	BOOL							_addSelected;
	BOOL							_removeSelected;
	BOOL							_infoSelected;
}

@property (nonatomic, assign) id<FooterBarViewDelegate>			delegate;
@property (nonatomic, retain) FooterBarConfigViewController*	config;
@property (nonatomic, retain) UIPopoverController*				configPopover;

- (IBAction) snapButtonHandler;
- (IBAction) colorButtonHandler;
- (IBAction) addButtonHandler;
- (IBAction) removeButtonHandler;
- (IBAction) shuffleButtonHandler;
- (IBAction) pirateButtonHandler;
- (IBAction) emailButtonHandler;
- (IBAction) infoButtonHandler;
- (IBAction) forward:(id)sender;

- (BOOL) isShuffleSelected;
- (BOOL) isColorSelected;
- (BOOL) isAddSelected;
- (BOOL) isRemoveSelected;
- (BOOL) isInfoSelected;

- (void) show;
- (void) hide;
- (void) toggleShow;
- (void) hideTimerInit;
- (void) hideTimerKill;
- (void) hideTimerHandler:(NSTimer *) timer;
- (void) showAnimationCompleteHandler:(NSString *)animationID finished:(NSNumber *)finished context:(void *)context;;
- (void) hideAnimationCompleteHandler:(NSString *)animationID finished:(NSNumber *)finished context:(void *)context;;

@end
