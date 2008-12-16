package com.processinghacks.arcball;

import processing.core.PApplet;

public class ArcBallTest extends PApplet 
{
	ArcBall2 arcball;
	
	float angleDegreesX = 0;
	float angleDegreesY = 0;
	
	public void setup ()
	{
		size( 600, 400, P3D );
		
		arcball = new ArcBall2
		(
			width * 0.5f,
			height * 0.5f,
			-height * 0.5f,
			300,
			this
		);
	}
	
	public void draw() 
	{
		background( 255 );
		
		arcball.update();
		
		translate( width/2, height/2, -height/2 );
		fill( 255, 0, 0, 40 );
		noStroke();
		for( int i=1; i<=5; i++ ) 
		{
			box( 60 * i );
		}
		stroke( 100, 0, 0 );
		noFill();
		box( 300 );
	}
	
	public void keyPressed()
	{
		if( keyCode == 37 )	// left.
		{
			angleDegreesY -= 1;
		}

		if( keyCode == 39 )	// right
		{
			angleDegreesY += 1;
		}
		
		if( keyCode == 38 )	// up
		{
			angleDegreesX += 1;
		}
		
		if( keyCode == 40 )	// down
		{
			angleDegreesX -= 1;
		}
		
		arcball.rotateX( angleDegreesX );
		arcball.rotateY( angleDegreesY );
	}
}
