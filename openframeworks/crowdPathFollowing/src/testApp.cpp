#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetVerticalSync( true );
	ofSetFrameRate( 60 );
	
	float offset = 60;
	
	path.init();
	path.addPoint( offset, offset );
	path.addPoint( ofGetWidth() - offset, offset );
	path.addPoint( ofGetWidth() - offset, ofGetHeight() - offset );
	path.addPoint( ofGetWidth() * 0.5, ofGetHeight() - offset * 3 );
	path.addPoint( offset, ofGetHeight() - offset );
	
	for( int i=0; i<120; i++ )
	{
		addBoid
		(
			ofRandom( 0, ofGetWidth() ),
			ofRandom( 0, ofGetHeight() )
		);
	}
}

void testApp :: addBoid( float x, float y )
{
	float maxspeed	= ofRandom( 2, 4 );
	float maxforce	= 0.3;
	
	ofxVec2f loc;
	loc.set( x, y );
	
	boids.push_back( Boid() );
	
	Boid& b = boids.back();
	b.init( loc, maxspeed, maxforce );
}

//--------------------------------------------------------------
void testApp::update()
{
	for( int i=0; i< boids.size(); i++ )
	{
		Boid& boid = boids.at( i );
		boid.applyForces( boids, path );
		boid.run();
	}
}

//--------------------------------------------------------------
void testApp::draw()
{
	ofBackground( 255, 255, 255 );
	path.display();
	
	for( int i=0; i< boids.size(); i++ )
	{
		Boid& boid = boids.at( i );
		boid.render();
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
	addBoid( x, y );
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

