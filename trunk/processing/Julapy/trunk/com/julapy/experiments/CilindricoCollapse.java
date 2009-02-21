package com.julapy.experiments;

////////////////////////////////////////////////////////////////////////////////
//
// Project: 		Julapy
// File: 			CilindricoCollapse.java
// Created by: 		lukaszkarluk, Feb 17, 2009
//
////////////////////////////////////////////////////////////////////////////////

import javax.media.opengl.GL;

import com.julapy.utils.TileSaver;

import processing.opengl.PGraphicsOpenGL;
import processing.core.*;

public class CilindricoCollapse extends PApplet 
{
	PGraphicsOpenGL pgl;
	GL gl;

	float sinLUT[];
	float cosLUT[];	
	float SINCOS_PRECISION=1.0f;
	int SINCOS_LENGTH= (int)(360.0/SINCOS_PRECISION);
	
	TileSaver tiler;
	
	Boolean isRecording	= false;
	Boolean isTiling	= false;
	
	ArcBar arcBar;
	
	public void setup() 
	{
		size( 600, 600, OPENGL );
		colorMode( RGB, 1.0f );
		frameRate( 25 );

		initLookUpTables();
		initOpenGL();
		initTileSaver();
		initArcBars();
	}
	
	////////////////////////////////////////////
	// HANDLERS.
	////////////////////////////////////////////
	
	public void keyPressed()
	{
		if(key == 'r')
		{
			isRecording = !isRecording;
			
			if(isRecording) println("started recording.");
			if(!isRecording) println("stopped recording.");
		}
		
		if( key=='t' )
		{
			isTiling = !isTiling;
			
			tiler.init("Simple"+nf(frameCount,10),10);
		}
	}
	
	////////////////////////////////////////////
	// INIT.
	////////////////////////////////////////////
	
	private void initLookUpTables ()
	{
		sinLUT = new float[SINCOS_LENGTH];
		cosLUT = new float[SINCOS_LENGTH];
		for (int i = 0; i < SINCOS_LENGTH; i++)
		{
			sinLUT[i]= (float)Math.sin( i * DEG_TO_RAD * SINCOS_PRECISION );
			cosLUT[i]= (float)Math.cos( i * DEG_TO_RAD * SINCOS_PRECISION );
		}
	}
	
	private void initOpenGL ()
	{
		pgl	= (PGraphicsOpenGL) g;
		gl	= pgl.gl;
		gl.setSwapInterval( 1 );
	}
	
	private void initTileSaver ()
	{
		tiler = new TileSaver( this );
	}
	
	private void initArcBars ()
	{
		arcBar = new ArcBar( 100, 0, 0 );
	}
	
	////////////////////////////////////////////
	// UPDATE.
	////////////////////////////////////////////

	private void updateArcBars ()
	{
		arcBar.update();
	}

	////////////////////////////////////////////
	// DRAW.
	////////////////////////////////////////////
	
	public void draw() 
	{
		if( !isTiling )
		{
			updateArcBars();
		}
		
		tiler.pre();
		
		background(0);

		// START DRAW.
		pgl.beginGL();

		gl.glPushMatrix();
		gl.glTranslatef
		( 
			width/2,
			width/2,
			-( height / 2.0f ) / PApplet.tan( PApplet.PI * 60 /360.0f )
		);
		
		drawArcBars();
		
		gl.glPopMatrix();
		
		// END DRAW.
		pgl.endGL();
		
		tiler.post();
		if( !tiler.checkStatus() && isTiling )
		{
			isTiling = false;
		}
		
		if(isRecording) save("export/image" + frameCount + ".png");
	}
	
	private void drawArcBars ()
	{
		arcBar.render();
	}
	

	////////////////////////////////////////////
	// ARC BAR CLASS.
	////////////////////////////////////////////
	
	public class ArcBar
	{
		float rx;
		float ry;
		float rz;
		float rInc;
		
		float deg;
		float radius;
		float width;
		float height;
		float scale;
		
		public ArcBar ()
		{
			rx = random( 2 * PI );
			ry = random( 2 * PI );
			rz = random( 2 * PI );
			
			init();
		}
		
		public ArcBar (float rx, float ry, float rz)
		{
			this.rx = rx;
			this.ry = ry;
			this.rz = rz;
			
			init();
		}
		
		public void init ()
		{
			deg 	= 200;
			radius	= 200;
			rInc	= 1;
			width	= 50;
			height	= 50;
			scale	= 1;
		}
		
		public void update ()
		{
			rx += rInc;
			ry += rInc;
			rz += rInc;
		}
		
		public void render ()
		{
			float x1, y1, x2, y2;
			float px1, py1, px2, py2;
			float p, r, g, b, a;
			int ang;
			
			x1 = y1 = x2 = y2 = 0;
			px1 = py1 = px2 = py2 = 0;
			p = r = g = b = a = 0;
			ang = (int)min( deg / SINCOS_PRECISION, SINCOS_LENGTH - 1 );
			
			gl.glPushMatrix();
			
			gl.glRotatef( rx, 1, 0, 0 );
			gl.glRotatef( ry, 0, 1, 0 );
			gl.glRotatef( rz, 0, 0, 1 );
			
			gl.glBegin( GL.GL_QUADS );
			
			for( int i = 0; i<ang; i++ ) 
			{
				p = 1.0f - (float)i / (float)( ang - 1 );
				r = 1;	// red
				g = 1;	// green
				b = 1;	// blue
				a = 1;	// alpha
				
				x1 = cosLUT[i] * ( radius );
				y1 = sinLUT[i] * ( radius );
				
				x2 = cosLUT[i] * ( radius + width );
				y2 = sinLUT[i] * ( radius + width );
				
				gl.glColor4f( r, g, b, a );
				
				if( i == 0  || i == ( ang - 1 ) )
				{
					// draw end faces.
					gl.glVertex3f( x1, y1, 0 );
					gl.glVertex3f( x2, y2, 0 );
					gl.glVertex3f( x2, y2, height );
					gl.glVertex3f( x1, y1, height );
				}
				
				if( i != 0 )
				{
					// draw inner faces.
					gl.glVertex3f( x1, y1, 0 );
					gl.glVertex3f( px1, py1, 0 );
					gl.glVertex3f( px1, py1, height );
					gl.glVertex3f( x1, y1, height );

					// draw outer faces.
					gl.glVertex3f( x2, y2, 0 );
					gl.glVertex3f( px2, py2, 0 );
					gl.glVertex3f( px2, py2, height );
					gl.glVertex3f( x2, y2, height );

					// draw bottom faces.
					gl.glVertex3f( x1, y1, 0 );
					gl.glVertex3f( px1, py1, 0 );
					gl.glVertex3f( px2, py2, 0 );
					gl.glVertex3f( x2, y2, 0 );

					// draw top faces.
					gl.glVertex3f( x1, y1, height );
					gl.glVertex3f( px1, py1, height );
					gl.glVertex3f( px2, py2, height );
					gl.glVertex3f( x2, y2, height );
				}

				px1 = x1;
				py1 = y1;
				px2 = x2;
				py2 = y2;
			}
			
			gl.glEnd();
			
			gl.glPopMatrix();
		}
	}	
}