#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	framenum=0;
	doCapture=false;
	
	ofSetFrameRate(36);
	ofBackground(200, 200, 200);
	
	for(int i=0; i<100; i++) {
		f.addBoid();
	}
}

//--------------------------------------------------------------
void testApp::update(){
	f.update();
	
}



//--------------------------------------------------------------
void testApp::draw(){
	f.draw();
	capture();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	if(key=='c') doCapture = !doCapture;
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::capture() {
	if(doCapture && ofGetFrameNum() % 4 == 0)
	{
		char filename[255];
		sprintf(filename, "frame%05d.png", framenum++);
		ofSaveScreen(filename);
	}
}