package com.julapy.opengl;

import java.nio.ByteBuffer;
import javax.media.opengl.GL;
import com.sun.opengl.util.BufferUtil;
import processing.core.PImage;

public class TextureLoader 
{
	GL gl;
	int[] textures;
	int ntextures = 0;
	
	public TextureLoader ( GL gl )
	{
		this.gl = gl;
	}
	
	public void init ()
	{
    	textures = new int[3];
        gl.glGenTextures(3, textures, 0);
	}
	
	public void loadTexture ( PImage texture, Boolean useAlphaChannel )
	{
		gl.glBindTexture(GL.GL_TEXTURE_2D, textures[ntextures]);
		glTexImage2D( texture, useAlphaChannel );
		gl.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_MIN_FILTER, GL.GL_LINEAR);
		gl.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_MAG_FILTER, GL.GL_LINEAR);
		
		++ntextures;
	}
	
	public int getTexture ( int tid )
	{
		return textures[tid];
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
}
