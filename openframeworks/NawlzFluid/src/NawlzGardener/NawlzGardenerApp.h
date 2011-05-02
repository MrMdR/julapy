//
//  NawlzGardenerApp.h
//  emptyExample
//
//  Created by lukasz karluk on 2/05/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"

#ifndef TARGET_OF_IPHONE

#include "ofxSimpleGuiToo.h"

#include "NawlzGardener.h"

class NawlzGardenerApp : public ofBaseApp
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
	
	bool				bDebug;
    
    ofImage             img;
	
	ofxSimpleGuiToo		gui;
    
    NawlzGardener       app;
    
};

#endif
