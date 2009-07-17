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
	
	setKerning( 1.0 );
	wrapRibbonSurface( false );
	noFill();
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

void RibbonType :: initCharacterContours ()
{
	for( int i=0; i<charactersTotal; i++ )
	{
		characterContours.push_back( font.getCharacterAsPoints( characters[ i ] ) );
	}
}

void RibbonType :: initCharacterRectangles()
{
	for( int i=0; i<charactersTotal; i++ )
	{
		string str = "";
		str += characters[ i ];
		ofRectangle rect = font.getStringBoundingBox( str, 0, 0 );
		characterRectangles.push_back( CharacterRect() );
		characterRectangles.back().width	= rect.width;
		characterRectangles.back().height	= rect.height;
	}
}

void RibbonType :: initCharacterVertices()
{
	charShapes = new CharShape[ charactersTotal ];
	
	for( int i=0; i<charactersTotal; i++ )
	{
		ofTTFCharacter ttfChar;
		ttfChar = characterContours.at( i );
		
		CharacterRect charRect;
		charRect = characterRectangles.at( i );
		
		int shapesTotal			= 0;
		int shapeIndex			= 0;
		int shapePointsTotal	= 0;
		
		shapesTotal = ttfChar.contours.size();
		for( int j=0; j<shapesTotal; j++ )
		{
			shapePointsTotal += ( ttfChar.contours[ j ].pts.size() + 1 ) * 3;
		}
		
		charShapes[ i ].shapesTotal			= shapesTotal;
		charShapes[ i ].shapePointsTotal	= shapePointsTotal;
		charShapes[ i ].shapeIndex			= new int[ shapesTotal ];
		charShapes[ i ].shapePointsLength	= new int[ shapesTotal ];
		charShapes[ i ].shapePoints			= new float[ shapePointsTotal ];
		charShapes[ i ].shapeVBOs			= new GLuint[ shapesTotal ];
		
		for( int j=0; j<shapesTotal; j++ )
		{
			charShapes[ i ].shapeIndex[ j ]	= shapeIndex;
			
			int n = 0;
			for( int k=0; k<ttfChar.contours[ j ].pts.size(); k++ )
			{
				n = shapeIndex + ( k * 3 );
				charShapes[ i ].shapePoints[ n + 0 ] = ttfChar.contours[ j ].pts[ k ].x - charRect.width * 0.5;
				charShapes[ i ].shapePoints[ n + 1 ] = ttfChar.contours[ j ].pts[ k ].y + fontSize * 0.5;
				charShapes[ i ].shapePoints[ n + 2 ] = 0;
			}
			n += 3;
			
			charShapes[ i ].shapePoints[ n + 0 ] = charShapes[ i ].shapePoints[ shapeIndex + 0 ];	// close loop by adding first value of shape 
			charShapes[ i ].shapePoints[ n + 1 ] = charShapes[ i ].shapePoints[ shapeIndex + 1 ];	// as last value.
			charShapes[ i ].shapePoints[ n + 2 ] = charShapes[ i ].shapePoints[ shapeIndex + 2 ];
			
			n += 3;
			
			charShapes[ i ].shapePointsLength[ j ] = ( n - shapeIndex );
			
			shapeIndex += n;
		}
	}
}

void RibbonType :: initCharacterOutline()
{
	int vboTotal = 0;
	for( int i=0; i<charactersTotal; i++ )
	{
		ofTTFCharacter ttfChar;
		ttfChar		= characterContours.at( i );
		vboTotal	+= ttfChar.contours.size();
	}
		
	charShapeVBOs = new GLuint[ vboTotal ];
	glGenBuffersARB( vboTotal, &charShapeVBOs[ 0 ] );
	
	int vboIndex = 0;
	for( int i=0; i<charactersTotal; i++ )
	{
		for( int j=0; j<charShapes[ i ].shapesTotal; j++ )
		{
			int shapeIndex			= charShapes[ i ].shapeIndex[ j ];
			int shapePointsLength	= charShapes[ i ].shapePointsLength[ j ];
			
			glBindBufferARB( GL_ARRAY_BUFFER_ARB, charShapeVBOs[ vboIndex ] );
			glBufferDataARB( GL_ARRAY_BUFFER_ARB, shapePointsLength * sizeof( float ), &(charShapes[ i ].shapePoints[ shapeIndex ]), GL_STATIC_DRAW_ARB );
			
			charShapes[ i ].shapeVBOs[ j ] = vboIndex;
			
			++vboIndex;
		}
	}
}

////////////////////////////////////////////////////////////
//	TESSELATION.
////////////////////////////////////////////////////////////

GLUtesselator *tessObj = NULL;
std::vector <int> tessShapeFillIndex;
std::vector <int> tessShapeFillTypes;
std::vector <float> tessShapeFillPoints;

void tesselationVertex( void *data );
void tesselationBegin( GLint type );
void tesselationEnd();

void tesselationVertex( void *data )
{
	tessShapeFillPoints.push_back( ( (double *)data)[0] );
	tessShapeFillPoints.push_back( ( (double *)data)[1] );
	tessShapeFillPoints.push_back( ( (double *)data)[2] );
}

void tesselationBegin( GLint type )
{
	tessShapeFillIndex.push_back( tessShapeFillPoints.size() );
	tessShapeFillTypes.push_back( type );
}

void tesselationEnd()
{
	//
}

void RibbonType :: initCharacterFill()
{
	int vboTotal = 0; 
	
	for( int i=0; i<charactersTotal; i++ )
	{
		tessObj = gluNewTess();
		gluTessCallback( tessObj, GLU_TESS_VERTEX,	(GLvoid(*)())&tesselationVertex );
		gluTessCallback( tessObj, GLU_TESS_BEGIN,	(GLvoid(*)())&tesselationBegin );
		gluTessCallback( tessObj, GLU_TESS_END,		(GLvoid(*)())&tesselationEnd );
		
		gluTessProperty( tessObj, GLU_TESS_WINDING_RULE, OF_POLY_WINDING_ODD );
		gluTessProperty( tessObj, GLU_TESS_BOUNDARY_ONLY, false );
		gluTessProperty( tessObj, GLU_TESS_TOLERANCE, 0 );
		gluTessNormal( tessObj, 0.0, 0.0, 1.0 );
		gluTessBeginPolygon( tessObj, NULL );
		
		vector <double*> shapePoints;
		
		for( int j=0; j<charShapes[ i ].shapesTotal; j++ )
		{
			int shapeIndex			= charShapes[ i ].shapeIndex[ j ];
			int shapePointsLength	= charShapes[ i ].shapePointsLength[ j ];
			
			gluTessBeginContour( tessObj );
			for( int k=shapeIndex; k<shapeIndex+shapePointsLength; k+=3 )
			{
				double *point = new double[ 3 ];
				point[ 0 ] = charShapes[ i ].shapePoints[ k + 0 ];
				point[ 1 ] = charShapes[ i ].shapePoints[ k + 1 ];
				point[ 2 ] = charShapes[ i ].shapePoints[ k + 2 ];
				
				shapePoints.push_back( point );
				
				gluTessVertex( tessObj, point, point );
			}
			gluTessEndContour( tessObj );
		}
		
		if( tessObj != NULL )
		{
			gluTessEndPolygon( tessObj );
			gluDeleteTess( tessObj );
			tessObj = NULL;
		}
		
		for( vector<double*>::iterator itr=shapePoints.begin(); itr!=shapePoints.end(); ++itr )
		{
			delete [] (*itr);
		}
		shapePoints.clear();
		
		int tessShapesTotal			= tessShapeFillIndex.size();
		int tessShapePointsTotal	= tessShapeFillPoints.size();
		
		charShapes[ i ].shapeFillTotal			= tessShapesTotal;
		charShapes[ i ].shapeFillPointsTotal	= tessShapePointsTotal;
		charShapes[ i ].shapeFillIndex			= new int[ tessShapesTotal ];
		charShapes[ i ].shapeFillTypes			= new int[ tessShapesTotal ];
		charShapes[ i ].shapeFillPointsLength	= new int[ tessShapesTotal ];
		charShapes[ i ].shapeFillPoints			= new float[ tessShapePointsTotal ];
		charShapes[ i ].shapeFillVBOs			= new GLuint[ tessShapesTotal ];

		vboTotal += tessShapesTotal;
		
		for( int j=0; j<tessShapesTotal; j++ )
		{
			int shapeFillIndex			= tessShapeFillIndex.at( j );
			int shapeFillTypes			= tessShapeFillTypes.at( j );
			int shapeFillPointsLength	= 0;
			
			if( j < tessShapesTotal - 1 )
			{
				shapeFillPointsLength = tessShapeFillIndex.at( j + 1 ) - tessShapeFillIndex.at( j );
			}
			else
			{
				shapeFillPointsLength = tessShapeFillPoints.size() - tessShapeFillIndex.at( j );
			}
			
			charShapes[ i ].shapeFillIndex[ j ]			= shapeFillIndex;
			charShapes[ i ].shapeFillTypes[ j ]			= shapeFillTypes;
			charShapes[ i ].shapeFillPointsLength[ j ]	= shapeFillPointsLength;
		}
		
		for( int j=0; j<tessShapePointsTotal; j++ )
		{
			charShapes[ i ].shapeFillPoints[ j ] = tessShapeFillPoints.at( j );
		}
		
		tessShapeFillIndex.clear();
		tessShapeFillTypes.clear();
		tessShapeFillPoints.clear();
	}
	
	charShapeFillVBOs = new GLuint[ vboTotal ];
	glGenBuffersARB( vboTotal, &charShapeFillVBOs[ 0 ] );
	
	int vboIndex = 0;
	for( int i=0; i<charactersTotal; i++ )
	{
		for( int j=0; j<charShapes[ i ].shapeFillTotal; j++ )
		{
			int shapeFillIndex			= charShapes[ i ].shapeFillIndex[ j ];
			int shapeFillPointsLength	= charShapes[ i ].shapeFillPointsLength[ j ];
			
			glBindBufferARB( GL_ARRAY_BUFFER_ARB, charShapeFillVBOs[ vboIndex ] );
			glBufferDataARB( GL_ARRAY_BUFFER_ARB, shapeFillPointsLength * sizeof( float ), &(charShapes[ i ].shapeFillPoints[ shapeFillIndex ]), GL_STATIC_DRAW_ARB );
			
			charShapes[ i ].shapeFillVBOs[ j ] = vboIndex;
			
			++vboIndex;
		}
	}
}

////////////////////////////////////////////////////////////
//	LOAD / PREP FONT.
////////////////////////////////////////////////////////////

void RibbonType :: loadTrueTypeFont( string fontName, int size )
{
	fontSize = size;
	font.loadFont( fontName, fontSize, true, true, true );
	
	initCharacterContours();
	initCharacterRectangles();
	initCharacterVertices();
	initCharacterOutline();
	initCharacterFill();
}

void RibbonType :: setRibbinColorArray( float *ribbonColorArray )
{
	ribbonColors		= ribbonColorArray;
	useRibbonColorArray	= true;
}

void RibbonType :: drawTypeOnRibbon( string copy, float *ribbonPositionArray, float *ribbonDirectionArray, int ribbonSize )
{
	ribbonPositions		= ribbonPositionArray;
	ribbonDirections	= ribbonDirectionArray;
	ribbonLength		= ribbonSize;
	ribbonLengths		= new float[ ribbonLength ];
	contourStartIndex	= 0;
	
	calcRibbonLengths();
	
	if( !useFill )	// smooth.
	{
		glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
		glEnable( GL_LINE_SMOOTH );
		glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	}
	
	char *cstr;
	cstr = new char[ copy.size() + 1 ];
	strcpy( cstr, copy.c_str() );
	
	float x = 0;
	
	for( int i=0; i<copy.size(); i++ )
	{
		if( wrapSurface )
		{
			drawLetterWrap( cstr[ i ], x, fontSize * 0.5 );
		}
		else
		{
			drawLetterAsPlane( cstr[ i ], x, fontSize * 0.5 );
		}
		
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
	
	useRibbonColorArray = false;
	
	delete[] cstr;
	delete[] ribbonLengths;
}

////////////////////////////////////////////////////////////
//	LIFE HACKER METHODS - MAKING LIFE EASIER.
////////////////////////////////////////////////////////////

void RibbonType :: calcRibbonLengths()
{
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

////////////////////////////////////////////////////////////
//	DRAWING ROUTINE.
////////////////////////////////////////////////////////////

void RibbonType :: drawLetterWrap( int letter, float xOffset, float yOffset )
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
	
	ofBeginShape();
	
	for( int k=0; k<ttfChar.contours.size(); k++ )
	{
		if( k != 0 )
		{
			ofNextContour(true);
		}
		
		for( int i=0; i<ttfChar.contours[ k ].pts.size(); i++ )
		{
			float cx = ttfChar.contours[ k ].pts[ i ].x + xOffset;
			float cy = ttfChar.contours[ k ].pts[ i ].y + yOffset;
			
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
				
				if( cx >= lx && cx < ux )	// found! contour lies between p1 and p2.
				{
					if( j > contourMaxIndex )
					{
						contourMaxIndex = j;
					}
					
					float p = ( cx - lx ) / ( ux - lx );
					
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
			cd *= cy;
			cp += cd;
			
			ofVertex( cp.x, cp.y, cp.z );
		}
		
		if( outsideOfRibbon )
		{
			break;
		}
	}
	
	contourStartIndex = contourMaxIndex;
	
	if( !outsideOfRibbon )
	{
		ofEndShape( true );
	}
}


void RibbonType :: drawLetterAsPlane( int letter, float xOffset, float yOffset )
{
	int characterIndex = getCharacterIndex( letter );
	if( characterIndex == -1 )
	{
		return;
	}
	
	CharacterRect charRect;
	charRect = characterRectangles.at( characterIndex );
	
	bool outsideOfRibbon = false;
	
	ofxVec3f cp;	// contour point.
	ofxVec3f cd;	// contour direction.
	ofxVec3f p1;	// current ribbon point position.
	ofxVec3f p2;	// next ribbon point position.
	ofxVec3f p21;	// direction from p1 to p2;

	float px = xOffset + charRect.width * 0.5;
	
	int j;
	for( j=contourStartIndex; j<( ribbonLength - 1 ) * 3; j+=3 )
	{
		int ribbonLengthIndex = (int)( j / 3 );
		float lx = ribbonLengths[ ribbonLengthIndex ];		// lower x bounds.
		float ux = ribbonLengths[ ribbonLengthIndex + 1 ];	// upper x bounds.
		
		if( px >= lx && px < ux )	// found! contour lies between p1 and p2.
		{
			contourStartIndex = j;	// TODO :: this can be optimised further so that it searches for the index at the end of the letter.

			p1	= ofxVec3f( ribbonPositions[ j + 0 ], ribbonPositions[ j + 1 ], ribbonPositions[ j + 2 ] );
			p2	= ofxVec3f( ribbonPositions[ j + 3 ], ribbonPositions[ j + 4 ], ribbonPositions[ j + 5 ] );
			p21	= p2 - p1;
			
			float p = ( px - lx ) / ( ux - lx );
			
			cp = p1 + p21 * p;
			
			cd = ofxVec3f( ribbonDirections[ j + 0 ], ribbonDirections[ j + 1 ], ribbonDirections[ j + 2 ] );
			
			break;
		}
		
		if( j == ( ribbonLength - 2 ) * 3 )	// last point.
		{
			outsideOfRibbon = true;
		}
	}
	
	if( outsideOfRibbon )
	{
		return;
	}
	
	glPushMatrix();
	
	ofxVec3f upVec		= cd;
	ofxVec3f rightVec	= p21.getNormalized();
	ofxVec3f outVec		= upVec.getCrossed( rightVec );
	
	float *mat = new float[ 16 ];
	mat[0]	= rightVec.x;
	mat[1]	= rightVec.y;
	mat[2]	= rightVec.z;
	mat[3]	= 0;
	mat[4]	= upVec.x;
	mat[5]	= upVec.y;
	mat[6]	= upVec.z;
	mat[7]	= 0;
	mat[8]	= outVec.x;
	mat[9]	= outVec.y;
	mat[10]	= outVec.z;
	mat[11]	= 0;
	mat[12]	= cp.x;
	mat[13]	= cp.y;
	mat[14]	= cp.z;
	mat[15]	= 1;
	
	glMultMatrixf( mat );
	
	delete[] mat;
	
	if( useRibbonColorArray )
	{
		int k = ( j / 3 ) * 4;
		glColor4f( ribbonColors[ k + 0 ], ribbonColors[ k + 1 ], ribbonColors[ k + 2 ], ribbonColors[ k + 3 ] );
	}

	if( useFill )
	{
		drawLetterFill( characterIndex );
	}
	else
	{
		drawLetterOutline( characterIndex );
	}
	
	
	glPopMatrix();
}

void RibbonType :: drawLetterOutline ( int characterIndex )
{
	for( int i=0; i<charShapes[ characterIndex ].shapesTotal; i++ )
	{
		int vboIndex	= charShapes[ characterIndex ].shapeVBOs[ i ];
		int vboLendth	= charShapes[ characterIndex ].shapePointsLength[ i ] / 3;
		
		glEnableClientState( GL_VERTEX_ARRAY );
		
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, charShapeVBOs[ vboIndex ] );
		glVertexPointer( 3, GL_FLOAT, 0, 0 );
		
		glDrawArrays( GL_LINE_STRIP, 0, vboLendth );
		
		glDisableClientState( GL_VERTEX_ARRAY );
		
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
	}
}

void RibbonType :: drawLetterFill ( int characterIndex )
{
	for( int i=0; i<charShapes[ characterIndex ].shapeFillTotal; i++ )
	{
		int vboIndex	= charShapes[ characterIndex ].shapeFillVBOs[ i ];
		int vboType		= charShapes[ characterIndex ].shapeFillTypes[ i ];
		int vboLendth	= charShapes[ characterIndex ].shapeFillPointsLength[ i ] / 3;
		
		glEnableClientState( GL_VERTEX_ARRAY );
		
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, charShapeFillVBOs[ vboIndex ] );
		glVertexPointer( 3, GL_FLOAT, 0, 0 );
		
		glDrawArrays( vboType, 0, vboLendth );
		
		glDisableClientState( GL_VERTEX_ARRAY );
		
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
	}
}

////////////////////////////////////////////////////////////
//	SETTERS.
////////////////////////////////////////////////////////////

void RibbonType :: setKerning( float value )
{
	kerning = value;
}

void RibbonType :: wrapRibbonSurface( bool value )
{
	wrapSurface = value;
}

void RibbonType :: fill()
{
	useFill = true;
}

void RibbonType :: noFill()
{
	useFill = false;
}
