#include "testApp.h"
#include "Particle.h"
#include "msaColor.h"
#include "ParticleSystem.h"


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
	float speed = dx * dx  + dy * dy * window.aspectRatio2;    // balance the x and y components of speed with the screen aspect ratio
	
	if(speed > 0)
	{
		if(x<0) x = 0; 
		else if(x>1) x = 1;
		if(y<0) y = 0; 
		else if(y>1) y = 1;
		
//		float colorMult = 50;
		float velocityMult = 30;
		
        int index = fluidSolver.getIndexForNormalizedPosition(x, y);
		
		if(addColor)
		{
			fluidSolver.r[index]  += fluidColorScale;
			fluidSolver.g[index]  += fluidColorScale;
			fluidSolver.b[index]  += fluidColorScale;
			
			if(drawParticles)
			{
				particleSystem.addParticles(x * window.width, y * window.height, 10);
			}
		}
		
		if(addForce)
		{
			fluidSolver.u[index] += dx * velocityMult;
			fluidSolver.v[index] += dy * velocityMult;
		}
		
		if(!drawFluid && ofGetFrameNum()%5 ==0) 
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
	
#ifdef USE_OPTICAL_FIELD
	initOpticalField();
#endif
	
#ifdef USE_VIDEO
	initVideo();
#endif
	
#ifdef USE_CAMERA
	initCamera();
#endif
	
#ifdef USE_TIME_DISTORTION
	#ifdef USE_VIDEO
		initTimeDistortionForVideo();
	#endif
	#ifdef USE_CAMERA
		initTimeDistortionForCamera();
	#endif
#endif
	
#ifdef USE_VIDEO	
	initFluidForVideo();
#endif
	
#ifdef USE_CAMERA
	initFluidForCamera();
#endif
	
#ifdef USE_TUIO
	tuioClient.start( 3333 );
#endif

#ifdef USE_GUI 
	initGui();
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

void testApp :: initOpticalField ()
{
#ifdef USE_OPTICAL_FIELD

	opticalField.init( camWidth, camHeight );
	opticalField.opticalFlowScale	= 0.001f;
	
#endif
}

void testApp :: initVideo ()
{
#ifdef USE_VIDEO
	
	int i;
	int j;
	unsigned char * videoPlayerPixels;
	
//	videoPlayer.loadMovie("movies/decompose_450x248.mov");
	videoPlayer.loadMovie("movies/timebomb_final_320x320.mov");
//	videoPlayer.loadMovie("movies/timebomb_final_600x600.mov");
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
	
#endif
}

void testApp :: initCamera ()
{
#ifdef USE_CAMERA
	
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
#ifdef USE_TIME_DISTORTION
#ifdef USE_VIDEO
	
	timeDistTexture.allocate( videoPlayerWidth, videoPlayerHeight, GL_RGB );
	timeDistPixels	= new unsigned char[ videoPlayerPixelsPerFrame ];
	
#endif
#endif
}

void testApp :: initTimeDistortionForCamera ()
{
#ifdef USE_TIME_DISTORTION
#ifdef USE_CAMERA
	
	timeDistTexture.allocate( videoGrabberWidth, videoGrabberHeight, GL_RGB );
	timeDistPixels	= new unsigned char[ videoGrabberPixelsPerFrame ];
	
#endif
#endif
}

void testApp :: initFluid()
{
	fluidSolver.setup(FLUID_WIDTH, FLUID_WIDTH / window.aspectRatio);
	fluidSolver.enableRGB( false ).setFadeSpeed(0.015).setDeltaT(0.5).setVisc(0.00015).setColorDiffusion(0);
	fluidDrawer.setup(&fluidSolver);
	
	fluidColorScale		= 1.0f;
	
	drawFluid			= true;
	drawParticles		= false;
	renderUsingVA		= true;
}

void testApp :: initFluidForVideo ()
{
#ifdef USE_VIDEO

	fluidSolver.setup(FLUID_WIDTH, FLUID_WIDTH / window.aspectRatio);
    fluidSolver.enableRGB( false ).setFadeSpeed( 0.0042 ).setDeltaT( 0.5 ).setVisc( 0.00007 ).setColorDiffusion( 0.00004 );
	fluidSolver.doVorticityConfinement	= true;
	fluidSolver.solverIterations		= 5;

	fluidDrawer.setRenderDimensions( videoPlayerWidth, videoPlayerHeight );
	fluidDrawer.setup(&fluidSolver);
	
	fluidColorScale		= 1.0f;
	
	drawFluid			= true;
	drawParticles		= false;
	renderUsingVA		= true;
	
#endif
}

void testApp :: initFluidForCamera ()
{
#ifdef USE_CAMERA	
	
	fluidSolver.setup(FLUID_WIDTH, FLUID_WIDTH / window.aspectRatio);
    fluidSolver.enableRGB( false ).setFadeSpeed(0.015).setDeltaT(0.5).setVisc(0.00015).setColorDiffusion(0);
	fluidDrawer.setRenderDimensions( videoGrabberWidth, videoGrabberHeight );
	fluidDrawer.setup(&fluidSolver);
	
	fluidColorScale		= 1.0f;	
	
	drawFluid			= true;
	drawParticles		= false;
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
	gui.addToggle("drawParticles", &drawParticles); 
	gui.addToggle("renderUsingVA", &renderUsingVA); 

	gui.addSlider("optical floor",		&opticalField.opticalFlowMin,	0.0f, 10.0f, 0.1f );
	gui.addSlider("optical ceil",		&opticalField.opticalFlowMax,	0.0f, 10.0f, 0.5f );
	gui.addSlider("optical scale",		&opticalField.opticalFlowScale, 0.0f, 0.001f, 0.1f );
	gui.addSlider("fluid color scale",	&fluidColorScale,				0.0, 2.0, 0.5 );
	
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
	
#ifdef USE_OPTICAL_FIELD
	updateOpticalField();
#endif
	
	updateFluid();

#ifdef USE_VIDEO
	updateVideo();
#endif
	
#ifdef USE_CAMERA
	updateCamera();
#endif
	
#ifdef USE_TIME_DISTORTION
	#ifdef USE_VIDEO	
		updateTimeDistortionForVideo();
	#endif
	#ifdef USE_CAMERA
		updateTimeDistortionForCamera();
	#endif
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

void testApp :: updateOpticalField ()
{
#ifdef USE_OPTICAL_FIELD
	
	if( isVideoGrabberNewFrame )
	{
		opticalField.update( videoGrabber.getPixels() );
		
//		float os = 0.005f;
//		float ot = 0.01f;
//		float cap = 0.2f;
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
				
//		for( y=0; y<camHeight; y++ )
//		{
//			for( x=0; x<camWidth; x++ )
//			{
//				if( x % res == 0 && y % res == 0 )
//				{
//					float dx = x / (float)camWidth;
//					float dy = y / (float)camHeight;
//					float ox, oy;
//					
//					opticalField.getVelAtPixel( x, y, &ox, &oy );
//					
//					ox *= os;
//					oy *= os;
//					
//					if( ox < ot && ox > -ot )
//						ox = 0;
//					
//					if( oy < ot && oy > -ot )
//						oy = 0;
//					
//					ox = MIN( cap, ox );
//					oy = MIN( cap, oy );
//					
//					addToFluid( dx, dy, ox, oy, true );
//				}
//			}
//		}
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
#ifdef USE_VIDEO
	
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
#ifdef USE_CAMERA
	
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
#ifdef USE_TIME_DISTORTION
#ifdef USE_VIDEO
	
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
#endif
}

void testApp :: updateTimeDistortionForCamera ()
{
#ifdef USE_TIME_DISTORTION
#ifdef USE_CAMERA
	
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
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// DRAW.
////////////////////////////////////////////////////////////////////////////////////////////////////

void testApp::draw()
{

#ifdef SHOW_DEBUG
	
	ofSetBackgroundAuto(drawFluid);
	
	#ifdef USE_VIDEO
		drawFluidToVideoDimensions();
	#endif
	
	#ifdef USE_CAMERA
		drawFluidToCameraDimensions();
	#endif
	
	if( drawParticles )
	{
		particleSystem.updateAndDraw();
	}

	#ifdef USE_GUI 
		gui.draw();
	#endif

	#ifdef USE_VIDEO
		drawVideoSource();
	#endif
	
	#ifdef USE_CAMERA
		drawCameraSource();
	#endif
	
	#ifdef USE_OPTICAL_FIELD
		drawCameraSourceForOpticalField();
	#endif
	
	#ifdef USE_TIME_DISTORTION
	
		#ifdef USE_VIDEO	
			drawTimeDistortionFromVideoSource();
		#endif
	
		#ifdef USE_CAMERA
			drawTimeDistortionFromCameraSource();
		#endif
	
	#endif
	
#else
	
	#ifdef USE_TIME_DISTORTION
	
		#ifdef USE_VIDEO
			drawTimeDistortionFromVideoSourceFullScreen();
		#endif
	
		#ifdef USE_CAMERA
			drawTimeDistortionFromCameraSourceFullScreen();
		#endif
	
	#endif
	
#endif
	
}

void testApp :: drawFluidFullScreen ()
{
	fluidDrawer.draw( 0, 0, window.width, window.height );
}

void testApp :: drawFluidToVideoDimensions ()
{
#ifdef USE_VIDEO
	
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
#ifdef USE_CAMERA	
	
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
#ifdef USE_VIDEO
	
	glColor3f( 1, 1, 1 );
	glPushMatrix();
	glTranslatef( 270, 74, 0 );
	videoPlayerTexture.draw( 0, 0 );
	glPopMatrix();
	
#endif
}

void testApp :: drawCameraSource ()
{
#ifdef USE_CAMERA	
	
	glColor3f( 1, 1, 1 );
	glPushMatrix();
	glTranslatef( 270, 74, 0 );
	videoGrabberTexture.draw( 0, 0 );
	glPopMatrix();
	
#endif
}

void testApp :: drawCameraSourceForOpticalField()
{
#ifdef USE_OPTICAL_FIELD
	
	glColor3f( 1, 1, 1 );
	glPushMatrix();
	glTranslatef( 270 + videoPlayerWidth + 10, 74 + videoPlayerHeight + 10, 0 );
	opticalField.drawCurrentColorImage();
	glPopMatrix();

	glPushMatrix();
	glTranslatef( 270 + videoPlayerWidth + 320 + 20, 74 + videoPlayerHeight + 10, 0 );
	opticalField.drawDifference();
	glPopMatrix();
	
#endif
}

void testApp :: drawTimeDistortionFromVideoSource ()
{
#ifdef USE_VIDEO
	
	glColor3f( 1, 1, 1 );
	glPushMatrix();
	glTranslatef( 270 + videoPlayerWidth + 10, 74, 0 );
	timeDistTexture.draw( 0, 0 );
	glPopMatrix();
	
#endif
}

void testApp :: drawTimeDistortionFromVideoSourceFullScreen ()
{
#ifdef USE_VIDEO	
	
	float wRatio, hRatio, scale;
	
	bool scaleVideo				= false;
	bool scaleToFitFullScreen	= true;
	
	wRatio = ofGetWidth() / (float)videoPlayerWidth;
	hRatio = ofGetHeight() / (float)videoPlayerHeight;
	
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
		(int)( ( ofGetWidth() - ( videoPlayerWidth * scale ) ) * 0.5f ),
		(int)( ( ofGetHeight() - ( videoPlayerHeight * scale ) ) * 0.5f ),
		0
	);
	glScalef( scale, scale, 0 );
	timeDistTexture.draw( 0, 0 );
	glPopMatrix();
	
#endif
}

void testApp :: drawTimeDistortionFromCameraSource ()
{
#ifdef USE_CAMERA		
	
	glColor3f( 1, 1, 1 );
	glPushMatrix();
	glTranslatef( 270 + videoGrabberWidth + 10, 74, 0 );
	timeDistTexture.draw( 0, 0 );
	glPopMatrix();
	
#endif
}

void testApp :: drawTimeDistortionFromCameraSourceFullScreen ()
{
#ifdef USE_CAMERA	
	
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

