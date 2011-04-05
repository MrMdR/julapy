/*
 *  RibbonType2D.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 03/04/11.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "RibbonType2D.h"

RibbonType2D :: RibbonType2D()
{
	ribbonPoints    = NULL;
	ribbonNormals   = NULL;
	ribbonColors    = NULL;
	ribbonLengths   = NULL;
    ribbonCopyChars = NULL;
    
    font        = NULL;
    fontSize    = 0;
    fontScale   = 1.0;
    fontOffsetY = 0;
    fontKerning = 0;
    
    setCopy( "" );
    
    bRibbonNormalsGenerated = false;
    bRibbonColorsGenerated  = false;
}

RibbonType2D :: ~RibbonType2D()
{
    delete[] characters;
	delete[] letters;
    
    reset();
}

////////////////////////////////////////////////////////////
//	TESSELATION.
////////////////////////////////////////////////////////////

GLUtesselator *tessObj = NULL;
vector<int>     tessShapeFillIndex;
vector<int>     tessShapeFillTypes;
vector<float>   tessShapeFillPoints;

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

////////////////////////////////////////////////////////////
//	SETTERS.
////////////////////////////////////////////////////////////

void RibbonType2D :: setFont ( ofTrueTypeFont* font, float fontSize, float fontScale, float fontOffsetY )
{
    this->font          = font;
    this->fontSize      = fontSize;
    this->fontScale     = fontScale;
    this->fontOffsetY   = fontOffsetY;
}

void RibbonType2D :: setKerning( float value )
{
	fontKerning = value;
}

void RibbonType2D :: setCopy ( const string& copy )
{
    ribbonCopy = copy;
    
    if( ribbonCopyChars )
    {
        delete[] ribbonCopyChars;
        ribbonCopyChars = NULL;
    }
    
	ribbonCopyChars = new char[ ribbonCopy.size() + 1 ];
	strcpy( ribbonCopyChars, ribbonCopy.c_str() );
}

////////////////////////////////////////////////////////////
//	SETUP.
////////////////////////////////////////////////////////////

void RibbonType2D :: setup ()
{
    if( !font )
    {
        cout << "ParticleType :: setup() - font not loaded.";
        return;
    }
    
    //----
    
	string supportedCharacter;
    supportedCharacter = "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz";
//    supportedCharacter = "AabCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz";     // capital B removed as its causing issues.
	
	lettersTotal	= supportedCharacter.size();
	characters		= new char[ lettersTotal + 1 ];
	strcpy( characters, supportedCharacter.c_str() );
    
    //----
    
    initCharacterVertices();
    initCharacterFill();
    initCharacterScale();
    initCharacterVBOs();
}

void RibbonType2D :: initCharacterVertices()
{
	letters = new Letter[ lettersTotal ];
	
	for( int i=0; i<lettersTotal; i++ )
	{
        //--- contours.
        
		ofTTFCharacter charOutline;
		charOutline = font->getCharacterAsPoints( characters[ i ] );
		
        //--- letter rectangle.
        
		string str = "";
		str += characters[ i ];
        
        ofRectangle charRect;
		charRect = font->getStringBoundingBox( str, 0, 0 );
        
        //--- creating letter.
		
		int shapesTotal			= 0;
		int shapeIndex			= 0;
		int shapePointsTotal	= 0;
		
		shapesTotal = charOutline.contours.size();
		for( int j=0; j<shapesTotal; j++ )
		{
			shapePointsTotal += ( charOutline.contours[ j ].pts.size() + 1 ) * 3;
		}

		letters[ i ].rect               = charRect;
		letters[ i ].shapesTotal		= shapesTotal;
		letters[ i ].shapePointsTotal	= shapePointsTotal;
		letters[ i ].shapeIndex			= new int[ shapesTotal ];
		letters[ i ].shapePointsLength	= new int[ shapesTotal ];
		letters[ i ].shapePoints		= new float[ shapePointsTotal ];
		letters[ i ].shapeVBOs			= new GLuint[ shapesTotal ];
		
		for( int j=0; j<shapesTotal; j++ )
		{
			letters[ i ].shapeIndex[ j ] = shapeIndex;
			
			int n = 0;
			for( int k=0; k<charOutline.contours[ j ].pts.size(); k++ )
			{
				n = shapeIndex + ( k * 3 );
				letters[ i ].shapePoints[ n + 0 ] = charOutline.contours[ j ].pts[ k ].x - charRect.width * 0.5;
				letters[ i ].shapePoints[ n + 1 ] = charOutline.contours[ j ].pts[ k ].y + fontSize * 0.5 + fontOffsetY;
				letters[ i ].shapePoints[ n + 2 ] = 0;
			}
			n += 3;
			
			letters[ i ].shapePoints[ n + 0 ] = letters[ i ].shapePoints[ shapeIndex + 0 ];	// close loop by adding first value of shape 
			letters[ i ].shapePoints[ n + 1 ] = letters[ i ].shapePoints[ shapeIndex + 1 ];	// as last value.
			letters[ i ].shapePoints[ n + 2 ] = letters[ i ].shapePoints[ shapeIndex + 2 ];
			
			n += 3;
			
			letters[ i ].shapePointsLength[ j ] = ( n - shapeIndex );
			
			shapeIndex += n;
		}
	}
}

void RibbonType2D :: initCharacterFill()
{
	for( int i=0; i<lettersTotal; i++ )
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
		
		for( int j=0; j<letters[ i ].shapesTotal; j++ )
		{
			int shapeIndex			= letters[ i ].shapeIndex[ j ];
			int shapePointsLength	= letters[ i ].shapePointsLength[ j ];
			
			gluTessBeginContour( tessObj );
			for( int k=shapeIndex; k<shapeIndex+shapePointsLength; k+=3 )
			{
				double *point = new double[ 3 ];
				point[ 0 ] = letters[ i ].shapePoints[ k + 0 ];
				point[ 1 ] = letters[ i ].shapePoints[ k + 1 ];
				point[ 2 ] = letters[ i ].shapePoints[ k + 2 ];
				
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
		
		int tessShapesTotal			= tessShapeFillIndex.size();
		int tessShapePointsTotal	= tessShapeFillPoints.size();
		
		letters[ i ].shapeFillTotal			= tessShapesTotal;
		letters[ i ].shapeFillPointsTotal	= tessShapePointsTotal;
		letters[ i ].shapeFillIndex			= new int[ tessShapesTotal ];
		letters[ i ].shapeFillTypes			= new int[ tessShapesTotal ];
		letters[ i ].shapeFillPointsLength	= new int[ tessShapesTotal ];
		letters[ i ].shapeFillPoints		= new float[ tessShapePointsTotal ];
		letters[ i ].shapeFillVBOs			= new GLuint[ tessShapesTotal ];
        
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
			
			letters[ i ].shapeFillIndex[ j ]		= shapeFillIndex;
			letters[ i ].shapeFillTypes[ j ]		= shapeFillTypes;
			letters[ i ].shapeFillPointsLength[ j ]	= shapeFillPointsLength;
		}
		
		for( int j=0; j<tessShapePointsTotal; j++ )
		{
			letters[ i ].shapeFillPoints[ j ] = tessShapeFillPoints.at( j );
		}
		
		tessShapeFillIndex.clear();
		tessShapeFillTypes.clear();
		tessShapeFillPoints.clear();
	}
}

void RibbonType2D :: initCharacterScale ()
{
    if( fontScale == 1.0 )
        return;
    
	for( int i=0; i<lettersTotal; i++ )
	{
        Letter& letter = letters[ i ];
        
        scaleLetter( letter, 0, 0, 0.1 );
	}
}

void RibbonType2D :: initCharacterVBOs ()
{
	for( int i=0; i<lettersTotal; i++ )
	{
        Letter& letter = letters[ i ];
        
        letter.createOutlineVBO();
        letter.createFillVBO();
	}
}

int RibbonType2D :: getCharacterIndex ( int letter )
{
	for( int i=0; i<lettersTotal; i++ )
	{
		if( characters[ i ] == letter )
		{
			return i;
		}
	}
	return -1;
}

////////////////////////////////////////////////////////////
//	GENERATE + DRAW - CUSTOM SHAPE
////////////////////////////////////////////////////////////

void RibbonType2D :: setRibbon ( float* ribbonPoints, int ribbonLength, float* ribbonNormals, float* ribbonColors )
{
    reset();

	this->ribbonPoints  = ribbonPoints;
    this->ribbonLength  = ribbonLength;
    this->ribbonNormals = ribbonNormals;
    this->ribbonColors  = ribbonColors;
    
    generateRibbonNormals();
    generateRibbonColors();
    generateRibbonLengths();
}

vector<Letter*> RibbonType2D :: generateTypeOnRibbon ()
{
    float ribbonPositionZero = 0;
    return  generateTypeOnRibbon( ribbonPositionZero );
}

vector<Letter*> RibbonType2D :: generateTypeOnRibbon ( float& ribbonPosX )
{
    lettersOnRibbon.clear();

	ribbonPositionX     = ribbonPosX;
    ribbonPointOffset   = 0;
    
	for( int i=0; i<ribbonCopy.size(); i++ )
	{
        bool bSuccess;
        bSuccess = generateLetterAsPlane( ribbonCopyChars[ i ], ribbonPositionX, fontSize * fontScale * 0.5 );
        
        if( !bSuccess )
            continue;
		
		if( ribbonCopyChars[ i ] == ' ' )
		{
			ribbonPositionX += fontSize * fontScale;
		}
		else
		{
			int characterIndex = getCharacterIndex( ribbonCopyChars[ i ] );
			if( characterIndex > -1 )
			{
				ofRectangle rect;
				rect = letters[ characterIndex ].rect;
				ribbonPositionX += rect.width + fontKerning * fontSize * fontScale;
			}
		}
	}
    
    ribbonPosX = ribbonPositionX;
    
    return lettersOnRibbon;
}

bool RibbonType2D :: generateLetterAsPlane ( int letter, float xOffset, float yOffset )
{
	int characterIndex = getCharacterIndex( letter );
	if( characterIndex == -1 )
		return false;
	
	ofRectangle charRect;
	charRect = letters[ characterIndex ].rect;
	
    ofxVec2f p, q, v;
	ofxVec2f cp;	// contour point.
    ofxVec2f cn;    // contour normal.
    
	float px = xOffset + charRect.width * 0.5;
	
	for( int i=ribbonPointOffset; i<ribbonLength - 1; i++ )
	{
        int j = i + 1;
        
		float lx = ribbonLengths[ i + 0 ];	// lower x bounds.
		float ux = ribbonLengths[ i + 1 ];	// upper x bounds.
		
		if( px >= lx && px < ux )	// found! contour lies between p1 and p2.
		{
			ribbonPointOffset = i;	// TODO :: this can be optimised further so that it searches for the index at the end of the letter.
            
            p.set( ribbonPoints[ i * 3 + 0 ], ribbonPoints[ i * 3 + 1 ] );
            q.set( ribbonPoints[ j * 3 + 0 ], ribbonPoints[ j * 3 + 1 ] );
            v = q - p;
            
			float k = ( px - lx ) / ( ux - lx );
			
			cp = p + v * k;
			
            ofxVec2f cn1( ribbonNormals[ i * 3 + 0 ], ribbonNormals[ i * 3 + 1 ] );
            ofxVec2f cn2( ribbonNormals[ j * 3 + 0 ], ribbonNormals[ j * 3 + 1 ] );
            cn = cn1 * ( 1 - k ) + cn2 * k;
            cn.normalize();
			
			break;
		}
		
        if( i == ribbonLength - 2 )    // last point reached and nothing found.
            return false;
	}

    //----
	
    Letter* ltr;
    ltr = new Letter();
    
    float a;
    a = 180 - cn.angle( ofxVec2f( 0, 1 ) );
    
    cloneLetter( letters[ characterIndex ], *ltr );
    translateLetter( *ltr, cp.x, cp.y );
    rotateLetter( *ltr, cp.x, cp.y, a );
    
    ltr->createFillVBO();
    ltr->createOutlineVBO();
    
    lettersOnRibbon.push_back( ltr );
    
    return true;
}

void RibbonType2D :: generateRibbonNormals ()
{
    if( ribbonNormals )
        return;
    
    int i = 0;
    int t = ribbonLength;
    int k = t * 3;
    
    ribbonNormals = new float[ k ];
    
    float* normalsLeftToRight = new float[ k ];
    float* normalsRightToLeft = new float[ k ];

    ofxVec2f q;
    ofxVec2f p;
    ofxVec2f v;
    
    i = 0;
    normalsLeftToRight[ i * 3 + 0 ] = 0;
    normalsLeftToRight[ i * 3 + 1 ] = 0;
    normalsLeftToRight[ i * 3 + 2 ] = 0;

    for( i=1; i<t; i++ )
    {
        int j = i - 1;

        q.set( ribbonPoints[ i * 3 + 0 ], ribbonPoints[ i * 3 + 1 ] );
        p.set( ribbonPoints[ j * 3 + 0 ], ribbonPoints[ j * 3 + 1 ] );
        v = q - p;
        v.perpendicular();
        v.normalize();
        v *= -1;            // reversed so its always pointing up.
        
        normalsLeftToRight[ i * 3 + 0 ] = v.x;
        normalsLeftToRight[ i * 3 + 1 ] = v.y;
        normalsLeftToRight[ i * 3 + 2 ] = 0;
    }
    
    i = t - 1;
    normalsRightToLeft[ i * 3 + 0 ] = 0;
    normalsRightToLeft[ i * 3 + 1 ] = 0;
    normalsRightToLeft[ i * 3 + 2 ] = 0;
    
    for( i=t-2; i>=0; i-- )
    {
        int j = i + 1;

        q.set( ribbonPoints[ i * 3 + 0 ], ribbonPoints[ i * 3 + 1 ] );
        p.set( ribbonPoints[ j * 3 + 0 ], ribbonPoints[ j * 3 + 1 ] );
        v = q - p;
        v.perpendicular();
        v.normalize();
        v *= 1;             // doesn't need to be reversed as its already pointing up.
        
        normalsRightToLeft[ i * 3 + 0 ] = v.x;
        normalsRightToLeft[ i * 3 + 1 ] = v.y;
        normalsRightToLeft[ i * 3 + 2 ] = 0;
    }
    
    for( int i=0; i<t; i++ )
    {
        p.set( normalsLeftToRight[ i * 3 + 0 ], normalsLeftToRight[ i * 3 + 1 ] );
        q.set( normalsRightToLeft[ i * 3 + 0 ], normalsRightToLeft[ i * 3 + 1 ] );
        v = p + q;
        v.normalize();
        
        ribbonNormals[ i * 3 + 0 ] = v.x;
        ribbonNormals[ i * 3 + 1 ] = v.y;
        ribbonNormals[ i * 3 + 2 ] = 0;
    }
    
    delete[] normalsLeftToRight;
    delete[] normalsRightToLeft;
    
    bRibbonNormalsGenerated = true;
}

void RibbonType2D :: generateRibbonColors ()
{
    if( ribbonColors )
        return;
    
    int t = ribbonLength * 4;
    
    ribbonColors = new float[ t ];
    for( int i=0; i<t; i++ )
        ribbonColors[ i ] = 1.0;
    
    bRibbonColorsGenerated = true;
}

void RibbonType2D :: generateRibbonLengths ()
{
    ribbonLengths = new float[ ribbonLength ];
    
    ribbonLengths[ 0 ] = 0;     // at the begining, ribbon length is zero.
    
	for( int i=1; i<ribbonLength; i++ )
	{
		int j = i - 1;

        float d;
        d = ofDist( ribbonPoints[ i * 3 + 0 ], ribbonPoints[ i * 3 + 1 ], ribbonPoints[ j * 3 + 0 ], ribbonPoints[ j * 3 + 1 ] );
        
        ribbonLengths[ i ] = ribbonLengths[ i - 1 ] + d;
	}
}

void RibbonType2D :: reset ()
{
	ribbonPoints    = NULL;
    ribbonLength    = 0;
    
    if( bRibbonNormalsGenerated )
        delete[] ribbonNormals;
    ribbonNormals   = NULL;
    
    if( bRibbonColorsGenerated )
        delete[] ribbonColors;
    ribbonColors    = NULL;
    
    if( ribbonLengths )
    {
        delete[] ribbonLengths;
        ribbonLengths = NULL;
    }
}

////////////////////////////////////////////////////////////
//	UTILS.
////////////////////////////////////////////////////////////

void RibbonType2D :: cloneLetter ( Letter& letterOrig, Letter& letterCopy )
{
    int t, p;
    
    t = letterOrig.shapesTotal;
    p = letterOrig.shapePointsTotal;
    
    letterCopy.shapesTotal          = t;
    letterCopy.shapePointsTotal     = p;
    letterCopy.shapeIndex           = new int[ t ];
    letterCopy.shapePointsLength    = new int[ t ];
    letterCopy.shapePoints          = new float[ p ];
    letterCopy.shapeVBOs            = new GLuint[ t ];
    
    memcpy( letterCopy.shapeIndex,          letterOrig.shapeIndex,          sizeof( int ) * t );
    memcpy( letterCopy.shapePointsLength,   letterOrig.shapePointsLength,   sizeof( int ) * t );
    memcpy( letterCopy.shapePoints,         letterOrig.shapePoints,         sizeof( float ) * p );

    t = letterOrig.shapeFillTotal;
    p = letterOrig.shapeFillPointsTotal;
    
    letterCopy.shapeFillTotal           = t;
    letterCopy.shapeFillPointsTotal     = p;
    letterCopy.shapeFillIndex           = new int[ t ];
    letterCopy.shapeFillTypes           = new int[ t ];
    letterCopy.shapeFillPointsLength    = new int[ t ];
    letterCopy.shapeFillPoints          = new float[ p ];
    letterCopy.shapeFillVBOs            = new GLuint[ t ];
    
    memcpy( letterCopy.shapeFillIndex,          letterOrig.shapeFillIndex,          sizeof( int ) * t );
    memcpy( letterCopy.shapeFillTypes,          letterOrig.shapeFillTypes,          sizeof( int ) * t );
    memcpy( letterCopy.shapeFillPointsLength,   letterOrig.shapeFillPointsLength,   sizeof( int ) * t );
    memcpy( letterCopy.shapeFillPoints,         letterOrig.shapeFillPoints,         sizeof( float ) * p );
}

void RibbonType2D :: translateLetter ( Letter& letter, float x, float y )
{
    int t;
    t = letter.shapePointsTotal / 3;
    
    for( int i=0; i<t; i++ )
    {
        letter.shapePoints[ i * 3 + 0 ] += x;
        letter.shapePoints[ i * 3 + 1 ] += y;
    }
    
    t = letter.shapeFillPointsTotal / 3;
    
    for( int i=0; i<t; i++ )
    {
        letter.shapeFillPoints[ i * 3 + 0 ] += x;
        letter.shapeFillPoints[ i * 3 + 1 ] += y;
    }
}

void RibbonType2D :: rotateLetter ( Letter& letter, float centerX, float centerY, float degrees )
{
    int t;
    t = letter.shapePointsTotal / 3;
    
    ofxVec2f c( centerX, centerY );
    ofxVec2f p;
    float l = 0;
    
    for( int i=0; i<t; i++ )
    {
        p.set( letter.shapePoints[ i * 3 + 0 ], letter.shapePoints[ i * 3 + 1 ] );
        p -= c;
        l = p.length();
        p.normalize();
        p.rotate( degrees );
        p *= l;
        p += c;
        
        letter.shapePoints[ i * 3 + 0 ] = p.x;
        letter.shapePoints[ i * 3 + 1 ] = p.y;
    }
    
    t = letter.shapeFillPointsTotal / 3;
    
    for( int i=0; i<t; i++ )
    {
        p.set( letter.shapeFillPoints[ i * 3 + 0 ], letter.shapeFillPoints[ i * 3 + 1 ] );
        p -= c;
        l = p.length();
        p.normalize();
        p.rotate( degrees );
        p *= l;
        p += c;
        
        letter.shapeFillPoints[ i * 3 + 0 ] = p.x;
        letter.shapeFillPoints[ i * 3 + 1 ] = p.y;
    }
}

void RibbonType2D :: scaleLetter ( Letter& letter, float centerX, float centerY, float scale )
{
    int t;
    t = letter.shapePointsTotal / 3;
    
    ofxVec2f c( centerX, centerY );
    ofxVec2f p;
    float l = 0;
    
    for( int i=0; i<t; i++ )
    {
        p.set( letter.shapePoints[ i * 3 + 0 ], letter.shapePoints[ i * 3 + 1 ] );
        p -= c;
        l = p.length();
        p.normalize();
        p *= l * scale;
        p += c;
        
        letter.shapePoints[ i * 3 + 0 ] = p.x;
        letter.shapePoints[ i * 3 + 1 ] = p.y;
    }
    
    t = letter.shapeFillPointsTotal / 3;
    
    for( int i=0; i<t; i++ )
    {
        p.set( letter.shapeFillPoints[ i * 3 + 0 ], letter.shapeFillPoints[ i * 3 + 1 ] );
        p -= c;
        l = p.length();
        p.normalize();
        p *= l * scale;
        p += c;
        
        letter.shapeFillPoints[ i * 3 + 0 ] = p.x;  
        letter.shapeFillPoints[ i * 3 + 1 ] = p.y;
    }
    
    letter.rect.width  *= scale;
    letter.rect.height *= scale;
}

////////////////////////////////////////////////////////////
//	DRAW.
////////////////////////////////////////////////////////////

void RibbonType2D :: drawLine ()
{
    if( !ribbonPoints || ribbonLength == 0 )
        return;
    
    int t = ribbonLength - 1;
    for( int i=0; i<t; i++ )
    {
        int j = i + 1;
        
        ofLine( ribbonPoints[ i * 3 + 0 ], ribbonPoints[ i * 3 + 1 ], ribbonPoints[ j * 3 + 0 ], ribbonPoints[ j * 3 + 1 ] );
    }
}

void RibbonType2D :: drawNormals ()
{
    if( !ribbonPoints || !ribbonNormals || ribbonLength == 0 )
        return;
    
    float s = fontSize * fontScale * 0.5;
    
    int t = ribbonLength;
    for( int i=0; i<t; i++ )
    {
        float px = ribbonPoints[ i * 3 + 0 ];
        float py = ribbonPoints[ i * 3 + 1 ];
        float nx = px + ribbonNormals[ i * 3 + 0 ] * s;
        float ny = py + ribbonNormals[ i * 3 + 1 ] * s;
        
        ofLine( px, py, nx, ny );
    }
}

void RibbonType2D :: drawBounds ()
{
    float s = fontSize * fontScale * 0.5;
    
    ofPoint p, q;
    
    int t = ribbonLength - 1;
    for( int i=0; i<t; i++ )
    {
        int j = i + 1;
        
        p.x = ribbonPoints[ i * 3 + 0 ] + ribbonNormals[ i * 3 + 0 ] * s;
        p.y = ribbonPoints[ i * 3 + 1 ] + ribbonNormals[ i * 3 + 1 ] * s;

        q.x = ribbonPoints[ j * 3 + 0 ] + ribbonNormals[ j * 3 + 0 ] * s;
        q.y = ribbonPoints[ j * 3 + 1 ] + ribbonNormals[ j * 3 + 1 ] * s;
        
        ofLine( p.x, p.y, q.x, q.y );
        
        p.x = ribbonPoints[ i * 3 + 0 ] - ribbonNormals[ i * 3 + 0 ] * s;
        p.y = ribbonPoints[ i * 3 + 1 ] - ribbonNormals[ i * 3 + 1 ] * s;
        
        q.x = ribbonPoints[ j * 3 + 0 ] - ribbonNormals[ j * 3 + 0 ] * s;
        q.y = ribbonPoints[ j * 3 + 1 ] - ribbonNormals[ j * 3 + 1 ] * s;
        
        ofLine( p.x, p.y, q.x, q.y );
    }
}

void RibbonType2D :: drawTestChar ()
{
    Letter& letter = letters[ 2 ];
    
    letter.drawFill();
    letter.drawOutline();
}
