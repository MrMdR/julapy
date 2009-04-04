package com.julapy.pargoliers;

import javax.media.opengl.GL;

import net.unlekker.tilesaver.TileSaver;

import processing.core.PApplet;
import processing.opengl.PGraphicsOpenGL;

public class Pargoliers01 extends PApplet
{
	PGraphicsOpenGL pgl;
	GL gl;

	float[] lightAmbient	= { 0.1f, 0.1f, 0.1f, 1.0f };
	float[] lightDiffuse	= { 0.3f, 0.3f, 0.3f, 1.0f };
	float[] lightSpecular	= { 0.5f, 0.5f, 0.5f, 1.0f };
	float[] lightPosition	= { -1.5f, 1.0f, -4.0f, 1.0f };
	
	PargoliersImage srcImage;
	PargoliersImage dstImage;
	PargoliersSystem psys;
	TileSaver tiler;
	
	float sceneCenterX	= 0;
	float sceneCenterY	= 0;
	float sceneCenterZ	= 0;
	
	Boolean isRecording			= false;
	Boolean isTiling			= false;
	Boolean isLightingEnabled	= true;
	Boolean isImageShowing		= true;
	
	public void setup() 
	{
		size( 600, 600, OPENGL );
		colorMode( RGB, 1.0f );
		frameRate( 25 );
		
		initScene();
		initOpenGL();
		initTileSaver();
		initImage();
		initParticles();
	}

	////////////////////////////////////////////
	// HANDLERS.
	////////////////////////////////////////////
	
	public void keyPressed()
	{
		if( key == 'i' )
		{
			srcImage.isShowing = !srcImage.isShowing;
		}
		
		if( key == 'l' )
		{
			isLightingEnabled = !isLightingEnabled;
		}
		
		if( key == 'r' )
		{
			isRecording = !isRecording;
			
			if(isRecording)
				println("started recording.");
			else
				println("stopped recording.");
		}
		
		if( key == 't' )
		{
			isTiling = !isTiling;
			
			tiler.init("Simple"+nf(frameCount,16),16);
		}
	}
	
	////////////////////////////////////////////
	// INIT.
	////////////////////////////////////////////

	private void initScene ()
	{
		sceneCenterX = width * 0.5f;
		sceneCenterY = height * 0.5f;
		sceneCenterZ = -( height * 0.5f ) / tan( PI * 60 / 360.0f );
	}
	
	private void initOpenGL ()
	{
		pgl	= (PGraphicsOpenGL) g;
		gl	= pgl.gl;
		gl.setSwapInterval( 1 );
		
        gl.glShadeModel( GL.GL_SMOOTH );              					// Enable Smooth Shading
        gl.glClearColor( 0.0f, 0.0f, 0.0f, 0.5f );						// Black Background
        gl.glClearDepth( 1.0f );										// Depth Buffer Setup
        gl.glEnable( GL.GL_DEPTH_TEST );								// Enables Depth Testing
        gl.glDepthFunc( GL.GL_LEQUAL );									// The Type Of Depth Testing To Do
        gl.glHint( GL.GL_PERSPECTIVE_CORRECTION_HINT, GL.GL_NICEST );	// Really Nice Perspective Calculations
        gl.glDisable( GL.GL_TEXTURE_2D );
        
        // Set up lighting
        gl.glLightfv( GL.GL_LIGHT1, GL.GL_AMBIENT, lightAmbient, 0 );
        gl.glLightfv( GL.GL_LIGHT1, GL.GL_DIFFUSE, lightDiffuse, 0 );
//      gl.glLightfv( GL.GL_LIGHT1, GL.GL_SPECULAR, lightSpecular, 0 );
        gl.glLightfv( GL.GL_LIGHT1, GL.GL_POSITION, lightPosition, 0 );
        gl.glEnable( GL.GL_LIGHTING );
        gl.glEnable( GL.GL_LIGHT1 );
	}
	
	private void initTileSaver ()
	{
		tiler = new TileSaver( this );
	}
	
	private void initImage ()
	{
		srcImage = new PargoliersImage( this, "face.jpg", PargoliersImage.FIT_MODE_CROP );
		dstImage = new PargoliersImage( this, "", PargoliersImage.FIT_MODE_NONE );
	}
	
	private void initParticles ()
	{
		psys = new PargoliersSystem( this );
	}
	
	////////////////////////////////////////////
	// DRAW.
	////////////////////////////////////////////

	public void draw() 
	{
		if( !isTiling )
		{
			psys.update( srcImage.getPixels(), dstImage.getPixels() );
		}
		
		tiler.pre();

		background( 0 );
		
		srcImage.draw();
//		dstImage.draw();
		
		// lights.
		if ( isLightingEnabled )
		{
        	gl.glEnable( GL.GL_LIGHTING );
        	gl.glEnable( GL.GL_LIGHT1 );
		}
        else
        {
        	gl.glDisable( GL.GL_LIGHTING );
        	gl.glDisable( GL.GL_LIGHT1 );
        }
		
		// START DRAW.
		pgl.beginGL();
			gl.glPushMatrix();
			gl.glTranslatef( sceneCenterX, sceneCenterY, sceneCenterZ );
				drawSomething();
			gl.glPopMatrix();
		pgl.endGL();
		
		tiler.post();
		if( !tiler.checkStatus() && isTiling )
		{
			isTiling = false;
		}
		
		if(isRecording) save("export/image" + frameCount + ".png");
	}

	private void drawSomething ()
	{
		
	}
}
