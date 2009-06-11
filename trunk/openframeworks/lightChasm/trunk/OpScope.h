/*
 *  OpScope.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 11/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"

class OpScope
{
	
public :
	
	OpScope();
	~OpScope();

	void init( int w, int h );
	void update( unsigned char * pixels );
	void draw();
	
private :
	
	int width;
	int height;
	
	unsigned char	*videoPixels;
	ofTexture		videoTexture;
	
};