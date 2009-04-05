package com.julapy.pargoliers.test;

import com.julapy.pargoliers.PargoliersUtil;

import processing.core.PApplet;
import toxi.geom.Vec3D;

public class BrightnessVectorTest extends PApplet
{
	float increment = 0.02f;
	int reach 		= 10;
	int gridWidth	= 0;
	int gridHeight	= 0;
	float[] gridColors; 
	
	public void setup() 
	{
		size( 600, 600, OPENGL );
		colorMode( RGB, 1.0f );
		frameRate( 25 );
		
		initGrid();
	}
	
	private void initGrid ()
	{
		int i;
		
		gridWidth	= (int)( width / ( reach * 2 + 1 ) );
		gridHeight	= (int)( height / ( reach * 2 + 1 ) );
		
		PargoliersUtil.resX = gridWidth;
		PargoliersUtil.resY = gridHeight;
		
		gridColors = new float[ ( reach * 2 + 1 ) * ( reach * 2 + 1 ) ];
		for( i=0; i<gridColors.length; i++ )
		{
			gridColors[ i ] = random( 1 );
		}
	}
	
	public void keyPressed()
	{
		if( keyCode == 37 )	// left
		{
			moveGridLeft();
		}

		if( keyCode == 39 )	// right
		{
			moveGridRight();
		}
		
		if( keyCode == 38 )	// up
		{
			moveGridUp();
		}
		
		if( keyCode == 40 )	// down
		{
			moveGridDown();
		}
	}
	
	public void draw ()
	{
		background( 0 );
		
//		drawNoiseGrid();
		drawGrid_01();
//		drawGrid_02();
		drawBrightnessVector();
	}
	
	private void drawNoiseGrid ()
	{
		// Optional: adjust noise detail here
		noiseDetail(8, 0.55f);
		  
		loadPixels();

		float xoff = 0.0f;
		  
		for (int x = 0; x < width; x++)
		{
			xoff += increment;   // Increment xoff
			float yoff = 0.0f;   // For every xoff, start yoff at 0
			for (int y = 0; y < height; y++) 
			{
				yoff += increment; // Increment yoff
		      
				float bright = noise( xoff, yoff );

				pixels[x+y*width] = color( bright );
			}
		}
		  
		updatePixels();
	}
	
	private void drawGrid_01 ()
	{
		int x;
		int y;
		int w;
		int h;
		
		noStroke();
		
		w = reach * 2 + 1;
		h = reach * 2 + 1;
		
		for( x=0; x<w; x++ )
		{
			for( y=0; y<h; y++ )
			{
				fill( gridColors[ x + y * w ] );
				rect( x * gridWidth, y * gridHeight, gridWidth, gridHeight );
			}
		}
	}
	
	private void drawGrid_02 ()
	{
		int x;
		int y;
		
		noStroke();
		
		for( x=0; x<width; x+=gridWidth )
		{
			for( y=0; y<height; y+=gridHeight )
			{
				if
				(
					( x == 0 && y == 0 )
				)
				{
					fill( 1 );
				}
				else if
				(
					( x == 0 && y == gridHeight * ( reach * 2 ) )
				)
				{
					fill( 0.5f );
				}
				else
				{
					fill( 0 );
				}
				
				rect( x, y, gridWidth, gridHeight );
			}
		}
	}
	
	private void moveGridLeft ()
	{
		int x;
		int y;
		int w;
		int h;
		
		noStroke();
		
		w = reach * 2 + 1;
		h = reach * 2 + 1;
		
		for( x=0; x<w; x++ )
		{
			for( y=0; y<h; y++ )
			{
				if( x < w - 1 )
				{
					gridColors[ x + y * w ] = gridColors[ ( x + 1 ) + y * w ];
				}
				else
				{
					gridColors[ x + y * w ] = random( 1 );
				}
			}
		}
	}
	
	private void moveGridRight ()
	{
		int x;
		int y;
		int w;
		int h;
		
		noStroke();
		
		w = reach * 2 + 1;
		h = reach * 2 + 1;
		
		for( x=w-1; x>=0; x-- )
		{
			for( y=0; y<h; y++ )
			{
				if( x > 0 )
				{
					gridColors[ x + y * w ] = gridColors[ ( x - 1 ) + y * w ];
				}
				else
				{
					gridColors[ x + y * w ] = random( 1 );
				}
			}
		}
	}
	
	private void moveGridUp ()
	{
		//
	}
	
	private void moveGridDown ()
	{
		//
	}
	
	private void drawBrightnessVector ()
	{
		Vec3D bv;
		
		loadPixels();
		
		PargoliersUtil.getColorValue( (int)(width*0.5f), (int)(height*0.5f), width, height, pixels );
		
		bv = PargoliersUtil.getBrightnessVector( (int)(width*0.5f), (int)(height*0.5f), width, height, pixels, reach );
		bv.scaleSelf( 200 );
		
		float bx;
		float by;
		
		bx = (int)(width*0.5f);
		by = (int)(height*0.5f);
		
		noFill();
		stroke( 1, 0, 0 );
		line( bx, by, bx + bv.x, by + bv.y );
		noStroke();
		fill( 255, 0, 0 );
		ellipseMode( ROUND );
		ellipse( bx + bv.x, by + bv.y, 6, 6 );
	}
}
