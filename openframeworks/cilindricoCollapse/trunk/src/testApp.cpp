#include "testApp.h"

//////////////////////////////////////////////
//	SETUP.
//////////////////////////////////////////////

void testApp :: setup()
{
	ofSetFrameRate( 30 );
	ofSetVerticalSync( true );
	ofBackground( 30, 30, 30 );
	
	renderAreaWindow.x      = 0;
	renderAreaWindow.y      = 0;
	renderAreaWindow.width  = 1280;
	renderAreaWindow.height = 720;
	
	renderAreaFullScreen.x      = ofGetScreenWidth();
	renderAreaFullScreen.y      = 0;
	renderAreaFullScreen.width  = 800;
	renderAreaFullScreen.height = 480;
	
	renderArea.x		= renderAreaWindow.x;
	renderArea.y		= renderAreaWindow.y;
	renderArea.width	= renderAreaWindow.width;
	renderArea.height	= renderAreaWindow.height;
	
	screenGrabUtil.setup( "movie/frame", &renderArea );
	tileSaver.init( 10, 0, true );
	
	julApp = NULL;
	
	setJulApp( 1 );
}

void testApp :: setJulApp ( int appID )
{
	if( julApp != NULL )
	{
		delete julApp;
		julApp = NULL;
	}
	
	switch ( appID )
	{
		case 1 :
			julApp = new CilindricoCollapse_01();
			break;
			
		case 2 :
			julApp = new PerlinStepExample();
			break;
			
		case 3 :
			return;
			break;
			
		case 4 :
			return;
			break;
			
		case 5 :
			return;
			break;
			
		case 6 :
			return;
			break;
			
		case 7 :
			return;
			break;
			
		case 8 :
			return;
			break;
			
		case 9 :
			return;
			break;
			
		case 0 :
			return;
			break;
	}
	
	if( julApp != NULL )
	{
		julApp->setRenderArea( renderArea.x, renderArea.y, renderArea.width, renderArea.height );
		julApp->setup();
	}
}


//////////////////////////////////////////////
//	UPDATE.
//////////////////////////////////////////////

void testApp :: update()
{
	if( tileSaver.bGoTiling )
		return;
	
	if( julApp != NULL )
		julApp->update();
}

//////////////////////////////////////////////
//	DRAW.
//////////////////////////////////////////////

void testApp :: draw()
{
	tileSaver.begin();
	
	if( julApp != NULL )
		julApp->draw();
	
	tileSaver.end();
	
	if( julApp != NULL )
		julApp->drawDebug();
	
	if( screenGrabUtil.isRecording() )
		screenGrabUtil.save();
}

//////////////////////////////////////////////
//	TOGGLE FULLSCREEN.
//////////////////////////////////////////////

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
	
	if( julApp != NULL )
		julApp->setRenderArea( renderArea.x, renderArea.y, renderArea.width, renderArea.height );
}

//////////////////////////////////////////////
//	HANDLERS.
//////////////////////////////////////////////

void testApp :: keyPressed( int key )
{
	if( julApp != NULL )
		julApp->keyPressed( key );
}

void testApp :: keyReleased( int key )
{
	if( key == 'f' )
		toggleFullScreen();
	
	if( key == 's' )
	{
		char str[255];
		sprintf( str, "image_%02d%02d%02d_%02d%02d%02d.png", ofGetYear() % 1000, ofGetMonth(), ofGetDay(), ofGetHours(), ofGetMinutes(), ofGetSeconds() );
		
		tileSaver.finish( str, false );
	}
	
	if( key == 'm' )
	{
		if( screenGrabUtil.isRecording() )
			screenGrabUtil.stop();
		else
			screenGrabUtil.start();
	}
	
	if( key == '1' || key == '2' || key == '3' || key == '4' || key == '5' ||
		key == '6' || key == '7' || key == '8' || key == '9' || key == '0' )
	{
		if( julApp != NULL )
		{
			delete julApp;
			julApp = NULL;
		}
		
		switch ( key )
		{
			case '1':
				setJulApp( 1 );
				break;
				
			case '2':
				return;
				break;
				
			case '3':
				return;
				break;
				
			case '4':
				return;
				break;
				
			case '5':
				return;
				break;
				
			case '6':
				return;
				break;
				
			case '7':
				return;
				break;
				
			case '8':
				return;
				break;
				
			case '9':
				return;
				break;
				
			case '0':
				return;
				break;
		}
	}
}

void testApp :: mouseMoved( int x, int y )
{
	if( julApp != NULL )
		julApp->mouseMoved( x, y );
}

void testApp :: mouseDragged( int x, int y, int button )
{
	if( julApp != NULL )
		julApp->mouseDragged( x, y, button );
}

void testApp :: mousePressed( int x, int y, int button )
{
	if( julApp != NULL )
		julApp->mousePressed( x, y, button );
}

void testApp :: mouseReleased( int x, int y, int button )
{
	if( julApp != NULL )
		julApp->mouseReleased( x, y, button );
}

void testApp :: windowResized( int w, int h )
{
	//
}

