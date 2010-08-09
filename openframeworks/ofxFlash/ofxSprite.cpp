/*
 *  ofxSprite.cpp
 *  emptyExample
 *
 *  Created by Lukasz Karluk on 6/08/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxSprite.h"

///////////////////////////////////////////////
//	STATIC.
///////////////////////////////////////////////

ofxSprite* ofxSprite :: stageRef = NULL;
vector<ofxSprite*> ofxSprite :: sprites;

void ofxSprite :: addSprite ( ofxSprite* sprite )
{
	sprites.push_back( sprite );
}

void ofxSprite :: removeSprite ( ofxSprite* sprite )
{
	for( int i=0; i<sprites.size(); i++ )
	{
		if( sprites[ i ] == sprite )
		{
			sprites.erase( sprites.begin() + i );
			
			return;
		}
	}
}

///////////////////////////////////////////////
//	DE/CONSTRUCTOR.
///////////////////////////////////////////////

ofxSprite :: ofxSprite ()
{
	setup();
}

ofxSprite :: ~ofxSprite ()
{
	clear();
}

///////////////////////////////////////////////
//	CORE.
///////////////////////////////////////////////

void ofxSprite :: setup ()
{
	initDisplayObjectProps();
	initInteractiveObjectProps();
	initDisplayObjectContainerProps();
	initSpriteProps();
	
	addSprite( this );
}

void ofxSprite :: clear ()
{
	if( parent != NULL )
	{
		parent->removeChild( this );
	}
	
	mask		= NULL;
	parent		= NULL;
//	stage		= NULL;
	
	dropTarget	= NULL;
	hitArea		= NULL;
	
	for( int i=0; i<children.size(); i++ )
	{
//		children[ i ]->stage	= NULL;
		children[ i ]->parent	= NULL;
	}
	
	children.clear();
	
	removeSprite( this );
}

void ofxSprite :: update ()
{
	//
}

void ofxSprite :: draw ()
{
	//
}

///////////////////////////////////////////////
//	INIT.
///////////////////////////////////////////////

void ofxSprite :: initDisplayObjectProps ()
{
	alpha		= 1.0;
	visible		= true;
	width		= 0.0;
	height		= 0.0;
	x			= 0.0;
	y			= 0.0;
	z			= 0.0;
	mouseX		= 0.0;
	mouseY		= 0.0;
	rotation	= 0.0;
	rotationX	= 0.0;
	rotationY	= 0.0;
	rotationZ	= 0.0;
	scaleX		= 1.0;
	scaleY		= 1.0;
	scaleZ		= 1.0;
	blendMode	= BLEND_MODE_NORMAL;
	name		= "sprite";
	mask		= NULL;
	parent		= NULL;
//	stage		= NULL;
}

void ofxSprite :: initInteractiveObjectProps ()
{
	doubleClickEnabled	= false;
	mouseEnabled		= false;
	tabEnabled			= false;
	tabIndex			= 0;
}

void ofxSprite :: initDisplayObjectContainerProps ()
{
	mouseChildren		= false;
	tabChildren			= false;
}

void ofxSprite :: initSpriteProps ()
{
	buttonMode			= false;
	dropTarget			= NULL;
	hitArea				= NULL;
	useHandCursor		= false;
}

///////////////////////////////////////////////
//	DISPLAY OBJECT.
///////////////////////////////////////////////

ofRectangle ofxSprite :: getRect ( ofxSprite* targetCoordinateSpace )
{
	return rect;
}

ofPoint ofxSprite :: globalToLocal ( const ofPoint& point )
{
	//
}

ofPoint ofxSprite :: globalToLocal3D ( const ofPoint& point )
{
	//
}

bool ofxSprite :: hitTestObject ( ofxSprite* obj )
{
	//
}

bool ofxSprite :: hitTestPoint ( float x, float y, bool shapeFlag )
{
	//
}

ofPoint ofxSprite :: local3DToGlobal ( const ofPoint& point )
{
	//
}

ofPoint ofxSprite :: localToGlobal ( const ofPoint& point )
{
	ofPoint global;
	global.x = x + point.x;
	global.y = y + point.y;
	global.z = z + point.z;
	
	localToGlobalRecursive( this->parent, global );
	
	return global;
}

void ofxSprite :: localToGlobalRecursive ( ofxSprite* sprite, ofPoint& point )
{
	if( sprite->parent != NULL )
	{
		point.x += sprite->x;
		point.y += sprite->y;
		point.z += sprite->z;
		
		localToGlobalRecursive( sprite->parent, point );
	}
}

///////////////////////////////////////////////
//	DISPLAY OBJECT CONTAINER.
///////////////////////////////////////////////

ofxSprite* ofxSprite :: addChild ( ofxSprite* child )
{
	children.push_back( child );
//	child->stage	= this->stage;
	child->parent	= this;
	
//	if( stage != NULL )
//		stage->addToStage( child );
	
	return child;
}

ofxSprite* ofxSprite :: addChildAt ( ofxSprite* child, int index )
{
	if( index < 0 || index > children.size() - 1 )
		return NULL;

	children.insert( children.begin() + index, child );
//	child->stage	= this->stage;
	child->parent	= this;
	
//	if( stage != NULL )
//		stage->addToStage( child );
	
	return child;
}

bool ofxSprite :: contains ( ofxSprite* child )
{
	for( int i=0; i<children.size(); i++ )
	{
		if( children[ i ] == child )
		{
			return true;
		}
	}
	
	return false;
}

ofxSprite* ofxSprite :: getChildAt ( int index )
{
	if( index < 0 || index > children.size() - 1 )
		return NULL;
	
	return children[ index ];
}

ofxSprite* ofxSprite :: getChildByName ( string name )
{
	for( int i=0; i<children.size(); i++ )
	{
		if( children[ i ]->name == name )
		{
			return children[ i ];
		}
	}
	
	return NULL;
}

int ofxSprite :: getChildIndex ( ofxSprite* child )
{
	for( int i=0; i<children.size(); i++ )
	{
		if( children[ i ] == child )
		{
			return i;
		}
	}
	
	return -1;
}

vector<ofxSprite*> ofxSprite :: getObjectsUnderPoint ( ofPoint point )
{
	//
}

ofxSprite* ofxSprite :: removeChild ( ofxSprite* child )
{
	for( int i=0; i<children.size(); i++ )
	{
		if( children[ i ] == child )
		{
//			child->stage	= NULL;
			child->parent	= NULL;
			
			children.erase( children.begin() + i );
			
//			if( stage != NULL )
//				stage->removeFromStage( child );
			
			return child;
		}
	}
	
	return child;
}

ofxSprite* ofxSprite :: removeChildAt ( int index )
{
	if( index < 0 || index > children.size() - 1 )
		return NULL;
	
	ofxSprite* child;
	child = children[ index ];
//	child->stage	= NULL;
	child->parent	= NULL;
	
	children.erase( children.begin() + index );
	
//	if( stage != NULL )
//		stage->removeFromStage( child );
	
	return child;
}

void ofxSprite :: setChildIndex ( ofxSprite* child, int index )
{
	if( index < 0 || index > children.size() - 1 )
		return;
	
	for( int i=0; i<children.size(); i++ )
	{
		if( children[ i ] == child )
		{
			children.erase( children.begin() + i );
			children.insert( children.begin() + index, child );
			
			return;
		}
	}
}

void ofxSprite :: swapChildren ( ofxSprite* child1, ofxSprite* child2 )
{
	int index1 = getChildIndex( child1 );
	int index2 = getChildIndex( child2 );
	
	if( index1 == -1 || index2 == -1 )
		return;
	
	for( int i=0; i<children.size(); i++ )
	{
		if( children[ i ] == child1 || children[ i ] == child2 )
		{
			children.erase( children.begin() + i-- );
		}
	}
	
	if( index1 < index2 )
	{
		children.insert( children.begin() + index1, child2 );
		children.insert( children.begin() + index2, child1 );
	}
	else
	{
		children.insert( children.begin() + index2, child1 );
		children.insert( children.begin() + index1, child2 );
	}
}

void ofxSprite :: swapChildrenAt ( int index1, int index2 )
{
	if( index1 == index2 )
		return;
	
	ofxSprite* child1 = getChildAt( index1 );
	ofxSprite* child2 = getChildAt( index2 );
	
	if( child1 == NULL || child2 == NULL )
		return;
	
	if( index2 > index1 )
	{
		children.erase( children.begin() + index2 );
		children.erase( children.begin() + index1 );
	}
	else
	{
		children.erase( children.begin() + index1 );
		children.erase( children.begin() + index2 );
	}

	if( index1 < index2 )
	{
		children.insert( children.begin() + index1, child2 );
		children.insert( children.begin() + index2, child1 );
	}
	else
	{
		children.insert( children.begin() + index2, child1 );
		children.insert( children.begin() + index1, child2 );
	}
}

int ofxSprite :: numChildren ()
{
	return children.size();
}

bool ofxSprite :: hasChildren ()
{
	return children.size() > 0;
}

///////////////////////////////////////////////
//	SPRITE.
///////////////////////////////////////////////

void ofxSprite :: startDrag ( bool lockCenter, const ofRectangle* bounds )
{
	//
}

void ofxSprite :: stopDrag ()
{
	//
}