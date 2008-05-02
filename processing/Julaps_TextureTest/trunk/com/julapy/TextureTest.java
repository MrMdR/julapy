package com.julapy;

////////////////////////////////////////////////////////////////////////////////
//
// Project: 		Julaps_Textures
// File: 			TextureTest.java
// Created by: 		lukaszkarluk, Apr 29, 2008
//
////////////////////////////////////////////////////////////////////////////////

import java.io.IOException;

import javax.media.opengl.GL;
import javax.media.opengl.glu.GLU;

import com.sun.opengl.util.BufferUtil;
import java.nio.ByteBuffer;

import processing.opengl.PGraphicsOpenGL;
import processing.core.*;
import toxi.geom.Vec3D;

public class TextureTest extends PApplet {
	
	PGraphicsOpenGL pgl;
	GL gl;
	
	GLU glu = new GLU();

	PImage img;
	
	int texture;
    float xrot;				// X Rotation ( NEW )
    float yrot;				// Y Rotation ( NEW )
    float zrot;				// Z Rotation ( NEW )

	public void setup() 
	{
		size(800, 800, OPENGL);

		frameRate(25);

		img = loadImage("data/exp.png");
		
		pgl = (PGraphicsOpenGL) g;
		gl = pgl.gl;
		
        gl.glShadeModel(GL.GL_SMOOTH);              // Enable Smooth Shading
        gl.glClearColor(0.0f, 0.0f, 0.0f, 0.5f);    // Black Background
        gl.glClearDepth(1.0f);                      // Depth Buffer Setup
        gl.glEnable(GL.GL_DEPTH_TEST);							// Enables Depth Testing
        gl.glDepthFunc(GL.GL_LEQUAL);								// The Type Of Depth Testing To Do
        gl.glHint(GL.GL_PERSPECTIVE_CORRECTION_HINT, GL.GL_NICEST);	// Really Nice Perspective Calculations
        gl.glEnable(GL.GL_TEXTURE_2D);
        texture = genTexture(gl);
        gl.glBindTexture(GL.GL_TEXTURE_2D, texture);
        gl.glTexImage2D(GL.GL_TEXTURE_2D, 0, GL.GL_RGB, img.width, img.height, 0, GL.GL_RGB, GL.GL_UNSIGNED_BYTE, getTextureByteBuffer( false ) );
        gl.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_MIN_FILTER, GL.GL_LINEAR);
        gl.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_MAG_FILTER, GL.GL_LINEAR);
	}
	
    private int genTexture(GL gl) {
        final int[] tmp = new int[1];
        gl.glGenTextures(1, tmp, 0);
        return tmp[0];
    }
    
    private ByteBuffer getTextureByteBuffer ( Boolean useAlphaChannel )
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
        gl.glScalef( 100, 100, 100);
        gl.glTranslatef(0.0f, 0.0f, -5.0f);

        gl.glRotatef(xrot, 1.0f, 0.0f, 0.0f);
        gl.glRotatef(yrot, 0.0f, 1.0f, 0.0f);
        gl.glRotatef(zrot, 0.0f, 0.0f, 1.0f);

        gl.glBindTexture(GL.GL_TEXTURE_2D, texture);
		
		glDrawCube();
		glDrawSphere( 1, 30 );
		
		pgl.endGL();
		
        xrot += 0.3f;
        yrot += 0.2f;
        zrot += 0.4f;
	}
	
	public void glDrawCube ()
	{
        gl.glBegin(GL.GL_QUADS);
        // Front Face
        gl.glTexCoord2f(0.0f, 0.0f);
        gl.glVertex3f(-1.0f, -1.0f, 1.0f);
        gl.glTexCoord2f(1.0f, 0.0f);
        gl.glVertex3f(1.0f, -1.0f, 1.0f);
        gl.glTexCoord2f(1.0f, 1.0f);
        gl.glVertex3f(1.0f, 1.0f, 1.0f);
        gl.glTexCoord2f(0.0f, 1.0f);
        gl.glVertex3f(-1.0f, 1.0f, 1.0f);
        // Back Face
        gl.glTexCoord2f(1.0f, 0.0f);
        gl.glVertex3f(-1.0f, -1.0f, -1.0f);
        gl.glTexCoord2f(1.0f, 1.0f);
        gl.glVertex3f(-1.0f, 1.0f, -1.0f);
        gl.glTexCoord2f(0.0f, 1.0f);
        gl.glVertex3f(1.0f, 1.0f, -1.0f);
        gl.glTexCoord2f(0.0f, 0.0f);
        gl.glVertex3f(1.0f, -1.0f, -1.0f);
        // Top Face
        gl.glTexCoord2f(0.0f, 1.0f);
        gl.glVertex3f(-1.0f, 1.0f, -1.0f);
        gl.glTexCoord2f(0.0f, 0.0f);
        gl.glVertex3f(-1.0f, 1.0f, 1.0f);
        gl.glTexCoord2f(1.0f, 0.0f);
        gl.glVertex3f(1.0f, 1.0f, 1.0f);
        gl.glTexCoord2f(1.0f, 1.0f);
        gl.glVertex3f(1.0f, 1.0f, -1.0f);
        // Bottom Face
        gl.glTexCoord2f(1.0f, 1.0f);
        gl.glVertex3f(-1.0f, -1.0f, -1.0f);
        gl.glTexCoord2f(0.0f, 1.0f);
        gl.glVertex3f(1.0f, -1.0f, -1.0f);
        gl.glTexCoord2f(0.0f, 0.0f);
        gl.glVertex3f(1.0f, -1.0f, 1.0f);
        gl.glTexCoord2f(1.0f, 0.0f);
        gl.glVertex3f(-1.0f, -1.0f, 1.0f);
        // Right face
        gl.glTexCoord2f(1.0f, 0.0f);
        gl.glVertex3f(1.0f, -1.0f, -1.0f);
        gl.glTexCoord2f(1.0f, 1.0f);
        gl.glVertex3f(1.0f, 1.0f, -1.0f);
        gl.glTexCoord2f(0.0f, 1.0f);
        gl.glVertex3f(1.0f, 1.0f, 1.0f);
        gl.glTexCoord2f(0.0f, 0.0f);
        gl.glVertex3f(1.0f, -1.0f, 1.0f);
        // Left Face
        gl.glTexCoord2f(0.0f, 0.0f);
        gl.glVertex3f(-1.0f, -1.0f, -1.0f);
        gl.glTexCoord2f(1.0f, 0.0f);
        gl.glVertex3f(-1.0f, -1.0f, 1.0f);
        gl.glTexCoord2f(1.0f, 1.0f);
        gl.glVertex3f(-1.0f, 1.0f, 1.0f);
        gl.glTexCoord2f(0.0f, 1.0f);
        gl.glVertex3f(-1.0f, 1.0f, -1.0f);
        gl.glEnd();
	}
	
	public void glDrawSphere ( float r, int n )
	{
	   int i,j;
	   float theta1, theta2, theta3;
	   Vec3D e = new Vec3D();
	   Vec3D p = new Vec3D();
	   
	   if (r < 0)
	      r = -r;
	   if (n < 0)
	      n = -n;
	   if (n < 4 || r <= 0) 
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
	         p.x = r * e.x;
	         p.y = r * e.y;
	         p.z = r * e.z;

	         gl.glNormal3f( e.x, e.y, e.z );
	         gl.glTexCoord2f( i/(float)n, 2*(j+1)/(float)n);
	         gl.glVertex3f(p.x,p.y,p.z);

	         e.x = cos(theta1) * cos(theta3);
	         e.y = sin(theta1);
	         e.z = cos(theta1) * sin(theta3);
	         p.x = r * e.x;
	         p.y = r * e.y;
	         p.z = r * e.z;

	         gl.glNormal3f(e.x,e.y,e.z);
	         gl.glTexCoord2f( i/(float)n, 2*j/(float)n );
	         gl.glVertex3f(p.x,p.y,p.z);
	      }
	      gl.glEnd();
	   }
	}
}