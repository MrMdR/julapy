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
	initCharacters();
	clearLetterVertices();
	
	setKerning( 1.0 );
}

RibbonType :: ~RibbonType()
{
	//
}

void RibbonType :: initCharacters()
{
	string supportedCharacter = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	
	charactersTotal	= supportedCharacter.size();
	characters		= new char[ supportedCharacter.size() + 1 ];
	strcpy( characters, supportedCharacter.c_str() );
}

void RibbonType :: loadTrueTypeFont( string fontName, int size )
{
	fontSize = size;
	font.loadFont( fontName, fontSize, true, true, true );
	
	for( int i=0; i<charactersTotal; i++ )
	{
		characterContours.push_back( font.getCharacterAsPoints( characters[ i ] ) );
		
		string str = "";
		str += characters[ i ];
		ofRectangle rect = font.getStringBoundingBox( str, 0, 0 );
		characterRectangles.push_back( CharacterRect() );
		characterRectangles.back().width	= rect.width;
		characterRectangles.back().height	= rect.height;
	}
}

int RibbonType :: getCharacterIndex ( int letter )
{
	for( int i=0; i<charactersTotal; i++ )
	{
		if( characters[ i ] == letter )
		{
			return i;
		}
	}
	return -1;
}

void RibbonType :: drawTypeOnRibbon( string copy, float *ribbonPositionArray, float *ribbonDirectionArray, int ribbonSize )
{
	ribbonPositions		= ribbonPositionArray;
	ribbonDirections	= ribbonDirectionArray;
	ribbonLength		= ribbonSize;
	ribbonLengths		= new float[ ribbonLength ];
	contourStartIndex	= 0;
	
	for( int i=0; i<ribbonLength; i++ )
	{
		int k = i * 3;
		
		if( i == 0 )
		{
			ribbonLengths[ i ] = 0;
		}
		else
		{
			ofxVec3f p1		= ofxVec3f( ribbonPositions[ k - 3 ], ribbonPositions[ k - 2 ], ribbonPositions[ k - 1 ] );
			ofxVec3f p2		= ofxVec3f( ribbonPositions[ k + 0 ], ribbonPositions[ k + 1 ], ribbonPositions[ k + 2 ] );
			ofxVec3f p21	= p2 - p1;
			
			ribbonLengths[ i ] = ribbonLengths[ i - 1 ] + p21.length();
		}
	}
	
	char * cstr;
	cstr = new char[ copy.size() + 1 ];
	strcpy( cstr, copy.c_str() );
	
	float x = 0;
	
	for( int i=0; i<copy.size(); i++ )
	{
		drawLetter( cstr[ i ], x, fontSize * 0.5 );
		
		if( cstr[ i ] == ' ' )
		{
			x += fontSize;
		}
		else
		{
			int characterIndex = getCharacterIndex( cstr[ i ] );
			if( characterIndex > -1 )
			{
				CharacterRect rect;
				rect = characterRectangles.at( characterIndex );
				x += rect.width + kerning;
			}
		}
	}
	
	delete[] cstr;

	delete ribbonPositionArray;
	delete ribbonDirectionArray;
	
	delete ribbonPositions;
	delete ribbonDirections;
	delete ribbonLengths;
}

void RibbonType :: drawLetter( int letter, float xOffset, float yOffset )
{
	int characterIndex = getCharacterIndex( letter );
	if( characterIndex == -1 )
	{
		return;
	}
	
	ofTTFCharacter ttfChar;
	ttfChar = characterContours.at( characterIndex );
	
	bool outsideOfRibbon = false;
	int contourMaxIndex = contourStartIndex;
	
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
			
			ofxVec3f cp;	// contour point.
			ofxVec3f p1;	// current ribbon point position.
			ofxVec3f p2;	// next ribbon point position.
			ofxVec3f p21;	// direction from p1 to p2;
			
			int j;
			for( j=contourStartIndex; j<( ribbonLength - 1 ) * 3; j+=3 )
			{
				p1	= ofxVec3f( ribbonPositions[ j + 0 ], ribbonPositions[ j + 1 ], ribbonPositions[ j + 2 ] );
				p2	= ofxVec3f( ribbonPositions[ j + 3 ], ribbonPositions[ j + 4 ], ribbonPositions[ j + 5 ] );
				p21	= p2 - p1;
				
				int ribbonLengthIndex = (int)( j / 3 );
				float lx = ribbonLengths[ ribbonLengthIndex ];		// lower x bounds.
				float ux = ribbonLengths[ ribbonLengthIndex + 1 ];	// upper x bounds.
				
				if( px >= lx && px < ux )	// found! contour lies between p1 and p2.
				{
					if( j > contourMaxIndex )
					{
						contourMaxIndex = j;
					}
					
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
	
	contourStartIndex = contourMaxIndex;
	
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

////////////////////////////////////////////////////////////
//	SETTERS.
////////////////////////////////////////////////////////////

void RibbonType :: setKerning( float value )
{
	kerning = value;
}
