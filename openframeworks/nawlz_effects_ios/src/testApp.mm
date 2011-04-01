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
	
	for( int i=0; i<7; i++ )
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
//    loadApp( NAWLZ_QUESTIONS_APP );
    loadApp( NAWLZ_JACKINBOX_APP );
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
	
	if( appID == NAWLZ_FLUID_APP )
	{
		NawlzFluid* nawlzFluid;
		nawlzFluid = new NawlzFluid();
		
		img = new Image();
		loadImage( ofToDataPath( "NawlzFluid/whale_bg_1024x768.png" ), *img );
		nawlzFluid->createBackgroundTexture( img->pixels, img->width, img->height, img->glType, 0, 0 );
		delete img;
        img = NULL;
		
		img = new Image();
		loadImage( ofToDataPath( "NawlzFluid/whale_477x223.png" ), *img );
		nawlzFluid->createImageTexture( img->pixels, img->width, img->height, img->glType, 358, 220 );
		delete img;
        img = NULL;
		
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
        img = NULL;
		
		img = new Image();
		loadImage( ofToDataPath( "NawlzBlah/blah_particle_2.png" ), *img );
		nawlzBlah->createParticleTexture( img->pixels, img->width, img->height, img->glType );
		delete img;
        img = NULL;
		
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
        img = NULL;
		
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
        img = NULL;
		
		img = new Image();
		loadImage( ofToDataPath( "NawlzFishBowl/fish_bowl_trace.png" ), *img );
		nawlzFishBowl->createBowlTexture( img->pixels, img->width, img->height, img->glType );
		delete img;
        img = NULL;
		
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
        img = NULL;

		img = new Image();
		loadImage( ofToDataPath( "NawlzInfodel/infodel_harley_0.png" ), *img );
		nawlzInfodel->createHarleyOneTexture( img->pixels, img->width, img->height, img->glType, 366, 311 );
		delete img;
        img = NULL;

		img = new Image();
		loadImage( ofToDataPath( "NawlzInfodel/infodel_harley_1.png" ), *img );
		nawlzInfodel->createHarleyOneTexture( img->pixels, img->width, img->height, img->glType, 438, 366 );
		delete img;
        
		img = new Image();
		loadImage( ofToDataPath( "NawlzInfodel/infodel_particle.png" ), *img );
		nawlzInfodel->createParticleTexture( img->pixels, img->width, img->height, img->glType );
		delete img;
        img = NULL;
		
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
    else if( appID == NAWLZ_JACKINBOX_APP )
    {
        NawlzJackInBox* nawlzJackInBox;
        nawlzJackInBox = new NawlzJackInBox();

		img = new Image();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_bg.png" ), *img );
		nawlzJackInBox->createBackgroundTexture( img->pixels, img->width, img->height, img->glType );
		delete img;
        
        //--- parts.
        
		img = new Image();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_body.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 629, 247 ), ofPoint( 0, 0 ), 0, 0, false );
        delete img;
        
		img = new Image();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_tongue.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 650, 534 ), ofPoint( 0, 0 ), 0, 0, false );
        delete img;
        
		img = new Image();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_lighter.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 759, 330 ), ofPoint( -5, -95 ), 120, 70, true );
        delete img;
        
		img = new Image();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_bomb_holder.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 768, 325 ), ofPoint( -8, -47 ), 70, 80, true );
        delete img;
        
		img = new Image();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_rocket.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 645, 326 ), ofPoint( -130, -47 ), 70, 270, true );
        delete img;
        
		img = new Image();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_arm_right.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 751, 425 ), ofPoint( -8, -95 ), 90, 75, true );
        delete img;
        
		img = new Image();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_arm_left.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 646, 426 ), ofPoint( -115, -73 ), 80, 270, true );
        delete img;
        
		img = new Image();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_bomb.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 723, 310 ), ofPoint( -6, -65 ), 105, 75, true );
        delete img;
        
        //--- ears right.
        
		img = new Image();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_ears_right_3.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 725, 183 ), ofPoint( -4, -13 ), 50, 100, true );
        delete img;
        
		img = new Image();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_ears_right_1.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 724, 180 ), ofPoint( -3, -33 ), 55, 70, true );
        delete img;
        
		img = new Image();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_ears_right_2.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 726, 182 ), ofPoint( -5, -25 ), 70, 90, true );
        delete img;
        
        //--- brain.
        
		img = new Image();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_arrow.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 710, 157 ), ofPoint( -37, -63 ), 35, 0, true );
        delete img;
        
		img = new Image();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_brain.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 714, 138 ), ofPoint( -23, -71 ), 55, 10, true );
        delete img;
        
        //--- head.
        
		img = new Image();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_head_solo.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 667, 134 ), ofPoint( 0, 0 ), 0, 0, false );
        delete img;
        
		img = new Image();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_head_spine.tiff" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 695, 205 ), ofPoint( 0, 0 ), 0, 0, false );
        delete img;
        
        //--- eyes left.
        
		img = new Image();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_eyes_left_L.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 698, 150 ), ofPoint( -69, -36 ), 60, -70, true );
        delete img;
        
		img = new Image();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_eyes_left_M.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 703, 147 ), ofPoint( -74, -66 ), 60, -45, true );
        delete img;
        
		img = new Image();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_eyes_left_R.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 708, 141 ), ofPoint( -30, -54 ), 45, -20, true );
        delete img;
        
        //--- eyes right.
        
		img = new Image();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_eyes_right_R.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 741, 150 ), ofPoint( -9, -55 ), 80, 60, true );
        delete img;
        
		img = new Image();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_eyes_right_M.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 739, 149 ), ofPoint( -7, -61 ), 55, 36, true );
        delete img;
        
		img = new Image();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_eyes_right_L.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 735, 147 ), ofPoint( -3, -64 ), 55, 13, true );
        delete img;
        
        //--- ears left.
        
		img = new Image();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_ears_left_3.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 679, 177 ), ofPoint( -78, -5 ), 75, -125, true );
        delete img;
        
		img = new Image();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_ears_left_2.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 678, 178 ), ofPoint( -69, -13 ), 45, -110, true );
        delete img;
        
		img = new Image();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_ears_left_1.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 677, 174 ), ofPoint( -76, -44 ), 70, -75, true );
        delete img;
        
        //--- mouth.
        
		img = new Image();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_mouth_base.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 699, 157 ), ofPoint( 0, 0 ), 0, 0, false );
        delete img;
        
		img = new Image();
		loadImage( ofToDataPath( "NawlzJackInBox/jackinbox_mouth_tongue.png" ), *img );
        nawlzJackInBox->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 752, 211 ), ofPoint( -12, -7 ), 20, 135, true );
        delete img;
        
		nawlzJackInBox->setup();
		
		nawlzApp = nawlzJackInBox;
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

