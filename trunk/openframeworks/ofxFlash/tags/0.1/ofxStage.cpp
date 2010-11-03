/*
 *  ofxStage.cpp
 *  emptyExample
 *
 *  Created by Lukasz Karluk on 6/08/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxStage.h"

ofxStage :: ofxStage ()
{
	//
}

ofxStage :: ~ofxStage ()
{
	//
}

/////////////////////////////////////////////
//	LISTENERS.
/////////////////////////////////////////////

void ofxStage :: addListeners ()
{
	ofAddListener( ofEvents.update,	this, &ofxStage::update	);
	ofAddListener( ofEvents.draw,	this, &ofxStage::draw	);
}

void ofxStage :: removeListeners ()
{
	ofRemoveListener( ofEvents.update,	this, &ofxStage::update );
	ofRemoveListener( ofEvents.draw,	this, &ofxStage::draw	);
}

/////////////////////////////////////////////
//	
/////////////////////////////////////////////

void ofxStage :: update ()
{
	updateChildren( children );
}

void ofxStage :: draw ()
{
	drawChildren( children );
}

/////////////////////////////////////////////
//	EVENT HANDLERS.
/////////////////////////////////////////////

void ofxStage :: update ( ofEventArgs &e )
{
	update();
}

void ofxStage :: draw ( ofEventArgs &e )
{
	draw();
}

/////////////////////////////////////////////
//	UPDATE CHILDREN.
/////////////////////////////////////////////

void ofxStage :: updateChildren ( vector<ofxSprite*>& children )
{
	for( int i=0; i<children.size(); i++ )
	{
		ofxSprite* child;
		child = children[ i ];
		
		child->globalX = child->parent->globalX + child->x;
		child->globalY = child->parent->globalY + child->y;
		child->globalZ = child->parent->globalZ + child->z;
		
		child->update();
		
		if( child->children.size() > 0 )
		{
			updateChildren( child->children );
		}
	}
}

/////////////////////////////////////////////
//	DRAW CHILDREN.
/////////////////////////////////////////////

void ofxStage :: drawChildren ( vector<ofxSprite*>& children )
{
	for( int i=0; i<children.size(); i++ )
	{
		ofxSprite* child;
		child = children[ i ];
		
		child->draw();

		if( child->children.size() > 0 )
		{
			drawChildren( child->children );
		}
	}
}
