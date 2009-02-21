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
import toxi.geom.Vec3D;

public class CilindricoCollapse extends PApplet 
{
	PGraphicsOpenGL pgl;
	GL gl;

	Boolean lightingEnabled	= true;
	float[] lightAmbient	= { 0.2f, 0.2f, 0.2f, 1.0f };
	float[] lightDiffuse	= { 1.0f, 0.0f, 0.0f, 1.0f };
	float[] lightSpecular	= { 0.5f, 0.5f, 0.5f, 1.0f };
	float[] lightPosition	= { -1.5f, 1.0f, -4.0f, 1.0f };
	
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
		if( key == 'l' )
		{
			lightingEnabled = !lightingEnabled;
		}
		
		if( key == 'r' )
		{
			isRecording = !isRecording;
			
			if(isRecording)
				println("started recording.");
			else
				println("stopped recording.");
		}
		
		if( key == 't' )
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
		
        gl.glShadeModel( GL.GL_SMOOTH );              					// Enable Smooth Shading
        gl.glClearColor( 0.0f, 0.0f, 0.0f, 0.5f );						// Black Background
        gl.glClearDepth( 1.0f );										// Depth Buffer Setup
        gl.glEnable( GL.GL_DEPTH_TEST );								// Enables Depth Testing
        gl.glDepthFunc( GL.GL_LEQUAL );									// The Type Of Depth Testing To Do
        gl.glHint( GL.GL_PERSPECTIVE_CORRECTION_HINT, GL.GL_NICEST );	// Really Nice Perspective Calculations
        gl.glDisable( GL.GL_TEXTURE_2D );
        
        // Set up lighting
        gl.glLightfv( GL.GL_LIGHT1, GL.GL_AMBIENT, lightAmbient, 0 );
        gl.glLightfv( GL.GL_LIGHT1, GL.GL_DIFFUSE, lightDiffuse, 0 );
        gl.glLightfv( GL.GL_LIGHT1, GL.GL_SPECULAR, lightSpecular, 0 );
        gl.glLightfv( GL.GL_LIGHT1, GL.GL_POSITION, lightPosition, 0 );
        gl.glEnable( GL.GL_LIGHTING );
        gl.glEnable( GL.GL_LIGHT1 );
	}
	
	private void initTileSaver ()
	{
		tiler = new TileSaver( this );
	}
	
	private void initArcBars ()
	{
		arcBar = new ArcBar( 30, 0, 0 );
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
		
		// lights.
		if ( lightingEnabled )
		{
        	gl.glEnable( GL.GL_LIGHTING );
        	gl.glEnable( GL.GL_LIGHT1 );
		}
        else
        {
        	gl.glDisable( GL.GL_LIGHTING );
        	gl.glEnable( GL.GL_LIGHT1 );
        }
		
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
		
		float normLength = 20;
		
		Boolean drawNormals = true;
		Boolean drawQuads	= true;
		
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
			radius	= 300;
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
			Vec3D vtemp, ntemp, n1, n2;
			Vec3D[] vtemps;
			int ang;
			int i, j;
			
			x1 = y1 = x2 = y2 = 0;
			px1 = py1 = px2 = py2 = 0;
			p = r = g = b = a = 0;
			n1 = n2 = new Vec3D();
			ang = (int)min( deg / SINCOS_PRECISION, SINCOS_LENGTH - 1 );
			
			gl.glPushMatrix();
			
			gl.glRotatef( rx, 1, 0, 0 );
			gl.glRotatef( ry, 0, 1, 0 );
			gl.glRotatef( rz, 0, 0, 1 );
			
			for( i = 0; i<ang; i++ ) 
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
				
				// normal pointing inwards.
				n1 = new Vec3D( 1, 0, 0 );
				n1 = n1.rotateAroundAxis( Vec3D.Z_AXIS, i * DEG_TO_RAD );
				
				if( drawQuads )
				{
					gl.glBegin( GL.GL_QUADS );
					
					gl.glColor4f( 0.3f, 0.8f, 0.5f, 1 );
					
					if( ( i == 0 ) || ( i == ( ang - 1 ) ) )
					{
						// draw end faces.
						ntemp = new Vec3D( 0, -1, 0 );
						ntemp = ntemp.rotateAroundAxis( Vec3D.Z_AXIS.copy(), i * DEG_TO_RAD );
						if( i == ( ang - 1 ) )
						{
							ntemp.invert();
						}

						gl.glNormal3f( ntemp.x, ntemp.y, ntemp.z );
						gl.glVertex3f( x1, y1, 0 );
						gl.glVertex3f( x2, y2, 0 );
						gl.glVertex3f( x2, y2, height );
						gl.glVertex3f( x1, y1, height );
					}
					
					if( i != 0 )
					{
						// draw inner faces.
						ntemp = n1.copy().invert();
						gl.glNormal3f( ntemp.x, ntemp.y, ntemp.z );
						gl.glVertex3f( x1, y1, 0 );
						
						ntemp = n2.copy().invert();
						gl.glNormal3f( ntemp.x, ntemp.y, ntemp.z );
						gl.glVertex3f( px1, py1, 0 );
						gl.glVertex3f( px1, py1, height );

						ntemp = n1.copy().invert();
						gl.glNormal3f( ntemp.x, ntemp.y, ntemp.z );
						gl.glVertex3f( x1, y1, height );
	
						// draw outer faces.
						ntemp = n1.copy();
						gl.glNormal3f( ntemp.x, ntemp.y, ntemp.z );
						gl.glVertex3f( x2, y2, 0 );

						ntemp = n2.copy();
						gl.glNormal3f( ntemp.x, ntemp.y, ntemp.z );
						gl.glVertex3f( px2, py2, 0 );
						gl.glVertex3f( px2, py2, height );
						
						ntemp = n1.copy();
						gl.glNormal3f( ntemp.x, ntemp.y, ntemp.z );
						gl.glVertex3f( x2, y2, height );
	
						// draw bottom faces.
						gl.glNormal3f( 0, 0, -1 );
						gl.glVertex3f( x1, y1, 0 );
						gl.glVertex3f( px1, py1, 0 );
						gl.glVertex3f( px2, py2, 0 );
						gl.glVertex3f( x2, y2, 0 );
	
						// draw top faces.
						gl.glNormal3f( 0, 0, 1 );
						gl.glVertex3f( x1, y1, height );
						gl.glVertex3f( px1, py1, height );
						gl.glVertex3f( px2, py2, height );
						gl.glVertex3f( x2, y2, height );
					}
					
					gl.glEnd();
				}
				
				if( drawNormals )
				{
					gl.glBegin( GL.GL_LINES );
					
					if( ( i == 0 ) || ( i == ( ang - 1 ) ) )
					{
						ntemp = new Vec3D( 0, -1, 0 );
						ntemp = ntemp.rotateAroundAxis( Vec3D.Z_AXIS.copy(), i * DEG_TO_RAD );
						if( i == ( ang - 1 ) )
						{
							ntemp.invert();
						}
						vtemps		= new Vec3D[ 4 ];
						vtemps[ 0 ]	= new Vec3D( x1, y1, 0 );
						vtemps[ 1 ]	= new Vec3D( x2, y2, 0 );
						vtemps[ 2 ]	= new Vec3D( x2, y2, height );
						vtemps[ 3 ]	= new Vec3D( x1, y1, height );

						for( j=0; j<vtemps.length; j++ )
						{
							vtemp = vtemps[ j ];
							
							gl.glVertex3f( vtemp.x, vtemp.y, vtemp.z );
							gl.glVertex3f
							(
								vtemp.x + ntemp.x * normLength,
								vtemp.y + ntemp.y * normLength, 
								vtemp.z + ntemp.z * normLength
							);
						}
					}
					
					if( ( i != 0 ) && ( i % 20 == 0 ) )
					{
						// draw inner face normals.
						ntemp		= n1.copy().invert();
						vtemps		= new Vec3D[ 4 ];
						vtemps[ 0 ]	= new Vec3D( x1, y1, 0 );
						vtemps[ 1 ]	= new Vec3D( px1, py1, 0 );
						vtemps[ 2 ]	= new Vec3D( px1, py1, height );
						vtemps[ 3 ]	= new Vec3D( x1, y1, height );
						
						for( j=0; j<vtemps.length; j++ )
						{
							vtemp = vtemps[ j ];
							
							gl.glVertex3f( vtemp.x, vtemp.y, vtemp.z );
							gl.glVertex3f
							(
								vtemp.x + ntemp.x * normLength,
								vtemp.y + ntemp.y * normLength, 
								vtemp.z + ntemp.z * normLength
							);
						}
						
						// draw outer face normals.
						ntemp		= n1.copy();
						vtemps		= new Vec3D[ 4 ];
						vtemps[ 0 ]	= new Vec3D( x2, y2, 0 );
						vtemps[ 1 ]	= new Vec3D( px2, py2, 0 );
						vtemps[ 2 ]	= new Vec3D( px2, py2, height );
						vtemps[ 3 ]	= new Vec3D( x2, y2, height );
						
						for( j=0; j<vtemps.length; j++ )
						{
							vtemp = vtemps[ j ];
							
							gl.glVertex3f( vtemp.x, vtemp.y, vtemp.z );
							gl.glVertex3f
							(
								vtemp.x + ntemp.x * normLength,
								vtemp.y + ntemp.y * normLength, 
								vtemp.z + ntemp.z * normLength
							);
						}
						
						// draw bottom face normals.
						ntemp		= new Vec3D( 0, 0, -1 );
						vtemps		= new Vec3D[ 4 ];
						vtemps[ 0 ]	= new Vec3D( x1, y1, 0 );
						vtemps[ 1 ]	= new Vec3D( px1, py1, 0 );
						vtemps[ 2 ]	= new Vec3D( px2, py2, 0 );
						vtemps[ 3 ]	= new Vec3D( x2, y2, 0 );
						
						for( j=0; j<vtemps.length; j++ )
						{
							vtemp = vtemps[ j ];
							
							gl.glVertex3f( vtemp.x, vtemp.y, vtemp.z );
							gl.glVertex3f
							(
								vtemp.x + ntemp.x * normLength,
								vtemp.y + ntemp.y * normLength, 
								vtemp.z + ntemp.z * normLength
							);
						}
						
						// draw top face normals.
						ntemp		= new Vec3D( 0, 0, 1 );
						vtemps		= new Vec3D[ 4 ];
						vtemps[ 0 ]	= new Vec3D( x1, y1, height );
						vtemps[ 1 ]	= new Vec3D( px1, py1, height );
						vtemps[ 2 ]	= new Vec3D( px2, py2, height );
						vtemps[ 3 ]	= new Vec3D( x2, y2, height );
						
						for( j=0; j<vtemps.length; j++ )
						{
							vtemp = vtemps[ j ];
							
							gl.glVertex3f( vtemp.x, vtemp.y, vtemp.z );
							gl.glVertex3f
							(
								vtemp.x + ntemp.x * normLength,
								vtemp.y + ntemp.y * normLength, 
								vtemp.z + ntemp.z * normLength
							);
						}
					}
					
					gl.glEnd();
				}
				
				px1 = x1;
				py1 = y1;
				px2 = x2;
				py2 = y2;
				n2  = n1;
			}
			
			gl.glPopMatrix();
		}
	}	
}