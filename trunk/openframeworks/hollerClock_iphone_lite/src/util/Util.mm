//
//  Util.m
//  iPhoneEmptyExample
//
//  Created by lukasz karluk on 24/02/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//


#include "Util.h"

Util* Util :: _instance = NULL;

void Util :: launchSite ( string url )
{
	[ [ UIApplication sharedApplication ] openURL:[NSURL URLWithString: [[[[NSString alloc] initWithCString: url.c_str()]stringByAddingPercentEscapesUsingEncoding: NSASCIIStringEncoding] autorelease] ]];
}