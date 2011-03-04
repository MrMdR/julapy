/*
 *  ParticleTrace.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 1/03/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ParticleTrace.h"

ParticleTrace :: ParticleTrace ()
{
	sampleRangeX	= 5;
	sampleRangeY	= 5;
	sampleW			= sampleRangeX * 2 + 1;
	sampleH			= sampleRangeY * 2 + 1;
	
	sampleGridW = 20;
	sampleGridH	= 20;
	
	exportScale = 10;
	lineWidth	= 1.0;
	
	pid = 0;
	
	testParticle = NULL;
	
	bImageLoaded		= false;
	bShowSourceImage	= true;
	bShowTraceImage		= true;
	bShowParticles		= true;
	bUseImageColour		= true;
	bEnableImageForce	= true;
	bEnableTraceForce	= true;
	bDrawParticleLines	= true;
	bDrawParticleStrip	= false;
	bDrawParticleHead	= true;
}

ParticleTrace :: ~ParticleTrace()
{
	//
}

//////////////////////////////////////////////////
//	SETUP
//////////////////////////////////////////////////

void ParticleTrace :: setup()
{
	//
}

//////////////////////////////////////////////////
//	IMAGE
//////////////////////////////////////////////////

void ParticleTrace :: loadImage( string fileName )
{
	bImageLoaded = img.loadImage( fileName );
	
	if( !bImageLoaded )
		return;
	
	if( img.bpp != 8 )
		img.setImageType( OF_IMAGE_GRAYSCALE );
	
	imgRect.width	= img.width;
	imgRect.height	= img.height;
	
	imgTrace.allocate( imgRect.width, imgRect.height, OF_IMAGE_COLOR_ALPHA );
	
	//-- initialise fbo.
	
	fboTrace.allocate( imgRect.width, imgRect.height );
	fboParticles.allocate( imgRect.width, imgRect.height );
	
	//-- pixel flow.
	
	pfImage.setPixels( img.getPixels(), imgRect.width, imgRect.height, OF_IMAGE_GRAYSCALE );
	pfTrace.setPixels( (unsigned char*)fboTrace.getPixels(), imgRect.width, imgRect.height, OF_IMAGE_COLOR_ALPHA );
}

//////////////////////////////////////////////////
//	PARTICLES
//////////////////////////////////////////////////

void ParticleTrace :: addRandomParticles ( int numOfParticles )
{
	if( !bImageLoaded )
		return;
	
	for( int i=0; i<numOfParticles; i++ )
	{
		addParticle( (int)ofRandom( 0, img.width ), (int)ofRandom( 0, img.height ) );
	}
}

void ParticleTrace :: addParticle ( float x, float y )
{
	if( !bImageLoaded )
		return;
	
	Particle* p;
	p = new Particle( &pfImage, &pfTrace );
	p->setInitialPosition( x, y );
	p->setInitialVelocity( 1, 1 );
	p->setPixelRange( sampleRangeX, sampleRangeY );
	p->pid = pid++;
	
	if( !testParticle )
	{
		testParticle = p;
		testParticle->bMarkAsTestParticle = true;
	}
	
	particles.push_back( p );
}

void ParticleTrace :: killAllParticles ()
{
	int t = particles.size();
	for( int i=0; i<t; i++ )
	{
		Particle* p;
		p = particles[ i ];
		
		particles.erase( particles.begin() + i );
		
		--i;
		--t;
		
		delete p;
		p = NULL;
	}
}

//////////////////////////////////////////////////
//	UPDATE
//////////////////////////////////////////////////

void ParticleTrace :: update()
{
	if( !bImageLoaded )
		return;
	
	//---

	pfImage.setPixels( img.getPixels(), imgRect.width, imgRect.height, OF_IMAGE_GRAYSCALE );
	pfTrace.setPixels( imgTrace.getPixels(), imgRect.width, imgRect.height, OF_IMAGE_COLOR_ALPHA );
	
	//---
	
	updateParticles();
	
	//---

	ofPoint pos;
	pos.x = testParticle->posVec.x;
	pos.y = testParticle->posVec.y;
	
	ofPoint mouse;
	mouse.x = ofClamp( mouseX, 0, imgRect.width  - 1 );
	mouse.y = ofClamp( mouseY, 0, imgRect.height - 1 );
	
	pfImage.getPixelsAt( pos, sampleRangeX, sampleRangeY, sampleImage0 );		// sample of image pixels around test particle.
	pfTrace.getPixelsAt( pos, sampleRangeX, sampleRangeY, sampleImage1 );		// sample of trace pixels around test particle.
	pfImage.getPixelsAt( mouse, sampleRangeX, sampleRangeY, sampleImage2 );		// sample of image pixels around mouse.
}

void ParticleTrace :: updateParticles ()
{
	int t = particles.size();
	for( int i=0; i<t; i++ )
	{
		Particle* p;
		p = particles[ i ];
		p->bUseImageColour		= bUseImageColour;
		p->bEnableImageForce	= bEnableImageForce;
		p->bEnableTraceForce	= bEnableTraceForce;
		p->update();
	}
}

//////////////////////////////////////////////////
//	DRAW
//////////////////////////////////////////////////

void ParticleTrace :: draw ( bool bTiling )
{
	if( !bImageLoaded )
		return;
	
	ofEnableAlphaBlending();
	
	if( bTiling )			// do this before fbo stuff as it stops it from tiling.
	{
		drawParticles( false, true );
		return;
	}

	drawSourceImage();
	drawTraceImage();
	drawParticles();
	
	drawSamples();
}

void ParticleTrace :: drawSourceImage ()
{
	if( !bShowSourceImage )
		return;

	glColor4f( 1.0, 1.0, 1.0, 1.0 );
	img.draw( 0, 0 );
}

void ParticleTrace :: drawTraceImage ()
{
	fboTrace.begin();
	
	ofSetLineWidth( 1.0 );
	
	int t = particles.size();
	for( int i=0; i<t; i++ )
	{
		Particle* p;
		p = particles[ i ];
		p->drawTrace();
	}
	
	fboTrace.end();
	
	imgTrace.setFromPixels( (unsigned char*)fboTrace.getPixels(), imgRect.width, imgRect.height, OF_IMAGE_COLOR_ALPHA, false );
	imgTrace.mirror( false, true );

	if( !bShowTraceImage )
		return;

	glColor4f( 1.0, 1.0, 1.0, 1.0 );
	fboTrace.draw( 0, 0 );
//	imgTrace.draw( 0, 0 );
}

void ParticleTrace :: drawParticles ( bool bDrawToFbo, bool bTiling )
{
	if( !bShowParticles )
		return;

	if( bDrawToFbo )
	{
		fboParticles.begin();
	}
	
	ofFill();
	
	float lw;
	lw = lineWidth;
	lw *= bTiling ? exportScale : 1;		// this is relative to the number of tiles.
	ofSetLineWidth( lw );
	
	glEnable(GL_BLEND);										// line smoothing.
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	
	int t = particles.size();
	for( int i=0; i<t; i++ )
	{
		Particle* p;
		p = particles[ i ];
		
		if( bDrawParticleLines )
		{
			p->drawLine();
		}
		
		if( bDrawParticleStrip )
		{
			p->drawStrip();
		}
		
		if( bDrawParticleHead && !bTiling )
		{
			p->drawHead();
		}
	}
	
	if( bDrawToFbo )
	{
		fboParticles.end();
		
		glColor4f( 1.0, 1.0, 1.0, 1.0 );
		fboParticles.draw( 0, 0 );
	}
}

void ParticleTrace :: drawSamples ()
{
	int p = 10; // padding.
	int w = sampleW * sampleGridW;
	int h = sampleH * sampleGridH;
	int x = ofGetWidth() - w - p;
	int y = p;
	
	glPushMatrix();
	glTranslatef( x, y, 0 );
	drawSampleImage( sampleImage0 );
	drawSampleVector( testParticle->imgVec );
	glPopMatrix();
	
	y += h + p;
	
	glPushMatrix();
	glTranslatef( x, y, 0 );
	drawSampleImage( sampleImage1 );
	drawSampleVector( testParticle->trcVec );
	glPopMatrix();
	
	y += h + p;
	
	glPushMatrix();
	glTranslatef( x, y, 0 );
	drawSampleImage( sampleImage2 );
	glPopMatrix();
}

void ParticleTrace :: drawSampleImage ( ofImage& img )
{
	int w = sampleGridW;
	int h = sampleGridH;
	int b = img.bpp / 8;
	
	for( int y=0; y<img.height; y++ )
	{
		for( int x=0; x<img.width; x++ )
		{
			int p = ( y * img.width + x ) * b;
			
			unsigned char* pixels = img.getPixels();			
			
			if( img.type == OF_IMAGE_GRAYSCALE )
			{
				ofSetColor( pixels[ p ], pixels[ p ], pixels[ p ] );
			}
			else if( img.type == OF_IMAGE_COLOR )
			{
				ofSetColor( pixels[ p + 0 ], pixels[ p + 1 ], pixels[ p + 2 ] );
			}
			else if( img.type == OF_IMAGE_COLOR_ALPHA )
			{
				ofSetColor( pixels[ p + 0 ], pixels[ p + 1 ], pixels[ p + 2 ], pixels[ p + 3 ] );
			}

			ofFill();
			ofRect( x * w, y * h, w, h );
			
			ofNoFill();
			ofSetColor( 50, 50, 50 );
			ofRect( x * w, y * h, w, h );
		}
	}
}

void ParticleTrace :: drawSampleVector ( const ofxVec2f& v )
{
	int cx = sampleW * 0.5 * sampleGridW;
	int cy = sampleH * 0.5 * sampleGridH;
	
	ofNoFill();
	ofSetColor( 255, 0, 0 );
	ofLine( cx, cy, cx + cx * v.x, cy + cy * v.y );
	
	//-- circle center.
	
	ofFill();
	ofSetColor( 255, 0, 0 );
	ofCircle( cx, cy, 4 );
	
	ofNoFill();
	ofSetColor( 255, 0, 0 );
	ofCircle( cx, cy, 4 );
}

//////////////////////////////////////////////////
//	HANDLERS
//////////////////////////////////////////////////

void ParticleTrace :: keyPressed ( int key )
{
	//
}

void ParticleTrace :: keyReleased ( int key )
{
	
}

void ParticleTrace :: mouseMoved ( int x, int y )
{
	mouseX = x;
	mouseY = y;
}

void ParticleTrace :: mouseDragged ( int x, int y, int button )
{
	
}

void ParticleTrace :: mousePressed ( int x, int y, int button )
{
	
}

void ParticleTrace :: mouseReleased ( int x, int y, int button )
{
	
}

void ParticleTrace :: windowResized ( int w, int h )
{
	
}
