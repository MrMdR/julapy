/*
 *  Background.cpp
 *  iPhoneEmptyExample
 *
 *  Created by lukasz karluk on 10/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Background.h"

Background :: Background( ofxFlashMovieClip* asset )
{
	this->asset	= asset;
	ix			= asset->x();
	iy			= asset->y();
	
	for( int x=0; x<4; x++ )
	{
		for( int y=0; y<4; y++ )
		{
			string tileID = "";
			char tileIDChar[ 255 ];
			sprintf( tileIDChar, "tile_%01d%01d", x, y );
			tileID.insert( 0, tileIDChar );
			
			BackgroundPanel* tile;
			tile = new BackgroundPanel( (ofxFlashMovieClip*)asset->getChildByName( tileID ) );
			tile->rect.x		= (int)tile->asset->x();
			tile->rect.y		= (int)tile->asset->y();
			tile->rect.width	= BG_TILE_WIDTH;
			tile->rect.height	= BG_TILE_HEIGHT;
			
			tiles.push_back( tile );
		}
	}
	
	for( int x=0; x<2; x++ )
	{
		for( int y=0; y<2; y++ )
		{
			string btnID = "";
			char btnIDChar[ 255 ];
			sprintf( btnIDChar, "btn_%01d%01d", x, y );
			btnID.insert( 0, btnIDChar );
			
			BackgroundBtn* btn;
			btn = new BackgroundBtn( (ofxFlashMovieClip*)asset->getChildByName( btnID ) );
			
			btns.push_back( btn );
		}
	}
	
	btn_home = new BackgroundBtn( (ofxFlashMovieClip*)asset->getChildByName( "btn_home" ) );
	btn_home->asset->visible( false );
	btns.push_back( btn_home );
	
	screenIndex = 0;
	
	screenPoints.push_back( ofPoint( 0,    0   ) );
	screenPoints.push_back( ofPoint( 0,    768 ) );	
	screenPoints.push_back( ofPoint( 1024, 0   ) );
	screenPoints.push_back( ofPoint( 1024, 768 ) );
	screenPoints.push_back( ofPoint( 512,  384 ) );
	
	screenCorners.push_back( ScreenCorner() );
	screenCorners.back().angle[ 0 ]		= 90;
	screenCorners.back().angle[ 1 ]		= 135;
	screenCorners.back().angle[ 2 ]		= 180;
	screenCorners.back().screen[ 0 ]	= 2;
	screenCorners.back().screen[ 1 ]	= 4;
	screenCorners.back().screen[ 2 ]	= 1;
	
	screenCorners.push_back( ScreenCorner() );
	screenCorners.back().angle[ 0 ]		= 0;
	screenCorners.back().angle[ 1 ]		= 45;
	screenCorners.back().angle[ 2 ]		= 90;
	screenCorners.back().screen[ 0 ]	= 0;
	screenCorners.back().screen[ 1 ]	= 4;
	screenCorners.back().screen[ 2 ]	= 3;
	
	screenCorners.push_back( ScreenCorner() );
	screenCorners.back().angle[ 0 ]		= 270;
	screenCorners.back().angle[ 1 ]		= 225;
	screenCorners.back().angle[ 2 ]		= 180;
	screenCorners.back().screen[ 0 ]	= 0;
	screenCorners.back().screen[ 1 ]	= 4;
	screenCorners.back().screen[ 2 ]	= 3;

	screenCorners.push_back( ScreenCorner() );
	screenCorners.back().angle[ 0 ]		= 0;
	screenCorners.back().angle[ 1 ]		= 315;
	screenCorners.back().angle[ 2 ]		= 270;
	screenCorners.back().screen[ 0 ]	= 2;
	screenCorners.back().screen[ 1 ]	= 4;
	screenCorners.back().screen[ 2 ]	= 1;
	
	asset->x( -BG_CENTER_X );
	asset->y( -BG_CENTER_Y );
	
	assetX = tweenX = -BG_CENTER_X;
	assetY = tweenY = -BG_CENTER_Y;
	
	screenRect.x		= BG_CENTER_X;
	screenRect.y		= BG_CENTER_Y;
	screenRect.width	= BG_TILE_WIDTH  * 2;
	screenRect.height	= BG_TILE_HEIGHT * 2;
	
	dragX = 0;
	dragY = 0;
	
	bTouchDown		= false;
	bCenterScreen	= true;
	bDragging		= false;
	
	gridScaleDivider = 8;
}

Background :: ~Background ()
{
	//
}

void Background :: update ()
{
	if( bDragging )
	{
		dragVec.set( dragX, dragY );
		dragVecNorm = dragVec.getNormalized();
	}
	else
	{
		float dragEase = 0.2;
		dragX += -dragX * dragEase;
		dragY += -dragY * dragEase;
		
		if( abs( dragX ) < 1.0 )
			dragX = 0;
		
		if( abs( dragY ) < 1.0 )
			dragY = 0;
	}
	
	float tweenEase	= 0.2;
	assetX += ( tweenX - assetX ) * tweenEase;
	assetY += ( tweenY - assetY ) * tweenEase;
	
	bool b1, b2;
	
	if( b1 = abs( tweenX - assetX ) < 1.0 )
		assetX = tweenX;
	
	if( b2 = abs( tweenY - assetY ) < 1.0 )
		assetY = tweenY;
	
	asset->x( (int)( assetX - dragX ) );
	asset->y( (int)( assetY - dragY ) );
	
	screenRect.x = -asset->x();
	screenRect.y = -asset->y();
	
	for( int i=0; i<tiles.size(); i++ )
	{
		BackgroundPanel* tile;
		tile = tiles[ i ];
		tile->checkTileIsOnScreen( screenRect );
	}
	
	for( int i=0; i<btns.size(); i++ )
	{
		BackgroundBtn* btn;
		btn = btns[ i ];
		btn->update();
		
		if( btn->isPressed() )
			btnPressedHandler( btn );
	}
}

void Background :: draw ()
{
	int s = gridScaleDivider;
	int w = BG_TILE_WIDTH  / s;
	int h = BG_TILE_HEIGHT / s;
	
	glPushMatrix();
	glTranslatef
	( 
		(int)( ( ofGetWidth()  - ( w * 4 ) ) * 0.5 ),
		(int)( ( ofGetHeight() - ( h * 4 ) ) * 0.5 ),
		0
	);
	
	drawGrid();
	drawGridVectors();
	
	glPopMatrix();
}

void Background :: drawGrid ()
{
	int s = gridScaleDivider;
	
	ofFill();
	ofSetColor( 255, 0, 0, 30 );
	ofEnableAlphaBlending();
	
	for( int i=0; i<tiles.size(); i++ )
	{
		BackgroundPanel* tile;
		tile = tiles[ i ];
		
		if( !tile->bOnScreen )
			continue;
		
		ofRect
		(
			(int)( tile->rect.x / s ),
			(int)( tile->rect.y / s ),
			(int)( tile->rect.width  / s ),
			(int)( tile->rect.height / s )
		);
	}
	
	ofNoFill();
	ofSetColor( 0x000000 );
	ofSetLineWidth( 1 );
	ofDisableAlphaBlending();
	
	for( int i=0; i<tiles.size(); i++ )
	{
		BackgroundPanel* tile;
		tile = tiles[ i ];
		
		ofRect
		(
			(int)( tile->rect.x / s ),
			(int)( tile->rect.y / s ),
			(int)( tile->rect.width  / s ),
			(int)( tile->rect.height / s )
		);
	}
	
	ofNoFill();
	ofSetColor( 0xFF0000 );
	ofSetLineWidth( 2 );
	ofDisableAlphaBlending();
	
	ofRect
	(
		(int)( screenRect.x / s ),
		(int)( screenRect.y / s ),
		(int)( screenRect.width  / s ),
		(int)( screenRect.height / s )
	);
}

void Background :: drawGridVectors ()
{
	int s  = gridScaleDivider;
	int cx = (int)( ( screenRect.x / s ) + ( screenRect.width  / s * 0.5 ) );
	int cy = (int)( ( screenRect.y / s ) + ( screenRect.height / s * 0.5 ) );
	int len = 30;
	
	if( !bCenterScreen )
	{
		ofNoFill();
		ofSetColor( 0x0000FF );
		ofSetLineWidth( 2 );
		
		const ScreenCorner& corner = screenCorners[ screenIndex ];
		
		for( int i=0; i<3; i++ )
		{
			float rad;
			rad = ( corner.angle[ i ] / 360.0 ) * TWO_PI - PI * 0.5;		// circle starts at 12 oclock and moves clock wise.
			
			ofLine
			(
				cx,
				cy,
				cx + len * cos( rad ),
				cy + len * sin( rad )
			);
		}
	}
	
	if( bDragging )
	{
		const ScreenCorner& corner = screenCorners[ screenIndex ];
		
		checkForClosestScreen( corner );
		
		if( closestCornerIndex > -1 )
		{
			float rad0;
			rad0 = ( corner.angle[ closestCornerIndex ] / 360.0 ) * TWO_PI - PI * 0.5;		// circle starts at 12 oclock and moves clock wise.
			
			ofNoFill();
			ofSetColor( 0x00FF00 );
			ofSetLineWidth( 2 );
			
			ofLine
			(
				cx,
				cy,
				cx + len * cos( rad0 ),
				cy + len * sin( rad0 )
			);
		}
		
		//--
		
		float ang;
		ang = dragVecNorm.angle( ofxVec2f( 0, -1 ) );					// return an angle between -180 and 180.
		
		float rad;
		rad = ( ( ang + 180 ) / 360.0 ) * TWO_PI - PI * 0.5;		// circle starts at 12 oclock and moves clock wise.
		rad *= -1;
		
		ofFill();
		ofSetColor( 0xFF0000 );
		ofCircle( cx, cy, 3 );
		
		ofNoFill();
		ofSetColor( 0xFF0000 );
		ofSetLineWidth( 2 );
		ofLine
		(
			cx,
			cy,
			cx + len * cos( rad ),
			cy + len * sin( rad )
		);
	}
}

void Background :: checkForClosestScreen ( const ScreenCorner& corner )
{
	closestScreenIndex = -1;		// reset values.
	closestCornerIndex = -1;
	
	ofxVec2f screenVec;

	float angMin = 9999999;
	
	for( int i=0; i<3; i++ )
	{
		float ang;
		float rad;
		
		ang = corner.angle[ i ];
		rad = ( ang / 360.0 ) * TWO_PI - PI * 0.5;		// circle starts at 12 oclock and moves clock wise.
		
		screenVec.x = cos( rad );
		screenVec.y = sin( rad );
		screenVec.normalize();
		
		float a = abs( screenVec.angle( dragVecNorm ) );
		
		if( a > 50 )	// if over 50 degrees in distance, ignore.
			continue;
		
		if( a < angMin )
		{
			closestCornerIndex	= i;
			closestScreenIndex	= corner.screen[ i ];
			angMin				= a;
		}
	}
}

void Background :: setScreenByID ( int screenID )
{
	screenIndex = screenID;
	
	ofPoint screenPoint;
	screenPoint = screenPoints[ screenIndex ];
	tweenX = -screenPoint.x;
	tweenY = -screenPoint.y;
	
	bCenterScreen = ( screenID == 4 );
	
	btn_home->asset->visible( !bCenterScreen );
}

void Background :: btnPressedHandler ( BackgroundBtn* btn )
{
	int i;
	for( i=0; i<btns.size(); i++ )
	{
		if( btns[ i ] == btn )
			break;
	}
	
	setScreenByID( i );
}

void Background :: touchDown ( int x, int y, int id )
{
	if( bCenterScreen )
		return;
	
//	cout << "touchDown" << endl;
	
	bTouchDown = true;
	
	dragX = 0;
	dragY = 0;
	
	touchPointLast.x = x;
	touchPointLast.y = y;
}

void Background :: touchMoved ( int x, int y, int id )
{
	if( bCenterScreen )
		return;
	
	if( !bTouchDown )
	{
		touchDown( x, y, id );		// touch down skipped because it was made in center screen, so redo.
	}
	
//	cout << "touchMoved" << endl;
	
	bDragging = true;
	
	float dragScale;
	dragScale = 0.5;
	
	dragX = ( touchPointLast.x - x ) * dragScale;
	dragY = ( touchPointLast.y - y ) * dragScale;
}

void Background :: touchUp ( int x, int y, int id )
{
	if( bCenterScreen )
		return;
	
	if( !bTouchDown )
		return;
	
//	cout << "touchUp" << endl;
	
	bTouchDown	= false;
	bDragging	= false;
	
	checkForClosestScreen( screenCorners[ screenIndex ] );
	
	if( closestScreenIndex > -1 )
	{
		if( dragVec.length() > 50 )
		{
			setScreenByID( closestScreenIndex );
		}
	}
}

void Background :: touchDoubleTap ( int x, int y, int id )
{
	//
}
