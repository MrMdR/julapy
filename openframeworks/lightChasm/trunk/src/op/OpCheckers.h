/*
 *  OpCheckers.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 10/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"
#include "ofFBOTexture.h"

class OpCheckers
{

public :

	OpCheckers();
	~OpCheckers();

	void init( int w, int h );
	void update();
	
	void drawToFBO();
	void drawFBOToScreen();
	void draw();
	
	void setSize( float value );
	void setSizeEase( float value );
	
private :
	
	ofFBOTexture fbo;

	int width;
	int height;
	float size;
	float sizeEase;
	
};