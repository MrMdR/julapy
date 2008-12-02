package com.julapy.bats;

import javax.media.opengl.GL;

import processing.core.PApplet;
import processing.opengl.PGraphicsOpenGL;

public class Main extends PApplet
{
	PGraphicsOpenGL pgl;
	GL gl;
	
	public void setup()
	{
		size( 1280, 720, OPENGL );
		
		frameRate( 25 );
		
		colorMode( RGB, 1.0f );
		
		hint( ENABLE_OPENGL_4X_SMOOTH );
	}
	
	public void draw ()
	{
		
	}
	
	
	static public void main( String args[] )
	{
		PApplet.main( new String[] { "com.julapy.bats.Main" } );
	}
}
