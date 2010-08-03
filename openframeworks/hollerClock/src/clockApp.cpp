#include "clockApp.h"

///////////////////////////////////////////
//	INIT.
///////////////////////////////////////////

void clockApp :: setup()
{
	ofSetFrameRate( frameRate = 60 );
	ofSetVerticalSync( true );
	ofSetCircleResolution( 100 );
	ofBackground( 30, 30, 30 );
	
	bDebug = false;
	
	//-- font.
	
	font1.loadFont( "fonts/1859_solferino_caps_light.ttf", 260 );
	font2.loadFont( "fonts/1859_solferino_caps_light.ttf", 70 );
	
	//-- sound.
	
	secTwoSound.loadSound( ofToDataPath( "sound/boop_1.wav" ) );
	secOneSound.loadSound( ofToDataPath( "sound/boop_2.wav" ) );
	
	//-- images.
	
	texBgSize.width		= ofGetWidth()  * 1.0;
	texBgSize.height	= ofGetHeight() * 1.0;
	
	image.loadImage( "image/background_1280x720.png" );
	texBg.allocate( texBgSize.width, texBgSize.height, GL_RGBA );
	texBg.loadData( image.getPixels(), texBgSize.width, texBgSize.height, GL_RGBA );
	image.clear();
	
	vector<string> cellNames;
	cellNames.push_back( "image/cell01_256x256.png" );
	cellNames.push_back( "image/cell02_256x256.png" );
	cellNames.push_back( "image/cell03_256x256.png" );
	
	texCellsNum = cellNames.size();
	texCells	= new ofTexture[ texCellsNum ];
	
	for( int i=0; i<cellNames.size(); i++ )
	{
		image.loadImage( cellNames[ i ] );
		texCells[ i ].allocate( image.width, image.height, GL_RGBA );
		texCells[ i ].loadData( image.getPixels(), image.width, image.height, GL_RGBA );
		image.clear();
	}
	
	vector<string> lineNames;
	lineNames.push_back( "image/line01.png" );
	lineNames.push_back( "image/line02.png" );
	lineNames.push_back( "image/line03.png" );
	
	texLinesNum = lineNames.size();
	texLines	= new ofTexture[ texLinesNum ];

	for( int i=0; i<lineNames.size(); i++ )
	{
		image.loadImage( lineNames[ i ] );
		texLines[ i ].allocate( image.width, image.height, GL_RGBA );
		texLines[ i ].loadData( image.getPixels(), image.width, image.height, GL_RGBA );
		image.clear();
	}
	
	//-- screen grabber.
	
	screenGrabber.setup( "movie/" );
	screenGrabber.setPause( true );
	
	//-- video saver.
	
	ofRectangle rect;
	rect.width	= ofGetWidth();
	rect.height	= ofGetHeight();
	
	videoSaver.setup( rect, OF_IMAGE_COLOR );
	videoSaver.setPause( true );
	
	initClock();
	initGui();
}
	
void clockApp :: initClock ()
{
	box2d.init();
	box2d.setGravity( 0, 0 );
	box2d.setFPS( 30.0 );
	box2d.registerGrabbing();
	box2d.getWorld()->SetContactListener( &contactListener );
	
	contactListener.addReceiver( &clock );
	
	clock.setBox2d( &box2d );
	clock.setSize( ofGetWidth(), ofGetHeight() );
	clock.setTimeFonts( &font1, &font2 );
	clock.setSound( &secTwoSound, &secOneSound );
	clock.setBgTexture( &texBg );
	clock.setCellTexture( texCells, texCellsNum );
	clock.setLineTexture( texLines, texLinesNum );
	clock.setup();
}

void clockApp :: initGui ()
{
	gui.addTitle( "clock" );
	gui.addSlider( "forceCenterPull",	clock.forceCenterPull,	0, 100 );
	gui.addSlider( "forceCenterPush",	clock.forceCenterPush,	0, 100 );
	gui.addSlider( "rayBlobPad",		clock.rayBlobPad,		0, 100 );
	gui.addSlider( "rayBlobEase",		clock.rayBlobEase,		0, 1 );
	
	gui.setPage( 1 );
	
	if( bDebug )
		gui.show();
}

///////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////

void clockApp::update()
{
	int hrs;
	int min;
	int sec;
	
	if( screenGrabber.isRecording() )
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
	
	clock.update( hrs, min, sec );
}

///////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////

void clockApp::draw()
{
	clock.draw();
	
	ofSetColor( 0x000000 );
	ofDrawBitmapString( ofToString( ofGetFrameRate(), 0 ), 15, ofGetHeight() - 15 );

	gui.draw();
	
	screenGrabber.save();
	
	screenImage.grabScreen( 0, 0, ofGetWidth(), ofGetHeight() );
	videoSaver.addFrame( screenImage.getPixels() );
}

///////////////////////////////////////////
//	HANDLERS.
///////////////////////////////////////////

void clockApp :: keyPressed(int key)
{
	if( key == 's' )
		screenGrabber.togglePause();
	
	if( key == 'v' )
	{
		videoSaver.togglePause();
		
		if( videoSaver.isPaused() )
		{
			videoSaver.saveToDisk( "movies/" );
		}
	}
	
	if( key == 'd' )
	{
		bDebug = !bDebug;
		
		if( bDebug )
		{
			gui.show();
		}
		else
		{
			gui.hide();
		}
	}
	
	if( key == 'm' )
	{
		clock.toggleClockMode();
	}
}

void clockApp :: keyReleased(int key)
{

}

void clockApp::mouseMoved(int x, int y )
{
	float px;
	px = x / (float)ofGetWidth();
	px = MAX( MIN( px , 1 ), 0 );

	float py;
	py = y / (float)ofGetHeight();
	py = MAX( MIN( py , 1 ), 0 );
	
	float gx;
	gx = ( px - 0.5 ) * 2;

	float gy;
	gy = ( py - 0.5 ) * 2;
	
	clock.setGravity( gx, gy );
}

void clockApp::mouseDragged(int x, int y, int button)
{

}

void clockApp::mousePressed(int x, int y, int button)
{

}

void clockApp::mouseReleased(int x, int y, int button)
{

}

void clockApp::windowResized(int w, int h)
{

}