/*
 *  Background.h
 *  iPhoneEmptyExample
 *
 *  Created by lukasz karluk on 10/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxFlashMovieClip.h"
#include "ofxVec2f.h"
#include "BackgroundPanel.h"
#include "BackgroundBtn.h"

#define		BG_WIDTH		2048
#define		BG_HEIGHT		1536
#define		BG_TILE_WIDTH	512
#define		BG_TILE_HEIGHT	384
#define		BG_CENTER_X		512
#define		BG_CENTER_Y		384

struct ScreenCorner
{
	float	angle[ 3 ];
	int		screen[ 3 ];
};

class Background
{

public:
	
	 Background( ofxFlashMovieClip* asset );
	~Background();
	
	void update	();
	void draw	();
	
	void drawGrid			();
	void drawGridVectors	();
	
	void setScreenByID			( int screenID );
	void checkForClosestScreen	( const ScreenCorner& corner );
	void btnPressedHandler		( BackgroundBtn* btn );
	
	void touchDown		( int x, int y, int id );
	void touchMoved		( int x, int y, int id );
	void touchUp		( int x, int y, int id );
	void touchDoubleTap	( int x, int y, int id );

	ofxFlashMovieClip*			asset;
	vector<BackgroundPanel*>	tiles;
	vector<BackgroundBtn*>		btns;
	BackgroundBtn*				btn_home;
	
	ofRectangle				screenRect;
	vector<ofPoint>			screenPoints;
	vector<ScreenCorner>	screenCorners;
	int						screenIndex;
	int						closestScreenIndex;
	int						closestCornerIndex;
	
	int			gridScaleDivider;
	
	bool		bTouchDown;
	bool		bCenterScreen;
	bool		bDragging;
	
	float		assetX;
	float		assetY;
	float		tweenX;
	float		tweenY;
	float		dragX;
	float		dragY;
	ofxVec2f	dragVec;
	ofxVec2f	dragVecNorm;

	ofPoint		touchPointLast;
	int			ix;
	int			iy;
};