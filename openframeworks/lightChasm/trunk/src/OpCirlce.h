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
	void setAudioInValue( float value );
	void setRgbScale( float r, float g, float b );
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

	float	audioInValue;
	float	rScale;
	float	gScale;
	float	bScale;
};