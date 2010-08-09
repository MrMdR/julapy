#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxFlash.h"

///////////////////////////////////////////
//	SPRITES.
///////////////////////////////////////////

class Magenta : public ofxSprite
{
public:
	
	Magenta()
	{
		x = 100;
		y = 384;
	}
	
	void update()
	{
		x += 1;
	}
	
	void draw ()
	{
		ofFill();
		ofSetColor( 255, 0, 255 );
		ofCircle( 0, 0, 350 );
	}
};

class Yellow : public ofxSprite
{
public:
	Yellow()
	{
		x = 924;
		y = 384;
	}
	
	void update()
	{
		x -= 1;
	}
	
	void draw ()
	{
		ofFill();
		ofSetColor( 255, 255, 0 );
		ofCircle( 0, 0, 350 );
	}
};

class Cyan : public ofxSprite
{
public:
	float radius;
	Cyan()
	{
		x = ofRandom( -100, 100 );
		y = ofRandom( -100, 100 );
		radius = ofRandom( 30, 70 );
	}
	
	void draw ()
	{
		ofFill();
		ofSetColor( 0, 255, 255 );
		ofCircle( 0, 0, radius );
	}
};

class Red : public ofxSprite
{
public:
	void draw ()
	{
		ofFill();
		ofSetColor( 255, 0, 0 );
		ofCircle( 0, 0, 10 );
	}
};

class BaseClass 
{
public:
	BaseClass()
	{
		init();
	};
	
	virtual void init ()
	{
		cout << "BaseClass" << endl;
	}
};

class SuperClass : BaseClass
{
public:
	SuperClass()
	{
		init();
	};
	
	virtual void init ()
	{
		cout << "SuperClass" << endl;
	}
};

///////////////////////////////////////////
//	BUSINESS AS USUAL.
///////////////////////////////////////////

class testApp : public ofBaseApp
{
public:
	
	void setup	();
	void update	();
	void draw	();

	void keyPressed		( int key );
	void keyReleased	( int key );
	void mouseMoved		( int x, int y );
	void mouseDragged	( int x, int y, int button );
	void mousePressed	( int x, int y, int button );
	void mouseReleased	( int x, int y, int button );
	void windowResized	( int w, int h );
	
	ofxStage* stage;
	SuperClass	sc;
};

#endif
