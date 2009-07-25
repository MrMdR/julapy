/*
 *  OpCheckers.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 10/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"

class OpCheckers
{

public :

	OpCheckers();
	~OpCheckers();

	void init( int w, int h );
	void update();
	void draw();
	
	void setSize( float value );
	void setSizeEase( float value );
	
private :

	int width;
	int height;
	float size;
	float sizeEase;
	
};