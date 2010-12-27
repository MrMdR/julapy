#include "testApp.h"

///////////////////////////////////////////
//	INIT.
///////////////////////////////////////////

void testApp::setup()
{
	ofSetFrameRate( 30 );							// OF setup.
	ofSetVerticalSync( true );
	ofSetCircleResolution( 100 );
	ofEnableAlphaBlending();
	ofBackground( 0, 0, 0 );
	
	backgroundImage.loadImage( "whale_bg_1024x768.png" );
	whaleImage.loadImage( "whale_1024x768.png" );
	particleImage.loadImage( "particle_4x4.png" );
	
	nawlzFluid.createBackgroundTexture( backgroundImage.getPixels(), backgroundImage.width, backgroundImage.height, GL_RGB );
	nawlzFluid.createWhaleTexture( whaleImage.getPixels(), whaleImage.width, whaleImage.height, GL_RGBA );
//	nawlzFluid.createParticleTexture( particleImage.getPixels(), particleImage.width, particleImage.height, GL_RGB );
//	nawlzFluid.useCircleMotion	= true;
	nawlzFluid.useMesh			= true;
	nawlzFluid.useParticles		= true;
	nawlzFluid.setup();
	
	bDebug = true;
	
	gui.addToggle( "draw mesh",			nawlzFluid.useMesh );
	gui.addToggle( "draw particles",	nawlzFluid.useParticles );
	gui.addSlider( "num of particles",	nawlzFluid.particlesNumPerShoot, 1, 20 );
	
	gui.addPage( "fluid" );
	gui.addToggle( "enableRGB",			nawlzFluid.fluidEnableRGB );
	gui.addSlider( "fadeSpeed",			nawlzFluid.fluidFadeSpeed,			0.0, 0.005 );
	gui.addSlider( "deltaT",			nawlzFluid.fluidDeltaT,				0.0, 1.0 );
	gui.addSlider( "visc",				nawlzFluid.fluidVisc,				0.0, 0.001 );
	gui.addSlider( "colorDiffusion",	nawlzFluid.fluidColorDiffusion,		0.0, 0.1 );
	gui.addSlider( "solverIterations",	nawlzFluid.fluidSolverIterations,	5.0, 20 );
	
	gui.setAutoSave( false );
	gui.show();
}

///////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////

void testApp::update()
{
	nawlzFluid.update();
}

///////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////

void testApp::draw()
{
	ofSetColor( 255, 255, 255, 255 );
	
	nawlzFluid.draw();

	if( bDebug )
	{
		gui.draw();
	}
}

///////////////////////////////////////////
//	HANDLERS.
///////////////////////////////////////////

void testApp::keyPressed(int key)
{
	if( key == 'f' )
	{
		ofToggleFullscreen();
	}
	
	if( key == 'd' )
	{
		bDebug = !bDebug;
	}
}

void testApp::keyReleased(int key)
{

}

void testApp::mouseMoved(int x, int y )
{
	nawlzFluid.mouseMoved( x, y );
}

void testApp::mouseDragged(int x, int y, int button)
{

}

void testApp::mousePressed(int x, int y, int button)
{

}

void testApp::mouseReleased(int x, int y, int button)
{

}

void testApp::windowResized(int w, int h)
{

}

