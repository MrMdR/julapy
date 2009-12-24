/*
 *  Blobs01.cpp
 *  julapy_softbodies
 *
 *  Created by lukasz karluk on 22/11/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Blobs01.h"

Blobs01 :: Blobs01()
{
	//
}

Blobs01 :: ~Blobs01()
{
	//
}

void Blobs01 :: setup()
{
	initBox2d();
	initVideoGrabber();
	initOpticalField();
}

void Blobs01 :: initBox2d()
{
	box2d.init();
	box2d.setGravity( 0, 2.5 );
	box2d.checkBounds( true );
	box2d.createBounds( renderArea.x, renderArea.y, renderArea.width, renderArea.height );
	box2d.setFPS( 30.0 );
	
	addBlob( ofGetWidth() * 0.2f, ofGetHeight() * 0.5f, 100, 40 );
	addBlob( ofGetWidth() * 0.4f, ofGetHeight() * 0.5f, 100, 40 );
	addBlob( ofGetWidth() * 0.6f, ofGetHeight() * 0.5f, 100, 40 );
	addBlob( ofGetWidth() * 0.8f, ofGetHeight() * 0.5f, 100, 40 );
	
	bDrawNodes	= true;
	bDrawJoints	= true;
	bDrawCurves	= true;
}

void Blobs01 :: addBlob( float bx, float by, float radius, int nodesTotal )
{
	if( nodesTotal % 2 != 0 )	// always even number of nodes, for opposite joins to balance.
		nodesTotal += 1;
	
	blobs.push_back( Blob() );
	
	// the joints
	for( int i=0; i<nodesTotal; i++ ) 
	{
		float p = ( i / (float)nodesTotal ) * PI * 2;
		float x = bx + cos( p ) * radius; 
		float y = by + sin( p ) * radius;
		blobs.back().nodes.push_back( ofxBox2dCircle() );
		blobs.back().nodes.back().setPhysics( 3.0, 0.53, 0.1 );
		blobs.back().nodes.back().setup( box2d.getWorld(), x, y, 10 );
	}
	
	float jointS = 20.0;		// joint stiffness.
	float jointD = 0.0;		// joint damping.
	
	// joins between neighbouring nodes.
	for( int i=0; i<blobs.back().nodes.size(); i++ )
	{
		blobs.back().nodeJoints.push_back( ofxBox2dJoint() );
		
		if( i < blobs.back().nodes.size() - 1)
		{
			blobs.back().nodeJoints.back().setWorld( box2d.getWorld() );
			blobs.back().nodeJoints.back().addJoint( blobs.back().nodes[ i ].body, blobs.back().nodes[ i + 1 ].body, jointS, jointD );
		}
		else
		{
			blobs.back().nodeJoints.back().setWorld( box2d.getWorld() );
			blobs.back().nodeJoints.back().addJoint( blobs.back().nodes[ i ].body, blobs.back().nodes[ 0 ].body, jointS, jointD );
		}
	}
	
	int nodesHalf = (int)( blobs.back().nodes.size() * 0.5 );
	
	// joins between opposite nodes.
	for( int i=0; i<nodesHalf; i++ )
	{
		int j = i + nodesHalf;
		
		blobs.back().nodeJoints.push_back( ofxBox2dJoint() );
		
		blobs.back().nodeJoints.back().setWorld( box2d.getWorld() );
		blobs.back().nodeJoints.back().addJoint( blobs.back().nodes[ i ].body, blobs.back().nodes[ j ].body, jointS, jointD );
	}
}

void Blobs01 :: initVideoGrabber ()
{
	videoGrabberLrgRect.width  = 640;
	videoGrabberLrgRect.height = 480;
	
	videoGrabberSmlRect.width  = 320;
	videoGrabberSmlRect.height = 240;
	
	videoGrabberSrcRect.width  = 320;
	videoGrabberSrcRect.height = 240;
	
	videoGrabberDstRect.width  = 320;
	videoGrabberDstRect.height = 240;
	
	videoGrabber.initGrabber( videoGrabberLrgRect.width, videoGrabberLrgRect.height );
	
	videoGrabberLrgImage.allocate( videoGrabberLrgRect.width, videoGrabberLrgRect.height );
	videoGrabberSmlImage.allocate( videoGrabberSmlRect.width, videoGrabberSmlRect.height );
	videoGrabberSrcImage.allocate( videoGrabberSrcRect.width, videoGrabberSrcRect.height );
	videoGrabberDstImage.allocate( videoGrabberDstRect.width, videoGrabberDstRect.height );
	
	videoGrabberWarper.setup( &videoGrabberSrcImage, &videoGrabberDstImage );
	videoGrabberWarper.setPosition( 380, 10 );
	videoGrabberWarper.load( "video_warper_data.txt" );
	
	isVideoGrabberNewFrame = false;
}

void Blobs01 :: initOpticalField ()
{
	opticalField.init( videoGrabberDstRect, videoGrabberDstRect );
	opticalField.setMirror( false, true );
	opticalField.setImageType( GL_LUMINANCE );
	opticalField.showDifferenceImage	= true;
	opticalField.opticalFlowScale		= 0.001f;
}


//////////////////////////////////////////////
//	UPDATE.
//////////////////////////////////////////////

void Blobs01 :: update()
{
	updateVideoGrabber();
	updateOpticalField();
	updateBox2d();
}

void Blobs01 :: updateVideoGrabber ()
{
	videoGrabber.grabFrame();
	
	isVideoGrabberNewFrame = videoGrabber.isFrameNew();
	
	if( isVideoGrabberNewFrame )
	{
		videoGrabberLrgImage.setFromPixels( videoGrabber.getPixels(), videoGrabberLrgRect.width, videoGrabberLrgRect.height );
		
		videoGrabberSmlImage.scaleIntoMe( videoGrabberLrgImage );
		videoGrabberSrcImage.setFromColorImage( videoGrabberSmlImage );
		
		videoGrabberWarper.warp();
	}
}

void Blobs01 :: updateOpticalField ()
{
	if( isVideoGrabberNewFrame )
	{
		opticalField.update( videoGrabberDstImage.getPixels() );
	}
}

void Blobs01 :: updateBox2d ()
{
	box2d.update();
	
	ofRectangle rect;
	rect = ofxResizeUtil :: cropToSize( videoGrabberDstRect, renderArea );
	
	float velScale = 10000000;
	
	for( int i=0; i<blobs.size(); i++ )
	{
		for( int j=0; j<blobs[ i ].nodes.size(); j++ )
		{
			ofPoint np;		// node position.
			ofPoint p;		// position.
			ofPoint v;		// velocity.
			
			np = blobs[ i ].nodes[ j ].getPosition();
			
			p.x = ( np.x - rect.x ) / rect.width;		// x percentage of particle on full screen.
			p.y = ( np.y - rect.y ) / rect.height;		// y percentage of particle on full screen.
			
			if( p.x < 0.0 || p.x > 1.0 || p.y < 0.0 || p.y > 1.0 )
				continue;
			
			p.x *= videoGrabberDstRect.width;
			p.y *= videoGrabberDstRect.height;
			
			opticalField.getVelAtPixel( (int)p.x, (int)p.y, &( v.x ), &( v.y ) );
			
			v *= velScale;
			
			if( v.x == 0 && v.y == 0 )
				continue;
			
			float damping  = 0.3f;
			
			blobs[ i ].nodes[ j ].addForce( np, v );
			blobs[ i ].nodes[ j ].addDamping( damping, damping );
		}
	}
}

//////////////////////////////////////////////
//	UTILS.
//////////////////////////////////////////////



//////////////////////////////////////////////
//	DRAW.
//////////////////////////////////////////////

void Blobs01 :: draw()
{
	glPushMatrix();
	glTranslatef( 10, 10, 0 );
		drawOpticalField();
	glPopMatrix();

	glPushMatrix();
	glTranslatef( renderArea.x, renderArea.y, 0 );
		drawBox2d();
	glPopMatrix();
}

void Blobs01 :: drawDebug ()
{
	//
}

void Blobs01 :: drawOpticalField()
{
	ofFill();
	ofSetColor( 150, 150, 150 );
	
	glColor3f( 1, 1, 1 );
	glPushMatrix();
	glTranslatef( 0, 0, 0 );
	ofRect( -1, -1, videoGrabberSrcImage.width + 2, videoGrabberSrcImage.height + 2 );
	videoGrabberSrcImage.draw( 0, 0 );
	glPopMatrix();
	
	glColor3f( 1, 1, 1 );
	glPushMatrix();
	glTranslatef( videoGrabberSrcRect.width + 10, 0, 0 );
	ofRect( -1, -1, videoGrabberDstImage.width + 2, videoGrabberDstImage.height + 2 );
	videoGrabberDstImage.draw( 0, 0 );
	glPopMatrix();
	
	glColor3f( 1, 1, 1 );
	glPushMatrix();
	glTranslatef( videoGrabberSrcRect.width + videoGrabberDstRect.width + 20, 0, 0 );
	ofRect( -1, -1, videoGrabberDstImage.width + 2, videoGrabberDstImage.height + 2 );
	opticalField.drawDifference();
	glPopMatrix();
	
	glColor3f( 1, 1, 1 );
	glPushMatrix();
	glTranslatef( videoGrabberSrcRect.width + videoGrabberDstRect.width * 2 + 30, 0, 0 );
	ofRect( -1, -1, videoGrabberDstImage.width + 2, videoGrabberDstImage.height + 2 );
	opticalField.drawOpticalFlow();
	glPopMatrix();
}

void Blobs01 :: drawVideoGrabberWarper()
{
	videoGrabberWarper.draw();
}

void Blobs01 :: drawBox2d ()
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
		for( int j=0; j<blobs[ i ].nodes.size(); j++)
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

void Blobs01 :: keyReleased( int key )
{
	if( key == 'n' )
		bDrawNodes = !bDrawNodes;
	
	if( key == 'j' )
		bDrawJoints = !bDrawJoints;
	
	if( key == 'c' )
		bDrawCurves = !bDrawCurves;
}

