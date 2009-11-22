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
	initBox2d();
	initAudioIn();
}

void Blobs02 :: initBox2d()
{
	box2d.init();
	box2d.setGravity( 0, 0 );
	box2d.createFloor();
	box2d.checkBounds( true );
	box2d.createBounds( renderArea.x, renderArea.y, renderArea.width, renderArea.height );
	box2d.setFPS( 30.0 );
	
	addBlob( ofGetWidth() * 0.5f, ofGetHeight() * 0.5f, 200, 20 );
	
	bDrawNodes	= true;
	bDrawJoints	= true;
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
	
	blobs.back().nodes.push_back( ofxBox2dCircle() );				// center node.
	blobs.back().nodes.back().setPhysics( nodeMass, nodeBounce, nodeFriction );
	blobs.back().nodes.back().setup( box2d.getWorld(), bx, by, 10, true );
	
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

void Blobs02 :: initAudioIn ()
{
	audio = new AudioLiveSpectrum();
	audio->init( "" );
}

//////////////////////////////////////////////
//	UPDATE.
//////////////////////////////////////////////

void Blobs02 :: update()
{
	updateBox2d();
	updateAudio();
}

void Blobs02 :: updateBox2d ()
{
	box2d.update();
	
	for( int i=0; i<blobs.size(); i++ )
	{
		for( int j=0; j<blobs[ i ].nodes.size(); j++)
		{
			blobs[ i ].nodes[ j ].addDamping( 0.98 );
		}
	}
}

void Blobs02 :: updateAudio ()
{
	audio->update();
}

//////////////////////////////////////////////
//	DRAW.
//////////////////////////////////////////////

void Blobs02 :: draw()
{
	glPushMatrix();
	glTranslatef( renderArea.x, renderArea.y, 0 );
		drawAudio();
		drawBox2d();
	glPopMatrix();
}


void Blobs02 :: drawBox2d ()
{
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
		
		ofEnableAlphaBlending();
		ofFill();
		ofSetColor( 255, 255, 0, 100 );
		ofBeginShape();
		for( int j=1; j<blobs[ i ].nodes.size(); j++)
		{
			ofPoint p;
			p = blobs[ i ].nodes[ j ].getPosition();
			
			if( bDrawCurves )
			{
				ofCurveVertex( p.x, p.y );
			}
			else
			{
				ofVertex( p.x, p.y );
			}
			
		}
		ofEndShape( true );
		ofDisableAlphaBlending();
	}
}

void Blobs02 :: drawAudio ()
{
	glPushMatrix();
	glTranslatef( 10, renderArea.height - 210, 0 );
	
	audio->draw( 400, 200 );
	
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

