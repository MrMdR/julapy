/*
 *  ofxEaseValue.h
 *  iPhoneEmptyExample
 *
 *  Created by Lukasz Karluk on 17/08/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

class ofxEaseValue
{
	
public:
	
	ofxEaseValue()
	{
		value	= 0;
		target	= 0;
		ease	= 0.1;
	};
	
	void update ()
	{
		value += ( target - value ) * ease;
	}
	
	float value;
	float target;
	float ease;
};