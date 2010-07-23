#include "contactApp.h"

void MyContactListener :: Add( const b2ContactPoint* point )
{
	b2Vec2 p;
	p = point->position;
	p *= OFX_BOX2D_SCALE;
	
	contactApp* appPtr = ((contactApp*)ofGetAppPtr());
	
	appPtr->contact_points.push_back( p );
	appPtr->contact_velocities.push_back( point->velocity );
}

void MyContactListener::Remove(const b2ContactPoint* point)
{
	//
}

//--------------------------------------------------------------
void contactApp::setup() {	 

	
	ofBackground(0, 0, 0);
	ofSetVerticalSync(true);
	ofEnableAlphaBlending();
	
	welt.init();
	welt.createBounds();
	welt.setGravity(0, 7.0);
	
	welt.getWorld()->SetContactListener(&contacts);
	
	ball.setPhysics(1.0, 1.0, 0.5);
	ball.setup(welt.getWorld(), 100, 100, 10, false);
	ofPoint pt(0, 0);
	ofPoint target(10000, 0);
	ball.addForce(target, pt);
	
	box.setPhysics(1.0, 0.1, 0.5);
	box.setup(welt.getWorld(), 300, 200, 100, 100, true);
	

}


//--------------------------------------------------------------
void contactApp::update(){
	
	welt.update();
	
}

//--------------------------------------------------------------
void contactApp::draw(){
	ofSetBackgroundAuto(true);

	ball.draw();
	box.draw();
	
	// show contacts
	for(int i=0; i<contact_points.size(); i++) {
		ofSetColor(0xffffff);
		ofLine(contact_points[i].x, contact_points[i].y, contact_points[i].x+contact_velocities[i].x, contact_points[i].y+contact_velocities[i].y);
		ofSetColor(0xff0000);
		ofCircle(contact_points[i].x, contact_points[i].y, 4);
	}	
	
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


