/*
 *  NawlzFluid.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 14/12/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "NawlzFluid.h"

NawlzFluid :: NawlzFluid()
{
	backgroundTexture	= NULL;
	whaleTexture		= NULL;
	particleTexture		= NULL;
	fluidTexture		= NULL;
	fluidPixels			= NULL;
	
	useMesh				= true;
	useParticles		= true;
	useCircleMotion		= false;
}

NawlzFluid :: ~NawlzFluid() 
{
	if( backgroundTexture )
	{
		backgroundTexture->clear();
		delete backgroundTexture;
		backgroundTexture = NULL;
	}
	
	if( whaleTexture )
	{
		whaleTexture->clear();
		delete whaleTexture;
		whaleTexture = NULL;
	}
	
	if( particleTexture )
	{
		particleTexture->clear();
		delete particleTexture;
		particleTexture = NULL;
	}
	
	if( fluidTexture )
	{
		fluidTexture->clear();
		delete fluidTexture;
		fluidTexture = NULL;
	}
}

///////////////////////////////////////////
//	INIT.
///////////////////////////////////////////

void NawlzFluid :: setup()
{
	mouseX = 0;
	mouseY = 0;
	
	initMesh();
	initFluid();
}

void NawlzFluid :: initMesh ()
{
//	int w = mesh_w_inc = 16;
//	int h = mesh_h_inc = 16;
	
//	int w = mesh_w_inc = 32;
//	int h = mesh_h_inc = 32;

	int w = mesh_w_inc = 64;
	int h = mesh_h_inc = 64;
	
	for( int y=0; y<=ofGetHeight(); y+= mesh_h_inc )
	{
		for( int x=0; x<=ofGetWidth(); x+=mesh_w_inc )
		{
			mesh_point mp;
			mp.p.x	= x;
			mp.p.y	= y;
			
			mesh_points.push_back( mp );
		}
	}
	
	int ix = 0;
	int iy = 0;
	int iw = ofGetWidth() / mesh_w_inc + 1;
	
	for( int y=0; y<ofGetHeight(); y+= mesh_h_inc )
	{
		for( int x=0; x<ofGetWidth(); x+=mesh_w_inc )
		{
			mesh_quad mq;
			mq.x	= x;
			mq.y	= y;
			mq.w	= w;
			mq.h	= h;
			
			int p1 = ix + iw * iy;
			int p2 = ix + iw * ( iy + 1 );
			
			mq.mesh_point_index[ 0 ] = p1;
			mq.mesh_point_index[ 1 ] = p1 + 1;
			mq.mesh_point_index[ 2 ] = p2 + 1;
			mq.mesh_point_index[ 3 ] = p2;
			
			mq.ver_coords[ 0 ] = x;
			mq.ver_coords[ 1 ] = y;
			
			mq.ver_coords[ 2 ] = x + w;
			mq.ver_coords[ 3 ] = y;
			
			mq.ver_coords[ 4 ] = x + w;
			mq.ver_coords[ 5 ] = y + h;
			
			mq.ver_coords[ 6 ] = x;
			mq.ver_coords[ 7 ] = y + h;
			
			if( !whaleTexture )
			{
				mesh_quads.push_back( mq );
				
				ix += 1;
				
				continue;
			}
			
			ofPoint p;
			float p1x, p1y, p2x, p2y;
			p1x = ( x + 0 - whaleTextureXY.x ) / (float)whaleTexture->getWidth();
			p1y = ( y + 0 - whaleTextureXY.y ) / (float)whaleTexture->getHeight();
			p2x = ( x + w - whaleTextureXY.x ) / (float)whaleTexture->getWidth();
			p2y = ( y + w - whaleTextureXY.y ) / (float)whaleTexture->getHeight();
			
			bool bHorCheck = false;
			bool bVerCheck = false;

			bHorCheck = ( bHorCheck || ( p1x >= 0 && p1x <= 1 ) );
			bHorCheck = ( bHorCheck || ( p2x >= 0 && p2x <= 1 ) );
			
			bVerCheck = ( bVerCheck || ( p1y >= 0 && p1y <= 1 ) );
			bVerCheck = ( bVerCheck || ( p2y >= 0 && p2y <= 1 ) );
			
			mq.bTextureInside = true;
			mq.bTextureInside = mq.bTextureInside && bHorCheck;
			mq.bTextureInside = mq.bTextureInside && bVerCheck;
			
			if( !mq.bTextureInside )
			{
				p1x = ofClamp( p1x, 0, 1 );
				p1y = ofClamp( p1y, 0, 1 );
				p2x = ofClamp( p2x, 0, 1 );
				p2y = ofClamp( p2y, 0, 1 );
			}
			
			p = whaleTexture->getCoordFromPercent( p1x, p1y );
			mq.tex_coords[ 0 ]	= p.x;
			mq.tex_coords[ 1 ]	= p.y;
			
			p = whaleTexture->getCoordFromPercent( p2x, p1y );
			mq.tex_coords[ 2 ]	= p.x;
			mq.tex_coords[ 3 ]	= p.y;
			
			p = whaleTexture->getCoordFromPercent( p2x, p2y );
			mq.tex_coords[ 4 ]	= p.x;
			mq.tex_coords[ 5 ]	= p.y;
			
			p = whaleTexture->getCoordFromPercent( p1x, p2y );
			mq.tex_coords[ 6 ]	= p.x;
			mq.tex_coords[ 7 ]	= p.y;
			
			mesh_quads.push_back( mq );
			
			ix += 1;
		}
		
		ix  = 0;
		iy += 1;
	}
}

void NawlzFluid :: initFluid ()
{
	fluidEnableRGB			= false;
	fluidFadeSpeed			= 0.002;
	fluidDeltaT				= 0.5;
	fluidVisc				= 0.00015;
	fluidColorDiffusion		= 0;
	fluidSolverIterations	= 10;
	
	fluidSolver.setup(100, 100);
	fluidSolver.enableRGB( fluidEnableRGB );
	fluidSolver.setFadeSpeed( fluidFadeSpeed );
	fluidSolver.setDeltaT( fluidDeltaT );
	fluidSolver.setVisc( fluidVisc );
	fluidSolver.setColorDiffusion( fluidColorDiffusion );
	fluidSolver.setSolverIterations( fluidSolverIterations );
//	fluidSolver.doVorticityConfinement = true;
	
//	fluidCellsX		= 150;
//	fluidCellsX		= 100;
	fluidCellsX		= 50;
	bResizeFluid	= true;
	
	createFluidTexture();
	
	particlesNumPerShoot = 5;
}

void NawlzFluid :: createBackgroundTexture ( unsigned char* pixels, int width, int height, int glType, int x, int y )
{
	backgroundTexture = new ofTexture();
	backgroundTexture->allocate( width, height, glType );
	backgroundTexture->loadData( pixels, width, height, glType );
	
	backgroundTextureXY.x = x;
	backgroundTextureXY.y = y;
}

void NawlzFluid :: createWhaleTexture ( unsigned char* pixels, int width, int height, int glType, int x, int y )
{
	int w, h;						// expanded width and height by a 1 pixel border.
	w = width  + 2;					// need to do this otherwise the corners of the texture smear across.
	h = height + 2;
	
	int pt;							// pixels total.
	pt = w * h * 4;
	
	unsigned char* pix;
	pix = new unsigned char[ pt ];
	
	int count;
	count = 0;
	
	for( int i=0; i<pt; i+=4 )
	{
		int p = i / 4;
		int y = p / w;
		int x = p - w * y;
		
		bool bHorCheck = false;
		bool bVerCheck = false;
		
		bHorCheck = ( x == 0 ) || ( x == w - 1 );
		bVerCheck = ( y == 0 ) || ( y == h - 1 );
		
		if( bHorCheck || bVerCheck )
		{
			pix[ i + 0 ] = 255;
			pix[ i + 1 ] = 255;
			pix[ i + 2 ] = 255;
			pix[ i + 3 ] = 0;
		}
		else
		{
			if( glType == GL_LUMINANCE )
			{
				pix[ i + 0 ] = pixels[ count ];
				pix[ i + 1 ] = pixels[ count ];
				pix[ i + 2 ] = pixels[ count ];
				pix[ i + 3 ] = 255;
				
				count += 1;
			}
			else if( glType == GL_RGB )
			{
				pix[ i + 0 ] = pixels[ count + 0 ];
				pix[ i + 1 ] = pixels[ count + 1 ];
				pix[ i + 2 ] = pixels[ count + 2 ];
				pix[ i + 3 ] = 255;
				
				count += 3;
			}
			else if( glType == GL_RGBA )
			{
				pix[ i + 0 ] = pixels[ count + 0 ];
				pix[ i + 1 ] = pixels[ count + 1 ];
				pix[ i + 2 ] = pixels[ count + 2 ];
				pix[ i + 3 ] = pixels[ count + 3 ];
				
				count += 4;
			}
			else
			{
				pix[ i + 0 ] = 255;
				pix[ i + 1 ] = 0;
				pix[ i + 2 ] = 0;
				pix[ i + 3 ] = 255;
			}
		}
	}
	
	whaleTexture = new ofTexture();
	whaleTexture->allocate( w, h, GL_RGBA );
	whaleTexture->loadData( pix, w, h, GL_RGBA );

	delete[] pix;
	pix = NULL;
	
	whaleTextureXY.x = x;
	whaleTextureXY.y = y;
}

void NawlzFluid :: createParticleTexture ( unsigned char* pixels, int width, int height, int glType )
{
	particleTexture = new ofTexture();
	particleTexture->allocate( width, height, glType );
	particleTexture->loadData( pixels, width, height, glType );
}

void NawlzFluid :: createFluidTexture ()
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

///////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////

void NawlzFluid :: update()
{
	if( useCircleMotion )
	{
		circleMotion.update();
		
		mouseMoved( circleMotion.x, circleMotion.y );
	}
	
	if( bResizeFluid )
	{
		float r;
		r = ofGetWidth() / (float)ofGetHeight();
		
		fluidSolver.setSize( fluidCellsX, fluidCellsX / r );
		
		createFluidTexture();
		
		bResizeFluid = false;
	}
	
	fluidSolver.enableRGB( fluidEnableRGB );
	fluidSolver.setFadeSpeed( fluidFadeSpeed );
	fluidSolver.setDeltaT( fluidDeltaT );
	fluidSolver.setVisc( fluidVisc );
	fluidSolver.setColorDiffusion( fluidColorDiffusion );
	fluidSolver.setSolverIterations( fluidSolverIterations );
	fluidSolver.update();
	
//	updateMeshPointsWithMouse();
	
	if( useMesh )
	{
		updateMeshPointsWithFluid();
		updateMeshQuads();
	}
	
	if( useParticles )
	{
		updateParticles();
	}
}

void NawlzFluid :: updateMeshPointsWithMouse ()
{
	Vec2f mouse;
	mouse.set( mouseX, mouseY );
	
	for( int i=0; i<mesh_points.size(); i++ )
	{
		mesh_point& mp = mesh_points[ i ];
		
		float t = 200;
		float d = mouse.distance( mp.p );
		
		if( d > 0 && d < t )		// in range.
		{
			mp.v = mp.p - mouse;
			
			float l = mp.v.length();
			float r = 1 - l / t;
			
			mp.v.normalize();
			mp.v *= r * 60;
		}
		else
		{
			mp.v *= 0.85;
		}
		
		
		mp.f = mp.p + mp.v;
	}
}

void NawlzFluid :: updateMeshPointsWithFluid ()
{
	int fw = fluidSolver.getWidth();
	int fh = fluidSolver.getHeight();
	
	Vec2f vel;
	
	float maxVel;
	maxVel = 0.00025;
	maxVel = 0.0005;
	maxVel = 0.001;
//	maxVel = 0.003;
	
	float velDrawMult	= 1;
	
	for( int i=0; i<mesh_points.size(); i++ )
	{
		mesh_point& mp = mesh_points[ i ];
		
		float px = mp.p.x / (float)ofGetWidth();
		float py = mp.p.y / (float)ofGetHeight();
		
		Vec2f p( px, py );
		vel = fluidSolver.getVelocityAtPos( p );
		
		float d2 = vel.lengthSquared();
		
		if( d2 > 0)
		{
			if( d2 > maxVel * maxVel )
			{
				float mult = maxVel * maxVel / d2;
				vel.x *= mult;
				vel.y *= mult;
			}
			
			vel *= velDrawMult * 50000;
		}
		
		mp.v.x = vel.x;
		mp.v.y = vel.y;
		
		mp.f = mp.p + mp.v;
	}
}

void NawlzFluid :: updateMeshQuads ()
{
	int t = mesh_quads.size();
	for( int i=0; i<t; i++ )
	{
		mesh_quad& mq = mesh_quads[ i ];
		
		int p0, p1, p2, p3;
		p0 = mq.mesh_point_index[ 0 ];
		p1 = mq.mesh_point_index[ 1 ];
		p2 = mq.mesh_point_index[ 2 ];
		p3 = mq.mesh_point_index[ 3 ];
		
		const mesh_point& mp0 = mesh_points[ p0 ];
		const mesh_point& mp1 = mesh_points[ p1 ];
		const mesh_point& mp2 = mesh_points[ p2 ];
		const mesh_point& mp3 = mesh_points[ p3 ];
		
		mq.ver_coords[ 0 ] = mp0.f.x;
		mq.ver_coords[ 1 ] = mp0.f.y;
		
		mq.ver_coords[ 2 ] = mp1.f.x;
		mq.ver_coords[ 3 ] = mp1.f.y;
		
		mq.ver_coords[ 4 ] = mp2.f.x;
		mq.ver_coords[ 5 ] = mp2.f.y;
		
		mq.ver_coords[ 6 ] = mp3.f.x;
		mq.ver_coords[ 7 ] = mp3.f.y;
	}
}

void NawlzFluid :: updateParticles ()
{
	int fw = fluidSolver.getWidth();
	int fh = fluidSolver.getHeight();
	
	Vec2f vel;
	
	float maxVel;
	maxVel = 0.00025;
	maxVel = 0.0005;
	maxVel = 0.001;
	//	maxVel = 0.003;
	
	float velDrawMult	= 1;
	
	int i = 0;
	int t = particles.size();
	for( int i=0; i<t; i++ )
	{
		NawlzParticle& particle = particles[ i ];
		
		float px = particle.pos.x / (float)ofGetWidth();
		float py = particle.pos.y / (float)ofGetHeight();
		
		Vec2f p( px, py );
		vel = fluidSolver.getVelocityAtPos( p ) * particle.mass;
		
		float d2 = vel.lengthSquared();
		
		if( d2 > 0)
		{
			if( d2 > maxVel * maxVel )
			{
				float mult = maxVel * maxVel / d2;
				vel.x *= mult;
				vel.y *= mult;
			}
			
			vel *= velDrawMult * 50000;
		}
		
		particle.vel += vel * 0.5;
		particle.vel *= 0.7;
		particle.pos += particle.vel;
		
		particle.alpha *= 0.97;
		
		if( particle.alpha < 0.05 )		// remove.
		{
			particles.erase( particles.begin() + i );
			
			--i;
			--t;
		}
	}
}


///////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////

void NawlzFluid :: draw()
{
	ofEnableAlphaBlending();
	ofSetColor( 255, 255, 255, 255 );
	
	if( backgroundTexture )
	{
		backgroundTexture->draw( backgroundTextureXY.x, backgroundTextureXY.y );
	}
	
	if( useMesh )
	{
		drawMeshQuads();
//		drawMeshPoints();
		drawMeshGrid();
	}
	
//	drawMeshLines();
	
//	drawVectors( 0, 0, ofGetWidth(), ofGetHeight() );
//	drawFluid();
	
	if( useParticles )
	{
		drawParticles();
	}
}

void NawlzFluid :: drawMeshQuads ()
{
	if( !whaleTexture )
		return;
	
	whaleTexture->bind();
	
	for( int i=0; i<mesh_quads.size(); i++ )
	{
		mesh_quad& mq = mesh_quads[ i ];
		
		if( !mq.bTextureInside )
			continue;
		
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		glTexCoordPointer( 2, GL_FLOAT, 0, mq.tex_coords );
		glEnableClientState( GL_VERTEX_ARRAY );		
		glVertexPointer( 2, GL_FLOAT, 0, mq.ver_coords );
		glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	}
	
	whaleTexture->unbind();
}

void NawlzFluid :: drawMeshPoints ()
{
	ofFill();
	ofSetColor( 0xFF0000 );
	
	for( int i=0; i<mesh_points.size(); i++ )
	{
		mesh_point& mp = mesh_points[ i ];
		
		ofCircle( mp.f.x, mp.f.y, 2 );
	}
}

void NawlzFluid :: drawMeshGrid ()
{
	ofNoFill();
	ofSetColor( 255, 255, 255, 40 );
	
	for( int i=0; i<mesh_quads.size(); i++ )
	{
		mesh_quad& mq = mesh_quads[ i ];
		
		// optimised but not pretty.
		
		ofLine( mq.ver_coords[ 0 ], mq.ver_coords[ 1 ], mq.ver_coords[ 2 ], mq.ver_coords[ 3 ] );	
		ofLine( mq.ver_coords[ 6 ], mq.ver_coords[ 7 ], mq.ver_coords[ 0 ], mq.ver_coords[ 1 ] );
		
		continue;
		
		// pretty but not optimised.
		
		ofPoint p0( mq.ver_coords[ 0 ], mq.ver_coords[ 1 ] );
		ofPoint p1( mq.ver_coords[ 2 ], mq.ver_coords[ 3 ] );
		ofPoint p2( mq.ver_coords[ 4 ], mq.ver_coords[ 5 ] );
		ofPoint p3( mq.ver_coords[ 6 ], mq.ver_coords[ 7 ] );
		
		ofLine( p0.x, p0.y, p1.x, p1.y );
//		ofLine( p1.x, p1.y, p2.x, p2.y );	// don't draw 2nd and 3rd line as it doubles up.
//		ofLine( p2.x, p2.y, p3.x, p3.y );	// don't draw 2nd and 3rd line as it doubles up.
		ofLine( p3.x, p3.y, p0.x, p0.y );
	}
}

void NawlzFluid :: drawMeshLines ()
{
	ofFill();
	ofSetColor( 255, 255, 255, 50 );
	
	int w = 1024;
	int h = 768;
	
	int mx = 0;
	int my = 0;
	
	while( mx <= w )
	{
		ofRect( mx, 0, 1, h );
		mx += mesh_w_inc;
	}
	
	while( my <= h )
	{
		ofRect( 0, my, w, 1 );
		my += mesh_h_inc;
	}
}

void NawlzFluid :: drawParticles ()
{
	for( int i=0; i<particles.size(); i++ )
	{
		NawlzParticle& particle = particles[ i ];
		particle.draw();
	}
}

///////////////////////////////////////////
//	FLUID.
///////////////////////////////////////////

void NawlzFluid :: addToFluid( Vec2f pos, Vec2f vel, bool addColor, bool addForce )
{
    float speed = vel.x * vel.x  + vel.y * vel.y * getWindowAspectRatio() * getWindowAspectRatio();    // balance the x and y components of speed with the screen aspect ratio
    
	if(speed > 0) 
	{
		pos.x = constrain(pos.x, 0.0f, 1.0f);
		pos.y = constrain(pos.y, 0.0f, 1.0f);
		
		const float colorMult = 100;
		const float velocityMult = 0.05;
//		const float velocityMult = 30;
//		const float velocityMult = 100;
		
        int index = fluidSolver.getIndexForPos(pos);
		
		if( addColor )
		{
			fluidSolver.addColorAtIndex( index, Color :: white() * colorMult );
		}
		
		if( addForce )
		{
			fluidSolver.addForceAtIndex( index, vel * velocityMult );
		}
    }
}

void NawlzFluid :: drawVectors( float x, float y, float renderWidth, float renderHeight )
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

void NawlzFluid :: drawFluid ()
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
	
	ofEnableAlphaBlending();
}

///////////////////////////////////////////
//	HANDLERS.
///////////////////////////////////////////

void NawlzFluid :: keyPressed(int key)
{
	if( key == 'f' )
	{
		ofToggleFullscreen();
	}
}

void NawlzFluid :: keyReleased(int key)
{
	
}

void NawlzFluid :: mouseMoved(int x, int y )
{
	mouseX = x;
	mouseY = y;
	
	Vec2f eventPos = Vec2f(x, y);
	Vec2f mouseNorm = Vec2f( eventPos) / getWindowSize();
	Vec2f mouseVel = Vec2f( eventPos - pMouse ) / getWindowSize();
//	addToFluid( mouseNorm, mouseVel, true, true );
	addToFluid( mouseNorm, mouseVel, false, true );
	pMouse = eventPos;
	
	for( int i=0; i<particlesNumPerShoot; i++ )
	{
		Vec2f vel = mouseVel;
		vel.normalize();
		vel.rotate( ofRandom( -40, 40 ) * DEG_TO_RAD );
		
		Vec2f pos = eventPos;
		pos += vel * 30;
		
		vel *= ofRandom( 1.0, 3.0 );
		
		particles.push_back( NawlzParticle( particleTexture, pos, vel ) );
	}
}

void NawlzFluid :: mouseDragged(int x, int y, int button)
{
	
}

void NawlzFluid :: mousePressed(int x, int y, int button)
{
	
}

void NawlzFluid :: mouseReleased(int x, int y, int button)
{
	
}

void NawlzFluid :: windowResized(int w, int h)
{
	
}

