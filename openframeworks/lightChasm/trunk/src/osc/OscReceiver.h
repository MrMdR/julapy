/*
 *  OscRemote.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 21/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef OSC_RECEIVER_H
#define OSC_RECEIVER_H

#include "ofMain.h"
#include "ofxOsc.h"

#define	OSC_RECEIVER_TYPE_INT		10001
#define	OSC_RECEIVER_TYPE_FLOAT		10002
#define	OSC_RECEIVER_TYPE_BOOL		10003

#define OSC_RECIEVER_PORT			12345


class OscReceiverInput
{
public:
	string	inputName;
	int		valueType;
	int		*valueInt;
	float	*valueFloat;
	bool	*valueBool;
};


class OscReceiver
{

public :
	
	OscReceiver();
	~OscReceiver();
	
	void init( ofxOscReceiver *recvr );
	void addInput( string inputName, int *value );
	void addInput( string inputName, float *value );
	void addInput( string inputName, bool *value );
	void ping();
	
private :
	
	ofxOscReceiver *receiver;
	vector <OscReceiverInput> inputs;
	
	int getInputIndex( string inputName );
	
};

#endif