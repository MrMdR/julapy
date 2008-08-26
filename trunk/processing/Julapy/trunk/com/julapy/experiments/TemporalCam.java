package com.julapy.experiments;

////////////////////////////////////////////////////////////////////////////////
//
// Project: 		Julaps_SlitScanning
// File: 			Julaps_TemporalCam.java
// Created by: 		lukaszkarluk, May 31, 2008
//
////////////////////////////////////////////////////////////////////////////////

import java.util.ArrayList;

import processing.core.*;
import processing.video.Capture;

public class TemporalCam extends PApplet 
{
	static int DIR_X = 1;
	static int DIR_Y = 2;
	
	Capture cap;
	ArrayList<PImage> frameBuffer;
	int frameBufferLimit = 320;
	int frameRate = 25;
	boolean isRecording = false;
	int dir;
	int capWidth = 320;
	int capHeight = 240;
	
	public void setup() 
	{
		size( capWidth, capHeight );

		frameRate( 60 );
		
		frameBuffer = new ArrayList<PImage>();
		
		cap = new Capture( this, capWidth, capHeight, frameRate );
		
		dir = DIR_Y;
		
		if( dir == DIR_X ) frameBufferLimit = capWidth;
		if( dir == DIR_Y ) frameBufferLimit = capHeight;
		
		println("capture device initialised.");
	}

	public void draw() 
	{
		if( cap.available() )
		{
			cap.read();
			
			PImage capImage = cap.get();
			
			frameBuffer.add( capImage );
			if( frameBuffer.size() > frameBufferLimit )
				frameBuffer.remove( 0 );
			
			println("frame buffer size :: " + frameBuffer.size());
		}
		
		drawSlits();
	}
	
	public void drawSlits ()
	{
		int frameIndex = 0;
		if( dir == DIR_X )
		{
			for( int i=0; i<width; i++ )
			{
				image( frameBuffer.get(frameIndex).get( i, 0, 1, height ), i, 0 );
				if( ++frameIndex > frameBuffer.size() - 1 )
					frameIndex = 0;
			}
		}
		if( dir == DIR_Y )
		{
			for( int i=0; i<height; i++ )
			{
				image( frameBuffer.get(frameIndex).get( 0, i, width, 1 ), 0, i );
				if( ++frameIndex > frameBuffer.size() - 1 )
					frameIndex = 0;
			}
		}
	}
}