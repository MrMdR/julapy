//
//  NawlzJackInBox.h
//  emptyExample
//
//  Created by lukasz karluk on 21/03/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"

class NawlzJackInBox : public ofBaseApp
{
    
public:
    
    void setup  ();
    void update ();
    void draw   ();
    
    void createBackgroundTexture	( unsigned char* pixels, int width, int height, int glType, int x=0, int y=0 );
    
	void keyPressed		( int key );
	void keyReleased	( int key );
	void mouseMoved		( int x, int y );
	void mouseDragged	( int x, int y, int button );
	void mousePressed	( int x, int y, int button );
	void mouseReleased	( int x, int y, int button );
	void windowResized	( int w, int h );
    
    ofTexture*  backgroundTexture;
    
    ofxBox2d    box2d;
    
};