#include "clockApp.h"

///////////////////////////////////////////
//	INIT.
///////////////////////////////////////////

void clockApp :: setup()
{
	ofSetFrameRate( frameRate = 60 );
//	ofSetVerticalSync( true );
	ofSetCircleResolution( 100 );
	ofBackground( 30, 30, 30 );
	ofSetFullscreen( bFullScreen = false );

	bDebug = false;
	
	standardRect.width	= 1280;
	standardRect.height	= 720;
	
	if( bFullScreen )
	{
		screenRect.width	= ofGetScreenWidth();
		screenRect.height	= ofGetScreenHeight();
	}
	else
	{
		screenRect.width	= standardRect.width;
		screenRect.height	= standardRect.height;
	}
	
	//-- font.
	
	font1.loadFont( "fonts/1859_solferino_caps_light.ttf", 260 );
	font2.loadFont( "fonts/1859_solferino_caps_light.ttf", 70 );
	
	//-- sound.
	
	secTwoSound.loadSound( ofToDataPath( "sound/beep_600hz_0_70.wav" ) );
	secOneSound.loadSound( ofToDataPath( "sound/beep_400hz_1_40.wav" ) );
	
	//-- images.
	
	texFsSize.width		= standardRect.width  * 1.0;
	texFsSize.height	= standardRect.height * 1.0;
	
	image.loadImage( "image/background_1280x720.png" );
	texBg.allocate( texFsSize.width, texFsSize.height, GL_RGB );
	texBg.loadData( image.getPixels(), texFsSize.width, texFsSize.height, GL_RGB );
	image.clear();

	image.loadImage( "image/info_1280x720.png" );
	texInfo.allocate( texFsSize.width, texFsSize.height, GL_RGBA );
	texInfo.loadData( image.getPixels(), texFsSize.width, texFsSize.height, GL_RGBA );
	image.clear();
	
	image.loadImage( "image/membrane2_1024x50.png" );
	texMembrane.allocate( image.width, image.height, GL_RGBA );
	texMembrane.loadData( image.getPixels(), image.width, image.height, GL_RGBA );
	image.clear();
	
	//-- cell textures.
	
	vector<string> cellNames;
	
	if( DIR.listDir( "image/cell_texture_0" ) > 0 )
		cellNames.push_back( "image/cell_texture_0/" + DIR.getName( 0 ) );

	if( DIR.listDir( "image/cell_texture_1" ) > 0 )
		cellNames.push_back( "image/cell_texture_1/" + DIR.getName( 0 ) );

	if( DIR.listDir( "image/cell_texture_2" ) > 0 )
		cellNames.push_back( "image/cell_texture_2/" + DIR.getName( 0 ) );
	
	if( DIR.listDir( "image/cell_texture_3" ) > 0 )
		cellNames.push_back( "image/cell_texture_3/" + DIR.getName( 0 ) );
	
	if( DIR.listDir( "image/cell_texture_4" ) > 0 )
		cellNames.push_back( "image/cell_texture_4/" + DIR.getName( 0 ) );
	
	if( DIR.listDir( "image/cell_texture_5" ) > 0 )
		cellNames.push_back( "image/cell_texture_5/" + DIR.getName( 0 ) );
	
	texCellsNum = cellNames.size();
	texCells	= new ofTexture[ texCellsNum ];
	
	for( int i=0; i<cellNames.size(); i++ )
	{
		image.loadImage( cellNames[ i ] );
		texCells[ i ].allocate( image.width, image.height, GL_RGBA );
		texCells[ i ].loadData( image.getPixels(), image.width, image.height, GL_RGBA );
		image.clear();
	}
	
	cellNames.clear();
	
	//-- line textures.
	
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

	vector<string> digitNames;
	digitNames.push_back( "image/digits/0_128x128.png" );
	digitNames.push_back( "image/digits/1_128x128.png" );
	digitNames.push_back( "image/digits/2_128x128.png" );
	digitNames.push_back( "image/digits/3_128x128.png" );
	digitNames.push_back( "image/digits/4_128x128.png" );
	digitNames.push_back( "image/digits/5_128x128.png" );
	digitNames.push_back( "image/digits/6_128x128.png" );
	digitNames.push_back( "image/digits/7_128x128.png" );
	digitNames.push_back( "image/digits/8_128x128.png" );
	digitNames.push_back( "image/digits/9_128x128.png" );
	
	texDigitsNum	= digitNames.size();
	texDigits		= new ofTexture[ texDigitsNum ];
	
	for( int i=0; i<digitNames.size(); i++ )
	{
		image.loadImage( digitNames[ i ] );
		texDigits[ i ].allocate( image.width, image.height, GL_RGBA );
		texDigits[ i ].loadData( image.getPixels(), image.width, image.height, GL_RGBA );
		image.clear();
	}
	
//	texCellAnim.addFrameSequence( ofToDataPath( "image/cell_anim_01/"), "Black_Organism_02_", 5, "png", 62 );
	
	//-- screen grabber.
	
	screenGrabber.setup( "movie/" );
	screenGrabber.setPause( true );
	
	//-- video saver.
	
	videoSaver.setup( standardRect, OF_IMAGE_COLOR );
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
	clock.setSize( screenRect );
	clock.setSound( &secTwoSound, &secOneSound );
	clock.setBgTexture( &texBg );
	clock.setCellTexture( texCells, texCellsNum );
//	clock.setCellAnimTex( &texCellAnim.getFrames() );
//	clock.setLineTexture( texLines, texLinesNum );
	clock.setInfoTexture( &texInfo );
	clock.setMembraneTex( &texMembrane );
	clock.setDigitTexture( texDigits, texDigitsNum );
	clock.setup();
}

void clockApp :: initGui ()
{
#ifdef USE_GUI
	
	gui.addTitle( "clock" );
	gui.addSlider( "forceCenterPull",	clock.forceCenterPull,	0, 100 );
	gui.addSlider( "forceCenterPush",	clock.forceCenterPush,	0, 100 );
	gui.addSlider( "rayBlobPad",		clock.rayBlobPad,		0, 100 );
	gui.addSlider( "rayBlobEase",		clock.rayBlobEase,		0, 1 );
	
	gui.setPage( 1 );
	
	if( bDebug )
		gui.show();
	
#endif
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
	ofDrawBitmapString( ofToString( ofGetFrameRate(), 0 ), 15, 15 );

#ifdef USE_GUI
	gui.draw();
#endif
	
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
		
#ifdef USE_GUI
		
		if( bDebug )
		{
			gui.show();
		}
		else
		{
			gui.hide();
		}
		
#endif
	}
	
	if( key == ' ' )
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
	
	box2d.setGravity( gx * 20, gy * 20 );
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