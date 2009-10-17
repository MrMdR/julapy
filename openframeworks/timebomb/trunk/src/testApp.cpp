#include "testApp.h"

void testApp :: fadeToColor(float r, float g, float b, float speed) 
{
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(r, g, b, speed);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f( renderArea.width, 0);
    glVertex2f( renderArea.width, renderArea.height );
    glVertex2f( 0, renderArea.height);
    glEnd();
}


void testApp::addToFluid(float x, float y, float dx, float dy, bool addColor, bool addForce)
{
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
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// SETUP.
////////////////////////////////////////////////////////////////////////////////////////////////////

void testApp::setup() 
{	 
	ofBackground( 0, 0, 0 );
//	ofSetVerticalSync( true );
	ofSetFrameRate( 25 );
	
	initRenderArea();
	initVideoGrabber();

#ifdef USE_VIDEO_INPUT
	initVideoInput();
#endif

#ifdef USE_VIDEO_INPUT
	initOpticalFieldForVideoInput();
#else
	initOpticalFieldForCameraInput();
#endif
	
	initVideoOutput();
	initTimeDistortionForVideo();
	initFluidForVideo();
	initGui();

	screenGrabUtil.setup( "movie/timebomb", &renderArea );
	
	bDebug			= true;
	bRightMonitor	= false;
}

void testApp :: initRenderArea()
{
	renderAreaWindow.x      = 0;
	renderAreaWindow.y      = 0;
	renderAreaWindow.width  = 1280;
	renderAreaWindow.height = 720;
	
	renderAreaFullScreen.x      = 0;
	renderAreaFullScreen.y      = 0;
	renderAreaFullScreen.width  = 1440;
	renderAreaFullScreen.height = 900;
	
	renderAreaRightMonitor.x		= 1440;
	renderAreaRightMonitor.y		= 0;
	renderAreaRightMonitor.width	= 800;
	renderAreaRightMonitor.height	= 600;
	
	renderArea.x		= renderAreaWindow.x;
	renderArea.y		= renderAreaWindow.y;
	renderArea.width	= renderAreaWindow.width;
	renderArea.height	= renderAreaWindow.height;
}

void testApp :: initVideoGrabber ()
{
	camWidth		= 640;
	camHeight		= 480;
	
	camWidthHalf	= 320;
	camHeightHalf	= 240;
	
#ifdef USE_POINT_GREY_CAMERA
	bool result = videoGrabber.initGrabber( camWidth, camHeight, VID_FORMAT_GREYSCALE, VID_FORMAT_GREYSCALE, 30, true, new Libdc1394Grabber );
	
	if( result )
	{
		ofLog(OF_LOG_NOTICE,"Camera succesfully initialized.");
	} 
	else
	{
		ofLog(OF_LOG_FATAL_ERROR,"Camera failed to initialize.");
	}
#else
	videoGrabber.initGrabber( camWidth, camHeight );
#endif
	
	videoGrabberSrcImage.allocate( 320, 240 );
	videoGrabberDstImage.allocate( 320, 320 );
	
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
	opticalField.setMirror( false, false );
	opticalField.showDifferenceImage	= true;
	opticalField.opticalFlowScale		= 0.001f;

#ifdef USE_POINT_GREY_CAMERA
	opticalField.setImageType( GL_LUMINANCE );
#else
	opticalField.setImageType( GL_RGB );
#endif
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
	int i;
	int j;
	unsigned char * videoPlayerPixels;
	
	videoPlayer.loadMovie("movies/timebomb_final_320x320.mov");
//	videoPlayer.loadMovie("movies/timebomb_final_600x600.mov");
//	videoPlayer.loadMovie("movies/timebomb_final_720x720.mov");
//	videoPlayer.loadMovie("movies/timebomb_final_1200x1200.mov");

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
}

void testApp :: initTimeDistortionForVideo ()
{
	timeDistTexture.allocate( videoPlayerWidth, videoPlayerHeight, GL_RGB );
	timeDistPixels	= new unsigned char[ videoPlayerPixelsPerFrame ];
}

void testApp :: initFluidForVideo ()
{
	fluidSolver.setup( FLUID_WIDTH, FLUID_WIDTH );		// square so it matches the timebomb source movie.
    fluidSolver.enableRGB( false ).setFadeSpeed( 0.005 ).setDeltaT( 0.5 ).setVisc( 0.00007 ).setColorDiffusion( 0.00002 );
	fluidSolver.doVorticityConfinement	= true;
	fluidSolver.solverIterations		= 5;

	fluidDrawer.setRenderDimensions( videoPlayerWidth, videoPlayerHeight );
	fluidDrawer.setup(&fluidSolver);
	
	fluidColorScale		= 0.2f;
}

void testApp :: initGui ()
{
	gui.addSlider( "fs.viscocity",				&fluidSolver.viscocity,			0.0, 0.0002, 0.5 );
	gui.addSlider( "fs.colorDiffusion",			&fluidSolver.colorDiffusion,	0.0, 0.0003, 0.5 ); 
	gui.addSlider( "fs.fadeSpeed",				&fluidSolver.fadeSpeed,			0.0, 0.1000, 0.5 ); 
	gui.addSlider( "fs.solverIterations",		&fluidSolver.solverIterations,	1, 20 ); 
	gui.addToggle( "fs.doRGB",					&fluidSolver.doRGB );
	gui.addToggle( "fs.doVorticityConfinement", &fluidSolver.doVorticityConfinement );
	gui.addSlider( "fluid color scale",			&fluidColorScale,				0.0, 2.0, 0.5 );

	gui.addPage( "optical flow" );
	
	gui.addSlider( "optical floor",		&opticalField.opticalFlowMin,	0.0f, 10.0f, 0.1f );
	gui.addSlider( "optical ceil",		&opticalField.opticalFlowMax,	0.0f, 10.0f, 0.5f );
	gui.addSlider( "optical scale",		&opticalField.opticalFlowScale, 0.0f, 0.001f, 0.1f );
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// UPDATE.
////////////////////////////////////////////////////////////////////////////////////////////////////

void testApp::update()
{
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
	updateVideo();
	updateTimeDistortionForVideo();
}

void testApp :: updateVideoGrabber ()
{
#ifdef USE_POINT_GREY_CAMERA
	videoGrabber.update();
#else
	videoGrabber.grabFrame();
#endif
	
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
}

void testApp :: updateVideo ()
{
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
}

void testApp :: updateTimeDistortionForVideo ()
{
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
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// DRAW.
////////////////////////////////////////////////////////////////////////////////////////////////////

void testApp::draw()
{
	ofBackground( 20, 20, 20 );
	
	if( bDebug )
	{
		gui.draw();
		
		glPushMatrix();
		glTranslatef( 270, 74, 0 );
		
		drawTimeDistortionFromVideoSource();
		drawFluidToVideoDimensions();
		drawCameraSourceForOpticalField();
		
		glPopMatrix();
	}	
	else
	{
		drawTimeDistortionFromVideoSourceFullScreen();
	}
	
	if( screenGrabUtil.isRecording() )
		screenGrabUtil.save();
	
	glBlendFunc( GL_ONE, GL_ZERO );
	
	drawDebugInfo();
}

void testApp :: drawVideoSource ()
{
	glColor3f( 1, 1, 1 );
	glPushMatrix();
	glTranslatef( 270 + camWidthHalf + 10, 74, 0 );
	videoPlayerTexture.draw( 0, 0, 320, 320 );
	glPopMatrix();
}

void testApp :: drawFluidToVideoDimensions ()
{
	glColor3f( 1, 1, 1 );
	glPushMatrix();
	glTranslatef( 330 + 10, 0, 0 );
	fluidDrawer.draw( 0, 0, 320, 320 );
	glPopMatrix();
}

void testApp :: drawCameraSourceForOpticalField()
{
	glColor3f( 1, 1, 1 );
	glPushMatrix();
	glTranslatef( 0, 330 + 10, 0 );
	opticalField.drawCurrentColorImage();
	glPopMatrix();

	glColor3f( 1, 1, 1 );
	glPushMatrix();
	glTranslatef( 330 + 10, 330 + 10, 0 );
	opticalField.drawDifference();
	glPopMatrix();

	glColor3f( 1, 1, 1 );
	glPushMatrix();
	glTranslatef( ( 330 + 10 ) * 2, 330 + 10, 0 );
	opticalField.drawOpticalFlow();
	glPopMatrix();
}

void testApp :: drawTimeDistortionFromVideoSource ()
{
	glColor3f( 1, 1, 1 );
	glPushMatrix();
	glTranslatef( 0, 0, 0 );
	timeDistTexture.draw( 0, 0, 320, 320 );
	glPopMatrix();
}

void testApp :: drawTimeDistortionFromVideoSourceFullScreen ()
{
	float wRatio, hRatio, scale;
	float scaleX, scaleY;
	
	bool scaleVideo				= false;
	bool scaleToFitFullScreen	= false;
	
	wRatio = renderArea.width  / (float)videoPlayerWidth;
	hRatio = renderArea.height / (float)videoPlayerHeight;
	
	scaleX = 0.00;		// stretch image if need be.
	scaleY = 0.00;
	
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
		(int)( ( renderArea.width  - ( videoPlayerWidth  * ( scale + scaleX ) ) ) * 0.5f ) + renderArea.x,
		(int)( ( renderArea.height - ( videoPlayerHeight * ( scale + scaleY ) ) ) * 0.5f ) + renderArea.y,
		0
	);
	glScalef( scale + scaleX, scale + scaleY, 0 );
	timeDistTexture.draw( 0, 0 );
	glPopMatrix();
}

void testApp :: drawDebugInfo()
{
	glColor3f( 1, 1, 1 );
	ofDrawBitmapString
	(
//		"fps :: " + ofToString(ofGetFrameRate(), 2) + "\n\n",
		"",
		20,
		20
	);
}

////////////////////////////////////////////////////////
// TOGGLE FULL SCREEN.
////////////////////////////////////////////////////////

void testApp :: updateRenderArea()
{
	if( ofGetWindowMode() == OF_WINDOW )
	{
		renderArea.x		= renderAreaWindow.x;
		renderArea.y		= renderAreaWindow.y;
		renderArea.width	= renderAreaWindow.width;
		renderArea.height	= renderAreaWindow.height;
	}
	
	if( ofGetWindowMode() == OF_FULLSCREEN )
	{
		if( bRightMonitor )
		{
			renderArea.x		= renderAreaRightMonitor.x;
			renderArea.y		= renderAreaRightMonitor.y;
			renderArea.width	= renderAreaRightMonitor.width;
			renderArea.height	= renderAreaRightMonitor.height;
		}
		else
		{
			renderArea.x		= renderAreaFullScreen.x;
			renderArea.y		= renderAreaFullScreen.y;
			renderArea.width	= renderAreaFullScreen.width;
			renderArea.height	= renderAreaFullScreen.height;
		}
	}
	
	screenGrabUtil.setArea( &renderArea );
}

////////////////////////////////////////////////////////
// KEY HANDLER.
////////////////////////////////////////////////////////

void testApp::keyPressed  (int key)
{
	if( key == ' ' )
		videoGrabber.videoSettings();
	
	if( key == 'f' )
	{
		ofToggleFullscreen();
		updateRenderArea();
	}
	
	if( key == 'm' )
	{
		if( screenGrabUtil.isRecording() )
			screenGrabUtil.stop();
		else
			screenGrabUtil.start();
	}
	
	if( key == 'd' )
	{
		bDebug = !bDebug;
	}
	
	if( key == 'r' )
	{
		bRightMonitor = !bRightMonitor;
		
		updateRenderArea();
	}
}

////////////////////////////////////////////////////////
// MOUSE HANDLER.
////////////////////////////////////////////////////////

void testApp::mouseMoved(int x, int y )
{
	//
}

void testApp::mouseDragged(int x, int y, int button)
{
	//
}

