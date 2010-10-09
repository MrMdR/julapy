#include "testApp.h"

///////////////////////////////////////////
//	INIT.
///////////////////////////////////////////

void testApp::setup()
{
	DIR.setVerbose( false );
	
	int t = DIR.listDir( "bom_data" );
	for( int i=0; i<t; i++ )
	{
		string fileName;
		fileName = DIR.getName( i );

		parser.clear();
		parser.loadData( "bom_data/" + fileName );
		parser.createPixelsFromData();
		
		int w = parser.width;
		int h = parser.height;
		
		image0.allocate( w, h );
		image0.setFromPixels( parser.pixels, w, h );
		image0.mirror( true, false );
		
		image1.allocate( w, h, OF_IMAGE_GRAYSCALE );
		image1.setFromPixels( image0.getPixels(), w, h, OF_IMAGE_GRAYSCALE );
		image1.saveImage( "bom_image/" + fileName + ".png" );
		
		image0.clear();
		image1.clear();
	}
	
	ofSetWindowShape( parser.width, parser.height );
}

///////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////

void testApp::update()
{

}

///////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////

void testApp::draw()
{
	float cx = image1.getWidth()  * 0.5;
	float cy = image1.getHeight() * 0.5;
	
	image1.draw( 0, 0 );
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

