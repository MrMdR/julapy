package com.julapy;

////////////////////////////////////////////////////////////////////////////////
//
// Project: 		GradientBackground
// File: 			Gradient.java
// Created by: 		lukaszkarluk, Apr 11, 2008
//
////////////////////////////////////////////////////////////////////////////////

import java.nio.ByteBuffer;
import javax.media.opengl.GL;
import com.sun.opengl.util.BufferUtil;

import damkjer.ocd.Camera;
import processing.opengl.PGraphicsOpenGL;
import processing.core.*;
import toxi.geom.Vec3D;

public class SphereTextures extends PApplet 
{
	PGraphicsOpenGL pgl;
	GL gl;
	
	int[] textures;
	
	Camera cam;
	Vec3D camVec;
	Vec3D camTarget;
	Vec3D camPosition;
	Vec3D camNormal;
	int camDirection = 1;

	float rotationX = 0;
	float rotationY = 0;
	float velocityX = 0;
	float velocityY = 0;

	public void setup()
	{
		size( 1280, 720, OPENGL);

		frameRate(25);

		PImage glow 	= loadImage("data/glow_spheretex.png");
		PImage nebula 	= loadImage("data/nebula_spheretex.png");
		
		pgl = (PGraphicsOpenGL) g;
		gl = pgl.gl;
		
		glGenTextures( gl );
		
        gl.glShadeModel(GL.GL_SMOOTH);              // Enable Smooth Shading
        gl.glClearColor(0.0f, 0.0f, 0.0f, 0.5f);    // Black Background
        gl.glClearDepth(1.0f);                      // Depth Buffer Setup
        gl.glEnable(GL.GL_DEPTH_TEST);							// Enables Depth Testing
        gl.glDepthFunc(GL.GL_LEQUAL);								// The Type Of Depth Testing To Do
        gl.glHint(GL.GL_PERSPECTIVE_CORRECTION_HINT, GL.GL_NICEST);	// Really Nice Perspective Calculations
        gl.glEnable(GL.GL_TEXTURE_2D);
        
        gl.glBindTexture(GL.GL_TEXTURE_2D, textures[0]);
        glTexImage2D( glow, true );
        gl.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_MIN_FILTER, GL.GL_LINEAR);
        gl.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_MAG_FILTER, GL.GL_LINEAR);

        gl.glBindTexture(GL.GL_TEXTURE_2D, textures[1]);
        glTexImage2D( nebula, true );
        gl.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_MIN_FILTER, GL.GL_LINEAR);
        gl.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_MAG_FILTER, GL.GL_LINEAR);
        
//		cam 		= new Camera( this, width/2, height/2, 1200, width/2, height/2, width/2);
//		camVec		= new Vec3D();
//		camTarget 	= new Vec3D();
//		camPosition = new Vec3D();
//		camNormal 	= new Vec3D();
	}
	
    private void glGenTextures (GL gl) {
    	textures = new int[2];
        gl.glGenTextures(2, textures, 0);
    }
    
    private void glTexImage2D( PImage img, Boolean useAlphaChannel )
    {
    	if( useAlphaChannel )
    	{
    		gl.glTexImage2D(GL.GL_TEXTURE_2D, 0, GL.GL_RGBA, img.width, img.height, 0, GL.GL_RGBA, GL.GL_UNSIGNED_BYTE, getTextureByteBuffer( img, true ) );
    	}
    	else
    	{
    		gl.glTexImage2D(GL.GL_TEXTURE_2D, 0, GL.GL_RGB, img.width, img.height, 0, GL.GL_RGB, GL.GL_UNSIGNED_BYTE, getTextureByteBuffer( img, false ) );
    	}
    }
    
    private ByteBuffer getTextureByteBuffer ( PImage img, Boolean useAlphaChannel )
    {
    	int bytesPerPixel = useAlphaChannel ? 4 : 3;
    	
        ByteBuffer unpackedPixels = BufferUtil.newByteBuffer( img.pixels.length * bytesPerPixel );

        for (int row = img.height - 1; row >= 0; row--) {
            for (int col = 0; col < img.width; col++) {
                int packedPixel = img.pixels[row * img.width + col];
                unpackedPixels.put((byte) ((packedPixel >> 16) & 0xFF));
                unpackedPixels.put((byte) ((packedPixel >> 8) & 0xFF));
                unpackedPixels.put((byte) ((packedPixel >> 0) & 0xFF));
                if ( useAlphaChannel ) {
                    unpackedPixels.put((byte) ((packedPixel >> 24) & 0xFF));
                }
            }
        }

        unpackedPixels.flip();
        
        return  unpackedPixels;
    }

	
	public void draw()
	{    
		background(0);
		
		pgl.beginGL();

        gl.glClear(GL.GL_COLOR_BUFFER_BIT | GL.GL_DEPTH_BUFFER_BIT);
        gl.glLoadIdentity();									// Reset The View
        
        gl.glRotatef( -rotationX, 1, 0, 0 );
        gl.glRotatef( 270 - rotationY, 0, 1, 0 );
        
        gl.glPushMatrix();
        gl.glScalef( 100, 100, 100);
        gl.glTranslatef(0.0f, 0.0f, 0.0f);
        gl.glBindTexture(GL.GL_TEXTURE_2D, textures[0]);
        glDrawSphere( 30 );
        gl.glPopMatrix();
        
        gl.glPushMatrix();
        gl.glScalef( 99, 99, 99 );
        gl.glTranslatef(0.0f, 0.0f, 0.0f);
        gl.glBindTexture(GL.GL_TEXTURE_2D, textures[1]);
        glDrawSphere( 30 );
        gl.glPopMatrix();
		
		pgl.endGL();
		
		rotationX += velocityX;
		rotationY += velocityY;
		velocityX *= 0.95;
		velocityY *= 0.95;
	  
		// Implements mouse control (interaction will be inverse when sphere is  upside down)
		if(mousePressed)
		{
			velocityX += (mouseY-pmouseY) * 0.01;
			velocityY -= (mouseX-pmouseX) * 0.01;
		}
	}
	
	public void glDrawSphere ( int n )
	{
	   int i,j;
	   float theta1, theta2, theta3;
	   Vec3D e = new Vec3D();
	   
	   if (n < 0)
	      n = -n;
	   if (n < 4) 
	   {
	      gl.glBegin( GL.GL_POINTS );
	      gl.glColor3f( 1, 1, 1 );
	      gl.glVertex3f( 0.0f, 0.0f, 0.0f);
	      gl.glEnd();
	      
	      return;
	   }

	   for (j=0;j<n/2;j++) 
	   {
	      theta1 = j * 2*PI / n - PI/2;
	      theta2 = (j + 1) * 2*PI / n - PI/2;

	      gl.glBegin( GL.GL_TRIANGLE_STRIP );
	      for (i=0;i<=n;i++) 
	      {
	    	 theta3 = i * 2*PI / n;

	         e.x = cos(theta2) * cos(theta3);
	         e.y = sin(theta2);
	         e.z = cos(theta2) * sin(theta3);

	         gl.glNormal3f( e.x, e.y, e.z );
	         gl.glTexCoord2f( i/(float)n, 2*(j+1)/(float)n);
	         gl.glVertex3f( e.x, e.y, e.z);

	         e.x = cos(theta1) * cos(theta3);
	         e.y = sin(theta1);
	         e.z = cos(theta1) * sin(theta3);

	         gl.glNormal3f( e.x, e.y, e.z );
	         gl.glTexCoord2f( i/(float)n, 2*j/(float)n );
	         gl.glVertex3f(  e.x, e.y, e.z );
	      }
	      gl.glEnd();
	   }
	}	
	
}