#include "testApp.h"

///////////////////////////////////////////
//	INIT.
///////////////////////////////////////////

void testApp::setup()
{
	ofRectangle rect;
	rect.width	= 886;
	rect.height	= 691;
	
	image0.allocate( rect.width, rect.height );
	image1.allocate( rect.width, rect.height );
	image2.allocate( rect.width, rect.height );
	
	image0copy.allocate( rect.width, rect.height );
	image1copy.allocate( rect.width, rect.height );
	
	image.loadImage( "meanT_9yr_run_mean_1915_0.05deg.png" );
	image0.setFromPixels( image.getPixels(), rect.width, rect.height );
	image.clear();
	
//	image.loadImage( "meanT_9yr_run_mean_1916_0.05deg.png" );
	image.loadImage( "meanT_9yr_run_mean_2005_0.05deg.png" );
	image1.setFromPixels( image.getPixels(), rect.width, rect.height );
	image.clear();
	
	blend = 0;
}

///////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////

void testApp::update()
{
	blend = mouseX / (float)ofGetWidth();
	
	cout << blend << endl;
	
	image0copy = image0;
	image1copy = image1;
	
	int r0 = blend * 255;
	int r1 = ( 1 - blend ) * 255;
	
	image0copy.convertToRange( 0, r0 );
	image1copy.convertToRange( 0, r1 );
	
	image2 = image0copy;
	image2 += image1copy;
}

///////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////

void testApp::draw()
{
	ofFill();
	ofSetColor( 0x000000 );
	ofRect( 0, 0, ofGetWidth(), ofGetHeight() );
	
	int w = (int)( image0.width  * 0.5 );
	int h = (int)( image0.height * 0.5 );
	
	ofSetColor( 0xFFFFFF );
	image0copy.draw( 0, 0, w, h );
	image1copy.draw( w, 0, w, h );
	image2.draw( (int)( w * 0.5 ), h, w, h );
}

///////////////////////////////////////////
//	HANDLERS.
///////////////////////////////////////////

void testApp::keyPressed(int key)
{

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

