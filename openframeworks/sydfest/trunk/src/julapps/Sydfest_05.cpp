/*
 *  Sydfest_05.cpp
 *  julapy_sydfest
 *
 *  Created by lukasz karluk on 29/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Sydfest_05.h"

Sydfest_05 :: Sydfest_05 ()
{
	ofEnableSmoothing();
	ofSetCircleResolution( 100 );
	ofBackground( 60, 60, 60 );
	
	frameStart = ofGetFrameNum();
}

Sydfest_05 :: ~Sydfest_05()
{
	//
}

void Sydfest_05 :: setup ()
{
	circlePacker01.loadFromFile( "circle_data_03a" );
	circlePacker02.loadFromFile( "circle_data_03b" );
	circlePacker03.loadFromFile( "circle_data_03c" );
	circlePacker04.loadFromFile( "circle_data_03d" );
	
	
	circles01 = circlePacker01.getCircles();
	circles02 = circlePacker02.getCircles();
	circles03 = circlePacker03.getCircles();
	circles04 = circlePacker04.getCircles();
	
	circlesAlpha01 = 255;
	circlesAlpha02 = 255;
	circlesAlpha03 = 255;
	circlesAlpha04 = 255;
	
	gui.addSlider( "circlesAlpha01", circlesAlpha01, 0, 255 );
	gui.addSlider( "circlesAlpha02", circlesAlpha02, 0, 255 );
	gui.addSlider( "circlesAlpha03", circlesAlpha03, 0, 255 );
	gui.addSlider( "circlesAlpha04", circlesAlpha04, 0, 255 );
	
//	for( int i=0; i<circles->size(); i++ )
//	{
//		circleAnims.push_back( CircleAnim() );
//		
//		CircleAnim *circle;
//		circle = &circleAnims.back();
//		
//		circle->color		= circles->at( i ).color;
//		circle->loc.x		= circles->at( i ).loc.x;
//		circle->loc.y		= circles->at( i ).loc.y;
//		circle->radiusMax	= circles->at( i ).radius;
//	}
	
	reset();
}

void Sydfest_05 :: update ()
{
	int frameNum;
	frameNum = ofGetFrameNum() - frameStart;
	
	for( int i=0; i<circleAnims.size(); i++ )
	{
		if( circleAnims[ i ].frameDelay == frameNum )
		{
			circleAnims[ i ].playing = true;
		}
		
		if( circleAnims[ i ].playing  )
		{
			circleAnims[ i ].radius += ( circleAnims[ i ].radiusMax - circleAnims[ i ].radius ) * circleAnims[ i ].radiusEase;
		}
	}
}

void Sydfest_05 :: draw ()
{
	drawCirclesLayer( circles01, circlesAlpha01 );
	drawCirclesLayer( circles02, circlesAlpha02 );
	drawCirclesLayer( circles03, circlesAlpha03 );
	drawCirclesLayer( circles04, circlesAlpha04 );
}

void Sydfest_05 :: drawCirclesLayer ( vector<Circle> *circles, int circlesAlpha )
{
	ofEnableAlphaBlending();
	
	for( int i=0; i<circles->size(); i++ )
	{
		RgbColor c = ofHexToRgb( circles->at( i ).color );
		
		float s;
		s = circles->at( i ).radius / 100.0;
		
		float si;
		si = 1 - s;
		
		ofSetColor( c.r, c.g, c.b, (int)( si * 255 ) );
		
		ofFill();
		ofCircle( circles->at( i ).loc.x, circles->at( i ).loc.y, circles->at( i ).radius );
		ofNoFill();
		ofCircle( circles->at( i ).loc.x, circles->at( i ).loc.y, circles->at( i ).radius );
	}
	
	ofDisableAlphaBlending();
}

void Sydfest_05 :: drawDebug ()
{
	gui.draw();
}

void Sydfest_05 :: reset ()
{
	frameStart = ofGetFrameNum();
	
	for( int i=0; i<circleAnims.size(); i++ )
	{
		float p;
		p = circles01->at( i ).radius / 35.0;
		
		circleAnims[ i ].radius		= 0;
		circleAnims[ i ].radiusEase	= 0.05;
		circleAnims[ i ].frameDelay	= (int)( ( 1 - p ) * ( 100 + ofRandom( 0, 300 ) ) );
		circleAnims[ i ].playing	= false;
	}
}

void Sydfest_05 :: keyReleased ( int key )
{
	if( key == 'r' )
	{
		reset();
	}
}