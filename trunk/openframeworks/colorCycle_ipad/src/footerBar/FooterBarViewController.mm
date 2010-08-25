//
//  FooterBarViewController.m
//  iPhoneEmptyExample
//
//  Created by Lukasz Karluk on 19/08/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "FooterBarViewController.h"


@implementation FooterBarViewController

@synthesize delegate		= _delegate;
@synthesize config			= _config;
@synthesize configPopover	= _configPopover;

 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
    }
    return self;
}


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad
{
	[ self.view setAlpha: 0.0 ];
	
	_showing			= NO;
	_shuffleSelected	= NO;
	_colorSelected		= NO;
	_addSelected		= NO;
	_removeSelected		= NO;
	
	if( [ MFMailComposeViewController canSendMail ] )
	{
		_emailButton.enabled = YES;
	}
	else
	{
		_emailButton.enabled = NO;
	}
	
//	_infoButton.buttonType = UIButtonTypeInfoLight;		// only way seems to instanciate the button programatically.
	
    [ super viewDidLoad ];
}


- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Overriden to allow any orientation.
    return YES;
}


- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}


- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


- (void)dealloc
{
    self.config			= nil;
    self.configPopover	= nil;
	
    [super dealloc];
}

//////////////////////////////////////////////////////
// TOUCH HANDLER.
//////////////////////////////////////////////////////

-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	UITouch *touchAny	= [ touches anyObject ];
	
//	NSArray *touchArray	= [ touches allObjects ];
//	UITouch *touchOne	= [ touchArray objectAtIndex:0 ];
	
//	CGPoint p1 = [ touchAny locationInView: self.view ];
	
	if( touchAny.view == self.view )
	{
		[ self hide ];
	}
	
	[ super touchesBegan:touches withEvent:event ];
}

-(void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	[ super touchesMoved:touches withEvent:event ];
}

-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	[ super touchesEnded:touches withEvent:event ];
}

//////////////////////////////////////////////////////
// METHODS
//////////////////////////////////////////////////////

-(void)snapButtonHandler
{
	ofxiPhoneScreenGrab( 0 );

	[ self hide ];
	
	UIAlertView* alertView = 
	[ 
		[ UIAlertView alloc] 
		initWithTitle		: @"" 
		message				: @"Composition Saved!"
		delegate			: self 
		cancelButtonTitle	: @"OK" 
		otherButtonTitles	: nil
	];
	
	[ alertView show ];
	[ alertView release ];
}

-(void)colorButtonHandler
{
	_colorSelected = YES;
}

-(void)addButtonHandler
{
	_addSelected = YES;
}

-(void)removeButtonHandler
{
	_removeSelected = YES;
}

-(void)shuffleButtonHandler
{
	_shuffleSelected = YES;
}

-(void)pirateButtonHandler
{
	UIAlertView* alertView = 
	[ 
		[ UIAlertView alloc] 
		initWithTitle		: @"" 
		message				: @"Argh, me hearties!"
		delegate			: self 
		cancelButtonTitle	: @"YAR"
		otherButtonTitles	: nil
	];
	
	[ alertView show ];
	[ alertView release ];
}

-(void)emailButtonHandler
{
	MFMailComposeViewController *controller = [ [ MFMailComposeViewController alloc ] init ];
	controller.mailComposeDelegate = self;
	[ controller setSubject:@"FooterBar Example" ];
	[ controller setMessageBody:@"hey, email is working in OpenFrameworks on the iPhone!" isHTML:NO ];
	[ self presentModalViewController:controller animated:YES ];
	[ controller release ];
}

-(void)infoButtonHandler
{
	//
}

-(void)forward:(id)sender
{
	if( _config == nil )
	{
		CGRect rect = [ [ UIScreen mainScreen ] applicationFrame ];
		CGSize size = CGSizeMake( rect.size.width * 0.5, rect.size.height * 0.5 );
		
		self.config = [ [ [ FooterBarConfigViewController alloc ] init ] autorelease ];
		_config.delegate = self;
		_config.contentSizeForViewInPopover = size;
		//		_config.view.clipsToBounds = YES;
		
		self.configPopover = [ UIPopoverController alloc ];
		[ self.configPopover initWithContentViewController:_config ];
		[ self.configPopover autorelease ];
	}
	
	[ self.configPopover presentPopoverFromBarButtonItem:sender permittedArrowDirections:UIPopoverArrowDirectionAny animated:YES ];
	//	[ self.configPopover presentPopoverFromRect:CGRectMake( 0, 0, 100, 100) inView:self.view permittedArrowDirections:UIPopoverArrowDirectionAny animated:true ];
}

//////////////////////////////////////////////////////
// MAIL DELEGATE.
//////////////////////////////////////////////////////

- (void)mailComposeController:(MFMailComposeViewController*)controller didFinishWithResult:(MFMailComposeResult)result error:(NSError*)error 
{
	[self becomeFirstResponder];
	[self dismissModalViewControllerAnimated:YES];
}

//////////////////////////////////////////////////////
// CHECKERS
//////////////////////////////////////////////////////

-(BOOL) isShuffleSelected
{
	if( _shuffleSelected )
	{
		_shuffleSelected = NO;
		
		return YES;
	}
	else
	{
		return NO;
	}
}

-(BOOL) isColorSelected
{
	if( _colorSelected )
	{
		_colorSelected = NO;
		
//		[ self hide ];
		
		return YES;
	}
	else
	{
		return NO;
	}
}

-(BOOL) isAddSelected
{
	if( _addSelected )
	{
		_addSelected = NO;
		
		return YES;
	}
	else
	{
		return NO;
	}
}

-(BOOL) isRemoveSelected
{
	if( _removeSelected )
	{
		_removeSelected = NO;
		
		return YES;
	}
	else
	{
		return NO;
	}
}

//////////////////////////////////////////////////////
// METHODS
//////////////////////////////////////////////////////

- (void) show
{
	if( _showing )
		return;
	
	_showing = YES;
	
	CGContextRef context = UIGraphicsGetCurrentContext();
	[ UIView beginAnimations:@"fadeIn" context:context ];
	[ UIView setAnimationCurve: UIViewAnimationCurveEaseOut ];
	[ UIView setAnimationDuration: 0.2 ];
	[ UIView setAnimationDelegate:self];
	[ UIView setAnimationDidStopSelector:@selector(showAnimationCompleteHandler:finished:context:)];
	[ self.view setAlpha: 1.0 ];
	[ UIView commitAnimations ];
	
//	[ self hideTimerInit ];
}

- (void) hide
{
	if( !_showing )
		return;
	
	_showing = NO;
	
	CGContextRef context = UIGraphicsGetCurrentContext();
	[ UIView beginAnimations:nil context:context ];
	[ UIView setAnimationCurve: UIViewAnimationCurveEaseOut ];
	[ UIView setAnimationDelegate:self];
	[ UIView setAnimationDidStopSelector:@selector(hideAnimationCompleteHandler:finished:context:)];
	[ UIView setAnimationDuration: 0.2 ];
	[ self.view setAlpha: 0.0 ];
	[ UIView commitAnimations ];
}

- (void) toggleShow
{
	if( _showing )
	{
		[ self hide ];
	}
	else
	{
		[ self show ];
	}
}

- (void) hideTimerInit
{
	[ self hideTimerKill ];
	
	_hideTimer =	
	[
		NSTimer scheduledTimerWithTimeInterval: 3.0
		target		: self
		selector	: @selector( hideTimerHandler: )
		userInfo	: nil
		repeats		: NO
	];
}

- (void) hideTimerKill
{
	if( _hideTimer != nil )
	{
		[ _hideTimer invalidate ];
		_hideTimer = nil;
	}
}

- (void) hideTimerHandler: (NSTimer *) timer
{
	[ self hideTimerKill ];
	[ self hide ];
}

- (void)showAnimationCompleteHandler:(NSString *)animationID finished:(NSNumber *)finished context:(void *)context
{
	NSLog( @"show complete" );
	
//	NSNotificationCenter *nc = [ NSNotificationCenter defaultCenter ];
//	[ nc postNotificationName:FBFadeInCompleteNotification object:self ];
}


- (void)hideAnimationCompleteHandler:(NSString *)animationID finished:(NSNumber *)finished context:(void *)context
{
	NSLog( @"hide complete" );
	
//	NSNotificationCenter *nc = [ NSNotificationCenter defaultCenter ];
//	[ nc postNotificationName:FBFadeOutCompleteNotification object:self ];
}

@end
