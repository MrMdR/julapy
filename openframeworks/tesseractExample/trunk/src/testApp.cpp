#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
	fontCooperBlack.loadFont( "fonts/cooperBlack.ttf", 30 );
	
	image.loadImage( "images/hello_tesseract_photo.jpg" );

//-- couple different ways of passing image data to tesseract. 
	
//	tessStr = tess.findTextInImage( image );
	tessStr = tess.findTextInImage( image.getPixels(), image.width, image.height, OF_IMAGE_COLOR );
}

//--------------------------------------------------------------
void testApp::update()
{
	//
}

//--------------------------------------------------------------
void testApp::draw()
{
	ofBackground( 255, 255, 255 );
	
	ofSetColor( 0xFFFFFF );
	image.draw( 0, 0 );
	
	ofSetColor( 0xFF00FF );
	fontCooperBlack.drawString( tessStr, 35, 500 );
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{

}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{

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

