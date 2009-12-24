/*
 *  Blobs01.cpp
 *  julapy_softbodies
 *
 *  Created by lukasz karluk on 22/11/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Blobs02.h"

Blobs02 :: Blobs02()
{
	//
}

Blobs02 :: ~Blobs02()
{
	//
}

void Blobs02 :: setup()
{
	ofSetCircleResolution( 50 );
	
	initBox2d();
	initAudioIn();
	initGui();
	
	blobScale		= 100.0;
	blobNodeDaming	= 0.87;
	blobNodeEase	= 0.4;
}

void Blobs02 :: initBox2d()
{
	box2d.init();
	box2d.setGravity( 0, 0 );
	box2d.checkBounds( true );
	box2d.createBounds( renderArea.x, renderArea.y, renderArea.width, renderArea.height );
	box2d.setFPS( 30.0 );
	
	addBlob( ofGetWidth() * 0.5f, ofGetHeight() * 0.5f, 200, 8 );

//	float ballMin = 30;
//	float ballMax = 80;
//	addBall( ofGetWidth() * 0.5f, ofGetHeight() * 0.5f, ofRandom( ballMin, ballMax ) );
//	addBall( ofGetWidth() * 0.5f, ofGetHeight() * 0.5f, ofRandom( ballMin, ballMax ) );
//	addBall( ofGetWidth() * 0.5f, ofGetHeight() * 0.5f, ofRandom( ballMin, ballMax ) );
//	addBall( ofGetWidth() * 0.5f, ofGetHeight() * 0.5f, ofRandom( ballMin, ballMax ) );
//	addBall( ofGetWidth() * 0.5f, ofGetHeight() * 0.5f, ofRandom( ballMin, ballMax ) );
//	addBall( ofGetWidth() * 0.5f, ofGetHeight() * 0.5f, ofRandom( ballMin, ballMax ) );
//	addBall( ofGetWidth() * 0.5f, ofGetHeight() * 0.5f, ofRandom( ballMin, ballMax ) );
//	addBall( ofGetWidth() * 0.5f, ofGetHeight() * 0.5f, ofRandom( ballMin, ballMax ) );
//	addBall( ofGetWidth() * 0.5f, ofGetHeight() * 0.5f, ofRandom( ballMin, ballMax ) );
//	addBall( ofGetWidth() * 0.5f, ofGetHeight() * 0.5f, ofRandom( ballMin, ballMax ) );
//	addBall( ofGetWidth() * 0.5f, ofGetHeight() * 0.5f, ofRandom( ballMin, ballMax ) );
//	addBall( ofGetWidth() * 0.5f, ofGetHeight() * 0.5f, ofRandom( ballMin, ballMax ) );
//	addBall( ofGetWidth() * 0.5f, ofGetHeight() * 0.5f, ofRandom( ballMin, ballMax ) );
	
	bDrawNodes	= false;
	bDrawJoints	= false;
	bDrawCurves	= true;
}

void Blobs02 :: addBlob( float bx, float by, float radius, int nodesTotal )
{
	if( nodesTotal % 2 != 0 )	// always even number of nodes, for opposite joins to balance.
		nodesTotal += 1;
	
	blobs.push_back( Blob() );
	
	float nodeMass;
	float nodeBounce;
	float nodeFriction;
	
	nodeMass		= 30.0;
	nodeBounce		= 0.53;
	nodeFriction	= 0.1;
	
	blobs.back().nodesTotal = nodesTotal;
	blobs.back().nodeRadius = radius;
	blobs.back().nodeScale	= new float[ nodesTotal ];
	
	for( int i=0; i<nodesTotal; i++ )
	{
		blobs.back().nodeScale[ i ] = 0;
	}
	
	blobs.back().nodes.push_back( ofxBox2dCircle() );				// center node.
	blobs.back().nodes.back().setPhysics( nodeMass, nodeBounce, nodeFriction );
	blobs.back().nodes.back().setup( box2d.getWorld(), bx, by, 10, true );
	blobs.back().nodes.back().disableCollistion();
	
	// the joints
	for( int i=0; i<nodesTotal; i++ ) 
	{
		float p = ( i / (float)nodesTotal ) * PI * 2;
		float x = bx + cos( p ) * radius; 
		float y = by + sin( p ) * radius;
		blobs.back().nodes.push_back( ofxBox2dCircle() );
		blobs.back().nodes.back().setPhysics( nodeMass, nodeBounce, nodeFriction );
		blobs.back().nodes.back().setup( box2d.getWorld(), x, y, 20 );
	}
	
	float jointStiffness;	// joint stiffness.
	float jointDamping;		// joint damping.

	jointStiffness	= 10.0;
	jointDamping	= 0.9;
	
	// joins between neighbouring nodes.
	for( int i=1; i<blobs.back().nodes.size(); i++ )
	{
		blobs.back().nodeJoints.push_back( ofxBox2dJoint() );
		
		if( i < blobs.back().nodes.size() - 1 )
		{
			blobs.back().nodeJoints.back().setWorld( box2d.getWorld() );
			blobs.back().nodeJoints.back().addJoint( blobs.back().nodes[ i ].body, blobs.back().nodes[ i + 1 ].body, jointStiffness, jointDamping );
		}
		else
		{
			blobs.back().nodeJoints.back().setWorld( box2d.getWorld() );
			blobs.back().nodeJoints.back().addJoint( blobs.back().nodes[ i ].body, blobs.back().nodes[ 1 ].body, jointStiffness, jointDamping );
		}
	}
	
	jointStiffness	= 1.0;
	jointDamping	= 0.9;
	
	// joins between opposite nodes.
	int nodesHalf = (int)( blobs.back().nodes.size() * 0.5 );
	for( int i=1; i<nodesHalf + 1; i++ )
	{
		int j = i + nodesHalf;
		
		blobs.back().nodeJoints.push_back( ofxBox2dJoint() );
		
		blobs.back().nodeJoints.back().setWorld( box2d.getWorld() );
		blobs.back().nodeJoints.back().addJoint( blobs.back().nodes[ i ].body, blobs.back().nodes[ j ].body, jointStiffness, jointDamping );
	}
	
	jointStiffness	= 1.0;
	jointDamping	= 0.9;
	
	// joins to center node.
	for( int i=1; i<blobs.back().nodes.size(); i++ )
	{
		blobs.back().nodeJoints.push_back( ofxBox2dJoint() );
		blobs.back().nodeJoints.back().setWorld( box2d.getWorld() );
		blobs.back().nodeJoints.back().addJoint( blobs.back().nodes[ 0 ].body, blobs.back().nodes[ i ].body, jointStiffness, jointDamping );
	}
}

void Blobs02 :: addBall( float bx, float by, float radius )
{
	float ballMass;
	float ballBounce;
	float ballFriction;
	
	ballMass		= 10;
	ballBounce		= 1.0;
	ballFriction	= 0.9;
	
	balls.push_back( Ball() );
	balls.back().circleRadius	= radius;
	balls.back().circle			= ofxBox2dCircle();
	balls.back().circle.setPhysics( ballMass, ballBounce, ballFriction );
	balls.back().circle.setup( box2d.getWorld(), bx, by, radius );
}

void Blobs02 :: initAudioIn ()
{
//	audio = new AudioLiveSpectrum();
	audio.init( "" );
	audio.setPeakDecay( 0.993 );
	audio.setMaxDecay( 0.999 );
}

void Blobs02 :: initGui ()
{
	gui.addToggle( "bDrawNodes",		bDrawNodes );
	gui.addToggle( "bDrawJoints",		bDrawJoints );
	gui.addToggle( "bDrawCurves",		bDrawCurves );
	gui.addSlider( "blobScale",			blobScale,			0.0, 300.0 );
	gui.addSlider( "blobNodeDaming",	blobNodeDaming,		0.0, 1.0 );
	gui.addSlider( "blobNodeEase",		blobNodeEase,		0.0, 1.0 );
	
	gui.loadFromXML( "ofxSimpleGuiToo.xml" );
}

//////////////////////////////////////////////
//	UPDATE.
//////////////////////////////////////////////

void Blobs02 :: update()
{
	updateAudio();
	updateBox2d();
}

void Blobs02 :: updateAudio ()
{
	audio.update();
	
	for( int i=0; i<blobs.size(); i++ )
	{
		audio.getFftPeakData( blobs[ i ].nodeScale, blobs[ i ].nodesTotal );
	}
}

void Blobs02 :: updateBox2d ()
{
	box2d.update();
	
	for( int i=0; i<blobs.size(); i++ )
	{
		for( int j=1; j<blobs[ i ].nodesTotal; j++)
		{
			ofxVec2f p1;
			ofxVec2f p2;
			ofxVec2f p3;
			
			p1.set( blobs[ i ].nodes[ 0 ].getPosition() );
			p2.set( blobs[ i ].nodes[ j ].getPosition() );
			
			float nodeScale;
			nodeScale = blobs[ i ].nodeScale[ j ];
			
			p3 = p2 - p1;
			p3.normalize();
			p3 *= blobs[ i ].nodeRadius + nodeScale * blobScale;
			
			p2.x += ( ( p1.x + p3.x ) - p2.x ) * blobNodeEase;
			p2.y += ( ( p1.y + p3.y ) - p2.y ) * blobNodeEase;
			
			blobs[ i ].nodes[ j ].setPosition( p2 );
			
			blobs[ i ].nodes[ j ].addDamping( blobNodeDaming );
		}
	}
}

//////////////////////////////////////////////
//	DRAW.
//////////////////////////////////////////////

void Blobs02 :: draw()
{
	glPushMatrix();
	glTranslatef( renderArea.x, renderArea.y, 0 );
		drawBox2d();
		drawAudio();
		gui.draw();
	glPopMatrix();
}

void Blobs02 :: drawDebug ()
{
	//
}

void Blobs02 :: drawBox2d ()
{
	ofSetColor( 255, 255, 255 );
	
	ofFill();				// fill
	drawBlobs();
	
	ofEnableSmoothing();	// lines smoothed.
	ofNoFill();
	drawBlobs();
	ofDisableSmoothing();
	
	for( int i=0; i<blobs.size(); i++ )
	{
		if( bDrawNodes )
		{
			for( int j=0; j<blobs[ i ].nodes.size(); j++)
			{
				blobs[ i ].nodes[ j ].draw();
			}
		}
		
		if( bDrawJoints )
		{
			for( int j=0; j<blobs[ i ].nodeJoints.size(); j++)
			{
				blobs[ i ].nodeJoints[ j ].draw();
			}
		}
	}
	
	ofSetColor( 30, 30, 30 );
	
	ofFill();				// ball fill
	drawBalls();
	
	ofEnableSmoothing();	// ball lines smoothed.
	ofNoFill();
	drawBalls();
	ofDisableSmoothing();
}

void Blobs02 :: drawBlobs ()
{
	ofBeginShape();
	
	for( int i=0; i<blobs.size(); i++ )
	{
		for( int j=1; j<blobs[ i ].nodes.size() + 3; j++)		// plus 3 to complete the curve.
		{
			int k = j;
			if( k >= blobs[ i ].nodes.size() )
				k -= blobs[ i ].nodes.size() - 1;
			
			ofPoint p;
			p = blobs[ i ].nodes[ k ].getPosition();
			
			if( bDrawCurves )
			{
				ofCurveVertex( p.x, p.y );
			}
			else
			{
				if( j < blobs[ i ].nodes.size() )
				{
					ofVertex( p.x, p.y );
				}
			}
			
		}
	}
	
	ofEndShape( true );
}

void Blobs02 :: drawBalls ()
{
	for( int i=0; i<balls.size(); i++ )
	{
		ofPoint p;
		p = balls[ i ].circle.getPosition();
		
		ofCircle( p.x, p.y, balls[ i ].circleRadius );
	}
}

void Blobs02 :: drawAudio ()
{
	glPushMatrix();
	glTranslatef( 10, renderArea.height - 210, 0 );
	
	audio.draw( 400, 200 );
	
	glPopMatrix();
}

void Blobs02 :: keyReleased( int key )
{
	if( key == 'n' )
		bDrawNodes = !bDrawNodes;
	
	if( key == 'j' )
		bDrawJoints = !bDrawJoints;
	
	if( key == 'c' )
		bDrawCurves = !bDrawCurves;
}

