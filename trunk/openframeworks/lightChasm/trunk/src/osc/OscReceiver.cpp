/*
 *  OscRemote.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 21/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "OscReceiver.h"

OscReceiver :: OscReceiver()
{

}

OscReceiver :: ~OscReceiver()
{

}

void OscReceiver :: init( ofxOscReceiver *recvr )
{
	receiver = recvr;
}

void OscReceiver :: addInput( string inputName, int *value )
{
	inputs.push_back( OscReceiverInput() );
	inputs.back().inputName = inputName;
	inputs.back().valueType = OSC_RECEIVER_TYPE_INT;
	inputs.back().valueInt	= value;
}

void OscReceiver :: addInput( string inputName, float *value )
{
	inputs.push_back( OscReceiverInput() );
	inputs.back().inputName	 = inputName;
	inputs.back().valueType	 = OSC_RECEIVER_TYPE_FLOAT;
	inputs.back().valueFloat = value;
}

void OscReceiver :: addInput( string inputName, bool *value )
{
	inputs.push_back( OscReceiverInput() );
	inputs.back().inputName = inputName;
	inputs.back().valueType = OSC_RECEIVER_TYPE_BOOL;
	inputs.back().valueBool	= value;
}

void OscReceiver :: ping()
{
	while( receiver->hasWaitingMessages() )
	{
		ofxOscMessage m;
		
		receiver->getNextMessage( &m );
		
		string inputName = m.getAddress();
		int	inputIndex	 = getInputIndex( inputName );
		
		if( inputIndex == -1 )
			break;
		
		for( int i=0; i<m.getNumArgs(); i++ )
		{
			if( inputs.at( inputIndex ).valueType == OSC_RECEIVER_TYPE_INT && 
			    m.getArgType( i ) == OFXOSC_TYPE_INT32 )
			{
				*( inputs[ inputIndex ].valueInt ) = m.getArgAsInt32( i );
			}
			
			if( inputs.at( inputIndex ).valueType == OSC_RECEIVER_TYPE_FLOAT && 
			    m.getArgType( i ) == OFXOSC_TYPE_FLOAT )
			{
				*( inputs[ inputIndex ].valueFloat ) = m.getArgAsFloat( i );
			}
			
			if( inputs.at( inputIndex ).valueType == OSC_RECEIVER_TYPE_BOOL && 
			    m.getArgType( i ) == OFXOSC_TYPE_INT32 )
			{
				if( m.getArgAsInt32( i ) == 0 )
					*( inputs[ inputIndex ].valueBool ) = false;

				if( m.getArgAsInt32( i ) == 1 )
					*( inputs[ inputIndex ].valueBool ) = true;
			}
		}
	}
}

int OscReceiver :: getInputIndex( string inputName )
{
	for( int i=0; i<inputs.size(); i++ )
	{
		if( inputs[ i ].inputName == inputName )
		{
			return i;
		}
	}
	
	return -1;
}