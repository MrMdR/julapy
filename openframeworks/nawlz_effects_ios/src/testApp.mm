#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
	ofxAccelerometer.setup();
	ofxiPhoneAlerts.addListener(this);
	iPhoneSetOrientation( OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT );
	ofBackground(127,127,127);
	ofSetFrameRate( 30 );
	
	stage = ofxFlashStage :: getInstance();			// ofxFlash setup.
	stage->setTouchMode( true );
	
	xfl = new ofxFlashXFL();
	xfl->loadFile( "Assets/DOMDocument.xml" );		// load XFL flash file.
	xfl->build();
	
	ofxFlashMovieClip* main;
	main = (ofxFlashMovieClip*)stage->root()->getChildByName( "main" );
	
	for( int i=0; i<6; i++ )
	{
		string btnID = "";
		char btnIDChar[ 255 ];
		sprintf( btnIDChar, "btn%01d", i );
		btnID.insert( 0, btnIDChar );
		
		ofxFlashMovieClip* btnMc;
		btnMc = (ofxFlashMovieClip*)main->getChildByName( btnID );
		
		if( !btnMc )
			continue;
		
		Btn* btn;
		btn = new Btn( btnMc );
		
		btns.push_back( btn );
	}
	
	nawlzApp	= NULL;
	appID		= -1;
	
//	loadApp( NAWLZ_FLUID_APP );
//	loadApp( NAWLZ_BLAH_APP );
//	loadApp( NAWLZ_SAND_APP );
//	loadApp( NAWLZ_FISH_BOWL_APP );
//    loadApp( NAWLZ_INFODEL_APP );
    loadApp( NAWLZ_QUESTIONS_APP );
}

void testApp :: loadApp ( int appID )
{
	if( this->appID == appID )
		return;
	
	this->appID = appID;
	
	if( nawlzApp )
	{
		delete nawlzApp;
		nawlzApp = NULL;
	}
	
	Image* img;
	
	if( appID == NAWLZ_FLUID_APP )
	{
		NawlzFluid* nawlzFluid;
		nawlzFluid = new NawlzFluid();
		
		img = new Image();
		loadImage( ofToDataPath( "NawlzFluid/whale_bg_1024x768.png" ), *img );
		nawlzFluid->createBackgroundTexture( img->pixels, img->width, img->height, img->glType, 0, 0 );
		delete img;
		
		img = new Image();
		loadImage( ofToDataPath( "NawlzFluid/whale_477x223.png" ), *img );
		nawlzFluid->createImageTexture( img->pixels, img->width, img->height, img->glType, 358, 220 );
		delete img;
		
		nawlzFluid->bDrawMeshGrid		= true;
		nawlzFluid->bDrawMeshQuads		= true;
		nawlzFluid->bDrawParticles		= true;
		
		nawlzFluid->setup();
		
		nawlzApp = nawlzFluid;
	}
	else if( appID == NAWLZ_BLAH_APP )
	{
		NawlzBlah* nawlzBlah;
		nawlzBlah = new NawlzBlah();

		img = new Image();
		loadImage( ofToDataPath( "NawlzBlah/blah_bg.png" ), *img );
		nawlzBlah->createBackgroundTexture( img->pixels, img->width, img->height, img->glType );
		delete img;
		
		img = new Image();
		loadImage( ofToDataPath( "NawlzBlah/blah_particle_2.png" ), *img );
		nawlzBlah->createParticleTexture( img->pixels, img->width, img->height, img->glType );
		delete img;
		
		nawlzBlah->setup();
		
		nawlzApp = nawlzBlah;
	}
	else if( appID == NAWLZ_SAND_APP )
	{
		NawlzSand* nawlzSand;
		nawlzSand = new NawlzSand();

		img = new Image();
		loadImage( ofToDataPath( "NawlzSand/sand_bg.png" ), *img );
		nawlzSand->createBackgroundTexture( img->pixels, img->width, img->height, img->glType );
		delete img;
		
		nawlzSand->setup();
		
		nawlzApp = nawlzSand;
	}
	else if( appID == NAWLZ_FISH_BOWL_APP )
	{
		NawlzFishBowl* nawlzFishBowl;
		nawlzFishBowl = new NawlzFishBowl();

		img = new Image();
		loadImage( ofToDataPath( "NawlzFishBowl/fish_bowl_bg.png" ), *img );
		nawlzFishBowl->createBackgroundTexture( img->pixels, img->width, img->height, img->glType );
		delete img;
		
		img = new Image();
		loadImage( ofToDataPath( "NawlzFishBowl/fish_bowl_trace.png" ), *img );
		nawlzFishBowl->createBowlTexture( img->pixels, img->width, img->height, img->glType );
		delete img;
		
		nawlzFishBowl->setup();
		
		nawlzApp = nawlzFishBowl;
	}
    else if( appID == NAWLZ_INFODEL_APP )
    {
        NawlzInfodel* nawlzInfodel;
        nawlzInfodel = new NawlzInfodel();
        
		img = new Image();
		loadImage( ofToDataPath( "NawlzInfodel/infodel_bg_clean.png" ), *img );
		nawlzInfodel->createBackgroundTexture( img->pixels, img->width, img->height, img->glType );
		delete img;

		img = new Image();
		loadImage( ofToDataPath( "NawlzInfodel/infodel_harley_0.png" ), *img );
		nawlzInfodel->createHarleyOneTexture( img->pixels, img->width, img->height, img->glType, 366, 311 );
		delete img;

		img = new Image();
		loadImage( ofToDataPath( "NawlzInfodel/infodel_harley_1.png" ), *img );
		nawlzInfodel->createHarleyOneTexture( img->pixels, img->width, img->height, img->glType, 438, 366 );
		delete img;
        
		img = new Image();
		loadImage( ofToDataPath( "NawlzInfodel/infodel_particle.png" ), *img );
		nawlzInfodel->createParticleTexture( img->pixels, img->width, img->height, img->glType );
		delete img;
		
		nawlzInfodel->setup();
		
		nawlzApp = nawlzInfodel;
    }
    else if( appID == NAWLZ_QUESTIONS_APP )
    {
        NawlzQuestions* nawlzQuestions;
        nawlzQuestions = new NawlzQuestions();
        
		img = new Image();
		loadImage( ofToDataPath( "NawlzQuestions/questions_bg.png" ), *img );
		nawlzQuestions->createBackgroundTexture( img->pixels, img->width, img->height, img->glType );
		delete img;
        
		img = new Image();
		loadImage( ofToDataPath( "NawlzQuestions/questions_particle.png" ), *img );
		nawlzQuestions->createParticleTexture( img->pixels, img->width, img->height, img->glType );
		delete img;
		
		nawlzQuestions->setup();
		
		nawlzApp = nawlzQuestions;
    }
}

void testApp :: loadImage ( string path, Image& imageOut )
{
	NSString*	imagePath;
	UIImage*	image;
	
	GLubyte* pixels;
	int imageWidth;
	int imageHeight;
	
	imagePath	= ofxStringToNSString( path );
	image		= [ [ UIImage alloc ] initWithContentsOfFile : imagePath ];
	
	CGContextRef spriteContext;
	CGImageRef	cgImage = image.CGImage;
	
	int bytesPerPixel	= CGImageGetBitsPerPixel(cgImage)/8;
	if(bytesPerPixel == 3) bytesPerPixel = 4;
	
	imageWidth	= CGImageGetWidth(cgImage);
	imageHeight	= CGImageGetHeight(cgImage);
	
	pixels			= (GLubyte *) malloc( imageWidth * imageHeight * bytesPerPixel);
	spriteContext	= CGBitmapContextCreate(pixels, imageWidth, imageHeight, CGImageGetBitsPerComponent(cgImage), imageWidth * bytesPerPixel, CGImageGetColorSpace(cgImage), bytesPerPixel == 4 ? kCGImageAlphaPremultipliedLast : kCGImageAlphaNone);
	CGContextDrawImage(spriteContext, CGRectMake(0.0, 0.0, (CGFloat)imageWidth, (CGFloat)imageHeight), cgImage);
	CGContextRelease(spriteContext);
	
	imageOut.width		= imageWidth;
	imageOut.height		= imageHeight;
	imageOut.pixelDepth	= bytesPerPixel;
	imageOut.glType		= GL_LUMINANCE;
	if( bytesPerPixel == 3 )
		imageOut.glType	= GL_RGB;
	if( bytesPerPixel == 4 )
		imageOut.glType	= GL_RGBA;
	imageOut.pixels		= new unsigned char[ imageWidth * imageHeight * bytesPerPixel ];
	
	memcpy( imageOut.pixels, pixels, imageWidth * imageHeight * bytesPerPixel );
	
	free( pixels );
	[ image release ];
}


//--------------------------------------------------------------
void testApp::update()
{
	stage->update();
	
	for( int i=0; i<btns.size(); i++ )
	{
		Btn* btn;
		btn = btns[ i ];
		btn->update();
		
		if( btn->isPressed() )
		{
			loadApp( i );
		}
	}
	
	if( nawlzApp )
	{
		nawlzApp->update();
	}
}

//--------------------------------------------------------------
void testApp::draw()
{
	if( nawlzApp )
	{
		nawlzApp->draw();
	}
	
	stage->draw();
}

//--------------------------------------------------------------
void testApp::exit(){

}

//--------------------------------------------------------------
void testApp::touchDown( int x, int y, int id )
{
	stage->mousePressed( x, y, id );
}

//--------------------------------------------------------------
void testApp::touchMoved( int x, int y, int id )
{
	stage->mouseMoved( x, y, id );
	nawlzApp->mouseMoved( x, y );
}

//--------------------------------------------------------------
void testApp::touchUp( int x, int y, int id )
{
	stage->mouseReleased( x, y, id );
}

//--------------------------------------------------------------
void testApp::touchDoubleTap( int x, int y, int id ){

}

//--------------------------------------------------------------
void testApp::lostFocus(){

}

//--------------------------------------------------------------
void testApp::gotFocus(){

}

//--------------------------------------------------------------
void testApp::gotMemoryWarning(){

}

//--------------------------------------------------------------
void testApp::deviceOrientationChanged(int newOrientation){

}

