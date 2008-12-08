package com.julapy.bats;

import processing.core.PApplet;
import processing.core.PImage;
import processing.video.Capture;

public class BlobDetectionTest extends PApplet
{
	Capture cap;
	
	int capWidth = 320;
	int capHeight = 240;
	
	public void setup() 
	{
		size( capWidth, capHeight );

		frameRate( 60 );
		
		cap = new Capture( this, capWidth, capHeight, 25 );
	}

	public void draw() 
	{
//		background( 0 );
		
		if( cap.available() )
		{
			cap.read();
			
//			PImage capImage = cap.get();
			
			image( cap, cap.width, cap.height );
		}
	}
}
