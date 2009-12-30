/*
 *  Sydfest_06.cpp
 *  julapy_sydfest
 *
 *  Created by lukasz karluk on 30/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Sydfest_06.h"

Sydfest_06 :: Sydfest_06()
{
	
}

Sydfest_06 :: ~Sydfest_06()
{
	
}

void Sydfest_06 :: setup ()
{
	ofBackground( 30, 30, 30 );
	ofEnableSmoothing();
	
	bDrawDebug		= true;
	bExternalData	= false;
	startFrame		= 0;
	circleAddRate	= 1;
	
	shapes.loadFromFile();
	
//	image.loadImage( "barracks1.jpg" );
//	image.loadImage( "barracks2.jpg" );
	image.loadImage( "barracks3.jpg" );
	
	imageRect.width		= image.width;
	imageRect.height	= image.height;
	
	imageColor.loadImage( "balloon.jpg" );
	
	imageAlpha			= 255;
	imageColorAlpha		= 255;
	
	box2d.init();
	box2d.setGravity( 0, 10 );
	box2d.createBounds( renderArea.x, renderArea.y, renderArea.width, renderArea.height );
	box2d.setFPS( 30 );
	
	for( int i=0; i<shapes.getShapesNum(); i++ )
	{
		box2dLineStrips.push_back( ofxBox2dLine() );
		
		ofxBox2dLine *lineStrip;
		lineStrip = &box2dLineStrips.back();
		
		ofPoint p;
		lineStrip->setWorld( box2d.getWorld() );
		
		vector<double*> poly = shapes.getShapeAt( i );
		
		bool bReversePoints;
		bReversePoints = false;
		
		if( i == 0 )
			bReversePoints = true;
		
		if( bReversePoints )
		{
			for( int j=poly.size()-1; j>0; j-- )
			{
				p.x = poly[ j ][ 0 ];
				p.y = poly[ j ][ 1 ];
				lineStrip->addPoint( p.x, p.y );
			}
		}
		else
		{
			for( int j=0; j<poly.size(); j++ )
			{
				p.x = poly[ j ][ 0 ];
				p.y = poly[ j ][ 1 ];
				lineStrip->addPoint( p.x, p.y );
			}
		}
		
		lineStrip->createShape();
	}
	
	gui.addSlider( "circleAddRate",		circleAddRate,		0, 10  );
	gui.addSlider( "imageAlpha",		imageAlpha,			0, 255 );
	gui.addSlider( "imageColorAlpha",	imageColorAlpha,	0, 255 );

//	gui.loadFromXML( "ofxSimpleGuiToo.xml" );
	
	loadFromFile();
	runFromExternalData();
}

void Sydfest_06 :: update ()
{
	shapes.update();
	
	int frame;
	
	float cMass		= 0.0001;
	float cBounce	= 0.1;
	float cFriction	= 0.01;
	
	if( bExternalData )
	{
		frame = ofGetFrameNum() - startFrame;
		
		for( int i=0; i<circles.size(); i++ )
		{
			Circle_06 *circle;
			circle = &circles[ i ];
			
			if( circle->tick )
				continue;
			
			if( circle->frame == frame )
			{
				box2dCircles.push_back( ofxBox2dCircle() );
				
				ofxBox2dCircle *box2dCircle;
				box2dCircle = &box2dCircles.back();
				box2dCircle->setPhysics( cMass, cBounce, cFriction );
				box2dCircle->setup( box2d.getWorld(), circle->p.x, circle->p.y, circle->radius );
				
				circle->tick = true;
			}
		}
	}
	else
	{
		frame = ofGetFrameNum();
		
		for( int i=0; i<circleAddRate; i++ )
		{
			box2dCircles.push_back( ofxBox2dCircle() );
			
			ofxBox2dCircle *circle;
			circle = &box2dCircles.back();
			
			circle->setPhysics( cMass, cBounce, cFriction );
			
			int x = (int)( renderArea.width * 0.5 );
			int y = (int)( 50 );
			int r = (int)( ofRandom( 5, 10 ) );
			
			x += (int)( ofRandom( -4, 4 ) );
			y += (int)( ofRandom( -4, 4 ) );
			
			circle->setup( box2d.getWorld(), x, y, r );
			
			//--
			
			circles.push_back( Circle_06() );
			
			Circle_06 *circ;
			circ = &circles.back();
			
			circ->id		= box2dCircles.size() - 1;
			circ->p.x		= x;
			circ->p.y		= y;
			circ->radius	= r;
			circ->frame		= frame;
			circ->color		= 0xFF00FF;
		}
	}
	
	box2d.update();
}

void Sydfest_06 :: draw ()
{
//	ofSetColor( 255, 255, 255, imageAlpha );
//	image.draw( 0, 0 );
//
//	ofSetColor( 255, 255, 255, imageColorAlpha );
//	imageColor.draw( 0, 0 );
	
//	box2d.draw();
	
	for( int i=0; i<box2dCircles.size(); i++ )
	{
		ofSetColor( circles[ i ].color );
		
		ofxBox2dCircle *circle;
		circle = &box2dCircles[ i ];
		
		ofPoint p = circle->getPosition();
		float	r = circle->getRadius();
		
		ofFill();
		ofCircle( p.x, p.y, r );
		ofNoFill();
		ofCircle( p.x, p.y, r );
	}
	
	for( int i=0; i<box2dLineStrips.size(); i++ )
	{
		box2dLineStrips[ i ].draw();
	}
	
//	shapes.draw();
}

void Sydfest_06 :: drawDebug ()
{
	if( !bDrawDebug )
		return;
	
	gui.draw();
}

void Sydfest_06 :: clear ()
{
	for( int i=0; i<box2dCircles.size(); i++ )
	{
		ofxBox2dCircle *circle;
		circle = &box2dCircles[ i ];
		
		circle->destroyShape();
	}
		
	box2dCircles.clear();
}

void Sydfest_06 :: runFromExternalData ()
{
	bExternalData	= true;
	startFrame		= ofGetFrameNum();
}

void Sydfest_06 :: saveColors ()
{
	unsigned char* pixels;
	pixels = imageColor.getPixels();
	
	for( int i=0; i<circles.size(); i++ )
	{
		Circle_06 *circle;
		circle = &circles[ i ];
		
		ofxBox2dCircle *box2dCircle;
		box2dCircle = &box2dCircles[ i ];
		
		int x = (int)( box2dCircle->getPosition().x );
		int y = (int)( box2dCircle->getPosition().y );
		
		int p = ( x * 3 ) + ( y * imageColor.width * 3 );
		
		unsigned char r = pixels[ p + 0 ];
		unsigned char g = pixels[ p + 1 ];
		unsigned char b = pixels[ p + 2 ];
		
		circle->color = ofRgbToHex( r, g, b );
	}
}

//////////////////////////////////////////////////
//	WRITE / LOAD.
//////////////////////////////////////////////////

void Sydfest_06 :: writeToFile ( string filename )
{
	ofstream fout;
	fout.open( ofToDataPath( filename ).c_str() );
	
	for( int i=0; i<circles.size(); i++ )
	{
		string circleDataStr;
		
		circleDataStr = 
		ofToString( circles[ i ].id,		0 ) + " " +
		ofToString( circles[ i ].p.x,		0 ) + " " +
		ofToString( circles[ i ].p.y,		0 ) + " " +
		ofToString( circles[ i ].color,		0 ) + " " +
		ofToString( circles[ i ].frame,		0 ) + " " +
		ofToString( circles[ i ].radius,	0 );
		
		fout << circleDataStr << endl;
	}
	
	fout.close();
}

void Sydfest_06 :: loadFromFile ( string filename )
{
	ifstream	circleDataFile;
	string		circleDataStr;
	
	circleDataFile.open( ofToDataPath( filename ).c_str() );
	
	if( circleDataFile.is_open() )
	{
		while( !circleDataFile.eof() )
		{
			getline( circleDataFile, circleDataStr );
			
			if( circleDataStr == "" )
				continue;
			
			vector<string> circleData;
			circleData = ofSplitString( circleDataStr, " " );
			
			circles.push_back( Circle_06() );
			
			Circle_06 *circle;
			circle = &circles.back();
			
			circle->id			= atoi( circleData[ 0 ].c_str() );
			circle->p.x			= atoi( circleData[ 1 ].c_str() );
			circle->p.y			= atoi( circleData[ 2 ].c_str() );
			circle->color		= atoi( circleData[ 3 ].c_str() );
			circle->frame		= atoi( circleData[ 4 ].c_str() );
			circle->radius		= atoi( circleData[ 5 ].c_str() );
			circle->tick		= false;
		}
	}
	
	circleDataFile.close();
}

//////////////////////////////////////////////////
//	HANDLERS.
//////////////////////////////////////////////////

void Sydfest_06 :: keyReleased ( int key )
{
	if( key == 'd' )
		bDrawDebug = !bDrawDebug;
	
//	if( key == 'c' )
//		shapes.clear();
//	
//	if( key == 'w' )
//		shapes.writeToFile();
//	
//	if( key == 'l' )
//		shapes.loadFromFile();

	if( key == 'c' )
		clear();
	
	if( key == 'w' )
	{
		circleAddRate = 0;
		
		writeToFile();
	}
	
	if( key == 'l' )
	{
		clear();
		loadFromFile();
		runFromExternalData();
	}
	
	if( key == ' ' )
		saveColors();
}

void Sydfest_06 :: mousePressed ( int x, int y, int button )
{
//	shapes.addVertex( x, y );
//	return;
}

