package com.julapy.opengl;

import javax.media.opengl.GL;

public class Overlay 
{
	GL gl;
	int width;
	int height;
	
	public Overlay( GL gl, int width, int height )
	{
		this.gl 	= gl;
		this.width	= width;
		this.height	= height;
	}
	  
	public void setOrtho()
	{
		//
		// save old MVP matrices and set ortho mode for screen size (pixels)
		// warning. after drawing you must call unsetOrtho() call
		//
		 
	    // Setting orthographic projection.  
		gl.glMatrixMode( GL.GL_PROJECTION );
		gl.glPushMatrix();    
		gl.glLoadIdentity();
		gl.glOrtho( 0.0, width, 0.0, height, -1.0, 1.0 );
		gl.glMatrixMode( GL.GL_MODELVIEW );
		gl.glPushMatrix();    
		gl.glLoadIdentity();
	}
	  
	public void unsetOrtho()
	{
		// restore old matrices MVP
		gl.glMatrixMode( GL.GL_PROJECTION );
		gl.glPopMatrix();  
		gl.glMatrixMode( GL.GL_MODELVIEW );
		gl.glPopMatrix();      
	}
	  
	public void draw( int texID )
	{
		// bind texture
		gl.glBindTexture( GL.GL_TEXTURE_2D, texID );
	    
		gl.glBegin( GL.GL_QUADS );
	    
		gl.glNormal3f( 0.0f, 0.0f, 1.0f );
		gl.glColor4f( 1, 1, 1, 1 );
	    
	    gl.glTexCoord2f( 0.0f, 0.0f );
	    gl.glVertex2f( 0.0f, 0.0f );
	 
	    gl.glTexCoord2f( 1.0f, 0.0f );
	    gl.glVertex2f( width, 0.0f );
	 
	    gl.glTexCoord2f( 1.0f, 1.0f );
	    gl.glVertex2f( width, height );
	 
	    gl.glTexCoord2f( 0.0f, 1.0f );
	    gl.glVertex2f( 0.0f, height );
	    gl.glEnd();
	    
	    // unbind texture
	    //_gl.glBindTexture( GL.GL_TEXTURE_2D, 0 );
	}
}
