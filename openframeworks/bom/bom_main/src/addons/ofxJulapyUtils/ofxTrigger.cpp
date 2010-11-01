/*
 *  ofxTrigger.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 5/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxTrigger.h"

ofxTrigger :: ofxTrigger()
{
	triggerIndex	= 0;
	triggerTotal	= 0;
	triggerMax		= 1000;
	triggers		= new ofxTriggerItem[ triggerMax ];
	
	setWriteFormat( "trigger" );
}

ofxTrigger :: ~ofxTrigger()
{
	//
}

void ofxTrigger :: addTrigger( float triggerPosition, float *valuePtr, float value, bool write, string valueName )
{
	triggers[ triggerTotal ].type			= OFX_TRIGGER_ITEM_TYPE_FLOAT;
	triggers[ triggerTotal ].position		= triggerPosition;
	triggers[ triggerTotal ].triggered		= false;
	triggers[ triggerTotal ].write			= write;
	triggers[ triggerTotal ].valueName		= valueName;
	triggers[ triggerTotal ].floatValuePtr	= valuePtr;
	triggers[ triggerTotal ].floatValue		= value;
	
	++triggerTotal;
}

void ofxTrigger :: addTrigger( float triggerPosition, int *valuePtr, int value, bool write, string valueName )
{
	triggers[ triggerTotal ].type			= OFX_TRIGGER_ITEM_TYPE_INT;
	triggers[ triggerTotal ].position		= triggerPosition;
	triggers[ triggerTotal ].triggered		= false;
	triggers[ triggerTotal ].write			= write;
	triggers[ triggerTotal ].valueName		= valueName;
	triggers[ triggerTotal ].intValuePtr	= valuePtr;
	triggers[ triggerTotal ].intValue		= value;
	
	++triggerTotal;
}

void ofxTrigger :: addTrigger( float triggerPosition, bool *valuePtr, bool value, bool write, string valueName )
{
	triggers[ triggerTotal ].type			= OFX_TRIGGER_ITEM_TYPE_BOOL;
	triggers[ triggerTotal ].position		= triggerPosition;
	triggers[ triggerTotal ].triggered		= false;
	triggers[ triggerTotal ].write			= write;
	triggers[ triggerTotal ].valueName		= valueName;
	triggers[ triggerTotal ].boolValuePtr	= valuePtr;
	triggers[ triggerTotal ].boolValue		= value;
	
	++triggerTotal;
}

void ofxTrigger :: reset()
{
	for( int i=0; i<triggerTotal; i++ )
	{
		triggers[ i ].triggered = false;
	}
}

void ofxTrigger :: update( float position )
{
	for( int i=0; i<triggerTotal; i++ )
	{
		ofxTriggerItem triggerItem;
		triggerItem = triggers[ i ];
		
		if( !triggerItem.triggered && position >= triggerItem.position )
		{
			if( triggerItem.type == OFX_TRIGGER_ITEM_TYPE_FLOAT )
				*( triggerItem.floatValuePtr ) = triggerItem.floatValue;
			
			if( triggers[ i ].type == OFX_TRIGGER_ITEM_TYPE_INT )
				*( triggerItem.intValuePtr ) = triggerItem.intValue;
			
			if( triggers[ i ].type == OFX_TRIGGER_ITEM_TYPE_BOOL )
				*( triggerItem.boolValuePtr ) = triggerItem.boolValue;
			
			triggers[ i ].triggered = true;
		}
	}
}

void ofxTrigger :: setWriteFormat( string triggerInstName )
{
	triggerInstanceName = triggerInstName;
}

void ofxTrigger :: writeToFile( string fileName )
{
	ofstream fout;
	fout.open( ofToDataPath( fileName ).c_str() );
	
	for( int i=0; i<triggerTotal; i++ )
	{
		if( triggers[ i ].write )
		{
			if( triggers[ i ].type == OFX_TRIGGER_ITEM_TYPE_FLOAT )
			{
				string triggerLineStr = triggerInstanceName + ".addTrigger( %1.9f, &" + triggers[ i ].valueName + ", %1.9f );";
				char *triggerLine = new char[ triggerLineStr.size() + 1 ];
				strcpy( triggerLine, triggerLineStr.c_str() );
				
				char triggerLineParsed[ 255 ];
				sprintf( triggerLineParsed, triggerLine, triggers[ i ].position, triggers[ i ].floatValue );
				
				delete[] triggerLine;
				
				fout << triggerLineParsed << endl;
			}
			
			if( triggers[ i ].type == OFX_TRIGGER_ITEM_TYPE_INT )
			{
				string triggerLineStr = triggerInstanceName + ".addTrigger( %1.9f, &" + triggers[ i ].valueName + ", %d );";
				char *triggerLine = new char[ triggerLineStr.size() + 1 ];
				strcpy( triggerLine, triggerLineStr.c_str() );
				
				char triggerLineParsed[ 255 ];
				sprintf( triggerLineParsed, triggerLine, triggers[ i ].position, triggers[ i ].intValue );
				
				delete[] triggerLine;
				
				fout << triggerLineParsed << endl;
			}
			
			if( triggers[ i ].type == OFX_TRIGGER_ITEM_TYPE_BOOL )
			{
				string triggerLineStr = triggerInstanceName + ".addTrigger( %1.9f, &" + triggers[ i ].valueName + ", %s );";
				char *triggerLine = new char[ triggerLineStr.size() + 1 ];
				strcpy( triggerLine, triggerLineStr.c_str() );
				
				char triggerLineParsed[ 255 ];
				sprintf( triggerLineParsed, triggerLine, triggers[ i ].position, ( triggers[ i ].boolValue ) ? "true" : "false" );
				
				delete[] triggerLine;
				
				fout << triggerLineParsed << endl;
			}
		}
	}
	
	fout.close();
}