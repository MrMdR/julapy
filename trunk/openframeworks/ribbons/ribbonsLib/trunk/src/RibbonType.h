/*
 *  RibbonType.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 30/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef RIBBON_TYPE
#define RIBBON_TYPE

#include "ofMain.h"
#include "ofxVec3f.h"

class CharacterRect
{
	public:
		float width;
		float height;
};

class CharShape 
{
public :
	int		shapesTotal;
	int		shapePointsTotal;
	int		*shapeIndex;
	int		*shapePointsLength;
	float	*shapePoints;
	GLuint	*shapeVBOs;
};

class RibbonType
{

public :
	
	RibbonType();
	~RibbonType();
	
	void loadTrueTypeFont( string fontName, int size = 50 );
	void setRibbinColorArray( float *ribbonColorArray );
	void drawTypeOnRibbon( string copy, float *ribbonPositionArray, float *ribbonDirectionArray, int ribbonSize );
	void setKerning( float value );
	void wrapRibbonSurface( bool value );
	
private :
	
	void initCharacters();
	void initCharacterContours();
	void initCharacterVBOs();
	
	void calcRibbonLengths();
	int  getCharacterIndex( int c );
	void drawLetterWrap( int letter, float xOffset = 0, float yOffset = 0 );
	void drawLetterAsPlane( int letter, float xOffset = 0, float yOffset = 0 );
	void drawLetterFromVBO( int characterIndex );
	
	ofTrueTypeFont font;
	int		fontSize;
	float	kerning;
	bool	wrapSurface;
	bool	useRibbonColorArray;
	
	float	*ribbonPositions;
	float	*ribbonDirections;
	float	*ribbonColors;
	float	*ribbonLengths;
	int		ribbonLength;
	
	int			charactersTotal;
	char		*characters;
	CharShape	*charShapes;
	GLuint		*charShapeVBOs;
	vector		<ofTTFCharacter> characterContours;
	vector		<CharacterRect> characterRectangles;
	int			contourStartIndex;

};

#endif
