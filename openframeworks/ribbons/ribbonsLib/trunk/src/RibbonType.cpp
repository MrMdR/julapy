/*
 *  RibbonType.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 30/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "RibbonType.h"

RibbonType :: RibbonType()
{
	clearLetterVertices();
}

RibbonType :: ~RibbonType()
{

}

void RibbonType :: loadTrueTypeFont( string fontName, int size )
{
	fontSize = size;
	font.loadFont( fontName, fontSize, true, true, true );
}

void RibbonType :: drawTypeOnRibbon( string copy, float *ribbonPositionArray, float *ribbonDirectionArray, int ribbonSize )
{
	ribbonPositions		= ribbonPositionArray;
	ribbonDirections	= ribbonDirectionArray;
	ribbonLength		= ribbonSize;
	
	char * cstr;
	cstr = new char[ copy.size() + 1 ];
	strcpy( cstr, copy.c_str() );
	
	int x = 0;
	int pad = 10;
	
	for( int i=0; i<copy.size(); i++ )
	{
		string str = "";
		str += cstr[ i ];
		
		drawLetter( cstr[ i ], x, fontSize * 0.5 );
		
		if( cstr[ i ] == ' ' )
		{
			x += 50;
		}
		else
		{
			ofRectangle rect = font.getStringBoundingBox( str, 0, 0 );
			x += rect.width + pad;
		}
	}
	
	delete[] cstr;

	delete ribbonPositionArray;
	delete ribbonDirectionArray;
	
	delete ribbonPositions;
	delete ribbonDirections;
}

void RibbonType :: drawLetter( int letter, float xOffset, float yOffset )
{
	ofTTFCharacter ttfChar;
	ttfChar = font.getCharacterAsPoints( letter );
	
	bool outsideOfRibbon = false;
	
	for( int k=0; k<ttfChar.contours.size(); k++ )
	{
		if( k != 0 )
		{
			drawLetterVertices();
		}
		
		for( int i=0; i<ttfChar.contours[ k ].pts.size(); i++ )
		{
			float px = ttfChar.contours[ k ].pts[ i ].x + xOffset;
			float py = ttfChar.contours[ k ].pts[ i ].y + yOffset;
			
			float rw = 0;	// ribbon width.
			float lx = 0;	// lower x bounds.
			float ux = 0;	// upper x bounds.
			
			ofxVec3f cp;	// contour point.
			ofxVec3f p1;	// current ribbon point position.
			ofxVec3f p2;	// next ribbon point position.
			ofxVec3f p21;	// direction from p1 to p2;
			
			int j;
			for( j=0; j<( ribbonLength - 1 ) * 3; j+=3 )
			{
				p1	= ofxVec3f( ribbonPositions[ j + 0 ], ribbonPositions[ j + 1 ], ribbonPositions[ j + 2 ] );
				p2	= ofxVec3f( ribbonPositions[ j + 3 ], ribbonPositions[ j + 4 ], ribbonPositions[ j + 5 ] );
				p21	= p2 - p1;
				
				lx = rw;
				ux = rw + p21.length();
				rw = ux;
				
				if( px >= lx && px < ux )
				{
					float p = ( px - lx ) / ( ux - lx );
					
					cp = p1 + p21 * p;
					
					break;
				}
				
				if( j == ( ribbonLength - 2 ) * 3 )	// last point.
				{
					outsideOfRibbon = true;
				}
			}
			
			if( outsideOfRibbon )
			{
				break;
			}
			
			ofxVec3f cd = ofxVec3f( ribbonDirections[ j + 0 ], ribbonDirections[ j + 1 ], ribbonDirections[ j + 2 ] );
			cd *= py;
			cp += cd;
			
			float *point = new float[ 3 ];
			point[ 0 ] = cp.x;
			point[ 1 ] = cp.y;
			point[ 2 ] = cp.z;
			
			polyVertices.push_back( point );
		}
		
		if( outsideOfRibbon )
		{
			break;
		}
	}
	
	if( !outsideOfRibbon )
	{
		drawLetterVertices();
	}
	
	clearLetterVertices();
}

void RibbonType :: drawLetterVertices()
{
	if( (int)polyVertices.size() > currentStartVertex )
	{
		float *point = new float[ 3 ];
		point[ 0 ] = polyVertices[ currentStartVertex ][ 0 ];
		point[ 1 ] = polyVertices[ currentStartVertex ][ 1 ];
		point[ 2 ] = polyVertices[ currentStartVertex ][ 2 ];
		polyVertices.push_back( point );
	}
	
	int numToDraw = polyVertices.size() - currentStartVertex;
	if( numToDraw > 0)
	{
		GLfloat *points = new GLfloat[ polyVertices.size() * 3 ];
		
		int k = 0;
		
		for( int i=currentStartVertex; i<(int)polyVertices.size(); i++ )
		{
			points[ k + 0 ]	= polyVertices[ i ][ 0 ];
			points[ k + 1 ] = polyVertices[ i ][ 1 ];
			points[ k + 2 ] = polyVertices[ i ][ 2 ];
			
			k += 3;
		}
		
		glColor4f( 0, 0, 0, 1 );
		glEnableClientState( GL_VERTEX_ARRAY );
		glVertexPointer( 3, GL_FLOAT, 0, &points[0] );
		glDrawArrays( GL_LINE_STRIP, 0, numToDraw );
		
		delete [] points;
		
		currentStartVertex = (int)polyVertices.size();
	}
}

void RibbonType :: clearLetterVertices()
{
	currentStartVertex = 0;
	for( vector<float*>::iterator itr=polyVertices.begin(); itr!=polyVertices.end(); ++itr )
	{
		delete [] (*itr);
	}
	polyVertices.clear();
}
