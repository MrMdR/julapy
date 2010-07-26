#include "contactApp.h"

//--------------------------------------------------------------	CONTACT LISTNER CLASS.

void MyContactListener :: Add( const b2ContactPoint* point )
{
	contactApp* appPtr = ((contactApp*)ofGetAppPtr());
	appPtr->box2dContactEventHandler( point );
}

void MyContactListener :: Remove(const b2ContactPoint* point)
{
	//
}

//--------------------------------------------------------------	CUSTOM BALL CLASS.

void CustomBall :: update ()
{
	r += ( 255 - r ) * 0.1;
	g += ( 255 - g ) * 0.1;
	b += ( 255 - b ) * 0.1;
}

void CustomBall :: draw ()
{
	ofSetColor( r, g, b );
	ofCircle( getPosition().x, getPosition().y, getRadius() );
}

//--------------------------------------------------------------	BUSINESS AS USUAL.

void contactApp :: setup()
{
	ofBackground(0, 0, 0);
	ofSetVerticalSync(true);
	ofSetCircleResolution( 100 );
	
	box2d.init();
	box2d.createBounds();
	box2d.setGravity( 0, 0 );
	box2d.registerGrabbing();
	box2d.getWorld()->SetContactListener( &contacts );		// register contact class.
	
	for( int i=0; i<20; i++ )
	{
		balls.push_back( CustomBall() );
		CustomBall& circle = balls.back();
		
		float mass		= 3.0;
		float bounce	= 0.53;
		float friction	= 0.1;
		float radius	= 40.0;
		
		circle.setPhysics( mass, bounce, friction );
		circle.setup( box2d.getWorld(), ofRandom( 0, ofGetWidth() ), ofRandom( 0, ofGetHeight() ), radius, false );
		circle.setVelocity( ofRandom( -10, 10 ), ofRandom( -10, 10 ) );
		
		circle.r = 255;
		circle.g = 255;
		circle.b = 255;
	}
}

void contactApp :: box2dContactEventHandler ( const b2ContactPoint* point )
{
	for( int i=0; i<balls.size(); i++ )
	{
		CustomBall& ball = balls[ i ];
		
		for( b2Shape* s=ball.body->GetShapeList(); s; s=s->GetNext() )
		{
			if( point->shape1 == s || point->shape2 == s )
			{
				float vel;								// weight colour change by velocity.
				vel = point->velocity.Length();
				vel /= 10;
				vel = ofClamp( vel, 0, 1 );
				
				ball.g = 255 - 255 * vel;
			}
		}
	}
}

//--------------------------------------------------------------

void contactApp :: update(){
	
	box2d.update();
	
	for( int i=0; i<balls.size(); i++ )
	{
		balls[ i ].update();
	}
}

//--------------------------------------------------------------

void contactApp :: draw()
{
	ofSetBackgroundAuto(true);
	
	//-- draw balls with soft edges.
	
	ofFill();
	for( int i=0; i<balls.size(); i++ )
		balls[ i ].draw();
	
	ofNoFill();
	ofEnableSmoothing();
	for( int i=0; i<balls.size(); i++ )
		balls[ i ].draw();
	
	ofDisableSmoothing();
}

void contactApp::windowResized(int w, int h) {

}


//--------------------------------------------------------------
void contactApp::keyPressed  (int key){ 

}
//--------------------------------------------------------------
void contactApp::keyReleased (int key){ 

}
//--------------------------------------------------------------
void contactApp::mousePressed(int x, int y, int button) {
	

}

//--------------------------------------------------------------
void contactApp::mouseReleased(int x, int y, int button){

	
}

//--------------------------------------------------------------
void contactApp::mouseMoved(int x, int y ){

}

void contactApp::mouseDragged(int x, int y, int button) {

}


