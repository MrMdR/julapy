#include "filteringApp.h"

///////////////////////////////////////////
//	INIT.
///////////////////////////////////////////

void filteringApp :: setup()
{
	ofSetFrameRate( 60 );
	ofSetVerticalSync( true );
	ofSetCircleResolution( 100 );
	
	box2d.init();
	box2d.setGravity( 0, 0 );
	box2d.setFPS( 30.0 );
	box2d.registerGrabbing();
	
	createBounds( ground1, 0, 0, ofGetWidth(), ofGetHeight(), 1 );		// create bounds with ground index 1.
	createBounds( ground2, 0, 0, ofGetWidth(), ofGetHeight(), 2 );		// create bounds with ground index 2.
	
	createCircles( blueCircles, 20, 1 );								// create circles with ground index 1.
	createCircles( redCircles,  20, 2 );								// create circles with ground index 2.
	
	// the idea is that groups 1 and 2 will not collide even though they are part of the same box2d simulation.
}

void filteringApp :: createBounds( b2Body* ground, float x, float y, float w, float h, int groupIndex ) 
{
	b2BodyDef bd;
	bd.position.Set(0, 0);
	ground = box2d.world->CreateBody(&bd);	
	b2PolygonDef sd;
	sd.filter.groupIndex = groupIndex;
	
	sd.density = 0.0f;
	sd.restitution = 0.0f;
	sd.friction = 0.6;
	float thick = 10/OFX_BOX2D_SCALE;
	
	// w h x y r 
	//right
	sd.SetAsBox(thick, (h/OFX_BOX2D_SCALE)/2, b2Vec2((w/OFX_BOX2D_SCALE), (h/OFX_BOX2D_SCALE)/2), 0.0);
	ground->CreateShape(&sd);
	//left
	sd.SetAsBox(thick, (h/OFX_BOX2D_SCALE)/2, b2Vec2(0, (h/OFX_BOX2D_SCALE)/2), 0.0);
	ground->CreateShape(&sd);
	//top
	sd.SetAsBox((w/OFX_BOX2D_SCALE)/2, thick, b2Vec2((w/OFX_BOX2D_SCALE)/2, 0), 0.0);
	ground->CreateShape(&sd);
	//bottom
	sd.SetAsBox((w/OFX_BOX2D_SCALE)/2, thick, b2Vec2((w/OFX_BOX2D_SCALE)/2, h/OFX_BOX2D_SCALE), 0.0);
	ground->CreateShape(&sd);
}

void filteringApp :: createCircles ( vector<ofxBox2dCircle>& circles, int circlesNum, int groupIndex )
{
	for( int i=0; i<circlesNum; i++ )
	{
		circles.push_back( ofxBox2dCircle() );
		ofxBox2dCircle& circle = circles.back();
		
		float mass		= 3.0;
		float bounce	= 0.53;
		float friction	= 0.1;
		float radius	= 40.0;
		
		circle.setPhysics( mass, bounce, friction );
		circle.setup( box2d.getWorld(), ofRandom( 0, ofGetWidth() ), ofRandom( 0, ofGetHeight() ), radius, false );
		circle.setVelocity( ofRandom( -10, 10 ), ofRandom( -10, 10 ) );
		
		//-------------------------------------------------
		
		b2FilterData filterData;
		filterData.groupIndex	= groupIndex;		// group filtering has higher precedence than category filtering.
//		filterData.categoryBits	= 0x0004;			// if groupIndex is provided, categoryBits & maskBits is ignored.
//		filterData.maskBits		= 0x0004;			// read more on filtering under fixtures, http://www.box2d.org/manual.html#_Toc258082972
		
		circle.setFilterData( filterData );
		
		//-------------------------------------------------
	}
}

///////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////

void filteringApp::update()
{
	box2d.update();
}

///////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////

void filteringApp::draw()
{
	ofBackground( 0, 0, 0 );
	
	drawCircles( blueCircles, 0x0000FF );
	drawCircles( redCircles, 0xFF0000 );
}

void filteringApp :: drawCircles ( vector<ofxBox2dCircle>& circles, int color )
{
	ofFill();
	ofSetColor( color );
	
	for( int i=0; i<circles.size(); i++ )
	{
		ofxBox2dCircle& circle = circles[ i ];
		
		ofCircle( circle.getPosition().x, circle.getPosition().y, circle.getRadius() );
	}
}

///////////////////////////////////////////
//	HANDLERS.
///////////////////////////////////////////

void filteringApp::keyPressed(int key)
{

}

void filteringApp::keyReleased(int key)
{

}

void filteringApp::mouseMoved(int x, int y )
{

}

void filteringApp::mouseDragged(int x, int y, int button)
{

}

void filteringApp::mousePressed(int x, int y, int button)
{

}

void filteringApp::mouseReleased(int x, int y, int button)
{

}

void filteringApp::windowResized(int w, int h)
{

}

