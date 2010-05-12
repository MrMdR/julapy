/*
 *  ofxCirclePacker.cpp
 *  julapy_sydfest
 *
 *  Created by lukasz karluk on 22/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxCirclePacker.h"

ofxCirclePacker :: ofxCirclePacker()
{
	setCircleRadiusMin( 2.0 );
	setCircleRadiusMax( 400.0 );
	setCircleGap( 2.0 );
	setCircleDeathGap ( 0.0 );
	setCircleDeathColor( 0x000000 );
	setCircleColorBounds( false );
	
	circleIdCount	= 0;
	
	useCircleColorMapImage		= false;
	useCircleColorBoundsImage	= false;
}

ofxCirclePacker :: ~ofxCirclePacker()
{
	reset();
}

void ofxCirclePacker :: addCircles( int numOfCircles, int color )
{
	for( int i=0; i<numOfCircles; i++ )
	{
		addCircle( color );
	}
}

void ofxCirclePacker :: addCircle( int color )
{
	circles.push_back( Circle() );
	Circle& circle = circles.back();
	
	circle.id				= circleIdCount;
	circle.loc.x			= ofRandom( (float)circleColorBoundsImageRect.x, (float)( circleColorBoundsImageRect.x + circleColorBoundsImageRect.width  ) );
	circle.loc.y			= ofRandom( (float)circleColorBoundsImageRect.y, (float)( circleColorBoundsImageRect.y + circleColorBoundsImageRect.height ) );
	circle.lifeCount		= 0;
	circle.growth			= ofRandom( 0.1, 0.5 );
	circle.radius			= 0;
	circle.radiusNext		= circle.growth;
	circle.alive			= true;
	circle.bUnderMinRadius	= true;
	
	
	if( useCircleColorMapImage )
	{
		circle.color	= getImageColor( (int)circle.loc.x, (int)circle.loc.y, circleColorMapImagePixels, circleColorMapImageRect );
	}
	else
	{
		circle.color	= color;
	}
	
	bool bValid;
	bValid = true;
	
	if( useCircleColorBoundsImage )
	{
		bValid = bValid && checkCircleImage( circle );					// fits on image.
	}
	
	if( bValid )
	{
		bValid = bValid && !checkCircleCollisionWithAll( circle );		// no collision.
	}
	
	if( bValid )									// circle is valid on first pass.
	{
		++circleIdCount;
		
		findNeighbours( circle );					// circle fits, find neighbours.
	}
	else											
	{
		circles.erase( circles.end() - 1 );			// did not fit, remove.
	}
}

void ofxCirclePacker :: update()
{
	for( int i=0; i<circles.size(); i++ )
	{
		Circle& circle = circles[ i ];

		bool bValid;
		bValid = circle.alive;
		
		if( bValid && useCircleColorBoundsImage )
		{
			bValid = bValid && checkCircleImage( circle );
		}
		
		if( bValid )
		{
//			bValid = bValid && !checkCircleCollisionWithNeighbours( circle );		// NOTE : partially working, some weird bug i can't figure out.
			bValid = bValid && !checkCircleCollisionWithAll( circle );
		}
		
		if( bValid || circle.bUnderMinRadius )					// circle is valid or is under min radius, increase in size.
		{
			circle.radius			+= circle.growth;
			circle.radiusNext		+= circle.growth;
			circle.lifeCount		+= 1;
			
			circle.alive			= ( circles[ i ].radius <= circleRadiusMax );
			circle.bUnderMinRadius	= ( circles[ i ].radius < circleRadiusMin );
		}
 	}
}

void ofxCirclePacker :: draw()
{
	for( int i=0; i<circles.size(); i++ )
	{
		ofSetColor( circles[ i ].color );
		
		ofCircle( circles[ i ].loc.x, circles[ i ].loc.y, circles[ i ].radius );
	}
}

void ofxCirclePacker :: reset ()
{
	circles.clear();
}

//////////////////////////////////////////
//	CHECKS.
//////////////////////////////////////////

void ofxCirclePacker :: findNeighbours ( Circle& c1 )
{
	for( int i=0; i<circles.size(); i++ )
	{
		Circle& c2 = circles[ i ];
		
		if( c1.id == c2.id )					// same circle, skip.
			continue;
		
		float d = distance( c1.loc, c2.loc );
		
		if( d < circleRadiusMax * 2 )			// we're neighbours!
		{
			c1.neighbours.push_back( &c2 );
			c2.neighbours.push_back( &c1 );
			
//			cout << "c_" << c1.id << " and " << "c_" << c2.id << " are neightbours." << endl;
		}
	}
}

bool ofxCirclePacker :: checkCircleCollisionWithAll ( Circle& c1 )
{
	for( int i=0; i<circles.size(); i++ )
	{
		Circle& c2 = circles[ i ];
		
		if( checkCircleCollision( c1, c2 ) )
		{
			return true;
		}
	}
	
	return false;
}

bool ofxCirclePacker :: checkCircleCollisionWithNeighbours ( Circle& c1 )
{
	for( int i=0; i<c1.neighbours.size(); i++ )
	{
		Circle& c2 = *c1.neighbours[ i ];
		
		if( checkCircleCollision( c1, c2 ) )
		{
			return true;
		}
	}
	
	return false;
}

bool ofxCirclePacker :: checkCircleCollision( Circle& c1, Circle& c2 )
{
	if( c1.id == c2.id )	// can't collide with itself.
		return false;
	
	float r1 = ( c1.bUnderMinRadius ) ? circleRadiusMin : c1.radiusNext;
	float r2 = ( c2.bUnderMinRadius ) ? circleRadiusMin : c2.radiusNext;
	
	bool bCollision;
	bCollision = fastCircleCollision
	(
		c1.loc,
		c2.loc,
		r1 + circleGap * 0.5,
		r2 + circleGap * 0.5
	);
	
	if( bCollision )
	{
		c1.alive = false;
		c2.alive = false;
	}
	
	return bCollision;
}

bool ofxCirclePacker :: checkCircleImage ( Circle& circle )
{
	float radius = ( circle.bUnderMinRadius ) ? circleRadiusMin : circle.radiusNext;
	
	//-- CHECK CIRCLE IS WITHIN IMAGE BOUNDS.
	
	bool lb = circle.loc.x - ( radius + circleDeathGap ) < circleColorBoundsImageRect.x;
	bool rb = circle.loc.x + ( radius + circleDeathGap ) > circleColorBoundsImageRect.x + circleColorBoundsImageRect.width;
	bool tb = circle.loc.y - ( radius + circleDeathGap ) < circleColorBoundsImageRect.y;
	bool bb = circle.loc.y + ( radius + circleDeathGap ) > circleColorBoundsImageRect.y + circleColorBoundsImageRect.height;
	
	if( lb || rb || tb || bb )
	{
		circle.alive = false;
		
		return false;
	}
	
	//-- CHECK CIRCLE IS WITHIN IMAGE SHAPE.
	
	int cx = (int)( circle.loc.x - ( radius + circleDeathGap ) );
	int cy = (int)( circle.loc.y - ( radius + circleDeathGap ) );
	int cw = (int)( ( radius + circleDeathGap ) * 2 );
	int ch = (int)( ( radius + circleDeathGap ) * 2 );
	
	for( int x=cx; x<=cx+cw; x++ )
	{
		for( int y=cy; y<=cy+ch; y++ )
		{
			int c = getImageColor( x, y, circleColorBoundsImagePixels, circleColorBoundsImageRect );
			
			if( circleColorBounds )
			{
				if( c != circle.color )
				{
					circle.alive = false;
					
					return false;
				}
			}
			
			if( c == circleDeathColor )
			{
				circle.alive = false;
				
				return false;
			}
		}
	}
	
	return true;
}

//////////////////////////////////////////
//	GET CIRCLES.
//////////////////////////////////////////

vector<Circle>& ofxCirclePacker :: getCircles ()
{
	return circles;
}

//////////////////////////////////////////
//	SETTERS.
//////////////////////////////////////////

void ofxCirclePacker :: setColorBoundsImage ( ofImage *image, const ofRectangle& imageRect )
{
	circleColorBoundsImage				= image;
	circleColorBoundsImagePixels		= circleColorBoundsImage->getPixels();
	
	circleColorBoundsImageRect.x		= imageRect.x;
	circleColorBoundsImageRect.y		= imageRect.y;
	circleColorBoundsImageRect.width	= imageRect.width;
	circleColorBoundsImageRect.height	= imageRect.height;
	
	useCircleColorBoundsImage = true;
}

void ofxCirclePacker :: setColorMapImage ( ofImage *image, const ofRectangle& imageRect )
{
	circleColorMapImage				= image;
	circleColorMapImagePixels		= circleColorMapImage->getPixels();
	
	circleColorMapImageRect.x		= imageRect.x;
	circleColorMapImageRect.y		= imageRect.y;
	circleColorMapImageRect.width	= imageRect.width;
	circleColorMapImageRect.height	= imageRect.height;
	
	useCircleColorMapImage = true;
}

void ofxCirclePacker :: setCircleRadiusMin	( float radiusMin )
{
	circleRadiusMin = radiusMin;
}

void ofxCirclePacker :: setCircleRadiusMax	( float radiusMax )
{
	circleRadiusMax = radiusMax;
}

void ofxCirclePacker :: setCircleGap ( float gap )
{
	circleGap = gap;
}

void ofxCirclePacker :: setCircleDeathGap ( float gap )
{
	circleDeathGap = gap;
}

void ofxCirclePacker :: setCircleDeathColor	( int color )
{
	circleDeathColor = color;
}

void ofxCirclePacker :: setCircleColorBounds ( bool useColorBounds )
{
	circleColorBounds = useColorBounds;
}

//////////////////////////////////////////
//	UTIL.
//////////////////////////////////////////

int ofxCirclePacker :: getImageColor ( int x, int y, unsigned char *pixels, const ofRectangle& imageRect )
{
	int p = ( ( x - imageRect.x ) * 3 ) + ( ( y - imageRect.y ) * imageRect.width * 3 );

	unsigned char r = pixels[ p + 0 ];
	unsigned char g = pixels[ p + 1 ];
	unsigned char b = pixels[ p + 2 ];
	
	return ofRgbToHex( r, g, b );
}

//////////////////////////////////////////
//	MATH.
//////////////////////////////////////////

float ofxCirclePacker :: distance( const ofPoint& p1, const ofPoint& p2 )
{
	return sqrtf( ( p1.x - p2.x ) * ( p1.x - p2.x ) + ( p1.y - p2.y ) * ( p1.y - p2.y ) );
}

float ofxCirclePacker :: fastDistance( const ofPoint& p1, const ofPoint& p2 )
{
	return ( p1.x - p2.x ) * ( p1.x - p2.x ) + ( p1.y - p2.y ) * ( p1.y - p2.y );
}

bool ofxCirclePacker :: circleCollision( const ofPoint& p1, const ofPoint& p2, float radius1, float radius2 )
{
	if ( distance( p1, p2 ) > radius1 + radius2 )
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool ofxCirclePacker :: fastCircleCollision( const ofPoint& p1, const ofPoint& p2, float radius1, float radius2 )
{
	if ( fastDistance( p1, p2 ) > ( radius1 + radius2 ) * ( radius1 + radius2 ) )
	{
		return false;
	}
	else
	{
		return true;
	}
}

//////////////////////////////////////////
//	SAVE / LOAD.
//////////////////////////////////////////

void ofxCirclePacker :: writeToFile ( string filename )
{
	ofstream fout;
	fout.open( ofToDataPath( filename ).c_str() );
	
	for( int i=0; i<circles.size(); i++ )
	{
		string circleDataStr;
		
		circleDataStr = 
		ofToString( circles[ i ].loc.x,		5 ) + " " +
		ofToString( circles[ i ].loc.y,		5 ) + " " +
		ofToString( circles[ i ].color,		0 ) + " " +
		ofToString( circles[ i ].growth,	5 ) + " " +
		ofToString( circles[ i ].alive,		0 ) + " " +
		ofToString( circles[ i ].lifeCount,	0 ) + " " +
		ofToString( circles[ i ].radius,	5 );
		
		fout << circleDataStr << endl;
	}
	
	fout.close();
}

void ofxCirclePacker :: loadFromFile ( string filename )
{
	reset();
	
	ifstream	circleDataFile;
	string		circleDataStr;
	
	circleDataFile.open( ofToDataPath( filename ).c_str() );
	
	if( circleDataFile.is_open() )
	{
		while( !circleDataFile.eof() )
		{
			getline( circleDataFile, circleDataStr );
			
			if( circleDataStr == "" )
				continue;
			
			vector<string> circleData;
			circleData = ofSplitString( circleDataStr, " " );
			
			circles.push_back( Circle() );
			Circle& circle = circles.back();
			
			circle.loc.x		= atof( circleData[ 0 ].c_str() );
			circle.loc.y		= atof( circleData[ 1 ].c_str() );
			circle.color		= atoi( circleData[ 2 ].c_str() );
			circle.growth		= atof( circleData[ 3 ].c_str() );
			circle.alive		= atoi( circleData[ 4 ].c_str() );
			circle.lifeCount	= atoi( circleData[ 5 ].c_str() );
			circle.radius		= atof( circleData[ 6 ].c_str() );
		}
	}
	
	circleDataFile.close();
}