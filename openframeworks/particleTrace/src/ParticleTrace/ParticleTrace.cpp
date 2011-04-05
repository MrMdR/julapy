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
    activeRect.width    = (int)( ofGetWidth()  * 0.58 );
    activeRect.height   = (int)( ofGetHeight() * 1.0 );
    activeRect.x        = (int)( ( ofGetWidth()  - activeRect.width  ) * 0.5 ) + 24;
    activeRect.y        = (int)( ( ofGetHeight() - activeRect.height ) * 0.5 );
    
	sampleRangeX	= 5;
	sampleRangeY	= 5;
	sampleW			= sampleRangeX * 2 + 1;
	sampleH			= sampleRangeY * 2 + 1;
	
	sampleGridW = 20;
	sampleGridH	= 20;
	
	exportScale = 10;
	lineWidth	= 1.0;
	lineAlpha	= 0.5;
	traceAlpha	= 0.2;
	traceBlur	= 0;
	minPosDist	= 2.0;
	
	pid = 0;
	
	testParticle = NULL;
	
	//--- booleans
	
	bImageLoaded		= false;
	
	bShowSourceImage	= true;
	bShowTraceImage		= true;
	bShowParticles		= true;
	bShowParticleLines	= false;
	bShowParticleStrip	= false;
    bShowParticleType   = true;
	bShowParticleHead	= true;
	
	bUseImageColour		= true;
	bUseImageForce		= true;
	bUseTraceForce		= true;
	bUseWanderForce		= true;
	
	//--- vector scales.
	
	imageVecScale	= 350;
	traceVecScale	= 250;
	wanderVecScale	= 1.0;
	
	velLimit		= 3.0;
	velEase			= 0.2;
    
    //--- ribbon type.
    
    ribbonCopy = "";
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
    ribbonType.setFont( &font, fontSize, fontScale, fontOffsetY );
    ribbonType.setKerning( fontKerning );
    ribbonType.setup();
}

//////////////////////////////////////////////////
//	IMAGE
//////////////////////////////////////////////////

void ParticleTrace :: loadImage( string fileName )
{
	bImageLoaded = img.loadImage( fileName );
	
	if( !bImageLoaded )
		return;
	
	if( img.bpp != 24 )
		img.setImageType( OF_IMAGE_COLOR );
	
	imgRect.width	= img.width;
	imgRect.height	= img.height;
    
    imgRectCenter   = imgRect;
    imgRectCenter.x = (int)( ( activeRect.width  - imgRectCenter.width  ) * 0.5 ) + activeRect.x;
    imgRectCenter.y = (int)( ( activeRect.height - imgRectCenter.height ) * 0.5 ) + activeRect.y;
    
    imgRectFit      = ofxResizeUtil :: fitToSize  ( imgRect, activeRect );
    imgRectCrop     = ofxResizeUtil :: cropToSize ( imgRect, activeRect );
    
    imgRectCurrent  = imgRectCenter;
    
    fboLargeScale       = 12;
    fboLargeRect.width  = (int)( imgRectCurrent.width  * fboLargeScale );
    fboLargeRect.height = (int)( imgRectCurrent.height * fboLargeScale );
	
	//-- opencv.
	
	imgSrc.allocate( imgRect.width, imgRect.height );
	imgSrc.setFromPixels( img.getPixels(), imgRect.width, imgRect.height );
	
	imgTrace.allocate( imgRect.width, imgRect.height );
	imgTrace.set( 0 );
	
	img.clear();
	
	//-- initialise fbo.
	
	fboTrace.allocate( imgRect.width, imgRect.height, GL_RGB );
    fboTrace.clear( 0, 0, 0, 0 );
    
	fboParticles.allocate( imgRect.width, imgRect.height );
    
    fboType.allocate( fboLargeRect.width, fboLargeRect.height, GL_RGB );
    fboType.clear( 0, 0, 0, 0 );
	
	//-- pixel flow.
	
	pfImage.setPixels( imgSrc.getPixels(), imgRect.width, imgRect.height, OF_IMAGE_COLOR );
	pfTrace.setPixels( imgTrace.getPixels(), imgRect.width, imgRect.height, OF_IMAGE_COLOR );
}

void ParticleTrace :: loadFont( string fileName )
{
    fontSize        = 50;                   // desired font size in pixels.
    fontSizeAdjust  = 1.04;                 // slight arbitrary font adjustment to make it as close to the desired font size.
    fontOffsetY     = -fontSize * 0.05;     // pushing the font up or down to make it fit better between the lines.
    fontScale       = 0.04;                  // scale percentage of the original font size.
    fontKerning     = 0.0625;               // kerning based on percentage of the font size.
    
	font.loadFont( fileName, fontSize * fontSizeAdjust, true, true, true );
}

void ParticleTrace :: loadCopy ( string fileName )
{
	ifstream myFile;
	string line;
    
    ribbonCopy = "";
	
    myFile.open( ofToDataPath( fileName ).c_str() );
    
	if( myFile.is_open() )
	{
		while( !myFile.eof() )
		{
            getline( myFile, line );
            
            ribbonCopy += line;
        }
    }

    myFile.close();
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
		bool bMarkAsTestParticle;
		bMarkAsTestParticle = ( i == 0 );
		
		addParticle( (int)ofRandom( 0, imgRect.width ), (int)ofRandom( 0, imgRect.height ), bMarkAsTestParticle );
	}
}

void ParticleTrace :: addParticle ( float x, float y, bool bMarkAsTestParticle )
{
	if( !bImageLoaded )
		return;
    
    if( !imgRectCurrent.inside( ofPoint( x, y ) ) )
        return;
	
	Particle* p;
	p = new Particle( &pfImage, &pfTrace );
    p->setBounds( imgRectCurrent );
	p->setInitialPosition( x, y );
	p->setInitialVelocity( ofRandom( -1, 1 ), ofRandom( -1, 1 ) );
	p->setPixelRange( sampleRangeX, sampleRangeY );
    p->setRibbonType( &ribbonType, ribbonCopy );
	p->pid = pid++;
	
	if( bMarkAsTestParticle )
	{
		if( testParticle )
			testParticle->bMarkAsTestParticle = false;
		
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

	pfImage.setPixels( imgSrc.getPixels(), imgRect.width, imgRect.height, OF_IMAGE_COLOR );
	pfTrace.setPixels( imgTrace.getPixels(), imgRect.width, imgRect.height, OF_IMAGE_COLOR );
	
	//---
	
	updateParticles();
	
	//---

	ofPoint pos;
	if( testParticle )
	{
		pos.x = testParticle->posVec.x - imgRectCurrent.x;
		pos.y = testParticle->posVec.y - imgRectCurrent.y;
	}
	
	ofPoint mouse;
	mouse.x = ofClamp( mouseX - imgRectCurrent.x, 0, imgRect.width  - 1 );
	mouse.y = ofClamp( mouseY - imgRectCurrent.y, 0, imgRect.height - 1 );
	
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
		p->bUseImageColour	= bUseImageColour;
		p->bUseImageForce	= bUseImageForce;
		p->bUseTraceForce	= bUseTraceForce;
		p->bUseWanderForce	= bUseWanderForce;
		p->imageVecScale	= imageVecScale;
		p->traceVecScale	= traceVecScale;
		p->wanderVecScale	= wanderVecScale;
		p->velLimit			= velLimit;
		p->velEase			= velEase;
		p->traceAlpha		= traceAlpha;
		p->minPosDist		= minPosDist;
		p->setLineAlpha( lineAlpha );
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
    
    //----
    
    glPushMatrix();
    glTranslatef( imgRectCurrent.x, imgRectCurrent.y, 0 );

	drawSourceImage();
	drawTraceImage();
	drawParticles();
    
    glPopMatrix();
    
    //----

    drawRectOutline( activeRect );
    drawRectOutline( imgRectCurrent );
	drawSamples();
}

void ParticleTrace :: drawSourceImage ()
{
	if( !bShowSourceImage )
		return;

	glColor4f( 1.0, 1.0, 1.0, 1.0 );
	imgSrc.draw( 0, 0 );
}

void ParticleTrace :: drawTraceImage ()
{
	fboTrace.clear( 0, 0, 0, 0 );
	fboTrace.begin();
	
	ofSetLineWidth( 1.0 );
//	ofSetLineWidth( 2.0 );
	
	glEnable(GL_BLEND);									// additive blending.
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	
    //----
    
    glPushMatrix();
    glTranslatef( -imgRectCurrent.x, -imgRectCurrent.y, 0 );
    
	int t = particles.size();
	for( int i=0; i<t; i++ )
	{
		Particle* p;
		p = particles[ i ];
		p->drawTrace();
	}
    
    glPopMatrix();
    
    //----
	
	glHint( GL_LINE_SMOOTH_HINT, GL_FASTEST );
	
	glEnable(GL_BLEND);									// return to alpha blending.
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	fboTrace.end();
	
	ofxCvColorImage imgTemp;
	imgTemp.allocate( imgRect.width, imgRect.height );
	imgTemp.setFromPixels( (unsigned char*)fboTrace.getPixels(), imgRect.width, imgRect.height );
	imgTemp.mirror( true, false );
	imgTemp.blur( traceBlur );
	
//	imgSrc -= imgTemp;		// particle decay.
	
	imgTrace += imgTemp;
	imgTrace.blur( traceBlur );
	
	imgTemp.clear();
	
	if( !bShowTraceImage )
		return;

	glEnable( GL_BLEND );
	glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_COLOR );
	
	glColor4f( 1.0, 1.0, 1.0, 1.0 );
//	fboTrace.draw( 0, 0 );
	imgTrace.draw( imgRectCurrent.x, imgRectCurrent.y );
	
	glEnable(GL_BLEND);									// return to alpha blending.
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

	Particle* p;
	int t = particles.size();
	
	for( int i=0; i<t; i++ )
	{
		p = particles[ i ];
		
		if( bShowParticleLines )
		{
			p->drawLine();
		}
		
		if( bShowParticleStrip )
		{
			p->drawStrip();
		}
        
        fboType.begin();
        
        glPushMatrix();
        glTranslatef( -imgRectCurrent.x * fboLargeScale, -imgRectCurrent.y * fboLargeScale, 0 );
        glScalef( fboLargeScale, fboLargeScale, 1 );

        p->drawType();
        
        glPopMatrix();
        
        fboType.end();
	}
    
    if( bShowParticleType )
    {
        glPushMatrix();
        glTranslatef( imgRectCurrent.x, imgRectCurrent.y, 0 );
        glScalef( 1 / fboLargeScale, 1 / fboLargeScale, 1 );
        
        glEnable( GL_BLEND );
        glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_COLOR );

		fboType.draw( 0, 0 );
        
        glEnable(GL_BLEND);									// return to alpha blending.
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glPopMatrix();
    }
	
	for( int i=0; i<t; i++ )
	{
		p = particles[ i ];
		
		if( bShowParticleHead && !bTiling )
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

void ParticleTrace :: drawRectOutline ( const ofRectangle& r )
{
    int x = r.x;
    int y = r.y;
    int w = r.width;
    int h = r.height;
    
    ofFill();
    ofSetColor( 255, 255, 255 );
    ofRect( r.x, r.y, r.width, 1 );
    ofRect( r.x, r.y, 1, r.height );
    ofRect( r.x, r.y + r.height - 1, r.width, 1 );
    ofRect( r.x + r.width - 1, r.y, 1, r.height );
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
	if( testParticle )
	{
		drawSampleVector( testParticle->imgVec );
	}
	glPopMatrix();
	
	y += h + p;
	
	glPushMatrix();
	glTranslatef( x, y, 0 );
	drawSampleImage( sampleImage1 );
	if( testParticle )
	{
		drawSampleVector( testParticle->trcVec );
	}
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
	
	unsigned char* pixels = img.getPixels();	
	
	for( int y=0; y<img.height; y++ )
	{
		for( int x=0; x<img.width; x++ )
		{
			int p = ( y * img.width + x ) * b;
			
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
			ofSetColor( 100, 100, 100 );
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
//	SAVE FBO.
//////////////////////////////////////////////////

void ParticleTrace :: saveTypeFBO ()
{
    int y = ofGetYear();
    int m = ofGetMonth();
    int d = ofGetDay();
    int h = ofGetHours();
    int n = ofGetMinutes();
    int s = ofGetSeconds();
    
    char str[255];
    sprintf( str, "screengrab_lrg/image_type_%02d%02d%02d_%02d%02d%02d.png", y % 1000, m, d, h, n, s );
    
	ofxCvColorImage imgTemp;
	imgTemp.allocate( fboLargeRect.width, fboLargeRect.height );
	imgTemp.setFromPixels( (unsigned char*)fboType.getPixels(), fboLargeRect.width, fboLargeRect.height );
	imgTemp.mirror( true, false );

	ofImage imgSave;
    imgSave.allocate( fboLargeRect.width, fboLargeRect.height, OF_IMAGE_COLOR );
    imgSave.setFromPixels( imgTemp.getPixels(), fboLargeRect.width, fboLargeRect.height, OF_IMAGE_COLOR );
    imgSave.saveImage( str );
    
	imgTemp.clear();
    imgSave.clear();
}

//////////////////////////////////////////////////
//	HANDLERS
//////////////////////////////////////////////////

void ParticleTrace :: keyPressed ( int key )
{
	if( key == 't' )
    {
        saveTypeFBO();
    }
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
