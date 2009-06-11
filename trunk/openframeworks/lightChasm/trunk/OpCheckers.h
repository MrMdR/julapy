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
	void setSize( float size );
	void update();
	void draw();
	
private :

	int width;
	int height;
	int size;
	
};