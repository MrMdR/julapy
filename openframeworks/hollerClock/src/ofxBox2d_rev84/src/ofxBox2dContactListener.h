
#pragma once
#include "ofMain.h"
#include "Box2D.h"
#include "ofxBox2dContactReceiver.h"

class ofxBox2dContactListener : public b2ContactListener
{
public:
	
	vector<ofxBox2dContactReceiver*> receivers;
	
	virtual void addReceiver ( ofxBox2dContactReceiver* receiver )
	{
		receivers.push_back( receiver );
	}
	
	virtual void Add( const b2ContactPoint* point)
	{
		for( int i=0; i<receivers.size(); i++ )
		{
			receivers[ i ]->box2dContactEventHandler( point );
		}
	}
	
	virtual void Remove(const b2ContactPoint* point)
	{
		//
	}
};



