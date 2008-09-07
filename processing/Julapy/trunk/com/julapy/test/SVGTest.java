package com.julapy.test;

////////////////////////////////////////////////////////////////////////////////
//
// Project: 		Julapy
// File: 			SVGTest.java
// Created by: 		lukaszkarluk, Aug 29, 2008
//
////////////////////////////////////////////////////////////////////////////////

import processing.candy.SVG;
import processing.core.*;

public class SVGTest extends PApplet 
{
	String[] svgFileList = { "svg/bird.svg" };
	SVG svgList[];
	
	public void setup() 
	{
		size( 1200, 780, "prosvg.SVGOut" );

		smooth( );
		frameRate( 25 );
		
		noLoop( );
		
		initSVG( );
	}
	
	public void initSVG ()
	{
		SVG svg;
		int i;
		
		svgList = new SVG[ svgFileList.length ];
		
		for( i=0; i<svgFileList.length; i++ )
		{
			svg = new SVG( this, "svg/bird.svg" );
			svg.drawMode( CENTER );
			svgList[ i ] = svg;
		}
	}

	public void draw() 
	{
		background( 0 );

		svgList[ 0 ].draw( width/2, height/2 );
		
		save( "data/export/svgTest.svg" );
	}
}