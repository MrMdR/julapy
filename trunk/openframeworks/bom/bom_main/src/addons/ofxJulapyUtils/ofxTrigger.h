/*
 *  ofxTrigger.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 5/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _OFX_TIGGER_H_
#define _OFX_TIGGER_H_

#define OFX_TRIGGER_ITEM_TYPE_FLOAT		0
#define OFX_TRIGGER_ITEM_TYPE_INT		1
#define OFX_TRIGGER_ITEM_TYPE_BOOL		2

#include "ofMain.h"
#include <fstream>

class ofxTriggerItem
{

public :

	float	position;
	int		type;
	bool	triggered;
	bool	write;
	string	valueName;
	
	float	*floatValuePtr;
	float	floatValue;
	
	int		*intValuePtr;
	int		intValue;

	bool	*boolValuePtr;
	bool	boolValue;
};

class ofxTrigger
{

public :
	
	ofxTrigger();
	~ofxTrigger();
	
	void addTrigger( float triggerPosition, float *valuePtr, float value, bool write = false, string valueName="" );
	void addTrigger( float triggerPosition, int   *valuePtr, int   value, bool write = false, string valueName="" );
	void addTrigger( float triggerPosition, bool  *valuePtr, bool  value, bool write = false, string valueName="" );
	void reset();
	void update( float position );
	
	void setWriteFormat( string triggerInstName );
	void writeToFile( string fileName );

	ofxTriggerItem *triggers;
	int	triggerIndex;
	int triggerTotal;
	int triggerMax;
	float position;
	
	string triggerInstanceName;
};

#endif