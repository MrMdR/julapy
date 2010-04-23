#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate( 25 );
	ofSetVerticalSync( true );
	
	fontVerdana.loadFont( "fonts/verdana.ttf", 22 );
	fontCooperBlack.loadFont( "fonts/cooperBlack.ttf", 22 );
	
	//-- image.

//	image.loadImage( "images/hello_tesseract.jpg" );
	image.loadImage( "images/predator_subtitles.jpg" );

	float px, py;
	px = 1.0;
	py = 0.3;
	
	roi.x		= 0;
	roi.y		= image.height * ( 1 - py );
	roi.width	= image.width  * px;
	roi.height	= image.height * py;
	
	imageColor.allocate( image.width, image.height );
	imageGray.allocate( image.width, image.height );
	imageGrayCrop.allocate( roi.width, roi.height );
	
	//-- camera.
	
	bCheckCameraImage = false;
	camera.initGrabber( 320, 240, true );
	
	cameraColorImage.allocate( camera.width, camera.height );
	cameraGrayImage.allocate( camera.width, camera.height );
	
	threshold_1		= 236;
	blur_1			= 1;
	threshold_2		= 21;
	bShowFullImage	= false;

	//-- gui.
	
	initGui();
}

void testApp :: initGui ()
{
	gui.addTitle( "" );
	gui.addSlider( "threshold_1 ",		threshold_1,	0, 255 );
	gui.addSlider( "blur_1 ",			blur_1,			0, 255 );
	gui.addSlider( "threshold_2 ",		threshold_2,	0, 255 );
	gui.addToggle( "bShowFullImage",	bShowFullImage );
	
	gui.show();
	gui.setPage( 1 );
}

//--------------------------------------------------------------
void testApp::update()
{
	findTextinStaticImage();
	
	camera.update();
	
	if( camera.isFrameNew() )
	{
		cameraColorImage.setFromPixels( camera.getPixels(), camera.width, camera.height );
		
//		findTextInCameraImage();
	}
}

void testApp :: findTextinStaticImage ()
{
	imageColor.setFromPixels( image.getPixels(), image.width, image.height );
	
	imageGray = imageColor;
	imageGray.threshold( threshold_1 );
	imageGray.blurGaussian( blur_1 );
	imageGray.threshold( threshold_2 );
	
	ofxCvGrayscaleImage imageGrayTemp;									// have to copy image due to bug with roi.
	imageGrayTemp.allocate( imageGray.width, imageGray.height );
	imageGrayTemp = imageGray;
	imageGrayTemp.setROI( roi );
	
	imageGrayCrop.setFromPixels( imageGrayTemp.getRoiPixels(), roi.width, roi.height );
	
	imageGrayTemp.clear();
	
	tessStr = tess.findTextInImage( imageGrayCrop.getPixels(), imageGrayCrop.width, imageGrayCrop.height, OF_IMAGE_GRAYSCALE );
	parseTesseractString( tessStr );
}

void testApp :: findTextInCameraImage ()
{
	if( camera.isFrameNew() )
	{
		cameraColorImage.setFromPixels( camera.getPixels(), camera.width, camera.height );
		cameraGrayImage = cameraColorImage;
		cameraGrayImage.threshold( threshold_1 );
		cameraGrayImage.blurGaussian( blur_1 );
		cameraGrayImage.threshold( threshold_2 );
	}
	
	if( bCheckCameraImage )
	{
		bCheckCameraImage = false;
		
		tessStr = tess.findTextInImage( cameraGrayImage.getPixels(), cameraGrayImage.width, cameraGrayImage.height, OF_IMAGE_GRAYSCALE );
		parseTesseractString( tessStr );
	}
}

void testApp :: parseTesseractString ( string &str )
{
	// remove unwanted bits.
}

//--------------------------------------------------------------
void testApp::draw()
{
	ofBackground( 20, 20, 20 );
	
	glPushMatrix();
	glTranslatef( 300, 100, 0 );

	drawSourceImage();

	glPushMatrix();
	glTranslatef( 0, imageColor.height + 30, 0 );
	
	if( bShowFullImage )
	{
		drawProcessedImage();
	}
	else
	{
		drawProcessedCropImage();
	}
	
	drawTesseractText();

	glPopMatrix();
	glPopMatrix();
	
//	drawVideoSource();
//	drawVideoProcessed();
	
	gui.draw();
}

void testApp :: drawSourceImage ()
{
	ofFill();
	ofSetColor( 0xFFFFFF );
	
	int b = 2;
	ofRect
	(
		-b,
		-b,
		imageColor.width  + b * 2,
		imageColor.height + b * 2
	);
	
	imageColor.draw( 0, 0 );
	
	drawROI();
}

void testApp :: drawProcessedImage ()
{
	ofFill();
	ofSetColor( 0xFFFFFF );

	int b = 2;
	ofRect
	(
		-b,
		-b,
		imageGray.width  + b * 2,
		imageGray.height + b * 2
	);
	
	imageGray.draw( 0, 0 );
	
	drawROI();
}

void testApp :: drawProcessedCropImage ()
{
	ofFill();
	ofSetColor( 0xFFFFFF );
	
	int b = 2;
	ofRect
	(
		-b,
		-b,
		imageGrayCrop.width  + b * 2,
		imageGrayCrop.height + b * 2
	 );
	
	imageGrayCrop.draw( 0, 0 );
}

void testApp :: drawVideoSource ()
{
	ofSetColor( 0xFFFFFF );
	
	cameraColorImage.draw
	(
		ofGetWidth() - cameraColorImage.width - 10,
		100
	);
}

void testApp :: drawVideoProcessed ()
{
	ofSetColor( 0xFFFFFF );
	
	cameraGrayImage.draw
	(
		ofGetWidth() - cameraGrayImage.width - 10,
		cameraColorImage.height + 60 + 10
	);
}

void testApp :: drawROI ()
{
	ofNoFill();
	ofSetLineWidth( 3 );
	ofSetColor( 0XFF0000 );
	ofRect( roi.x, roi.y, roi.width, roi.height );
}

void testApp :: drawTesseractText ()
{
	int y;
	y = 0;
	
	if( bShowFullImage )
	{
		y += imageGray.height;
	}
	else
	{
		y += imageGrayCrop.height;
	}
	
	y += 70;
	
	ofSetColor( 255, 0, 255 );
	fontCooperBlack.drawString( tessStr, 0, y );
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{

}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{
	if( key == ' ' )
	{
		bCheckCameraImage = true;
	}

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

