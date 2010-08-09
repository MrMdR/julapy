/*
 *  ofxSprite.h
 *  emptyExample
 *
 *  Created by Lukasz Karluk on 6/08/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"

#define	BLEND_MODE_NORMAL		0
#define	BLEND_MODE_LAYER		1
#define	BLEND_MODE_MULTIPLY		2
#define	BLEND_MODE_SCREEN		3
#define	BLEND_MODE_LIGHTEN		4
#define	BLEND_MODE_DARKEN		5
#define	BLEND_MODE_DIFFERENCE	6
#define	BLEND_MODE_ADD			7
#define	BLEND_MODE_SUBTRACT		8
#define	BLEND_MODE_INVERT		9
#define	BLEND_MODE_ALPHA		10
#define	BLEND_MODE_ERASE		11
#define	BLEND_MODE_OVERLAY		12
#define	BLEND_MODE_HARDLIGHT	13

class ofxSprite
{	
	
public :
	
	 ofxSprite();
	~ofxSprite();
	
	virtual void setup	();
	virtual void clear	();
	
	virtual void update	();
	virtual void draw	();
	
	///////////////////////////////////////////////
	//
	//	DISPLAY OBJECT.
	//	http://livedocs.adobe.com/flex/3/langref/flash/display/DisplayObject.html
	//
	///////////////////////////////////////////////
	
	float		alpha;
	bool		visible;
	float		width;
	float		height;
	float		x;
	float		y;
	float		z;
	int			mouseX;
	int			mouseY;
	float		rotation;
	float		rotationX;
	float		rotationY;
	float		rotationZ;
	float		scaleX;
	float		scaleY;
	float		scaleZ;
	int			blendMode;
	string		name;
	ofxSprite*	mask;			// DisplayObject in AS3.
	ofxSprite*	parent;			// DisplayObjectContainer in AS3.
	
	ofRectangle getRect			( ofxSprite* targetCoordinateSpace );
	ofPoint		globalToLocal	( const ofPoint& point );
	ofPoint		globalToLocal3D	( const ofPoint& point );
	bool		hitTestObject	( ofxSprite* obj );
	bool		hitTestPoint	( float x, float y, bool shapeFlag = false);
	ofPoint		local3DToGlobal	( const ofPoint& point );
	ofPoint		localToGlobal	( const ofPoint& point );

//	TODO :: cacheAsBitmap - maybe this can be an FBO?
//	TODO :: transform :: http://livedocs.adobe.com/flex/3/langref/flash/geom/Transform.html
//	TODO :: events - added, addedToStage, enterFrame, exitFrame, frameConstructed, removed, removedFromStage, render

	
	///////////////////////////////////////////////
	//
	//	INTERACTIVE OBJECT.
	//	http://livedocs.adobe.com/flex/3/langref/flash/display/InteractiveObject.html
	//
	///////////////////////////////////////////////
	
	bool		doubleClickEnabled;
	bool		focusRect;
	bool		mouseEnabled;
	bool		tabEnabled;
	int			tabIndex;
	
	
	///////////////////////////////////////////////
	//
	//	DISPLAY OBJECT CONTAINER.
	//	http://livedocs.adobe.com/flex/3/langref/flash/display/DisplayObjectContainer.html
	//
	///////////////////////////////////////////////
	
	bool		mouseChildren;
	bool		tabChildren;
	
	vector<ofxSprite*>	children;
	
	ofxSprite*			addChild				( ofxSprite* child );
	ofxSprite*			addChildAt				( ofxSprite* child, int index );
	bool				contains				( ofxSprite* child );
	ofxSprite*			getChildAt				( int index );
	ofxSprite*			getChildByName			( string name );
	int					getChildIndex			( ofxSprite* child );
	vector<ofxSprite*>	getObjectsUnderPoint	( ofPoint point );
	ofxSprite*			removeChild				( ofxSprite* child );
	ofxSprite*			removeChildAt			( int index );
	void				setChildIndex			( ofxSprite* child, int index );
	void				swapChildren			( ofxSprite* child1, ofxSprite* child2 );
	void				swapChildrenAt			( int index1, int index2 );
	
	//-- added methods.
	
	int					numChildren				();
	bool				hasChildren				();
	
	
	///////////////////////////////////////////////
	//
	//	SPRITE.
	//	http://livedocs.adobe.com/flex/3/langref/flash/display/Sprite.html
	//
	///////////////////////////////////////////////
	
	bool		buttonMode;
	ofxSprite*	dropTarget;
	ofxSprite*	hitArea;
	bool		useHandCursor;
	
	void	startDrag	( bool lockCenter = false, const ofRectangle* bounds = NULL );
	void	stopDrag	();
	
//	TODO :: graphics :: http://livedocs.adobe.com/flex/3/langref/flash/display/Graphics.html
	
	///////////////////////////////////////////////
	//
	//	PROTECTED.
	//
	///////////////////////////////////////////////
	
protected:

	static ofxSprite* stageRef;
	static vector<ofxSprite*> sprites;
	
	static void addSprite		( ofxSprite* sprite );
	static void removeSprite	( ofxSprite* sprite );
	
	///////////////////////////////////////////////
	//
	//	PRIVATE.
	//
	///////////////////////////////////////////////
	
private:
	
	ofRectangle	rect;
	
	void	initDisplayObjectProps			();
	void	initInteractiveObjectProps		();
	void	initDisplayObjectContainerProps	();
	void	initSpriteProps					();
	
	void	localToGlobalRecursive			( ofxSprite* parent, ofPoint& point );
};
