package com.julapy.test;

////////////////////////////////////////////////////////////////////////////////
//
// Project: 		Julapy
// File: 			TileSaverTest.java
// Created by: 		lukaszkarluk, Feb 11, 2009
//
////////////////////////////////////////////////////////////////////////////////

import javax.media.opengl.GL;

import com.julapy.utils.TileSaver;

import processing.opengl.PGraphicsOpenGL;
import processing.core.*;

public class TileSaverTest extends PApplet 
{
	PGraphicsOpenGL pgl;
	GL gl;
	TileSaver tiler;

	public void setup() 
	{
		size(500,500, OPENGL);
		noStroke();
		tiler = new TileSaver( this );
	}

	public void draw ()
	{
		if(tiler==null) return; // Not initialized

		// call aTileSaver.pre() to prepare frame and setup camera if it exists.
		tiler.pre();

		smooth();
		background(0,50,0);
		lights();

		translate(width/2,height/2);
		rotateY(PI/4);
		rotateX(PI/4);
		scale(8);
		fill(240,255,0,220);
		box(10,50,10);
		fill(150,255,0,220);
		box(50,10,10);
		fill(255,150,0,220);
		box(10,10,50);

		// call aTileSaver.post() to update tiles if tiler is active
		tiler.post();
	}

	//	 Saves tiled imaged when 't' is pressed
	public void keyPressed() 
	{
		if(key=='t') tiler.init("Simple"+nf(frameCount,10),10);
	}
}