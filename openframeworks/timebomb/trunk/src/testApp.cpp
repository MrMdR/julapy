#include "testApp.h"
#include "msaColor.h"

#define FLUID_WIDTH			150


#pragma mark Custom methods


void fadeToColor(float r, float g, float b, float speed) {
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(r, g, b, speed);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(myApp->window.width, 0);
    glVertex2f(myApp->window.width, myApp->window.height);
    glVertex2f(0, myApp->window.height);
    glEnd();
}


void testApp::addToFluid(float x, float y, float dx, float dy, bool addColor, bool addForce)
{
//	float speed = dx * dx  + dy * dy * window.aspectRatio2;    // balance the x and y components of speed with the screen aspect ratio
	float speed = dx * dx  + dy * dy;
	
	if(speed > 0)
	{
		if(x<0) x = 0; 
		else if(x>1) x = 1;
		if(y<0) y = 0; 
		else if(y>1) y = 1;
		
		float velocityMult = 30;
//		float velocityMult = ( 1 - interactionScale ) * 1000 + 1;
		
        int index = fluidSolver.getIndexForNormalizedPosition(x, y);
		
		if( addColor )
		{
			fluidSolver.r[ index ] += fluidColorScale;
			fluidSolver.g[ index ] += fluidColorScale;
			fluidSolver.b[ index ] += fluidColorScale;
		}
		
		if( addForce )
		{
			fluidSolver.u[ index ] += dx * velocityMult;
			fluidSolver.v[ index ] += dy * velocityMult;
		}
		
		if( !drawFluid && ofGetFrameNum() % 5 == 0 ) 
		{
			fadeToColor(0, 0, 0, 0.1);
		}
    }
}


#pragma mark App callbacks

////////////////////////////////////////////////////////////////////////////////////////////////////
// SETUP.
////////////////////////////////////////////////////////////////////////////////////////////////////

void testApp::setup() 
{	 
	ofBackground(0, 0, 0);
	ofSetVerticalSync(true);
	ofSetFrameRate( 30 );
	
	windowResized( ofGetWidth(), ofGetHeight() );		// initialize stuff according to current window size
	
	initVideoGrabber();

#ifdef USE_VIDEO_INPUT
	initVideoInput();
#endif

#ifdef USE_VIDEO_INPUT
	initOpticalFieldForVideoInput();
#else
	initOpticalFieldForCameraInput();
#endif
	
#ifdef USE_VIDEO_OUTPUT
	initVideoOutput();
#endif
	
#ifdef USE_CAMERA_OUTPUT
	initCameraOutput();
#endif
	
#ifdef USE_VIDEO_OUTPUT
	initTimeDistortionForVideo();
#endif

#ifdef USE_CAMERA_OUTPUT
	initTimeDistortionForCamera();
#endif
	
#ifdef USE_VIDEO_OUTPUT	
	initFluidForVideo();
#endif
	
#ifdef USE_CAMERA_OUTPUT
	initFluidForCamera();
#endif
	
#ifdef USE_TUIO
	tuioClient.start( 3333 );
#endif

#ifdef USE_GUI 
	initGui();
#endif
	
#ifdef USE_VIDEO_SAVER
	initVideoSaver();
#endif
}

void testApp :: initVideoGrabber ()
{
//	camWidth	= 640;
//	camHeight	= 480;
	
	camWidth	= 320;
	camHeight	= 240;
	
	videoGrabber.setVerbose( true );
	videoGrabber.initGrabber( camWidth, camHeight );
	
	isVideoGrabberNewFrame = false;
}

void testApp :: initVideoInput()
{
#ifdef USE_VIDEO_INPUT
	videoInput.loadMovie( "tb_input.mov" );
	videoPlayer.setPaused( true );
	
	videoInputWidth		= videoInput.width;
	videoInputHeight	= videoInput.height;
	videoInputPosition	= 0;
#endif
}

void testApp :: initOpticalFieldForCameraInput ()
{
	opticalField.init( camWidth, camHeight );
	opticalField.showDifferenceImage	= true;
	opticalField.opticalFlowScale		= 0.001f;
}

void testApp :: initOpticalFieldForVideoInput()
{
#ifdef USE_VIDEO_INPUT
	opticalField.init( videoInputWidth, videoInputHeight );
	opticalField.showDifferenceImage	= true;
	opticalField.opticalFlowScale		= 0.001f;
#endif
}

void testApp :: initVideoOutput ()
{
#ifdef USE_VIDEO_OUTPUT
	
	int i;
	int j;
	unsigned char * videoPlayerPixels;
	
#ifdef SHOW_DEBUG
	videoPlayer.loadMovie("movies/timebomb_final_320x320.mov");
#else
//	videoPlayer.loadMovie("movies/timebomb_final_720x720.mov");
	videoPlayer.loadMovie("movies/timebomb_final_600x600.mov");
//	videoPlayer.loadMovie("movies/timebomb_final_1200x1200.mov");
#endif
	videoPlayer.setPaused( true );
	
	videoPlayerWidth			= videoPlayer.getWidth();
	videoPlayerHeight			= videoPlayer.getHeight();
	videoPlayerPixelsPerFrame	= videoPlayerWidth * videoPlayerHeight * 3;
	videoPlayerTexturePixels	= new unsigned char[ videoPlayerPixelsPerFrame ];
	
	frameBufferTotal		= videoPlayer.getTotalNumFrames();
	frameBufferPlayIndex	= 0;
	frameBuffer				= new unsigned char[ videoPlayerPixelsPerFrame * frameBufferTotal ];
	
	for( i=0; i<frameBufferTotal; i++ )
	{
		videoPlayer.setPosition( (float)i / (float)( frameBufferTotal - 1 ) );
		videoPlayerPixels = videoPlayer.getPixels();
		
		for( j=0; j<videoPlayerPixelsPerFrame; j++ )
		{
			frameBuffer[ i * videoPlayerPixelsPerFrame + j ] = videoPlayerPixels[ j ];
		}
	}
	
	videoPlayerTexture.allocate( videoPlayerWidth, videoPlayerHeight, GL_RGB );
	
#endif
}

void testApp :: initCameraOutput ()
{
#ifdef USE_CAMERA_OUTPUT
	
//	videoGrabberWidth	= 640;
//	videoGrabberHeight	= 480;

	videoGrabberWidth	= 320;
	videoGrabberHeight	= 240;
	
	videoGrabberPixelsPerFrame = videoGrabberWidth * videoGrabberHeight * 3;
	
	frameBufferLimit	= 255;
	frameBufferIndex	= 0;
	frameBufferCount	= 0;
	
	videoGrabberTexture.allocate( videoGrabberWidth, videoGrabberHeight, GL_RGB );
	
	videoGrabberTexturePixels 	= new unsigned char[ videoGrabberPixelsPerFrame ];
	frameBuffer					= new unsigned char[ videoGrabberPixelsPerFrame * frameBufferLimit ];
	
#endif
}

void testApp :: initTimeDistortionForVideo ()
{
#ifdef USE_VIDEO_OUTPUT
	
	timeDistTexture.allocate( videoPlayerWidth, videoPlayerHeight, GL_RGB );
	timeDistPixels	= new unsigned char[ videoPlayerPixelsPerFrame ];
	
#endif
}

void testApp :: initTimeDistortionForCamera ()
{
#ifdef USE_CAMERA_OUTPUT
	
	timeDistTexture.allocate( videoGrabberWidth, videoGrabberHeight, GL_RGB );
	timeDistPixels	= new unsigned char[ videoGrabberPixelsPerFrame ];
	
#endif
}

void testApp :: initFluid()
{
	fluidSolver.setup(FLUID_WIDTH, FLUID_WIDTH / window.aspectRatio);
	fluidSolver.enableRGB( false ).setFadeSpeed(0.015).setDeltaT(0.5).setVisc(0.00015).setColorDiffusion(0);
	fluidDrawer.setup(&fluidSolver);
	
	fluidColorScale		= 1.0f;
	
	drawFluid			= true;
	renderUsingVA		= true;
}

void testApp :: initFluidForVideo ()
{
#ifdef USE_VIDEO_OUTPUT

	fluidSolver.setup(FLUID_WIDTH, FLUID_WIDTH / window.aspectRatio);
    fluidSolver.enableRGB( false ).setFadeSpeed( 0.005 ).setDeltaT( 0.5 ).setVisc( 0.00007 ).setColorDiffusion( 0.00002 );
	fluidSolver.doVorticityConfinement	= true;
	fluidSolver.solverIterations		= 5;

	fluidDrawer.setRenderDimensions( videoPlayerWidth, videoPlayerHeight );
	fluidDrawer.setup(&fluidSolver);
	
	fluidColorScale		= 0.2f;
	
	drawFluid			= true;
	renderUsingVA		= true;
	
#endif
}

void testApp :: initFluidForCamera ()
{
#ifdef USE_CAMERA_OUTPUT	
	
	fluidSolver.setup(FLUID_WIDTH, FLUID_WIDTH / window.aspectRatio);
    fluidSolver.enableRGB( false ).setFadeSpeed(0.015).setDeltaT(0.5).setVisc(0.00015).setColorDiffusion(0);
	fluidDrawer.setRenderDimensions( videoGrabberWidth, videoGrabberHeight );
	fluidDrawer.setup(&fluidSolver);
	
	fluidColorScale		= 1.0f;	
	
	drawFluid			= true;
	renderUsingVA		= true;
	
#endif
}

void testApp :: initGui ()
{
#ifdef USE_GUI 
	
	gui.addSlider("fs.viscocity", &fluidSolver.viscocity, 0.0, 0.0002, 0.5);
	gui.addSlider("fs.colorDiffusion", &fluidSolver.colorDiffusion, 0.0, 0.0003, 0.5); 
	gui.addSlider("fs.fadeSpeed", &fluidSolver.fadeSpeed, 0.0, 0.1, 0.5); 
	gui.addSlider("fs.solverIterations", &fluidSolver.solverIterations, 1, 20); 
	gui.addSlider("fd.drawMode", &fluidDrawer.drawMode, 0, FLUID_DRAW_MODE_COUNT-1); 
	gui.addToggle("fs.doRGB", &fluidSolver.doRGB); 
	gui.addToggle("fs.doVorticityConfinement", &fluidSolver.doVorticityConfinement); 
	gui.addToggle("drawFluid", &drawFluid); 
	gui.addToggle("renderUsingVA", &renderUsingVA); 

	gui.addSlider("optical floor",		&opticalField.opticalFlowMin,	0.0f, 10.0f, 0.1f );
	gui.addSlider("optical ceil",		&opticalField.opticalFlowMax,	0.0f, 10.0f, 0.5f );
	gui.addSlider("optical scale",		&opticalField.opticalFlowScale, 0.0f, 0.001f, 0.1f );
	gui.addSlider("fluid color scale",	&fluidColorScale,				0.0, 2.0, 0.5 );
	
#endif
}

void testApp :: initVideoSaver()
{
#ifdef USE_VIDEO_SAVER
	videoSaverWidth		= ofGetWidth();
	videoSaverHeight	= ofGetHeight();
	
	char str[255];
	sprintf( str, "tb_%02d%02d%02d_%02d%02d%02d_%dx%d", ofGetYear() % 1000, ofGetMonth(), ofGetDay(), ofGetHours(), ofGetMinutes(), ofGetSeconds(), videoSaverWidth, videoSaverHeight );

	videoSaverPath = str;
	
	videoSaverImage.allocate( videoSaverWidth, videoSaverHeight, GL_RGB );
	
	videoSaver.listCodecs();
	videoSaver.setCodecType( 2 );
	videoSaver.setCodecQualityLevel( OF_QT_SAVER_CODEC_QUALITY_LOSSLESS );
	videoSaver.setup( videoSaverWidth, videoSaverHeight, videoSaverPath );
	
	videoSaverRecording = false;
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// UPDATE.
////////////////////////////////////////////////////////////////////////////////////////////////////

void testApp::update()
{

#ifdef USE_TUIO
	updateTuio();
#endif
	
	updateVideoGrabber();
	
#ifdef USE_VIDEO_INPUT
	updateVideoInput();
#endif
	
#ifdef USE_VIDEO_INPUT
	updateOpticalFieldFromVideoInput();
#else
	updateOpticalFieldFromCameraInput();
#endif
	
	updateFluid();

#ifdef USE_VIDEO_OUTPUT
	updateVideo();
#endif
	
#ifdef USE_CAMERA_OUTPUT
	updateCamera();
#endif
	
#ifdef USE_VIDEO_OUTPUT	
	updateTimeDistortionForVideo();
#endif
#ifdef USE_CAMERA_OUTPUT
	updateTimeDistortionForCamera();
#endif

}

void testApp :: updateTuio ()
{
#ifdef USE_TUIO
	
	tuioClient.getMessage();
	
	// do finger stuff
	list<ofxTuioCursor*>cursorList = tuioClient.getTuioCursors();
	for(list<ofxTuioCursor*>::iterator it=cursorList.begin(); it != cursorList.end(); it++) {
		ofxTuioCursor *tcur = (*it);
        float vx = tcur->getXSpeed() * tuioCursorSpeedMult;
        float vy = tcur->getYSpeed() * tuioCursorSpeedMult;
        if(vx == 0 && vy == 0) {
            vx = ofRandom(-tuioStationaryForce, tuioStationaryForce);
            vy = ofRandom(-tuioStationaryForce, tuioStationaryForce);
        }
        addToFluid(tcur->getX(), tcur->getY(), vx, vy);
    }
	
#endif
}

void testApp :: updateVideoGrabber ()
{
	videoGrabber.grabFrame();
	
	isVideoGrabberNewFrame = videoGrabber.isFrameNew();
}

void testApp :: updateVideoInput()
{
#ifdef USE_VIDEO_INPUT	
	int videoInputTotalFrames	= videoInput.getTotalNumFrames();
	int videoInputFrame			= ofGetFrameNum() % videoInputTotalFrames;
	
	videoInputPosition			= videoInputFrame / (float)( videoInputTotalFrames - 1 );
	
	videoInput.setPosition( videoInputPosition );
#endif
}

void testApp :: updateOpticalFieldFromCameraInput ()
{
	if( isVideoGrabberNewFrame )
	{
		opticalField.update( videoGrabber.getPixels() );
		
		opticalField.getInteractionScale( &interactionScale );
		
		interactionScale *= 100;
		if( interactionScale > 1 )
			interactionScale = 1;
		
		int res = 1;
		int x, y;
		
		int fh = fluidSolver.getHeight();
		int fw = fluidSolver.getWidth();
		
		int ow = opticalField.getWidth();
		int oh = opticalField.getHeight();
		
		for(int j=1; j < fh-1; j++) 
		{
			for(int i=1; i < fw-1; i++) 
			{
				if( i % res == 0 && j % res == 0 )
				{
					float dx = i / (float)fw;
					float dy = j / (float)fh;
					int x = dx * ow;
					int y = dy * oh;
					float ox, oy;
					
					opticalField.getVelAtPixel( x, y, &ox, &oy );
					
					addToFluid( dx, dy, ox, oy, true );
				}
			}
		}
	}
}

void testApp :: updateOpticalFieldFromVideoInput()
{
#ifdef USE_VIDEO_INPUT	
	opticalField.update( videoInput.getPixels() );
	
	opticalField.getInteractionScale( &interactionScale );
	
	interactionScale *= 100;
	if( interactionScale > 1 )
		interactionScale = 1;
	
	int res = 1;
	int x, y;
	
	int fh = fluidSolver.getHeight();
	int fw = fluidSolver.getWidth();
	
	int ow = opticalField.getWidth();
	int oh = opticalField.getHeight();
	
	for(int j=1; j < fh-1; j++) 
	{
		for(int i=1; i < fw-1; i++) 
		{
			if( i % res == 0 && j % res == 0 )
			{
				float dx = i / (float)fw;
				float dy = j / (float)fh;
				int x = dx * ow;
				int y = dy * oh;
				float ox, oy;
				
				opticalField.getVelAtPixel( x, y, &ox, &oy );
				
				addToFluid( dx, dy, ox, oy, true );
			}
		}
	}
#endif
}

void testApp :: updateFluid ()
{
	fluidSolver.update();
	fluidDrawer.update();
	
	pmouseX = mouseX;
	pmouseY = mouseY;
}

void testApp :: updateVideo ()
{
#ifdef USE_VIDEO_OUTPUT
	
	for( int i=0; i<videoPlayerPixelsPerFrame; i++ )
	{
		videoPlayerTexturePixels[ i ] = frameBuffer[ frameBufferPlayIndex * videoPlayerPixelsPerFrame + i ];
	}
	
	videoPlayerTexture.loadData( videoPlayerTexturePixels, videoPlayerWidth, videoPlayerHeight, GL_RGB );
	
	++frameBufferPlayIndex;
	
	if( frameBufferPlayIndex > frameBufferTotal - 1 )
	{
		frameBufferPlayIndex = 0;
	}
	
#endif
}

void testApp :: updateCamera ()
{
#ifdef USE_CAMERA_OUTPUT
	
	if( isVideoGrabberNewFrame )
	{
		unsigned char *vidPixels = videoGrabber.getPixels();
		
		if( frameBufferCount < frameBufferLimit - 1 )	// add frames to frameBuffer until the buffer is full.
		{
			++frameBufferCount;
			++frameBufferIndex;
			
			for( int i=0; i<videoGrabberPixelsPerFrame; i++ ) 
			{
				frameBuffer[ ( frameBufferIndex * videoGrabberPixelsPerFrame ) + i ] = vidPixels[ i ];
				
				videoGrabberTexturePixels[ i ] = vidPixels[ i ];
			}
			
			videoGrabberTexture.loadData( videoGrabberTexturePixels, videoGrabberWidth, videoGrabberHeight, GL_RGB );			
		}
		else
		{
			if( ++frameBufferIndex > frameBufferLimit - 1 )
			{
				frameBufferIndex = 0;
			}
			
			for( int i=0; i<videoGrabberPixelsPerFrame; i++ )	// add new frame to buffer at frame buffer index.
			{
				frameBuffer[ ( frameBufferIndex * videoGrabberPixelsPerFrame ) + i ] = vidPixels[ i ];
				
				videoGrabberTexturePixels[ i ] = vidPixels[ i ];
			}
			
			videoGrabberTexture.loadData( videoGrabberTexturePixels, videoGrabberWidth, videoGrabberHeight, GL_RGB );
		}
	}
	
#endif
}

void testApp :: updateTimeDistortionForVideo ()
{
#ifdef USE_VIDEO_OUTPUT
	
	unsigned char *fluidPixels = fluidDrawer.getFluidPixels();
	
	for( int i=0; i<videoPlayerPixelsPerFrame; i+=3 )
	{
//		float p = fluidPixels[ i ] / 255.0f;
		float p = 1.0f - fluidPixels[ i ] / 255.0f;
		int j	= (int)( p * ( frameBufferTotal - 1 ) );
		
		timeDistPixels[ i + 0 ] = frameBuffer[ j * videoPlayerPixelsPerFrame + i + 0 ];
		timeDistPixels[ i + 1 ] = frameBuffer[ j * videoPlayerPixelsPerFrame + i + 1 ];
		timeDistPixels[ i + 2 ] = frameBuffer[ j * videoPlayerPixelsPerFrame + i + 2 ];
	}
	
	timeDistTexture.loadData( timeDistPixels, videoPlayerWidth, videoPlayerHeight, GL_RGB );
	
#endif
}

void testApp :: updateTimeDistortionForCamera ()
{
#ifdef USE_CAMERA_OUTPUT
	
	unsigned char *fluidPixels = fluidDrawer.getFluidPixels();
	
	for( int i=0; i<videoGrabberPixelsPerFrame; i+=3 )
	{
		float p = fluidPixels[ i ] / 255.0f;
		int j	= (int)( p * frameBufferCount );
		int k	= frameBufferIndex - j;
		if( k < 0 )
		{
			k += frameBufferLimit - 1;
		}
		
		timeDistPixels[ i + 0 ] = frameBuffer[ k * videoGrabberPixelsPerFrame + i + 0 ];
		timeDistPixels[ i + 1 ] = frameBuffer[ k * videoGrabberPixelsPerFrame + i + 1 ];
		timeDistPixels[ i + 2 ] = frameBuffer[ k * videoGrabberPixelsPerFrame + i + 2 ];
	}
	
	timeDistTexture.loadData( timeDistPixels, videoGrabberWidth, videoGrabberHeight, GL_RGB );
	
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// DRAW.
////////////////////////////////////////////////////////////////////////////////////////////////////

void testApp::draw()
{

#ifdef SHOW_DEBUG
	
	ofSetBackgroundAuto(drawFluid);
	
	#ifdef USE_VIDEO_OUTPUT
		drawFluidToVideoDimensions();
	#endif
	
	#ifdef USE_CAMERA_OUTPUT
		drawFluidToCameraDimensions();
	#endif
	
	#ifdef USE_GUI 
		gui.draw();
	#endif

	#ifdef USE_VIDEO_OUTPUT
		drawVideoSource();
	#endif
	
	#ifdef USE_CAMERA_OUTPUT
		drawCameraSource();
	#endif
	
	drawCameraSourceForOpticalField();
	
	#ifdef USE_VIDEO_OUTPUT	
		drawTimeDistortionFromVideoSource();
	#endif
	
	#ifdef USE_CAMERA_OUTPUT
		drawTimeDistortionFromCameraSource();
	#endif
	
#else
	
	#ifdef USE_VIDEO_OUTPUT
		drawTimeDistortionFromVideoSourceFullScreen();
	#endif
	
	#ifdef USE_CAMERA_OUTPUT
		drawTimeDistortionFromCameraSourceFullScreen();
	#endif
	
#endif
	
#ifdef USE_VIDEO_SAVER
	drawToVideoSaver();
#endif
	
	drawDebugInfo();
}

void testApp :: drawFluidFullScreen ()
{
	fluidDrawer.draw( 0, 0, window.width, window.height );
}

void testApp :: drawFluidToVideoDimensions ()
{
#ifdef USE_VIDEO_OUTPUT
	
	if( drawFluid )
	{
		glColor3f( 1, 1, 1 );
		glPushMatrix();
		glTranslatef( 270, 74 + videoPlayerHeight + 10, 0 );
		fluidDrawer.draw( 0, 0, videoPlayerWidth, videoPlayerHeight );
		glPopMatrix();
	}
	
#endif
}

void testApp :: drawFluidToCameraDimensions ()
{
#ifdef USE_CAMERA_OUTPUT	
	
	if( drawFluid )
	{
		glColor3f( 1, 1, 1 );
		glPushMatrix();
		glTranslatef( 270, 74 + videoGrabberHeight + 10, 0 );
		fluidDrawer.draw( 0, 0, videoGrabberWidth, videoGrabberHeight );
		glPopMatrix();
	}
	
#endif
}

void testApp :: drawVideoSource ()
{
#ifdef USE_VIDEO_OUTPUT
	
	glColor3f( 1, 1, 1 );
	glPushMatrix();
	glTranslatef( 270 + camWidth + 10, 74, 0 );
	videoPlayerTexture.draw( 0, 0 );
	glPopMatrix();
	
#endif
}

void testApp :: drawCameraSource ()
{
#ifdef USE_CAMERA_OUTPUT	
	
	glColor3f( 1, 1, 1 );
	glPushMatrix();
	glTranslatef( 270 + videoGrabberWidth + 10, 74, 0 );
	videoGrabberTexture.draw( 0, 0 );
	glPopMatrix();
	
#endif
}

void testApp :: drawCameraSourceForOpticalField()
{
	glColor3f( 1, 1, 1 );
	glPushMatrix();
	glTranslatef( 270 + videoPlayerWidth + 320 + 20, 74 + videoPlayerHeight - 240, 0 );
	opticalField.drawCurrentColorImage();
	glPopMatrix();

	glColor3f( 1, 1, 1 );
	glPushMatrix();
	glTranslatef( 270 + videoPlayerWidth + 320 + 20, 74 + videoPlayerHeight + 10, 0 );
	opticalField.drawDifference();
	glPopMatrix();

	glColor3f( 1, 1, 1 );
	glPushMatrix();
	glTranslatef( 270 + videoPlayerWidth, 74 + videoPlayerHeight + 10, 0 );
	opticalField.drawOpticalFlow();
	glPopMatrix();
}

void testApp :: drawTimeDistortionFromVideoSource ()
{
#ifdef USE_VIDEO_OUTPUT
	
	glColor3f( 1, 1, 1 );
	glPushMatrix();
	glTranslatef( 270, 74, 0 );
	timeDistTexture.draw( 0, 0 );
	glPopMatrix();
	
#endif
}

void testApp :: drawTimeDistortionFromVideoSourceFullScreen ()
{
#ifdef USE_VIDEO_OUTPUT	
	
	float wRatio, hRatio, scale;
	float scaleX;
	
	bool scaleVideo				= false;
	bool scaleToFitFullScreen	= false;
	
	wRatio = ofGetWidth() / (float)videoPlayerWidth;
	hRatio = ofGetHeight() / (float)videoPlayerHeight;
	
	scaleX = 0.05;
	
	if( scaleVideo )
	{
		if( scaleToFitFullScreen )
			scale = MIN( wRatio, hRatio );
		else
			scale = MAX( wRatio, hRatio );
	}
	else
	{
		scale = 1;
	}
	
	glColor3f( 1, 1, 1 );
	glPushMatrix();
	glTranslatef
	( 
		(int)( ( ofGetWidth() - ( videoPlayerWidth * ( scale + scaleX ) ) ) * 0.5f ),
		(int)( ( ofGetHeight() - ( videoPlayerHeight * scale ) ) * 0.5f ),
		0
	);
	glScalef( scale + scaleX, scale, 0 );
	timeDistTexture.draw( 0, 0 );
	glPopMatrix();
	
#endif
}

void testApp :: drawTimeDistortionFromCameraSource ()
{
#ifdef USE_CAMERA_OUTPUT		
	
	glColor3f( 1, 1, 1 );
	glPushMatrix();
	glTranslatef( 270, 74, 0 );
	timeDistTexture.draw( 0, 0 );
	glPopMatrix();
	
#endif
}

void testApp :: drawTimeDistortionFromCameraSourceFullScreen ()
{
#ifdef USE_CAMERA_OUTPUT	
	
	float wRatio, hRatio, scale;
	
	bool scaleToFitFullScreen = false;
	
	wRatio = ofGetWidth() / (float)videoGrabberWidth;
	hRatio = ofGetHeight() / (float)videoGrabberHeight;
	
	if( scaleToFitFullScreen )
		scale = MIN( wRatio, hRatio );
	else
		scale = MAX( wRatio, hRatio );
	
	glColor3f( 1, 1, 1 );
	glPushMatrix();
	glTranslatef
	( 
		(int)( ( ofGetWidth() - ( videoGrabberWidth * scale ) ) * 0.5f ),
		(int)( ( ofGetHeight() - ( videoGrabberHeight * scale ) ) * 0.5f ),
		0
	);
	glScalef( scale, scale, 0 );
	timeDistTexture.draw( 0, 0 );
	glPopMatrix();
	
#endif
}

void testApp :: drawToVideoSaver()
{
#ifdef USE_VIDEO_SAVER
	if( videoSaverRecording )
	{
		videoSaverImage.grabScreen( 0, 0, videoSaverWidth, videoSaverHeight );
		videoSaver.addFrame( videoSaverImage.getPixels(), 1.0f / 30.0f ); 
	}
#endif
}

void testApp :: drawDebugInfo()
{
	ofSetColor( 0xFFFFFF );
	ofDrawBitmapString
	(
		"fps :: " + ofToString(ofGetFrameRate(), 2) + "\n\n"
#ifdef USE_VIDEO_SAVER	 
		+ "videoSaverRecording :: " + ofToString( videoSaverRecording ) + "\n\n"
#endif
#ifdef USE_VIDEO_INPUT
		+ "videoInputPosition :: " + ofToString( videoInputPosition ) + "\n\n"
#endif
		,
		20,
		20
	);
}

////////////////////////////////////////////////////////
// RESIZE.
////////////////////////////////////////////////////////

void testApp::windowResized(int w, int h)
{
	printf("TEST windowResized(%i, %i)\n", w, h);
	window.width		= w;
	window.height		= h;
	
	window.invWidth		= 1.0f/window.width;
	window.invHeight	= 1.0f/window.height;
	window.aspectRatio	= window.width * window.invHeight;
	window.aspectRatio2 = window.aspectRatio * window.aspectRatio;
}


#pragma mark Input callbacks

////////////////////////////////////////////////////////
// KEY HANDLER.
////////////////////////////////////////////////////////

void testApp::keyPressed  (int key)
{
    switch(key)
	{
		case ' ' :
			videoGrabber.videoSettings();
			break;

#ifdef USE_GUI
		case 'g':
			gui.toggleDraw();	
			glClear(GL_COLOR_BUFFER_BIT);
			break; 
#endif			
		case 'f':
			ofToggleFullscreen();
			break;
		case 's':
			static char fileNameStr[255];
			sprintf(fileNameStr, "output_%0.4i.png", ofGetFrameNum());
			static ofImage imgScreen;
			imgScreen.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
			printf("Saving file: %s\n", fileNameStr);
			imgScreen.saveImage(fileNameStr);
			break;
			
    }
	
#ifdef USE_VIDEO_SAVER
	if( key == 'r' )
	{
		videoSaverRecording = !videoSaverRecording;
		
		if( !videoSaverRecording )
		{
			videoSaver.finishMovie();
		}
	}
#endif
}


////////////////////////////////////////////////////////
// MOUSE HANDLER.
////////////////////////////////////////////////////////

void testApp::mouseMoved(int x, int y )
{
	float mouseNormX = x * window.invWidth;
    float mouseNormY = y * window.invHeight;
    float mouseVelX = (x - pmouseX) * window.invWidth;
    float mouseVelY = (y - pmouseY) * window.invHeight;

    addToFluid(mouseNormX, mouseNormY, mouseVelX, mouseVelY, true);
}

void testApp::mouseDragged(int x, int y, int button)
{
	float mouseNormX = x * window.invWidth;
    float mouseNormY = y * window.invHeight;
    float mouseVelX = (x - pmouseX) * window.invWidth;
    float mouseVelY = (y - pmouseY) * window.invHeight;
	
	addToFluid(mouseNormX, mouseNormY, mouseVelX, mouseVelY, false);
}

