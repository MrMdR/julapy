/*
 *  RibbonType.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 30/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxVec3f.h"

class CharShape 
{
public :
	int		shapesTotal;
	int		shapePointsTotal;
	int		*shapeIndex;
	int		*shapePointsLength;
	float	*shapePoints;
	GLuint	*shapeVBOs;

	int		shapeFillTotal;
	int		shapeFillPointsTotal;
	int		*shapeFillIndex;
	int		*shapeFillTypes;
	int		*shapeFillPointsLength;
	float	*shapeFillPoints;
	GLuint	*shapeFillVBOs;
};

class ParticleType
{

public :
	
	 ParticleType();
	~ParticleType();
    
    void setFont                ( ofTrueTypeFont* font );
    void setKerning             ( float value );
    
    void setup                  ();
	void initCharacterVertices  ();
	void initCharacterOutline   ();
	void initCharacterFill      ();
	
	void setRibbinColorArray    ( float *ribbonColorArray );
	void drawTypeOnRibbon       ( string copy, float *ribbonPositionArray, float *ribbonDirectionArray, int ribbonSize );
	
	void fill                   ();
	void noFill                 ();
	
	void calcRibbonLengths      ();
	int  getCharacterIndex      ( int c );
	void drawLetterAsPlane      ( int letter, float xOffset = 0, float yOffset = 0 );
	void drawLetterOutline      ( int characterIndex );
	void drawLetterFill         ( int characterIndex );
    
    bool            bInitialised;
	
	ofTrueTypeFont* font;
	int             fontSize;
	float           kerning;
	bool            useRibbonColorArray;
	bool            useFill;
	
	float	*ribbonPositions;
	float	*ribbonDirections;
	float	*ribbonColors;
	float	*ribbonLengths;
	int		ribbonLength;
	
	int			charactersTotal;
	char		*characters;
	CharShape	*charShapes;
	GLuint		*charShapeVBOs;
	GLuint		*charShapeFillVBOs;
	vector		<ofTTFCharacter> characterContours;
	vector		<ofRectangle> characterRectangles;
	int			contourStartIndex;
};
