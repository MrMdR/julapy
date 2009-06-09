/*
 *  OpCirlce.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 8/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#define		MAX_CIRCLE_PTS		200
#define		MIN_CIRCLE_PTS		2

#include "ofMain.h"

class OpCirlce
{

public:
	
	OpCirlce();
	~OpCirlce();
	
	void setCirlceResolution( int res );
	void setInverse( );
	void setInverse( bool b );
	void setRotation( float degrees );
	void update();
	void draw();
	
	float	x;
	float	y;
	float	radius;
	
private :

	int		resolution;
	float	circlePts[ MAX_CIRCLE_PTS * 2 ];
	bool	inverse;
	float	rotation;
};