#include "testApp.h"

///////////////////////////////////////////////////
//	SETUP.
///////////////////////////////////////////////////

void testApp :: setup ()
{
	ofSetFrameRate( 25 );
	ofSetVerticalSync( true );
	
	bDebug	= true;
	
	initBoids();
	initGui();
}

void testApp :: initGui ()
{
	gui.addTitle( "boids" );
	gui.addSlider( "boidSeperationWeight ", boidSeperationWeight,	0, 10.0 );
	gui.addSlider( "boidAlignmentWeight ",	boidAlignmentWeight,	0, 10.0 );
	gui.addSlider( "boidCohesionWeight ",	boidCohesionWeight,		0, 10.0 );
	gui.addSlider( "boidSeparationDist ",	boidSeparationDist,		0, 500.0 );
	gui.addSlider( "boidNeighbourDist ",	boidNeighbourDist,		0, 500.0 );
	gui.addSlider( "boidMaxSpeed ",			boidMaxSpeed,			0, 100.0 );
	gui.addSlider( "boidMaxForce ",			boidMaxForce,			0, 10.0 );
	
	gui.addPage();
	gui.addTitle( "contour analysis" );
	
//	gui.loadFromXML();
	
	gui.show();
	gui.setPage( 1 );
}

///////////////////////////////////////////////////
//	BOIDS.
///////////////////////////////////////////////////

void testApp :: initBoids ()
{
	boidsNum = 50;
	
	for( int i=0; i<boidsNum; i++ )
	{
		boids.push_back( Boid() );
		
		Boid &boid = boids.back();
		boid.setBoids( &boids );
		boid.setPosition
		(
			ofRandom( 0, ofGetWidth()  ),
			ofRandom( 0, ofGetHeight() )
		);
		boid.setVelocity
		(
			ofRandom( -2, 2 ),
			ofRandom( -2, 2 )
		);
		
		boid.trailCol.push_back( 0xFFFFFF );
	}
	
	//--
	
	Boid &boid = boids[ 0 ];		// store the boid group setting based on the first boid.
	
	boidSeperationWeight	= boid.separationWeight;
	boidAlignmentWeight		= boid.alignmentWeight;
	boidCohesionWeight		= boid.cohesionWeight;
	
	boidSeparationDist		= boid.separationDist;
	boidNeighbourDist		= boid.neighbourDist;
	boidMaxSpeed			= boid.maxSpeed;
	boidMaxForce			= boid.maxForce;
}

void testApp :: updateBoids ()
{
	for( int i=0; i<boids.size(); i++ )
	{
		Boid &boid = boids[ i ];
		
		boid.separationWeight	= boidSeperationWeight;
		boid.alignmentWeight	= boidAlignmentWeight;
		boid.cohesionWeight		= boidCohesionWeight;
		
		boid.separationDist		= boidSeparationDist;
		boid.neighbourDist		= boidNeighbourDist;
		boid.maxSpeed			= boidMaxSpeed;
		boid.maxForce			= boidMaxForce;
	}
		
	for( int i=0; i<boids.size(); i++ )
	{
		Boid &boid = boids[ i ];
		boid.update_acc();
		boid.update_vel();
		boid.update_pos();
	}
	
	for( int i=0; i<boids.size(); i++ )
	{
		Boid &boid = boids[ i ];
		boid.update_final();
	}
}

void testApp :: drawBoids ()
{
	ofFill();
	
	for( int i=0; i<boids.size(); i++ )
	{
		Boid &boid = boids[ i ];
		boid.draw();
	}
	
	ofNoFill();
	ofEnableSmoothing();
	
	for( int i=0; i<boids.size(); i++ )
	{
		Boid &boid = boids[ i ];
		boid.draw();
	}
	
	ofDisableSmoothing();
}

///////////////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////////////

void testApp :: update()
{
	updateBoids();
}

///////////////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////////////

void testApp :: draw()
{
	ofFill();
	ofSetColor( 0x000000 );
	ofRect( 0, 0, ofGetWidth(), ofGetHeight() );
	
	drawBoids();
	
	gui.draw();
}

///////////////////////////////////////////////////
//	EVENTS.
///////////////////////////////////////////////////

void testApp :: keyPressed(int key)
{
	if( key == 'd' )
	{
		bDebug = !bDebug;
	}
}

void testApp::keyReleased(int key)
{

}

void testApp::mouseMoved(int x, int y )
{

}

void testApp::mouseDragged(int x, int y, int button)
{

}

void testApp::mousePressed(int x, int y, int button)
{

}

void testApp::mouseReleased(int x, int y, int button)
{

}

void testApp::windowResized(int w, int h)
{

}

