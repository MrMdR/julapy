package com.julapy.bats;

import java.awt.Dimension;
import java.awt.Toolkit;

import com.julapy.blob.BlobDetect;

import blobDetection.Blob;
import blobDetection.BlobDetection;
import blobDetection.EdgeVertex;
import processing.core.PApplet;
import processing.core.PImage;
import processing.video.Capture;

public class BlobDetectionTest extends PApplet
{
	int capWidth	= 320;
	int capHeight	= 240;

	Capture cap;
	BlobDetect bd;
	
	static public void main( String args[] )
	{
		PApplet.main( 	new String[]
	                    {
							"--display=1",
							"--present",
							"--bgcolor=#000000",
							"--present-stop-color=#000000", 
							"com.julapy.bats.Main" 
						} 
		);
	}
	
	public void setup()
	{
//		Dimension screen = Toolkit.getDefaultToolkit().getScreenSize();
//		size( screen.width, screen.height, OPENGL );
		size( 1280, 720, OPENGL );

		frameRate( 25 );
		
		background( 0 );
		
		// The name of the capture device is dependent those
		// plugged into the computer. To get a list of the 
		// choices, uncomment the following line 
		println( Capture.list() );
		
		cap	= new Capture( this, capWidth, capHeight, "USB Video Class Video", 25 );
		bd	= new BlobDetect( this, cap );
	}

	public void draw() 
	{
		background( 0 );
		
		bd.draw();
	}
}
