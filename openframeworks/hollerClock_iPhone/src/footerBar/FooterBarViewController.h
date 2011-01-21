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
		MFMailComposeViewControllerDelegate
	>
{
	id<FooterBarViewDelegate>	_delegate;
	
	IBOutlet UIBarButtonItem*	_snapButton;
	IBOutlet UIBarButtonItem*	_forwardButton;
	IBOutlet UIBarButtonItem*	_emailButton;
	IBOutlet UIBarButtonItem*	_refreshButton;
	IBOutlet UIBarButtonItem*	_infoButton;
	
	FooterBarConfigViewController*	_config;
	UIPopoverController*			_configPopover;
	NSTimer*						_hideTimer;
	BOOL							_showing;
}

@property (nonatomic, assign) id<FooterBarViewDelegate>			delegate;
@property (nonatomic, retain) FooterBarConfigViewController*	config;
@property (nonatomic, retain) UIPopoverController*				configPopover;

- (IBAction) snap;
- (IBAction) forward:(id)sender;
- (IBAction) email;
- (IBAction) refresh;
- (IBAction) info;

- (void) show;
- (void) hide;
- (void) toggleShow;
- (void) hideTimerInit;
- (void) hideTimerKill;
- (void) hideTimerHandler:(NSTimer *) timer;
- (void) showAnimationCompleteHandler:(NSString *)animationID finished:(NSNumber *)finished context:(void *)context;;
- (void) hideAnimationCompleteHandler:(NSString *)animationID finished:(NSNumber *)finished context:(void *)context;;

@end
