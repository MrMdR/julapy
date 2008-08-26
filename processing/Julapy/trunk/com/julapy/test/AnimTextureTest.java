package com.julapy.test;

////////////////////////////////////////////////////////////////////////////////
//
// Project: 		Julapy
// File: 			AnimTextureTest.java
// Created by: 		lukaszkarluk, Aug 23, 2008
//
////////////////////////////////////////////////////////////////////////////////

import javax.media.opengl.GL;

import com.julapy.opengl.Primitive;
import com.julapy.opengl.TextureLoader;
import com.sun.tools.doclets.internal.toolkit.util.TextTag;

import processing.opengl.PGraphicsOpenGL;
import processing.video.Movie;
import processing.core.*;

public class AnimTextureTest extends PApplet 
{
	PGraphicsOpenGL pgl;
	GL gl;
	
	TextureLoader texLoader;
	int textureIndex = 0;
	int particleCallList;
	
	Movie mov;
	int movWidth		= 0;
	int movHeight		= 0;
	int movFrameRate	= 25;
	int movIndex		= 0;
	PImage[] frameBuffer;
	

	///////////////////////////////////////////////////
	//
	// SETUP
	//
	///////////////////////////////////////////////////
	
	public void setup() 
	{
		size( 1280, 720, OPENGL );

		frameRate( 25 );

		pgl	= (PGraphicsOpenGL) g;
		gl	= pgl.gl;
		
		initGL();
		initTextureList();
		initVideo();
	}

	///////////////////////////////////////////////////
	//
	// INIT
	//
	///////////////////////////////////////////////////
	
	public void initGL ()
	{
		pgl = (PGraphicsOpenGL) g;
		gl = pgl.gl;

		/* opengl init */
		gl.glShadeModel( GL.GL_SMOOTH );              					// Enable Smooth Shading
		gl.glClearColor( 0, 0, 0, 0.5f );		    					// Black Background
		gl.glClearDepth( 1 );	                      					// Depth Buffer Setup
		gl.glEnable( GL.GL_DEPTH_TEST );								// Enables Depth Testing
		gl.glDepthFunc( GL.GL_LEQUAL );									// The Type Of Depth Testing To Do
		gl.glHint( GL.GL_PERSPECTIVE_CORRECTION_HINT, GL.GL_NICEST );	// Really Nice Perspective Calculations
		gl.glEnable( GL.GL_TEXTURE_2D );
	}
	
	private void initTextureList ()
	{
		particleCallList = gl.glGenLists( 1 );
		gl.glNewList( particleCallList, GL.GL_COMPILE );
			gl.glBegin( GL.GL_QUADS );
			gl.glNormal3f( 0, 0, 1 );
			gl.glTexCoord2f( 0, 1 );	gl.glVertex3f( -0.5f, -0.5f, 0 );
			gl.glTexCoord2f( 0, 0 );	gl.glVertex3f( -0.5f,  0.5f, 0 );
			gl.glTexCoord2f( 1, 0 );	gl.glVertex3f(  0.5f,  0.5f, 0 );
			gl.glTexCoord2f( 1, 1 );	gl.glVertex3f(  0.5f, -0.5f, 0 );
			gl.glEnd();
		gl.glEndList();
	}

	public void initVideo ()
	{
		mov = new Movie( this, "video/jellyfishsky_debug.mov", movFrameRate );
		mov.loop();
		mov.speed( 0 );
		mov.read();

		movWidth = mov.width;
		movHeight = mov.height;

		frameBuffer = new PImage[ (int)(25 * mov.duration()) ];
		
		println( "frameBuffer length :: " + frameBuffer.length + ", movWidth :: " + movWidth + ", movHeight :: " + movHeight );
		
		for( int i=0; i<frameBuffer.length; i++ )
		{
			mov.jump( i/(float)25 );
			mov.read();
			
			frameBuffer[ i ] = mov.get();
			
			if( i % 50 == 0 && i != 0 ) println( i + " frames added to buffer" ); 
		}
		
		mov.dispose();
		mov = null;
		
		texLoader = new TextureLoader( gl, frameBuffer.length );
		
		for( int j=0; j<frameBuffer.length; j++ )
		{
			texLoader.loadTexture( frameBuffer[ j ], false );
		}
		
		frameBuffer = null;
	}
	
	///////////////////////////////////////////////////
	//
	// DRAW
	//
	///////////////////////////////////////////////////
	
	public void draw() 
	{
		background( 0 );

		camera( 0, 0, -1000, 0, 0, 0, 0, 1, 0 );

		pgl.beginGL();

		gl.glEnable(GL.GL_TEXTURE_2D);
		gl.glBindTexture( GL.GL_TEXTURE_2D, texLoader.getTexture( textureIndex ) );
		
		if( ++textureIndex > texLoader.getTextureLength() - 1 )
			textureIndex = 0;
		
		gl.glScalef( 300, 300, 0 );
		gl.glCallList( particleCallList );

		pgl.endGL();
	}
	
	///////////////////////////////////////////////////
	//
	// EVENTS
	//
	///////////////////////////////////////////////////
	
}