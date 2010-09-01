//
//  FooterBarConfigViewController.h
//  iPhoneEmptyExample
//
//  Created by Lukasz Karluk on 19/08/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol FooterBarConfigViewDelegate
	// empty.
@end

@interface FooterBarConfigViewController : UIViewController 
{
	id<FooterBarConfigViewDelegate> _delegate;
}

@property (nonatomic, assign) id<FooterBarConfigViewDelegate> delegate;

@end
