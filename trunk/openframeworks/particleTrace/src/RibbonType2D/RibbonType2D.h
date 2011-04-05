/*
 *  RibbonType2D.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 03/04/11.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "Letter.h"
#include "ofxVec2f.h"

////////////////////////////////////////////////////////////
//	RIBBON TYPE 2D.
////////////////////////////////////////////////////////////

class RibbonType2D
{

public :
	
	 RibbonType2D();
	~RibbonType2D();
    
    void setFont                ( ofTrueTypeFont* font, float fontSize, float fontScale = 1.0, float fontOffsetY = 0 );
    void setKerning             ( float value );
    void setCopy                ( const string& copy );
    
    void setup                  ();
	void initCharacterVertices  ();
	void initCharacterOutline   ();
	void initCharacterFill      ();
    void initCharacterScale     ();
    void initCharacterVBOs      ();
    int  getCharacterIndex      ( int c );
	
    void setRibbon              ( float* ribbonPoints, int ribbonLength, float* ribbonNormals = NULL, float* ribbonColors = NULL );

    vector<Letter*> generateTypeOnRibbon    ();
    vector<Letter*> generateTypeOnRibbon    ( float& ribbonPositionX );
    void generateRibbonNormals  ();
    void generateRibbonColors   ();
    void generateRibbonLengths  ();
    bool generateLetterAsPlane  ( int letter, float xOffset = 0, float yOffset = 0 );
    void reset                  ();
    
    void cloneLetter            ( Letter& letterOrig, Letter& letterCopy );
    void translateLetter        ( Letter& letter, float x, float y );
    void rotateLetter           ( Letter& letter, float centerX, float centerY, float degrees );
    void scaleLetter            ( Letter& letter, float centerX, float centerY, float scale );
    
    void drawLine               ();
    void drawNormals            ();
    void drawBounds             ();
    void drawTestChar           ();
    
	ofTrueTypeFont* font;
	float           fontSize;
    float           fontScale;
    float           fontOffsetY;
	float           fontKerning;
    
    string          ribbonCopy;
    char*           ribbonCopyChars;
	
	float*          ribbonPoints;
	float*          ribbonNormals;
	float*          ribbonColors;
	float*          ribbonLengths;
	int             ribbonLength;
    
    bool            bRibbonNormalsGenerated;
    bool            bRibbonColorsGenerated;
	
	char*           characters;
	Letter*         letters;
    int             lettersTotal;
    vector<Letter*> lettersOnRibbon;
    
    int             ribbonPointOffset;
    float           ribbonPositionX;
};
