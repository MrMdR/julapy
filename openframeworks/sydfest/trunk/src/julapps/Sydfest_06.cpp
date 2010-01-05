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
	ofSetCircleResolution( 100 );
	ofEnableSmoothing();
	
	bDrawDebug		= true;
	circleAddRate	= 1;
	
	cMass			= 0.0001;
	cBounce			= 0.1;
	cFriction		= 0.01;
	
#if MODE == 0
	
	shapes.loadFromFile();
	
#endif
	
#if MODE == 1 || MODE == 2 || MODE == 3
	
	shapes.loadFromFile( "shapes_data_01" );
	
#endif
	
//	image.loadImage( "barracks1.jpg" );
//	image.loadImage( "barracks2.jpg" );
	image.loadImage( "barracks3.jpg" );
	
	imageRect.width		= image.width;
	imageRect.height	= image.height;
	
	imageColor.loadImage( "balloon.jpg" );
//	imageColor.loadImage( "grandmasterflash.jpg" );
	
	imageAlpha			= 255;
	imageColorAlpha		= 255;
	
	gravity.x = 0;
	gravity.y = 10;
	
	box2d.init();
	box2d.setGravity( gravity.x, gravity.y );
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
	gui.addSlider( "gravity.x",			gravity.x,		  -10, 10  );
	gui.addSlider( "gravity.y",			gravity.y,		  -10, 10  );

//	gui.loadFromXML( "ofxSimpleGuiToo.xml" );
	
#if MODE == 2
	
	loadFromFile();
	
#endif
	
#if MODE == 3

	loadFromFile();
	createCirclesFromData();
	createDynamicContour();
	updateDynamicContour();
	
#endif
}

void Sydfest_06 :: update ()
{
#if MODE == 0	
	
	shapes.update();
	
#endif

#if MODE == 1
	
	int frame;
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
		circ->p1.x		= x;
		circ->p1.y		= y;
		circ->radius	= r;
		circ->frame		= frame;
		circ->color		= 0xFF00FF;
	}
	
#endif
	
#if MODE == 2

	int frame;
	frame = ofGetFrameNum();
	
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
			box2dCircle->setup( box2d.getWorld(), circle->p1.x, circle->p1.y, circle->radius );
			
			circle->tick = true;
		}
	}

#endif	

#if MODE == 3
	
	updateDynamicContour();
	
#endif
	
	for( int i=0; i<circles.size(); i++ )
	{
		ofxBox2dCircle *box2dCircle;
		box2dCircle = &box2dCircles.at( i );
		
		Circle_06 *circle;
		circle = &circles[ i ];
		
		if( circle->pop == false )
			continue;
		
		circle->bounce.update();
		circle->radius	= MAX( 0.1, circle->bounce.position() );
		
		circle->vel.x	= ofRandom( -0.1, 0.1 );
		circle->vel.y	= MAX( -10, circle->vel.y - 0.1 );
		
		box2dCircle->setRadius( circle->radius );
		box2dCircle->setVelocity( circle->vel.x, circle->vel.y );
		box2dCircle->enableGravity( false );
		box2dCircle->setPhysics( cMass, 0.5, cFriction );
	}
	
	box2d.setGravity( gravity.x, gravity.y );
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
	
#if MODE == 0
	
	ofSetColor( 255, 255, 255 );
	image.draw( 0, 0 );
	
	shapes.draw();
	
#endif
	
#if MODE == 1 || MODE == 2 || MODE == 3
	
	for( int i=0; i<box2dCircles.size(); i++ )
	{
		ofSetColor( circles[ i ].color );
		
		ofxBox2dCircle *circle;
		circle = &box2dCircles[ i ];
		
		ofPoint p = circle->getPosition();
		float	r = circle->getRadius();
		
		float	d = 1;
		
		ofFill();
		ofCircle( p.x, p.y, r - d );
		ofNoFill();
		ofCircle( p.x, p.y, r - d );
	}
	
	for( int i=0; i<box2dLineStrips.size(); i++ )
	{
		box2dLineStrips[ i ].draw();
	}
	
#endif
	
#if MODE == 3
	
//	ofNoFill();
//	
//	ofBeginShape();
//	for( int i=0; i<dynContour.pointsTotal; i++ )
//		ofVertex( dynContour.points[ i ].x, dynContour.points[ i ].y );
//	ofEndShape( true );
	
	dynContourLine->draw();
	
//	for( int i=0; i<dynContour.pointsTotal; i++ )
//	{
//		dynContourCircles[ i ].draw();
//	}
		
#endif
}

void Sydfest_06 :: drawDebug ()
{
	if( !bDrawDebug )
		return;
	
	gui.draw();
}

//////////////////////////////////////////////////
//	CIRCLES.
//////////////////////////////////////////////////

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

void Sydfest_06 :: saveCircleData ()
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
		
		circle->color	= ofRgbToHex( r, g, b );
		
		circle->p2.x	= box2dCircle->getPosition().x;
		circle->p2.y	= box2dCircle->getPosition().y;
	}
}

void Sydfest_06 :: createCirclesFromData ()
{
	for( int i=0; i<circles.size(); i++ )
	{
		Circle_06 *circle;
		circle = &circles[ i ];
		
		box2dCircles.push_back( ofxBox2dCircle() );
			
		ofxBox2dCircle *box2dCircle;
		box2dCircle = &box2dCircles.back();
		box2dCircle->setPhysics( cMass, cBounce, cFriction );
		box2dCircle->setup( box2d.getWorld(), circle->p2.x, circle->p2.y, circle->radius );
	}
}

void Sydfest_06 :: createDynamicContour ()
{
	dynContour.pointsTotal = 50;
	dynContour.points	= new ofPoint[ dynContour.pointsTotal ];
	dynContour.offset	= 0;
	dynContour.rotation	= TWO_PI * 0.01;
	dynContour.radius	= 80;
	
	dynContourLine		= new ofxBox2dLine();
	
	dynContourCircles	= new ofxBox2dCircle[ dynContour.pointsTotal ];
}

void Sydfest_06 :: updateDynamicContour ()
{
	//-- dynamic contour shape.
	
	for( int i=0; i<dynContour.pointsTotal; i++ )
	{
		float p;
		p = i / (float)dynContour.pointsTotal;
		p = p * TWO_PI;
		
		float x;
		float y;
		float r;
		
		r = sin( ( p + dynContour.offset ) * 4 ) + 2;
		r = r * 0.2;
		x = dynContour.center.x + dynContour.radius * cos( p ) * r;
		y = dynContour.center.y + dynContour.radius * sin( p ) * r;
		
		dynContour.points[ i ].x = x;
		dynContour.points[ i ].y = y;
	}
	
	dynContour.offset += TWO_PI * ( 1 / 128.0 );
	
	//-- box2d contour.
	
	dynContourLine->destroyShape();
	delete dynContourLine;
	
	dynContourLine = new ofxBox2dLine();
	dynContourLine->setWorld( box2d.getWorld() );
	dynContourLine->update();
	
	bool bReversePoints;
	bReversePoints = true;
	
	int i;
	
	if( false )
	{
		for( i=dynContour.pointsTotal-1; i>0; i-- )
			dynContourLine->addPoint( dynContour.points[ i ].x, dynContour.points[ i ].y );
		
		i = dynContour.pointsTotal - 1;
		dynContourLine->addPoint( dynContour.points[ i ].x, dynContour.points[ i ].y );
	}
	else
	{
		for( i=0; i<dynContour.pointsTotal; i++ )
			dynContourLine->addPoint( dynContour.points[ i ].x, dynContour.points[ i ].y );
		
		i = 0;
		dynContourLine->addPoint( dynContour.points[ i ].x, dynContour.points[ i ].y );
	}
	
	dynContourLine->createShape();
	
	//-- box2d contour circles.
	
//	for( int i=0; i<dynContour.pointsTotal; i++ )
//	{
//		dynContourCircles[ i ].destroyShape();
//	}
//	delete dynContourCircles;
//	dynContourCircles = new ofxBox2dCircle[ dynContour.pointsTotal ];
//	
//	for( int i=0; i<dynContour.pointsTotal; i++ )
//	{
//		dynContourCircles[ i ].setPhysics( cMass, cBounce, cFriction );
//		dynContourCircles[ i ].setup( box2d.getWorld(), dynContour.points[ i ].x, dynContour.points[ i ].y, 4 );
//		dynContourCircles[ i ].update();
//	}
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
		ofToString( circles[ i ].p1.x,		0 ) + " " +
		ofToString( circles[ i ].p1.y,		0 ) + " " +
		ofToString( circles[ i ].p2.x,		0 ) + " " +
		ofToString( circles[ i ].p2.y,		0 ) + " " +
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
			circle->p1.x		= atoi( circleData[ 1 ].c_str() );
			circle->p1.y		= atoi( circleData[ 2 ].c_str() );
			circle->p2.x		= atoi( circleData[ 3 ].c_str() );
			circle->p2.y		= atoi( circleData[ 4 ].c_str() );
			circle->color		= atoi( circleData[ 5 ].c_str() );
			circle->frame		= atoi( circleData[ 6 ].c_str() );
			circle->radius		= atoi( circleData[ 7 ].c_str() );
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

#if MODE == 0
	
	if( key == 'c' )
		shapes.clear();
	
	if( key == 'w' )
		shapes.writeToFile();
	
	if( key == 'l' )
		shapes.loadFromFile();
	
#endif
	
#if MODE == 1 || MODE == 2

	if( key == 'c' )
		clear();
	
	if( key == 'w' )
	{
		circleAddRate = 0;
		
		writeToFile();
	}
	
	if( key == ' ' )
		saveCircleData();
	
#endif
}

void Sydfest_06 :: mousePressed ( int x, int y, int button )
{
#if MODE == 0
	
	shapes.addVertex( x, y );
	
#endif

	int r;
	r = ofRandom( 0, circles.size() - 1 );
	
	Circle_06 *circ;
	circ = &circles.at( r );
	
	circ->pop = true;
	circ->bounce.center( circ->radius + 20 );
	circ->bounce.position( circ->radius );
	
	//--
	
	ofxBox2dCircle *box2dCircle;
	box2dCircle = &box2dCircles.at( r );
	
	ofPoint center;
	center.x = renderArea.width  * 0.5;
	center.y = renderArea.height * 0.5;
	
	box2dCircle->setVelocity( center.x, center.y );
}

void Sydfest_06 :: mouseMoved ( int x, int y )
{
#if MODE == 3
	
	dynContour.center.x = x;
	dynContour.center.y = y;
	
#endif
}
