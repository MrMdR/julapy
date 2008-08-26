package com.julapy.experiments;

////////////////////////////////////////////////////////////////////////////////
//
// Project: 		Julaps_SlitScanning
// File: 			Julaps_SlitScanning.java
// Created by: 		lukaszkarluk, May 19, 2008
//
////////////////////////////////////////////////////////////////////////////////

import processing.core.*;
import processing.video.Capture;

public class SlitScanning extends PApplet 
{
	static int DIR_VERT = 0;
	static int DIR_HORZ = 1;
	
	Capture cap;
	boolean isRecording = false;
	int dir;
	int capWidth = 320;
	int capHeight = 240;
	int imgHeight;
	
	public void setup() 
	{
		dir = DIR_HORZ;
		
		if( dir == DIR_VERT  ) imgHeight = capWidth;
		if( dir == DIR_HORZ  ) imgHeight = capHeight;
		
		size( 800, imgHeight );
		
		frameRate( 25 );

		colorMode(RGB, 1.0f);
		
		background( 0 );
		
		cap = new Capture( this, capWidth, capHeight, 25 );
	}

	public void draw() 
	{
		if( cap.available() )
		{
			cap.read();
			
			PImage pimg;
			
			if( dir == DIR_HORZ )
			{
				pimg = cap.get( capWidth/2, 0, 1, capHeight );
			}
			else if( dir == DIR_VERT )
			{
				pimg = cap.get( 0, capHeight/2, capWidth, 1 );
			}
			else
			{
				pimg = cap.get( capWidth/2, 0, 1, capHeight );	// defualt.
			}

			image( get(), 1, 0 );
			image( pimg, 0, 0 );
		}
		
		if( isRecording ) save("data/slitscan_screenshot"+frameCount+".png");
	}
	
	public void keyPressed()
	{
		if(key == 's')
		{
			save("data/slitscan_export"+frameCount+".png");
		}
		
		if( key == 'r' )
		{
			isRecording = !isRecording;
		}
	}
}