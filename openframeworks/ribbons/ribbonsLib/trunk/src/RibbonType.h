/*
 *  RibbonType.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 30/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"
#include "ofxVec3f.h"

#define MAX_CHARACTERS_SUPPORTED	52

class CharacterRect
{
	public:
		float width;
		float height;
};

class RibbonType
{

public :
	
	RibbonType();
	~RibbonType();
	
	void loadTrueTypeFont( string fontName, int size = 50 );
	void drawTypeOnRibbon( string copy, float *ribbonPositionArray, float *ribbonDirectionArray, int ribbonSize );
	void setKerning( float value );
	
private :
	
	void initCharacters();
	int  getCharacterIndex( int c );
	void drawLetter( int letter, float xOffset = 0, float yOffset = 0 );
	void drawLetterVertices();
	void clearLetterVertices();
	
	ofTrueTypeFont font;
	int fontSize;
	float kerning;
	
	float *ribbonPositions;
	float *ribbonDirections;
	float *ribbonLengths;
	int ribbonLength;
	
	vector <float*> polyVertices;
	int currentStartVertex;
	
	int		charactersTotal;
	char	*characters;
	vector <ofTTFCharacter> characterContours;
	vector <CharacterRect> characterRectangles;
	int		contourStartIndex;
};
