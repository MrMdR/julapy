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
	
	useCircleColorMapImage		= false;
	useCircleColorBoundsImage	= false;
	bPaused						= false;
}

ofxCirclePacker :: ~ofxCirclePacker()
{
	reset();
}

void ofxCirclePacker :: addCircle( int color )
{
	if( bPaused )
		return;
	
	circles.push_back( Circle() );
	
	Circle *circle;
	circle = &circles.back();
	
	circle->loc.x		= ofRandom( circleColorBoundsImageRect.x, circleColorBoundsImageRect.x + circleColorBoundsImageRect.width  );
	circle->loc.y		= ofRandom( circleColorBoundsImageRect.y, circleColorBoundsImageRect.y + circleColorBoundsImageRect.height );
	circle->radius		= circleRadiusMin;
	circle->alive		= true;
	circle->growth		= ofRandom( 0.1, 0.5 );
	circle->lifeCount	= 0;
	
	if( useCircleColorMapImage )
		circle->color	= getImageColor( (int)circle->loc.x, (int)circle->loc.y, circleColorMapImagePixels, &circleColorMapImageRect );
	else
		circle->color	= color;
}

void ofxCirclePacker :: update()
{
	if( circles.size() == 0 )
		return;
	
	if( bPaused )
		return;
	
	checkCircleCollision();
	
	if( useCircleColorBoundsImage )
		checkCircleImage();
	
	removeInvalidCircles();
	
	for( int i=0; i<circles.size(); i++ )
	{
		if( circles[ i ].alive )
		{
			circles[ i ].radius		+= circles[ i ].growth;
			circles[ i ].lifeCount	+= 1;
			
			if( circles[ i ].radius > circleRadiusMax )
			{
				circles[ i ].alive = false;
			}
		}
	}
}

void ofxCirclePacker :: draw()
{
	for( int i=0; i<circles.size(); i++ )
	{
		ofSetColor( circles[ i ].color );
		
		ofFill();
		ofCircle( circles[ i ].loc.x, circles[ i ].loc.y, circles[ i ].radius );
		ofNoFill();
		ofCircle( circles[ i ].loc.x, circles[ i ].loc.y, circles[ i ].radius );
	}
}

void ofxCirclePacker :: reset ()
{
	circles.clear();
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
	
	bPaused = true;
	
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
			
			Circle *circle;
			circle = &circles.back();
			
			circle->loc.x		= atof( circleData[ 0 ].c_str() );
			circle->loc.y		= atof( circleData[ 1 ].c_str() );
			circle->color		= atoi( circleData[ 2 ].c_str() );
			circle->growth		= atof( circleData[ 3 ].c_str() );
			circle->alive		= atoi( circleData[ 4 ].c_str() );
			circle->lifeCount	= atoi( circleData[ 5 ].c_str() );
			circle->radius		= atof( circleData[ 6 ].c_str() );
		}
	}
	
	circleDataFile.close();
}

//////////////////////////////////////////
//	GET CIRCLES.
//////////////////////////////////////////

vector<Circle> * ofxCirclePacker :: getCircles ()
{
	return &circles;
}

//////////////////////////////////////////
//	CHECKS.
//////////////////////////////////////////

void ofxCirclePacker :: checkCircleCollision ()
{
	for( int i=0; i<circles.size()-1; i++)
	{
		for( int j=i+1; j<circles.size(); j++ )
		{
			if( !circles[ i ].alive && !circles[ j ].alive )
				continue;
			
			if
			( 
				fastCircleCollision
				(
					circles[ i ].loc.x, circles[ i ].loc.y,
					circles[ j ].loc.x, circles[ j ].loc.y,
					circles[ i ].radius, 
					circles[ j ].radius + circleGap
				)
			)
			{
				circles[ i ].alive = false;
				circles[ j ].alive = false;
			}
		}
	}
}

void ofxCirclePacker :: checkCircleImage()
{
	circleColorBoundsImagePixels = circleColorBoundsImage->getPixels();
	
	for( int i=0; i<circles.size(); i++)
	{
		if( !circles[ i ].alive )
			continue;
		
		//-- CHECK CIRCLE IS WITHIN IMAGE BOUNDS.
		
		if
			(
			 circles[ i ].loc.x - ( circles[ i ].radius + circleDeathGap ) < circleColorBoundsImageRect.x										||
			 circles[ i ].loc.x + ( circles[ i ].radius + circleDeathGap ) > circleColorBoundsImageRect.x + circleColorBoundsImageRect.width	||
			 circles[ i ].loc.y - ( circles[ i ].radius + circleDeathGap ) < circleColorBoundsImageRect.y										||
			 circles[ i ].loc.y + ( circles[ i ].radius + circleDeathGap ) > circleColorBoundsImageRect.y + circleColorBoundsImageRect.height
			)
		{
			circles[ i ].alive = false;
			
			continue;
		}
		
		//-- CHECK CIRCLE IS WITHIN IMAGE SHAPE.
		
		int cx = (int)( circles[ i ].loc.x - ( circles[ i ].radius + circleDeathGap ) );
		int cy = (int)( circles[ i ].loc.y - ( circles[ i ].radius + circleDeathGap ) );
		int cw = (int)( ( circles[ i ].radius + circleDeathGap ) * 2 );
		int ch = (int)( ( circles[ i ].radius + circleDeathGap ) * 2 );
		bool inBounds = true;
		
		for( int x=cx; x<=cx+cw; x++ )
		{
			for( int y=cy; y<=cy+ch; y++ )
			{
				int c = getImageColor( x, y, circleColorBoundsImagePixels, &circleColorBoundsImageRect );
				
				if( circleColorBounds )
				{
					if( c != circles[ i ].color )
					{
						circles[ i ].alive = false;
						
						inBounds = false;
						
						break;
					}
				}
				
				if( c == circleDeathColor )
				{
					circles[ i ].alive = false;
					
					inBounds = false;
					
					break;
				}
			}
			
			if( !inBounds )
				break;
		}
	}
}

void ofxCirclePacker :: removeInvalidCircles ()
{
	for( int i=0; i<circles.size(); i++)
	{
		if( circles[ i ].lifeCount == 0 && !circles[ i ].alive )
		{
			circles.erase( circles.begin() + i-- );
		}
	}
}

//////////////////////////////////////////
//	SETTERS.
//////////////////////////////////////////

void ofxCirclePacker :: setColorBoundsImage ( ofImage *image, ofRectangle *imageRect )
{
	circleColorBoundsImage				= image;
	circleColorBoundsImagePixels		= circleColorBoundsImage->getPixels();
	
	circleColorBoundsImageRect.x		= imageRect->x;
	circleColorBoundsImageRect.y		= imageRect->y;
	circleColorBoundsImageRect.width	= imageRect->width;
	circleColorBoundsImageRect.height	= imageRect->height;
	
	useCircleColorBoundsImage = true;
}

void ofxCirclePacker :: setColorMapImage ( ofImage *image, ofRectangle *imageRect )
{
	circleColorMapImage				= image;
	circleColorMapImagePixels		= circleColorMapImage->getPixels();
	
	circleColorMapImageRect.x		= imageRect->x;
	circleColorMapImageRect.y		= imageRect->y;
	circleColorMapImageRect.width	= imageRect->width;
	circleColorMapImageRect.height	= imageRect->height;
	
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

void ofxCirclePacker :: togglePause ()
{
	bPaused = !bPaused;
}

//////////////////////////////////////////
//	UTIL.
//////////////////////////////////////////

int ofxCirclePacker :: getImageColor ( int x, int y, unsigned char *pixels, ofRectangle *imageRect )
{
	int p = ( ( x - imageRect->x ) * 3 ) + ( ( y - imageRect->y ) * imageRect->width * 3 );

	unsigned char r = pixels[ p + 0 ];
	unsigned char g = pixels[ p + 1 ];
	unsigned char b = pixels[ p + 2 ];
	
	return ofRgbToHex( r, g, b );
}

//////////////////////////////////////////
//	MATH.
//////////////////////////////////////////

float ofxCirclePacker :: distance( float x1, float y1, float x2, float y2 )
{
	return sqrtf( (x1-x2) * (x1-x2) + (y1-y2) * (y1-y2) );
}

float ofxCirclePacker :: fastDistance( float x1, float y1, float x2, float y2 )
{
	return (x1-x2) * (x1-x2) + (y1-y2) * (y1-y2);
}

bool ofxCirclePacker :: circleCollision( float x1, float y1, float x2, float y2, float radius1, float radius2 )
{
	if ( distance( x1, y1, x2, y2 ) > radius1 + radius2 )
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool ofxCirclePacker :: fastCircleCollision( float x1, float y1, float x2, float y2, float radius1, float radius2 )
{
	if ( fastDistance( x1, y1, x2, y2 ) > ( radius1 + radius2 ) * ( radius1 + radius2 ) )
	{
		return false;
	}
	else
	{
		return true;
	}
}