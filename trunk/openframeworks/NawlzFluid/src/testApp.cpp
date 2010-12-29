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
	whaleImage.loadImage( "whale_477x223.png" );
	particleImage.loadImage( "particle_4x4.png" );

	nawlzFluid.createImageTexture( whaleImage.getPixels(), whaleImage.width, whaleImage.height, GL_RGBA, 358, 220 );
	nawlzFluid.createBackgroundTexture( backgroundImage.getPixels(), backgroundImage.width, backgroundImage.height, GL_RGB );
//	nawlzFluid.createParticleTexture( particleImage.getPixels(), particleImage.width, particleImage.height, GL_RGB );
//	nawlzFluid.useCircleMotion	= true;
	nawlzFluid.bDrawMeshGrid	= true;
	nawlzFluid.bDrawMeshQuads	= true;
	nawlzFluid.bDrawParticles	= true;
	nawlzFluid.bDrawBackground	= true;
	nawlzFluid.bDrawVectors		= false;
	nawlzFluid.bDrawFluid		= false;
	nawlzFluid.setup();
	
	bDebug = true;
	
	gui.addToggle( "draw mesh grid",		nawlzFluid.bDrawMeshGrid );
	gui.addToggle( "draw mesh quads",		nawlzFluid.bDrawMeshQuads );
	gui.addToggle( "draw particles",		nawlzFluid.bDrawParticles );
	gui.addToggle( "draw background",		nawlzFluid.bDrawBackground );
	gui.addToggle( "draw vectors",			nawlzFluid.bDrawVectors );
	gui.addToggle( "draw fluid",			nawlzFluid.bDrawFluid );
	gui.addSlider( "num of particles",		nawlzFluid.particlesNumPerShoot, 1, 20 );
	
	gui.addPage( "fluid" );
	gui.addToggle( "enableRGB",				nawlzFluid.fluidEnableRGB );
	gui.addSlider( "fadeSpeed",				nawlzFluid.fluidFadeSpeed,			0.0, 0.005 );
	gui.addSlider( "deltaT",				nawlzFluid.fluidDeltaT,				0.0, 1.0 );
	gui.addSlider( "visc",					nawlzFluid.fluidVisc,				0.0, 0.001 );
	gui.addSlider( "colorDiffusion",		nawlzFluid.fluidColorDiffusion,		0.0, 0.1 );
	gui.addSlider( "solverIterations",		nawlzFluid.fluidSolverIterations,	5.0, 20 );
	gui.addToggle( "vorticityConfinement",	nawlzFluid.fluidEnableVorticityConfinement );
	gui.addToggle( "wrapX",					nawlzFluid.fluidWrapX );
	gui.addToggle( "wrapY",					nawlzFluid.fluidWrapY );
	
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
	if( key >= '0' && key <= '9' )
	{
		gui.setPage( key - '0' );
		gui.show();
	}
	
	switch(key)
	{
		case '[': gui.prevPage(); break;
		case ']': gui.nextPage(); break;
	}
	
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

