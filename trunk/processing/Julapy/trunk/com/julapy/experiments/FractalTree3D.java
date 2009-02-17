package com.julapy.experiments;

////////////////////////////////////////////////////////////////////////////////
//
// Project: 		Julaps_TexturedFractal
// File: 			TexturedFractal.java
// Created by: 		lukaszkarluk, Apr 19, 2008
//
////////////////////////////////////////////////////////////////////////////////

import javax.media.opengl.GL;

import damkjer.ocd.Camera;

import processing.core.*;
import processing.opengl.PGraphicsOpenGL;
import toxi.geom.Vec3D;

public class FractalTree3D extends PApplet 
{
	PGraphicsOpenGL pgl;
	GL gl;
	int texList;

	PImage texture;
	
	Camera cam;
	Vec3D camVec;
	Vec3D camTarget;
	Vec3D camPosition;
	Vec3D camNormal;
	int camDirection = 1;
	
	float bx;		// branch x.
	float by;		// branch y.
	float ba;		// branch angle.
	float bh = 200;	// original branch height.
	int brachSplit = 3;
	
	boolean isRecording = false;
	int imageCount;
	
	int mx = 0;
	
	public void setup() 
	{
		size( 1280, 720, OPENGL );

		frameRate( 25 );
		
		hint( ENABLE_OPENGL_4X_SMOOTH );
		colorMode( RGB, 1.0f );
		
		pgl = (PGraphicsOpenGL) g;
		gl  = pgl.gl;
		
		texture = loadImage( "/texture/magnet.png" );

		gl.glDepthMask(false);
		gl.glEnable( GL.GL_DEPTH_TEST );
		gl.glEnable( GL.GL_TEXTURE_2D );
		pgl.texture( texture );
		
		initTextureList();
		
		cam 		= new Camera( this, width/2, height/2, 1500, width/2, height/2, width/2);
		camVec		= new Vec3D();
		camTarget 	= new Vec3D();
		camPosition = new Vec3D();
		camNormal 	= new Vec3D();
	}
	
	private void initTextureList ()
	{
		texList = gl.glGenLists( 1 );
		gl.glNewList( texList, GL.GL_COMPILE );
			gl.glBegin( GL.GL_TRIANGLES );
			gl.glTexCoord2f( 0.5f, 0.0f );	gl.glVertex3f(  0,  0,  0 );
			gl.glTexCoord2f( 0.0f, 1.0f );	gl.glVertex3f( -1,  1,  0 );
			gl.glTexCoord2f( 1.0f, 1.0f );	gl.glVertex3f(  1,  1,  0 );
			gl.glEnd();
		gl.glEndList();
	}
	
	public void keyPressed()
	{
		if(key == 'r')
		{
			isRecording = !isRecording;
			
			if(isRecording) println("started recording.");
			if(!isRecording) println("stopped recording.");
		}
	}

	public void draw() 
	{
		background(0);
		
		cam.circle( radians( ( 1 * camDirection ) ) );
		cam.feed();
		
		mx += 3;
		
		ba = radians( ( mx / (float)width ) * 100f );
		bx = (float)Math.cos( ba - Math.PI/2 );
		by = (float)Math.sin(-ba - Math.PI/2 );

		pgl.beginGL();
		
		gl.glPushMatrix();
		gl.glTranslatef( width/2, height/2, width/2 );
		
		int astep = brachSplit;
		for( int i=0; i<astep; i++)
		{
			// right branch.
			gl.glPushMatrix();
			gl.glRotatef( i*360/astep, 0, 1, 0 );
			gl.glRotatef( degrees(ba), 0, 0, 1 );
			gl.glRotatef( 90, 0, 1, 0 );
			renderTree( 1 ); // scale down next branch.
			gl.glPopMatrix();
		}
		gl.glPopMatrix();

		pgl.endGL();
		
		if(isRecording) save("export/image" + imageCount++ + ".png");
	}
	
	public void renderTree ( float bs )
	{
		if( bs > 0.4 )
		{
			gl.glPushMatrix();
			gl.glScalef( bx, by, 0 );
			gl.glScalef( bh*bs, bh*bs, bh*bs );
			gl.glCallList( texList );
			gl.glPopMatrix();
			
			bs *= 0.8f;
			
			int astep = brachSplit;
			for( int i=0; i<astep; i++)
			{
				// right branch.
				gl.glPushMatrix();
				gl.glTranslatef( bx*bs*bh, by*bs*bh, 0 );
				gl.glRotatef( i*360/astep, 0, 1, 0 );
				gl.glRotatef( degrees(ba), 0, 0, 1 );
				gl.glRotatef( 90, 0, 1, 0 );
				renderTree( bs ); // scale down next branch.
				gl.glPopMatrix();
				
				// right branch.
				gl.glPushMatrix();
				gl.glTranslatef( -bx*bs*bh, by*bs*bh, 0 );
				gl.glRotatef( i*360/astep, 0, 1, 0 );
				gl.glRotatef( -degrees(ba), 0, 0, 1 );
				gl.glRotatef( 90, 0, 1, 0 );
				renderTree( bs ); // scale down next branch.
				gl.glPopMatrix();
			}
		}
	}
}