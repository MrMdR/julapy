#include "testApp.h"

//////////////////////////////////////////////
//	SETUP.
//////////////////////////////////////////////

void testApp :: setup()
{
	ofSetFrameRate( 30 );
	ofSetVerticalSync( true );
	ofEnableSmoothing();
	ofEnableAlphaBlending();
	ofDisableArbTex();
	ofBackground( 150, 150, 150 );

	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	
	initRenderArea();
	initApp();
	initColorMap();
	initShereBg();
	initRibbons();
	initFields();
	initAudio();
}

void testApp :: initRenderArea()
{
	renderAreaWindow.x      = 0;
	renderAreaWindow.y      = 0;
	renderAreaWindow.width  = 1280;
	renderAreaWindow.height = 720;
	
	renderAreaFullScreen.x      = 1440;
	renderAreaFullScreen.y      = 0;
	renderAreaFullScreen.width  = 800;
	renderAreaFullScreen.height = 480;
	
	renderArea.x		= renderAreaWindow.x;
	renderArea.y		= renderAreaWindow.y;
	renderArea.width	= renderAreaWindow.width;
	renderArea.height	= renderAreaWindow.height;
}

void testApp :: initApp()
{
	screenGrabUtil.setup( "movie/ar", &renderArea );
	tileSaver.init( 10, 0, true );
	
	debugMode	= true;
	incDir		= 1;
}

void testApp :: initColorMap()
{
//	colorMap.loadImage( "bw_map.jpg" );
	colorMap.loadImage( "bw_red_map.jpg" );
//	colorMap.loadImage( "3426927909_2c8066cec6.jpg" );
//	colorMap.loadImage( "3276739319_f3b90fef70.jpg" );
//	colorMap.loadImage( "3277282859_41d0df132b.jpg" );
}

void testApp :: initShereBg()
{
	sphereBg.init();
}
 
void testApp :: initFields()
{
	sphericalField.setCenter( 0, 0, 0 );
	sphericalField.setRadius( posSpread );
}

void testApp :: initRibbons()
{
	pTotal		= MAX_PARTICLES;
	posSpread	= 600;
	velSpread	= 2;
	
	ribbons = new Ribbon *[ pTotal ];
	for(int i=0; i<pTotal; i++)
	{
		ribbons[ i ] = new RibbonFill();
		
		float rx, ry, rz;
		
		rx = ofRandom( -1, 1 ) * posSpread;
		ry = ofRandom( -1, 1 ) * posSpread;
		rz = ofRandom( -1, 1 ) * posSpread;
		
		ribbons[ i ]->setInitPosition( rx, ry, rz );

		rx = ofRandom( -1, 1 ) * velSpread;
		ry = ofRandom( -1, 1 ) * velSpread;
		rz = ofRandom( -1, 1 ) * velSpread;
		
		ribbons[ i ]->setInitVelocity( rz, ry, rz );
	}

	ribbonViews = new RibbonView *[ pTotal ];
	for( int i=0; i<pTotal; i++ )
	{
		ribbonViews[ i ] = new RibbonFillView();
		ribbonViews[ i ]->setup( ribbons[ i ] );
		ribbonViews[ i ]->setType( RIBBON_VIEW_FILL_TYPE );
		
		RibbonFillView *ribbonFillView = ( RibbonFillView *)ribbonViews[ i ];
		ribbonFillView->useRibbonGLitch( true );
	}
}

void testApp :: initAudio()
{
#ifdef USE_LIVE_AUDIO
//	ofSoundStreamSetup( 0, 2, this, 44100, ribbonLiveAudio.getBufferSize(), 4 );
	audio = new AudioLiveSpectrum();
#else
	audio = new AudioFileSpectrum();
#endif
	
	ribbonAudio.setup( audio );

//	ribbonAudio.init( "../../../../_audio/dj_krush_08_decks_athron.mp3" );
//	ribbonAudio.init( "../../../../_audio/dat_politics_12_nude_Noodle.mp3" );
//	ribbonAudio.init( "../../../../_audio/autechre_ep7_06_dropp.mp3" );
//	ribbonAudio.init( "../../../../_audio/radiohead_06_I_Am_Citizen_Insane.mp3" );
//	ribbonAudio.init( "../../../../_audio/aphex_twin_07_come_to_daddy_mummy_mix.mp3" );
	ribbonAudio.init( "../../../../_audio/principles_of_geometry_01_arp_center.mp3" );
//	ribbonAudio.init( "../../../../_audio/massive_attack_05_exchange.mp3" );
//	ribbonAudio.init( "../../../../_audio/lukid_06_chord.mp3" );	

	ribbonAudio.setNoOfBands( 100 );
	ribbonAudio.setThreshold( 0.55 );
	ribbonAudio.setPosition( 0.3 );
//	ribbonAudio.setFrameRateSync( true );
	
}

//////////////////////////////////////////////
//	UPDATE.
//////////////////////////////////////////////

void testApp :: update()
{
	if( tileSaver.bGoTiling )
		return;

	ribbonAudio.update();
	ribbonAudio.getGlitchData( &vboGlitch[ 0 ], MAX_TRAIL_LENGTH );
	
	noiseField.update();
	
	for( int i=0; i<pTotal; i++ )
	{
		ofxVec3f vel;
		ofxVec3f sph = sphericalField.getNormalisedForce( ribbons[ i ]->position[ 0 ], ribbons[ i ]->position[ 1 ], ribbons[ i ]->position[ 2 ] );
		ofxVec3f noi = noiseField.getNormalisedForce( ribbons[ i ]->position[ 0 ], ribbons[ i ]->position[ 1 ], ribbons[ i ]->position[ 2 ] );
		
		sph *= 1.2;
		noi *= ribbonAudio.getAveragePeakNorm() + 0.2;
		
		vel = sph + noi;
		vel.normalize();
		vel *= 15;
		
		vel *= ribbonAudio.getAveragePeakNorm();
		
		ribbons[ i ]->addVelocity( vel.x, vel.y, vel.z );

		float cx, cy;
		cx = ( ribbons[ i ]->position[ 0 ] + posSpread * 0.5 ) / posSpread;
		cy = ( ribbons[ i ]->position[ 2 ] + posSpread * 0.5 ) / posSpread;
		
		int r, g, b;
		colorMap.getColorXY( cx, cy, &r, &g, &b );
		ribbons[ i ]->addColor( r / 255.0, g / 255.0, b / 255.0, 0.95 );
		
		if( ribbonViews[ i ]->getType() == RIBBON_VIEW_FILL_TYPE )
		{
			RibbonFillView *ribbonFillView = ( RibbonFillView *)ribbonViews[ i ];
			ribbonFillView->setRibbonGlitch( &vboGlitch[ 0 ], MAX_TRAIL_LENGTH );
		}
		
		ribbons[ i ]->update();
	}
	
	sphereBg.update();

	rotateY += 0.1;
}

void testApp :: updtRibbons()
{
	//
}

void testApp :: toggleFullScreen()
{
	ofToggleFullscreen();
	
	if( ofGetWindowMode() == OF_WINDOW )
	{
		renderArea.x		= renderAreaWindow.x;
		renderArea.y		= renderAreaWindow.y;
		renderArea.width	= renderAreaWindow.width;
		renderArea.height	= renderAreaWindow.height;
	}
	
	if( ofGetWindowMode() == OF_FULLSCREEN )
	{
		renderArea.x		= renderAreaFullScreen.x;
		renderArea.y		= renderAreaFullScreen.y;
		renderArea.width	= renderAreaFullScreen.width;
		renderArea.height	= renderAreaFullScreen.height;
	}
	
	screenGrabUtil.setArea( &renderArea );
}

//////////////////////////////////////////////
//	DRAW.
//////////////////////////////////////////////

void testApp :: draw()
{
	ofSetColor( 0xFFFFFF );

	tileSaver.begin();
	
	glPushMatrix();
	glTranslatef( (int)( renderArea.x + renderArea.width * 0.5 ), (int)( renderArea.y + renderArea.height * 0.5 ), 0 );
	glRotatef( rotateY, 0, 1, 0 );

	sphereBg.draw();
	drawRibbons();
	
	glPopMatrix();
	
	tileSaver.end();
	
	if( screenGrabUtil.isRecording() )
		screenGrabUtil.save();

	drawDebug();
}

void testApp :: drawRibbons()
{
	for( int i=0; i<pTotal; i++ )
	{
		ribbonViews[ i ]->draw();
	}
}

void testApp :: drawDebug()
{
	if( debugMode )
	{
		glDisable( GL_DEPTH_TEST );
		
		ofSetColor( 0x000000 );
		ofDrawBitmapString
		(
			"fps			 :: " + ofToString(ofGetFrameRate(), 2) + "\n\n" +
			"incDir			 :: " + ofToString( incDir ) + "\n\n" +
			"audio threshold :: " + ofToString( ribbonAudio.getThreshold() ) + "\n\n",
			20,
			20
		);
		
		glPushMatrix();
		glTranslatef( 10, 510, 0 );
		ribbonAudio.draw( 512, 200 );
		glPopMatrix();
		
		glEnable( GL_DEPTH_TEST );
	}
}


//////////////////////////////////////////////
//	HANDLERS.
//////////////////////////////////////////////

void testApp :: audioReceived( float * input, int bufferSize, int nChannels )
{
#ifdef USE_LIVE_AUDIO
//	ribbonAudio.audioReceived( input, bufferSize, nChannels );
#endif
}

void testApp :: keyPressed( int key )
{

}

void testApp :: keyReleased( int key )
{
	if( key == 's' )
	{
		char str[255];
		sprintf( str, "ribbons_%02d%02d%02d_%02d%02d%02d.png", ofGetYear() % 1000, ofGetMonth(), ofGetDay(), ofGetHours(), ofGetMinutes(), ofGetSeconds() );
		
		tileSaver.finish( str, false );
	}
	
	if( key == 'm' )
	{
		if( screenGrabUtil.isRecording() )
		{
			screenGrabUtil.stop();
		}
		else
		{
			screenGrabUtil.start();
		}
	}
	
	if( key == 'd' )
		debugMode = !debugMode;
	
	if( key == 'f' )
		toggleFullScreen();
	
	if( key == '-' )
		incDir = -1;
	
	if( key == '=' )
		incDir = 1;
	
	if( key == 't' )
	{
		ribbonAudio.setThreshold( ribbonAudio.getThreshold() + 0.05 * incDir );
	}
}

void testApp :: mouseMoved( int x, int y )
{

}

void testApp :: mouseDragged( int x, int y, int button )
{

}

void testApp :: mousePressed( int x, int y, int button )
{

}

void testApp :: mouseReleased( int x, int y, int button )
{

}

void testApp :: windowResized( int w, int h )
{
	//
}

