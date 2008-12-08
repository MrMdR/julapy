package com.julapy.experiments;

////////////////////////////////////////////////////////////////////////////////
//
// Project: 		Julapy_Patterns2D
// File: 			Pattern_01.java
// Created by: 		lukaszkarluk, Jul 27, 2008
//
////////////////////////////////////////////////////////////////////////////////

import processing.core.*;

public class Pattern_01 extends PApplet 
{
	Pattern p;
	
	public void setup() 
	{
		size( 1280, 720 );
		frameRate( 25 );
		smooth();
		
		p = new Pattern( );
	}
	
	public void draw() 
	{
		background( 0 );
		
		translate( 100, 100 );
		for( int i=0; i<10; i++ )
		{
			translate( 5, 0 );
			rotate( PI * 0.05f );
			p.draw();
		}
	}
	
	class Pattern
	{
		Point[] points;
		
		public Pattern()
		{
			float baseWidth		= random( 2, 10 );
			float baseHeight	= random( 20, 40 );
			
			points		= new Point[4];
			points[ 0 ]	= new Point(  baseWidth * 0.5f, 0.0f );
			points[ 1 ]	= new Point( -baseWidth * 0.5f, 0.0f );
			points[ 2 ]	= new Point( -baseWidth * 0.5f - random( 2, 20 ), -baseHeight - random( 2, 30 ) );
			points[ 3 ]	= new Point(  baseWidth * 0.5f + random( 2, 20 ), -baseHeight - random( 2, 30 ) );
		}
		
		public void draw ()
		{
			noStroke();
			fill( 255, 0, 0, 100 );
			beginShape( QUADS );
			for( int i=0; i<points.length; i++ )
			{
				vertex( points[i].x, points[i].y );
			}
			endShape();
		}
	}
	
	class Point
	{
		float x;
		float y;
		
		public Point( float x, float y )
		{
			this.x = x;
			this.y = y;
		}
	}
}