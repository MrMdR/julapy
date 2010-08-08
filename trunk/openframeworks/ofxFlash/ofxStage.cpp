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
	ofAddListener( ofEvents.update,	this, &ofxStage::update	);
	ofAddListener( ofEvents.draw,	this, &ofxStage::draw	);
}

ofxStage :: ~ofxStage ()
{
	//
}

void ofxStage :: update ( ofEventArgs &e )
{
	updateChildren( children );
}

void ofxStage :: draw ( ofEventArgs &e )
{
	drawChildren( children );
}

void ofxStage :: updateChildren ( vector<ofxSprite*>& children )
{
	for( int i=0; i<children.size(); i++ )
	{
		ofxSprite* child;
		child = children[ i ];
		child->update();
		
		if( child->children.size() > 0 )
		{
			updateChildren( child->children );
		}
	}
}

void ofxStage :: drawChildren ( vector<ofxSprite*>& children )
{
	for( int i=0; i<children.size(); i++ )
	{
		ofxSprite* child;
		child = children[ i ];
		
		child->draw();

		bool bTranslate;
		bTranslate = ( child->x != 0 || child->y != 0 || child->z != 0 );
		
		if( bTranslate )
		{
			glPushMatrix();
			glTranslatef( child->x, child->y, child->z );
		}
		
		if( child->children.size() > 0 )
		{
			drawChildren( child->children );
		}
		
		if( bTranslate )
		{
			glPopMatrix();
		}
	}
}
