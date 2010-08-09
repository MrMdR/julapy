/*
 *  ofxStage.cpp
 *  emptyExample
 *
 *  Created by Lukasz Karluk on 6/08/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxStage.h"

///////////////////////////////////////////////
//	SINGLETON.
///////////////////////////////////////////////

ofxStage* ofxStage :: _instance = NULL;
ofxStage* ofxStage :: stage()
{
	if( _instance == NULL )
		_instance = new ofxStage();
	
	return _instance;
}

///////////////////////////////////////////////
//	DE/CONSTRUCTOR.
///////////////////////////////////////////////

ofxStage :: ofxStage ()
{
	setup();
}

ofxStage :: ~ofxStage ()
{
	clear();
}

///////////////////////////////////////////////
//	CORE.
///////////////////////////////////////////////

void ofxStage :: setup ()
{
	stageRef = this;
	
	ofAddListener( ofEvents.update,	this, &ofxStage::updateHandler	);
	ofAddListener( ofEvents.draw,	this, &ofxStage::drawHandler	);
	
	removeSprite( this );	// remove stage from sprites array as stage is the container for all other sprites.
}

void ofxStage :: clear ()
{
	//
}

void ofxStage :: update ()
{
	//
}

void ofxStage :: draw ()
{
	//
}

///////////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////////

void ofxStage :: updateHandler ( ofEventArgs &e )
{
	updateChildren( children );
}

void ofxStage :: updateChildren ( vector<ofxSprite*>& children )
{
	for( int i=0; i<children.size(); i++ )
	{
		ofxSprite* child;
		child = children[ i ];
		child->update();
		
		if( child->hasChildren() )
		{
			updateChildren( child->children );
		}
	}
}

///////////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////////

void ofxStage :: drawHandler ( ofEventArgs &e )
{
	drawChildren( children );
}

void ofxStage :: drawChildren ( vector<ofxSprite*>& children )
{
	for( int i=0; i<children.size(); i++ )
	{
		ofxSprite* child;
		child = children[ i ];

		bool bTranslate;
		bTranslate = ( child->x != 0 || child->y != 0 || child->z != 0 );
		
		if( bTranslate )
		{
			glPushMatrix();
			glTranslatef( child->x, child->y, child->z );
		}
		
		child->draw();
		
		if( child->hasChildren() )
		{
			drawChildren( child->children );
		}
		
		if( bTranslate )
		{
			glPopMatrix();
		}
	}
}