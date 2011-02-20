/*
 *  NawlzFishBowl.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 12/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "NawlzFishBowl.h"

NawlzFishBowl :: NawlzFishBowl ()
{
	backgroundTexture	= NULL;
	particleTexture		= NULL;
	bowlTexture			= NULL;
	bowlPixels			= NULL;
	fluidTexture		= NULL;
	fluidPixels			= NULL;
	
	bDrawBackground = true;
	bDrawParticles	= true;
	bDrawBowl		= true;
	bOverRoi		= false;
	bTouchDown		= false;
}

NawlzFishBowl :: ~NawlzFishBowl ()
{
	if( backgroundTexture )
	{
		backgroundTexture->clear();
		delete backgroundTexture;
		backgroundTexture = NULL;
	}
	
	if( particleTexture )
	{
		particleTexture->clear();
		delete particleTexture;
		particleTexture = NULL;
	}
	
	if( bowlTexture )
	{
		bowlTexture->clear();
		delete bowlTexture;
		bowlTexture = NULL;
		
		delete[] bowlPixels;
		bowlPixels = NULL;
	}
	
	if( fluidTexture )
	{
		fluidTexture->clear();
		delete fluidTexture;
		fluidTexture = NULL;
	}
}

void NawlzFishBowl :: setup ()
{
	mouseX = 0;
	mouseY = 0;
	
//	initFluid();
//	createFluidTexture();
	createParticles();
}

void NawlzFishBowl :: setROI ( const ofRectangle& roi )
{
	this->roi = roi;
}

void NawlzFishBowl :: initFluid ()
{
	switch ( 1 )
	{
		case 0:
			
			fluidEnableRGB					= false;
			fluidFadeSpeed					= 0.002;
			fluidDeltaT						= 0.5;
			fluidVisc						= 0.00015;
			fluidColorDiffusion				= 0;
			fluidSolverIterations			= 10;
			fluidEnableVorticityConfinement	= false;
			fluidWrapX						= false;
			fluidWrapY						= false;
			fluidInputVelocityMult			= 0.2;
			
			break;
			
		case 1:
			
			fluidEnableRGB					= false;
			fluidFadeSpeed					= 0.002;
			fluidDeltaT						= 0.15;
			fluidVisc						= 0.001;
			fluidColorDiffusion				= 0;
			fluidSolverIterations			= 5;
			fluidEnableVorticityConfinement	= false;
			fluidWrapX						= false;
			fluidWrapY						= false;
			fluidInputVelocityMult			= 0.2;
			
			break;
			
		default:
			break;
	}
	
	
	fluidSolver.setup( 100, 100 );
	fluidSolver.enableRGB( fluidEnableRGB );
	fluidSolver.setFadeSpeed( fluidFadeSpeed );
	fluidSolver.setDeltaT( fluidDeltaT );
	fluidSolver.setVisc( fluidVisc );
	fluidSolver.setColorDiffusion( fluidColorDiffusion );
	fluidSolver.setSolverIterations( fluidSolverIterations );
	fluidSolver.enableVorticityConfinement( fluidEnableVorticityConfinement );
	fluidSolver.setWrap( fluidWrapX, fluidWrapY );
	
//	fluidCellsX		= 150;
//	fluidCellsX		= 100;
	fluidCellsX		= 50;
	fluidScale		= 0;
	bResizeFluid	= true;
}

void NawlzFishBowl :: createFluidTexture ()
{
	if( fluidPixels )
	{
		delete[] fluidPixels;
	}
	
	int texWidth	= fluidSolver.getWidth()  - 2;
	int texHeight	= fluidSolver.getHeight() - 2;
	int texPixCount	= texWidth * texHeight * 4;
	
	fluidPixels		= new unsigned char[ texPixCount ];
	
	for( int i=0; i<texPixCount; i+=4 )
	{
		fluidPixels[ i + 0 ] = 0;
		fluidPixels[ i + 1 ] = 0;
		fluidPixels[ i + 2 ] = 0;
		fluidPixels[ i + 3 ] = 124;
	}
	
	fluidTexture = new ofTexture();
	fluidTexture->allocate( texWidth, texHeight, GL_RGBA );
	fluidTexture->loadData( fluidPixels, texWidth, texHeight, GL_RGBA );
}

void NawlzFishBowl :: createBackgroundTexture ( unsigned char* pixels, int width, int height, int glType, int x, int y )
{
	backgroundTexture = new ofTexture();
	backgroundTexture->allocate( width, height, glType );
	backgroundTexture->loadData( pixels, width, height, glType );
	
	backgroundTextureXY.x = x;
	backgroundTextureXY.y = y;
}

void NawlzFishBowl :: createParticleTexture ( unsigned char* pixels, int width, int height, int glType, int x, int y )
{
	particleTexture = new ofTexture();
	particleTexture->allocate( width, height, glType );
	particleTexture->loadData( pixels, width, height, glType );
}

void NawlzFishBowl :: createBowlTexture ( unsigned char* pixels, int width, int height, int glType, int x, int y )
{
	int bowlPixelDepth = 1;
	if( glType == GL_RGB )
		bowlPixelDepth = 3;
	else if( glType == GL_RGBA )
		bowlPixelDepth = 4;
	
	int t = width * height * bowlPixelDepth;
	bowlPixels = new unsigned char[ t ];
	for( int i=0; i<t; i++ )
	{
		bowlPixels[ i ] = pixels[ i ];
	}
	
	bowlTexture = new ofTexture();
	bowlTexture->allocate( width, height, glType );
	bowlTexture->loadData( bowlPixels, width, height, glType );
	
	bowlTextureXY.x = x;
	bowlTextureXY.y = y;
	
	bowlRect.x		= x;
	bowlRect.y		= y;
	bowlRect.width	= width;
	bowlRect.height	= height;
}

void NawlzFishBowl :: createParticles ()
{
	for( int i=0; i<200; i++ )
	{
		Vec2f vel;
		vel.set( 1, 0 );
		vel.rotate( ofRandom( -360, 360 ) * DEG_TO_RAD );
		vel *= ofRandom( 1.0, 3.0 );
		
		float gap = 0.4;
		
		Vec2f pos;
		pos.x = roi.x + ( roi.width  * gap ) + ofRandom( 0, roi.width  - ( roi.width  * gap * 2 ) );
		pos.y = roi.y + ( roi.height * gap ) + ofRandom( 0, roi.height - ( roi.height * gap * 2 ) );
		
		particles.push_back( NawlzFishBowlParticle() );
		particles.back().setLoc( pos.x, pos.y );
		particles.back().setVel( vel.x, vel.y );
		particles.back().setBounds( roi );
		particles.back().setImageBounds( bowlRect, bowlPixels, bowlPixelDepth );
		particles.back().setTexture( particleTexture );
	}
}

///////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////

void NawlzFishBowl :: update ()
{
	if( bTouchDown )
	{
		fluidScale += ( 1 - fluidScale ) * 0.8;
	}
	else
	{
		fluidScale += ( 0 - fluidScale ) * 0.1;
	}
	
	if( bResizeFluid )
	{
		float r;
		r = ofGetWidth() / (float)ofGetHeight();
		
//		fluidSolver.setSize( fluidCellsX, fluidCellsX / r );
		
		createFluidTexture();
		
		bResizeFluid = false;
	}
	
//	fluidSolver.enableRGB( fluidEnableRGB );
//	fluidSolver.setFadeSpeed( fluidFadeSpeed );
//	fluidSolver.setDeltaT( fluidDeltaT );
//	fluidSolver.setVisc( fluidVisc );
//	fluidSolver.setColorDiffusion( fluidColorDiffusion );
//	fluidSolver.setSolverIterations( fluidSolverIterations );
//	fluidSolver.enableVorticityConfinement( fluidEnableVorticityConfinement );
//	fluidSolver.setWrap( fluidWrapX, fluidWrapY );
//	fluidSolver.update();
	
	if( bDrawParticles )
	{
		updateParticles();
	}
}

void NawlzFishBowl :: updateParticles ()
{
	Vec2f vel;
	Vec2f roiCenter;
	roiCenter.set( roi.x + roi.width * 0.5, roi.y + roi.height * 0.5 );
	
	float maxVel;
	maxVel = 0.00025;
	maxVel = 0.0005;
	maxVel = 0.001;
//	maxVel = 0.003;
	
	float velDrawMult = 0.5;
	
	int i = 0;
	int t = particles.size();
	for( int i=0; i<t; i++ )
	{
		NawlzFishBowlParticle& particle = particles[ i ];
		
		particle.update();
		continue;
		
		//-- fluid forces.
		
		bool b1 = isPointInsideROI( particle.loc.x, particle.loc.y );
		bool b2 = isPointInsideBowl( particle.loc.x, particle.loc.y );
		
		if( b2 )
		{
			float px = ( particle.loc.x - roi.x ) / (float)roi.width;
			float py = ( particle.loc.y - roi.y ) / (float)roi.height;
			
			float particleMass = 1.0;
			
			Vec2f p( px, py );
			vel = fluidSolver.getVelocityAtPos( p ) * particleMass;
			
			float d2 = vel.lengthSquared();
			
			if( d2 > 0 )
			{
				if( d2 > maxVel * maxVel )
				{
					float mult = maxVel * maxVel / d2;
					vel.x *= mult;
					vel.y *= mult;
				}
				
				vel *= velDrawMult * 50000;
			}
		}
		else
		{
			Vec2f dir;
			dir.x = roiCenter.x - particle.loc.x;
			dir.y = roiCenter.y - particle.loc.y;
			dir.normalize();
			vel = dir * 10;
		}
		
		particle.vel.x += vel.x * 0.5;
		particle.vel.y += vel.y * 0.5;
		particle.vel *= 0.7;
		particle.loc.x += particle.vel.x;
		particle.loc.y += particle.vel.y;
	}
}

bool NawlzFishBowl :: isPointInsideROI ( float x, float y )
{
	bool b1 = ( x >= roi.x );
	bool b2 = ( x <  roi.x + roi.width );
	bool b3 = ( y >= roi.y );
	bool b4 = ( y <  roi.y + roi.height );
	
	return ( b1 && b2 ) && ( b3 && b4 );
}

bool NawlzFishBowl :: isPointInsideBowl ( float x, float y )
{
	float px = ( x - roi.x ) / (float)roi.width;
	float py = ( y - roi.y ) / (float)roi.height;
	px = ofClamp( px, 0, 1 );
	py = ofClamp( py, 0, 1 );

	int iw	= bowlTexture->getWidth();
	int ih	= bowlTexture->getHeight();
	int ix	= px * iw;
	int iy	= py * ih;
	
	int p	= ( ( iy * iw ) + ix ) * bowlPixelDepth;
	
	return bowlPixels[ p ] < 255;
}

///////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////

void NawlzFishBowl :: draw ()
{
	ofEnableAlphaBlending();
	ofSetColor( 255, 255, 255, 255 );
	
	if( backgroundTexture && bDrawBackground )
	{
		backgroundTexture->draw( backgroundTextureXY.x, backgroundTextureXY.y );
	}
	
	if( bowlTexture && bDrawBowl )
	{
		if( bOverRoi )
		{
			ofSetColor( 0, 0, 255, 255 );
		}
		
		bowlTexture->draw( bowlTextureXY.x, bowlTextureXY.y );
	}
	
	if( bDrawParticles )
	{
		drawParticles();
	}
}

void NawlzFishBowl :: drawParticles ()
{
	for( int i=0; i<particles.size(); i++ )
	{
		NawlzFishBowlParticle& particle = particles[ i ];
		particle.draw();
	}
}

///////////////////////////////////////////
//	FLUID.
///////////////////////////////////////////

void NawlzFishBowl :: addToFluid( Vec2f pos, Vec2f vel, bool addColor, bool addForce )
{
    float speed = vel.x * vel.x  + vel.y * vel.y * getWindowAspectRatio() * getWindowAspectRatio();    // balance the x and y components of speed with the screen aspect ratio
    
	if(speed > 0) 
	{
		pos.x = constrain(pos.x, 0.0f, 1.0f);
		pos.y = constrain(pos.y, 0.0f, 1.0f);
		
		const float colorMult = 100;
		
        int index = fluidSolver.getIndexForPos(pos);
		
		if( addColor )
		{
			fluidSolver.addColorAtIndex( index, Color :: white() * colorMult );
		}
		
		if( addForce )
		{
			fluidSolver.addForceAtIndex( index, vel * fluidInputVelocityMult );
		}
    }
}

void NawlzFishBowl :: drawVectors( float x, float y, float renderWidth, float renderHeight )
{
	int fw = fluidSolver.getWidth();
	int fh = fluidSolver.getHeight();
	
	glPushMatrix();
	glTranslatef( x, y, 0 );
	glScalef( renderWidth / ( fw - 2 ), renderHeight / ( fh - 2 ), 1.0 );
	
	float maxVel = 5.0f/20000;
	
	float velDrawThreshold = 0;
	
	MSA::Vec2f vel;
	float vt = velDrawThreshold * fluidSolver.getInvWidth() * fluidSolver.getInvHeight();
	vt *= vt;
	
	int vectorSkipCount	= 0;
	float velDrawMult	= 1;
	float brightness	= 1;
	
	for (int j=0; j<fh-2; j+=vectorSkipCount+1 ){
		for (int i=0; i<fw-2; i+=vectorSkipCount+1 ){
			vel = fluidSolver.getVelocityAtCell(i+1, j+1);
			float d2 = vel.lengthSquared();
			if(d2>vt) {
				if(d2 > maxVel * maxVel) {
					float mult = maxVel * maxVel/ d2;
					//				float mult = (d2 - maxVel * maxVel) / d2;
					vel.x *= mult;
					vel.y *= mult;
				}
				vel *= velDrawMult * 50000;
				
#ifndef MSA_TARGET_OPENGLES
				float b = MSA :: mapRange(d2, vt, maxVel, 0.0f, brightness);
				b = brightness;
				glColor3f(b, b, b);
				
				glBegin(GL_LINES);
				glVertex2f(i, j);
				glVertex2f(i + vel.x, j + vel.y);
				glEnd();
#endif
			}
		}
	}
	glPopMatrix();
}

void NawlzFishBowl :: drawFluid ()
{
	//	glBlendFunc(GL_ONE, GL_ONE);
	//	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);		// screen.
	//	glBlendFunc(GL_ZERO, GL_SRC_COLOR);					// multiply - fake.
	//	glEnable(GL_BLEND);
	
	int fw = fluidSolver.getWidth();
	int fh = fluidSolver.getHeight();
	
	Vec2f vel;
	Color color;
	int index = 0;
	for( int j=1; j < fh-1; j++ )
	{
		for( int i=1; i < fw-1; i++ )
		{
			fluidSolver.getInfoAtCell( i, j, &vel, &color );
			int r = (unsigned char)min( color.r * 255, 255.0f);
			int g = (unsigned char)min( color.g * 255, 255.0f);
			int b = (unsigned char)min( color.b * 255, 255.0f);
			
			fluidPixels[ index++ ] = r;
			fluidPixels[ index++ ] = g;
			fluidPixels[ index++ ] = b;
			
			fluidPixels[ index++ ] = r; // max( b, max( r, g ) );
		}
	}
	
	ofFill();
	ofSetColor( 0xFFFFFF );
	
	int texWidth	= (int)fluidTexture->getWidth();
	int texHeight	= (int)fluidTexture->getHeight();
	
	fluidTexture->loadData( fluidPixels, texWidth, texHeight, GL_RGBA );
	fluidTexture->draw( 0, 0, ofGetWidth(), ofGetHeight() );
}

///////////////////////////////////////////
//	HANDLERS.
///////////////////////////////////////////

void NawlzFishBowl :: keyPressed(int key)
{
	if( key == 'f' )
	{
		ofToggleFullscreen();
	}
}

void NawlzFishBowl :: keyReleased(int key)
{
	//
}

void NawlzFishBowl :: mouseMoved(int x, int y )
{
	if( !isPointInsideROI( x, y ) )
		return;
	
	float px = ( x - roi.x ) / roi.width;
	float py = ( y - roi.y ) / roi.height;
	
	mouseX = px * ofGetWidth();
	mouseY = py * ofGetHeight();
	
	Vec2f eventPos	= Vec2f( mouseX, mouseY );
	Vec2f mouseNorm	= Vec2f( eventPos ) / getWindowSize();
	Vec2f mouseVel	= Vec2f( eventPos - pMouse ) / getWindowSize();
//	addToFluid( mouseNorm, mouseVel, false, true );
	
	pMouse = eventPos;
}

void NawlzFishBowl :: mouseDragged(int x, int y, int button)
{
	//
}

void NawlzFishBowl :: mousePressed(int x, int y, int button)
{
	bTouchDown = true;
}

void NawlzFishBowl :: mouseReleased(int x, int y, int button)
{
	bTouchDown = false;
}

void NawlzFishBowl :: windowResized(int w, int h)
{
	//
}