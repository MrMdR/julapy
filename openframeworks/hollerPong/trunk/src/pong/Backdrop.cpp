/*
 *  Backdrop.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 7/04/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Backdrop.h"

void Backdrop :: init ()
{
	colors.push_back( 0xFFFFFF );
	colors.push_back( 0xFF00FF );
	colors.push_back( 0x00FFFF );
	
	starsTotal = STAR_LAYERS_TOTAL;
	
	starImages[ 0 ].loadImage( "assets/hp_bg_00.png" );
	starImages[ 1 ].loadImage( "assets/hp_bg_01.png" );
	starImages[ 2 ].loadImage( "assets/hp_bg_02.png" );
	
	starEase[ 0 ] = 0.10;
	starEase[ 1 ] = 0.15;
	starEase[ 2 ] = 0.20;
	
	for( int i=0; i<starsTotal; i++ )
	{
		float min, max;
		
		min = -starEase[ i ];
		max = 1 + starEase[ i ];
		
		scatter( stars[ i ], 50, min, max );
	}
}

void Backdrop :: scatter ( vector<Star>& stars, int noStars, float min, float max )
{
	for( int i=0; i<noStars; i++ )
	{
		stars.push_back( Star() );
		
		Star& s = stars.back();
		s.p.x	= ofRandom( min, max );
		s.p.y	= ofRandom( min, max );
		s.c		= colors.at( (int)( ofRandom( 0, colors.size() ) ) );
	}
}

void Backdrop :: update ()
{

}

void Backdrop :: setBallPos	( float x, float y )
{
	float px, py;
	
	px = ( x / (float)ofGetWidth()  ) * 2 - 1;		// between -1 and 1
	py = ( y / (float)ofGetHeight() ) * 2 - 1;
	
	px *= -1;
	py *= -1;
	
	float layerEase;
	layerEase = 0.5;
	
	for( int i=0; i<starsTotal; i++ )
	{
		starOffset[ i ].x += ( starEase[ i ] * px - starOffset[ i ].x ) * layerEase;
		starOffset[ i ].y += ( starEase[ i ] * py - starOffset[ i ].y ) * layerEase;
	}
}

void Backdrop :: draw ()
{
	drawDivider();
	drawStarLayers();
}

void Backdrop :: drawDivider ()
{
	ofEnableAlphaBlending();
	
	ofFill();
	ofSetColor( 255, 255, 255, 60 );
	
	int cx;
	cx = (int)( ofGetWidth() * 0.5 );
	
	int w;		// width bar.
	w = 8;
	
	int h;		// height bar.
	h = 40;
	
	int g;		// gap between bars.
	g = 20;
	
	int y;		// y position.
	y = 0;
	
	while( y < ofGetHeight() )
	{
		ofRect( cx - (int)( w * 0.5 ), y, w, h );
		y += h + g;
	}
	
	ofDisableAlphaBlending();
}

void Backdrop :: drawStarLayers ()
{
	ofEnableAlphaBlending();
	
	int dx, dy;
	
	for( int i=0; i<starsTotal; i++ )
	{
		dx = (int)( starOffset[ i ].x * ofGetWidth()  );
		dy = (int)( starOffset[ i ].y * ofGetHeight() );
		
		glPushMatrix();
		glTranslatef( dx, dy, 0 );
		
		drawStars( stars[ i ], starImages[ i ] );
		
		glPopMatrix();
	}
	
	ofDisableAlphaBlending();
}

void Backdrop :: drawStars ( vector<Star>& stars, ofImage& image )
{
	for( int i=0; i<stars.size(); i++ )
	{
		Star& s = stars.at( i );
		
		ofSetColor( s.c );
		
		int sx, sy;
		sx = (int)( s.p.x * ofGetWidth()  );
		sy = (int)( s.p.y * ofGetHeight() );
		
		image.draw( sx, sy );
	}
}