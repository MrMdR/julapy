#include "clockApp.h"

///////////////////////////////////////////
//	SETUP.
///////////////////////////////////////////

void clockApp :: setup()
{	
	ofRegisterTouchEvents(this);
	ofxAccelerometer.setup();
	ofxiPhoneAlerts.addListener(this);
	ofxiPhoneSetOrientation( OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT );
	
	//-------------------
	
	deviceType = DEVICE_IPAD;
	deviceType = DEVICE_IPHONE;

	ofBackground( 0, 0, 0 );
	ofSetFrameRate( frameRate = 60 );
//	ofSetVerticalSync( true );
	ofSetCircleResolution( 100 );
	
	//-- size.
	
	if( deviceType == DEVICE_IPHONE )
	{
		screenSize.width	= 480;
		screenSize.height	= 320;
	}
	else if( deviceType == DEVICE_IPAD )
	{
		screenSize.width	= 1024;
		screenSize.height	= 768;
	}
	
	//-- font.
	
	font1.loadFont( "fonts/1859_solferino_caps_light.ttf", 20 );
	font2.loadFont( "fonts/1859_solferino_caps_light.ttf", 20 );
	
	//-- sound.
	
	secTwoSound.loadSound( ofToDataPath( "sound/boop_1.caf" ) );
	secOneSound.loadSound( ofToDataPath( "sound/boop_2.caf" ) );
	
	//-- images.
	
	texFsSize.width		= screenSize.width  * 1.0;
	texFsSize.height	= screenSize.height * 1.0;
	
	if( deviceType == DEVICE_IPHONE )
	{
		image.loadImage( "image/background_480x320.png" );
	}
	else if( deviceType == DEVICE_IPAD )
	{
		image.loadImage( "image/background_1024x768.png" );
	}
	texBg.allocate( texFsSize.width, texFsSize.height, GL_RGBA );
	texBg.loadData( image.getPixels(), texFsSize.width, texFsSize.height, GL_RGBA );
	image.clear();
	
	if( deviceType == DEVICE_IPHONE )
	{
		image.loadImage( "image/info_480x320.png" );
	}
	else if( deviceType == DEVICE_IPAD )
	{
		image.loadImage( "image/info_1024x768.png" );
	}
	texInfo.allocate( texFsSize.width, texFsSize.height, GL_RGBA );
	texInfo.loadData( image.getPixels(), texFsSize.width, texFsSize.height, GL_RGBA );
	image.clear();

	if( deviceType == DEVICE_IPHONE )
	{
		image.loadImage( "image/membrane2_512x25.png" );
	}
	else if( deviceType == DEVICE_IPAD )
	{
		image.loadImage( "image/membrane2_512x25.png" );
	}
	texMembrane.allocate( image.width, image.height, GL_RGBA );
	texMembrane.loadData( image.getPixels(), image.width, image.height, GL_RGBA );
	image.clear();
	
	vector<string> cellNames;
	if( deviceType == DEVICE_IPHONE )
	{
		cellNames.push_back( "image/cell01_64x64.png" );
		cellNames.push_back( "image/cell02_64x64.png" );
		cellNames.push_back( "image/cell03_64x64.png" );
	}
	else if( deviceType == DEVICE_IPAD )
	{
		cellNames.push_back( "image/cell01_128x128.png" );
		cellNames.push_back( "image/cell02_128x128.png" );
		cellNames.push_back( "image/cell03_128x128.png" );
	}
	
	texCellsNum = cellNames.size();
	texCells	= new ofTexture[ texCellsNum ];
	
	for( int i=0; i<cellNames.size(); i++ )
	{
		image.loadImage( cellNames[ i ] );
		texCells[ i ].allocate( image.width, image.height, GL_RGBA );
		texCells[ i ].loadData( image.getPixels(), image.width, image.height, GL_RGBA );
		image.clear();
	}
	
	//--
	
	initClock();
}

void clockApp :: initClock ()
{
	box2d.init();
	box2d.setGravity( 0, 0 );
	box2d.setFPS( 30.0 );
	box2d.registerGrabbing();
	box2d.setIterations( 20, 10 );
	box2d.getWorld()->SetContactListener( &contactListener );
	
	contactListener.addReceiver( &clock );

	int w = screenSize.width;
	int h = screenSize.height;
	
	float a1 = w * h;
	float a2 = 1280 * 720;
	float fs = a1 / (float)a2;
	
	clock.setBox2d( &box2d );
	clock.setSize( w, h );
	clock.setForceScale( fs );
//	clock.setTimeFonts( &font1, &font2 );
	clock.setSound( &secTwoSound, &secOneSound );
	clock.setBgTexture( &texBg );
	clock.setCellTexture( texCells, texCellsNum );
	clock.setInfoTexture( &texInfo );
	clock.setMembraneTex( &texMembrane );
	clock.setup();
}

///////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////

void clockApp :: update()
{
	int hrs;
	int min;
	int sec;
	
	if( false )
	{
		int frameNum = ofGetFrameNum();
		
		int s = 00 + frameNum / frameRate;		// start time is add at front.
		int m = 48 + s / 60;					// start time is add at front.
		int h = 12 + m / 60;					// start time is add at front.
		
		hrs = h % 24;
		min = m % 60;
		sec = s % 60;
	}
	else
	{
		hrs = ofGetHours();
		min = ofGetMinutes();
		sec = ofGetSeconds();
	}
	
	//-- orientation.
	
//	float ax = ofxAccelerometer.getForce().x;
//	float ot = 0.6;								// orientation threshold.
//	
//	if( ax > ot )
//	{
//		if( ofxiPhoneGetOrientation() != OFXIPHONE_ORIENTATION_LANDSCAPE_LEFT )
//		{
//			ofxiPhoneSetOrientation( OFXIPHONE_ORIENTATION_LANDSCAPE_LEFT );
//		}
//	}
//	else if( ax < -ot )
//	{
//		if( ofxiPhoneGetOrientation() != OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT )
//		{
//			ofxiPhoneSetOrientation( OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT );
//		}
//	}
	
	//-- gravity slant.
	
	float gx;
	gx = ofxAccelerometer.getForce().y;
	gx *= 2;									// increase the reaction to tilt.
	gx = MIN( 1.0, MAX( -1.0, gx ) );			// between -1 and 1.
	gx *= ( ofxiPhoneGetOrientation() == OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT ) ? -1 : 1;

	float gy;
	gy = ofxAccelerometer.getForce().x;
	cout << gy << endl;
	gy *= 2;									// increase the reaction to tilt.
	gy = MIN( 1.0, MAX( -1.0, gy ) );			// between -1 and 1.
	gy *= ( ofxiPhoneGetOrientation() == OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT ) ? -1 : 1;
	
	clock.setGravity( gx, gy );
	
	//-- update.
	
	clock.update( hrs, min, sec );
}

///////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////

void clockApp :: draw()
{
	int bg = 0;
	ofBackground( bg, bg, bg );
	
	clock.draw();
	
	ofSetColor( 0x000000 );
	ofDrawBitmapString( ofToString( ofGetFrameRate(), 0 ), 15, ofGetHeight() - 15 );
}

void clockApp :: exit()
{

}

///////////////////////////////////////////
//	HANDLERS.
///////////////////////////////////////////

void clockApp::touchDown(ofTouchEventArgs &touch){

}

void clockApp::touchMoved(ofTouchEventArgs &touch){

}

void clockApp::touchUp(ofTouchEventArgs &touch){

}

void clockApp::touchDoubleTap( ofTouchEventArgs &touch )
{
	clock.toggleClockMode();
}

void clockApp::lostFocus(){

}

void clockApp::gotFocus(){

}

void clockApp::gotMemoryWarning(){

}

void clockApp::deviceOrientationChanged(int newOrientation)
{
	//
}

