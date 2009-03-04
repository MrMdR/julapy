package com.julapy.experiments;

////////////////////////////////////////////////////////////////////////////////
//
// Project: 		Julapy
// File: 			CilindricoCollapse.java
// Created by: 		lukaszkarluk, Feb 17, 2009
//
////////////////////////////////////////////////////////////////////////////////

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import javax.media.opengl.GL;

import com.julapy.noise.PerlinStep;
import com.julapy.utils.TileSaver;

import processing.opengl.PGraphicsOpenGL;
import processing.core.*;
import toxi.geom.Matrix4x4;
import toxi.geom.Quaternion;
import toxi.geom.Vec3D;

public class CilindricoCollapse extends PApplet 
{
	PGraphicsOpenGL pgl;
	GL gl;

	Boolean lightingEnabled	= true;
	float[] lightAmbient	= { 0.1f, 0.1f, 0.1f, 1.0f };
	float[] lightDiffuse	= { 0.7f, 0.7f, 0.7f, 1.0f };
	float[] lightSpecular	= { 0.5f, 0.5f, 0.5f, 1.0f };
	float[] lightPosition	= { -1.5f, 1.0f, -4.0f, 1.0f };
	
	float sinLUT[];
	float cosLUT[];	
	float SINCOS_PRECISION=1.0f;
	int SINCOS_LENGTH= (int)(360.0/SINCOS_PRECISION);
	
	float sceneCenterX	= 0;
	float sceneCenterY	= 0;
	float sceneCenterZ	= 0;
	float sceneRotX 	= 0;
	float sceneRotZ 	= 0;
	
	TileSaver tiler;
	
	Boolean isRecording	= false;
	Boolean isTiling	= false;
	
	ArcBar[] arcBars;

//	float[][] colorPalette = 
//	{ 
//		{ 0.96f, 0.00f, 0.64f, 1.00f },		// velvet blue.
//		{ 0.00f, 0.05f, 1.00f, 1.00f },		// fuchsia.
//	};
	
//	float[][] colorPalette = 
//	{ 
//		{ 1.00f, 0.76f, 0.06f, 1.00f },		// yellow.
//		{ 0.90f, 0.18f, 0.04f, 1.00f },		// red.
//		{ 0.88f, 0.44f, 0.13f, 1.00f },		// orange.
//		{ 0.74f, 0.33f, 0.05f, 1.00f },		// orange dark.
//		{ 0.42f, 0.45f, 0.03f, 1.00f }		// olive dark.
//	};

	float[][] colorPalette = 
	{ 
		{ 0.13f, 0.21f, 0.23f, 1.00f },		// dark green 01.
		{ 0.20f, 0.25f, 0.26f, 1.00f },		// dark green 02.
		{ 0.18f, 0.30f, 0.33f, 1.00f },		// green.
		{ 0.32f, 0.42f, 0.45f, 1.00f },		// green / grey.
		{ 0.28f, 0.28f, 0.26f, 1.00f },		// brown.
		{ 0.75f, 0.67f, 0.56f, 1.00f },		// light brown.
	};
	
	public void setup() 
	{
		size( 600, 600, OPENGL );
		colorMode( RGB, 1.0f );
		frameRate( 25 );

		initLookUpTables();
		initScene();
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
			
			tiler.init("Simple"+nf(frameCount,16),16);
		}
		
		if( keyCode == 37 )	// left.
		{
			sceneRotZ -= 1 * DEG_TO_RAD;
		}

		if( keyCode == 39 )	// right
		{
			sceneRotZ += 1 * DEG_TO_RAD;
		}
		
		if( keyCode == 38 )	// up
		{
			sceneRotX += 1 * DEG_TO_RAD;
		}
		
		if( keyCode == 40 )	// down
		{
			sceneRotX -= 1 * DEG_TO_RAD;
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
	
	private void initScene ()
	{
		sceneCenterX = width * 0.5f;
		sceneCenterY = width * 0.5f;
		sceneCenterZ = -( height * 0.5f ) / tan( PI * 60 / 360.0f );
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
//      gl.glLightfv( GL.GL_LIGHT1, GL.GL_SPECULAR, lightSpecular, 0 );
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
		int arcsTotal		= 200;
		float arcHeight		= 8;
		float arcLocZInc	= 4;
		float arcLocZ 		= -(int)( ( arcsTotal * ( arcHeight + arcLocZInc ) ) * 0.25f );
		float arcAngleMin	= 10;
		float arcAngleMax	= 270;
		
		float arcRadiusMin	= 50;
		float arcRadiusMax	= 500;
		float arcWidthMin	= 50;
		float arcWidthMax	= 80;
		int arcColorIndex	= 0;
		float[] arcColors	= {};
		ArcBar arcBar;
		int i;

		arcBars = new ArcBar[ arcsTotal ];
		
		for( i=0; i<arcsTotal; i++ )
		{
			arcBars[ i ] 	= arcBar = new ArcBar( );
			arcBar.loc.z 	= arcLocZ;
			arcBar.height	= arcHeight;
			arcBar.radius	= random( arcRadiusMin, arcRadiusMax );
			arcBar.width	= random( arcWidthMin, arcWidthMax );
			arcBar.angle	= random( arcAngleMin, arcAngleMax );
			arcBar.rz		= random( 360 );
			
			arcColorIndex	= (int)( random( colorPalette.length ) );
			arcColors		= colorPalette[ arcColorIndex ];
			arcBar.colour	= arcColors;
			
			arcBar.init();
			
			arcLocZ += arcLocZInc;
		}
	}
	
	////////////////////////////////////////////
	// UPDATE.
	////////////////////////////////////////////

	private void updateSceneRotation ()
	{
		sceneRotX += 1 * DEG_TO_RAD;
		sceneRotZ += 1 * DEG_TO_RAD;
	}
	
	private void updateArcBars ()
	{
		for( int i=0; i<arcBars.length; i++ )
		{
			arcBars[ i ].update();
		}
	}

	////////////////////////////////////////////
	// DRAW.
	////////////////////////////////////////////
	
	public void draw() 
	{
		if( !isTiling )
		{
//			updateSceneRotation();
			updateArcBars();
		}
		
		tiler.pre();
		
//		background( 0.59f, 0.18f, 0.47f ); // pinkish
//		background( 0.61f, 0.66f, 0.47f ); // olive.
//		background( 0.80f, 0.75f, 0.69f ); // brown / grey.
//		background( 0.79f, 0.72f, 0.62f ); // brown / grey.
//		background( 0.87f, 0.73f, 0.52f ); // orange / grey.
//		background( 0.58f, 0.69f, 0.74f ); // blue / grey.
		background( 0.82f, 0.83f, 0.78f ); // grey.
//		background( 0.4f ); // greyish.
		
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
		gl.glTranslatef( sceneCenterX, sceneCenterY, sceneCenterZ );
		
		applySceneRotation();
		
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
	
	private void applySceneRotation ()
	{
		Vec3D xrot, zrot;
		Quaternion nowQuat, xrotQuat, zrotQuat;
		Matrix4x4 mtemp;
		float[] m;

		xrot = Vec3D.Y_AXIS.copy().rotateAroundAxis( Vec3D.X_AXIS.copy(), sceneRotX );
		zrot = Vec3D.X_AXIS.copy().rotateAroundAxis( Vec3D.Z_AXIS.copy(), sceneRotZ );
		
		xrotQuat = new Quaternion( 1, new Vec3D( 0, 0, 0 ) );
		xrotQuat.set( xrot.dot( Vec3D.Y_AXIS.copy() ), xrot.cross( Vec3D.Y_AXIS.copy() ) );

		zrotQuat = new Quaternion( 1, new Vec3D( 0, 0, 0 ) );
		zrotQuat.set( zrot.dot( Vec3D.X_AXIS.copy() ), zrot.cross( Vec3D.X_AXIS.copy() ) );
		
		nowQuat = xrotQuat.multiply( zrotQuat );
		nowQuat.normalize();
		mtemp	= nowQuat.getMatrix();
		
		m = new float[]
        {
			(float)mtemp.matrix[0][0], (float)mtemp.matrix[0][1], (float)mtemp.matrix[0][2], (float)mtemp.matrix[0][3],
			(float)mtemp.matrix[1][0], (float)mtemp.matrix[1][1], (float)mtemp.matrix[1][2], (float)mtemp.matrix[1][3],
			(float)mtemp.matrix[2][0], (float)mtemp.matrix[2][1], (float)mtemp.matrix[2][2], (float)mtemp.matrix[2][3],
			(float)mtemp.matrix[3][0], (float)mtemp.matrix[3][1], (float)mtemp.matrix[3][2], (float)mtemp.matrix[3][3]
		};
		
		FloatBuffer rotMatrix;
		rotMatrix = ByteBuffer.allocateDirect(4 * 4 * 4).order(ByteOrder.nativeOrder()).asFloatBuffer();
		rotMatrix.put( m );
		rotMatrix.flip();

		gl.glMultMatrixf( rotMatrix );
	}
	
	private void drawArcBars ()
	{
		for( int i=0; i<arcBars.length; i++ )
		{
			arcBars[ i ].render();
		}
	}
	

	////////////////////////////////////////////
	// ARC BAR CLASS.
	////////////////////////////////////////////
	
	public class ArcBar
	{
		Vec3D loc	= new Vec3D();
		Vec3D vel	= new Vec3D();
		float rx	= 0;
		float ry	= 0;
		float rz	= 0;
		float rInc	= 2;
		
		float[] colour = { 1, 1, 1, 1 };
		
		float angle		= 200;
		int	  angleStep	= 200;
		float radius	= 300;
		float width		= 50;
		float height	= 50;
		float scale		= 1;
		
		PerlinStep ps;
		
		float normLength = 20;
		
		Boolean drawQuads	= true;
		Boolean drawNormals = false;
		Boolean drawAngles	= false;
		
		public ArcBar ()
		{
			rx = random( 2 * PI );
			ry = random( 2 * PI );
			rz = random( 2 * PI );
		}
		
		public ArcBar (float rx, float ry, float rz)
		{
			this.rx = rx;
			this.ry = ry;
			this.rz = rz;
		}

		public ArcBar (float rx, float ry, float rz, float cr, float cg, float cb )
		{
			this.rx = rx;
			this.ry = ry;
			this.rz = rz;
			
			colour[ 0 ] = cr;
			colour[ 1 ] = cg;
			colour[ 2 ] = cb;
		}
		
		public void init ()
		{
			angleStep	= (int)( angle / SINCOS_PRECISION );
			ps			= new PerlinStep( angleStep, 1.5f, 5, 30 );
		}
		
		public void update ()
		{
			rx += rInc;
			ry += rInc;
			rz += rInc;
		}
		
		public void render ()
		{
			float x1, y1, x2, y2, x3, y3, x4, y4;
			float fx1, fy1, fx2, fy2;
			float ps1, ps2;
			Vec3D vtemp, ntemp, n1, n2;
			Vec3D[] vtemps;
			int i, j, k;
			
			x1 = y1 = x2 = y2 = x3 = y3 = x4 = y4 = 0;
			fx1 = fy1 = fx2 = fy2 = 0;
			n1 = n2 = new Vec3D();
			ps1 = 0;
			ps2 = 0;
			
			gl.glPushMatrix();

			gl.glRotatef( 90, 1, 0, 0 );
			gl.glTranslatef( loc.x, loc.y, loc.z );
			gl.glRotatef( rz, 0, 0, 1 );
//			gl.glRotatef( 90, 0, 1, 0 );
//			gl.glRotatef( 90, 1, 0, 0 );
			
			for( i = 0; i<angleStep; i++ ) 
			{
				j = ( i < angleStep - 1 ) ? ( i + 1 ) : 0;
				
				ps1 = ps.perlinSteps[ i ];
				ps2 = ps.perlinSteps[ j ];
				
				x1	= cosLUT[ j ] * ( radius );
				y1	= sinLUT[ j ] * ( radius );
				x2	= cosLUT[ j ] * ( radius + ( width * ps1 ) );
				y2	= sinLUT[ j ] * ( radius + ( width * ps1 ) );
				
				x3	= cosLUT[ i ] * ( radius );
				y3	= sinLUT[ i ] * ( radius );
				x4	= cosLUT[ i ] * ( radius + ( width * ps1 ) );
				y4	= sinLUT[ i ] * ( radius + ( width * ps1 ) );
				
				// normal pointing inwards.
				n1	= new Vec3D( 1, 0, 0 );
				n1	= n1.rotateAroundAxis( Vec3D.Z_AXIS, j * DEG_TO_RAD );
				
				n2  = new Vec3D( 1, 0, 0 );
				n2	= n2.rotateAroundAxis( Vec3D.Z_AXIS, i * DEG_TO_RAD );
				
				if( drawQuads )
				{
					gl.glBegin( GL.GL_QUADS );
					
					gl.glMaterialfv( GL.GL_FRONT, GL.GL_AMBIENT_AND_DIFFUSE, colour, 0 );
					
					if( ps1 != ps2 )
					{
						// draw edge faces.
						ntemp = new Vec3D( 0, -1, 0 );
						
						if( ps1 < ps2  )	// facing left.
						{
							fx1	= cosLUT[ j ] * ( radius + ( width * ps1 ) );
							fy1	= sinLUT[ j ] * ( radius + ( width * ps1 ) );
							fx2	= cosLUT[ j ] * ( radius + ( width * ps2 ) );
							fy2	= sinLUT[ j ] * ( radius + ( width * ps2 ) );
							
							ntemp = ntemp.rotateAroundAxis( Vec3D.Z_AXIS.copy(), j * DEG_TO_RAD );
						}
						else				// facing right.
						{
							fx1	= cosLUT[ j ] * ( radius + ( width * ps2 ) );
							fy1	= sinLUT[ j ] * ( radius + ( width * ps2 ) );
							fx2	= cosLUT[ j ] * ( radius + ( width * ps1 ) );
							fy2	= sinLUT[ j ] * ( radius + ( width * ps1 ) );
							
							ntemp = ntemp.rotateAroundAxis( Vec3D.Z_AXIS.copy(), i * DEG_TO_RAD );
							ntemp.invert();
						}
						
						gl.glNormal3f( ntemp.x, ntemp.y, ntemp.z );
						gl.glVertex3f( fx1, fy1, 0 );
						gl.glVertex3f( fx1, fy1, height );
						gl.glVertex3f( fx2, fy2, height );
						gl.glVertex3f( fx2, fy2, 0 );
					}
					
					if( ( ps1 > 0 ) || ( ps2 > 0 ) && ( ps1 != 0 ) )
					{
						// draw inner faces.
						ntemp = n1.copy().invert();
						gl.glNormal3f( ntemp.x, ntemp.y, ntemp.z );
						gl.glVertex3f( x1, y1, 0 );
						
						ntemp = n2.copy().invert();
						gl.glNormal3f( ntemp.x, ntemp.y, ntemp.z );
						gl.glVertex3f( x3, y3, 0 );
						gl.glVertex3f( x3, y3, height );

						ntemp = n1.copy().invert();
						gl.glNormal3f( ntemp.x, ntemp.y, ntemp.z );
						gl.glVertex3f( x1, y1, height );
	
						// draw outer faces.
						ntemp = n1.copy();
						gl.glNormal3f( ntemp.x, ntemp.y, ntemp.z );
						gl.glVertex3f( x2, y2, 0 );

						ntemp = n2.copy();
						gl.glNormal3f( ntemp.x, ntemp.y, ntemp.z );
						gl.glVertex3f( x4, y4, 0 );
						gl.glVertex3f( x4, y4, height );
						
						ntemp = n1.copy();
						gl.glNormal3f( ntemp.x, ntemp.y, ntemp.z );
						gl.glVertex3f( x2, y2, height );
	
						// draw bottom faces.
						gl.glNormal3f( 0, 0, -1 );
						gl.glVertex3f( x1, y1, 0 );
						gl.glVertex3f( x3, y3, 0 );
						gl.glVertex3f( x4, y4, 0 );
						gl.glVertex3f( x2, y2, 0 );
	
						// draw top faces.
						gl.glNormal3f( 0, 0, 1 );
						gl.glVertex3f( x1, y1, height );
						gl.glVertex3f( x3, y3, height );
						gl.glVertex3f( x4, y4, height );
						gl.glVertex3f( x2, y2, height );
					}
					
					gl.glEnd();
				}
				
				if( drawNormals )
				{
					gl.glBegin( GL.GL_LINES );
					
					// draw end faces.
					ntemp = new Vec3D( 0, -1, 0 );
					
					if( ps2 != ps1 )
					{
						if( ps2 > ps1 )
						{
							fx1	= cosLUT[ j ] * ( radius + ( width * ps1 ) );
							fy1	= sinLUT[ j ] * ( radius + ( width * ps1 ) );
							fx2	= cosLUT[ j ] * ( radius + ( width * ps2 ) );
							fy2	= sinLUT[ j ] * ( radius + ( width * ps2 ) );
	
							ntemp = ntemp.rotateAroundAxis( Vec3D.Z_AXIS.copy(), j * DEG_TO_RAD );
						}
						else
						{
							fx1	= cosLUT[ i ] * ( radius + ( width * ps2 ) );
							fy1	= sinLUT[ i ] * ( radius + ( width * ps2 ) );
							fx2	= cosLUT[ i ] * ( radius + ( width * ps1 ) );
							fy2	= sinLUT[ i ] * ( radius + ( width * ps1 ) );
							
							ntemp = ntemp.rotateAroundAxis( Vec3D.Z_AXIS.copy(), i * DEG_TO_RAD );
							ntemp.invert();
						}
						
						vtemps		= new Vec3D[ 4 ];
						vtemps[ 0 ]	= new Vec3D( fx1, fy1, 0 );
						vtemps[ 1 ]	= new Vec3D( fx2, fy2, 0 );
						vtemps[ 2 ]	= new Vec3D( fx2, fy2, height );
						vtemps[ 3 ]	= new Vec3D( fx1, fy1, height );
						
						for( k=0; k<vtemps.length; k++ )
						{
							vtemp = vtemps[ k ];
							
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
						vtemps[ 1 ]	= new Vec3D( x3, y3, 0 );
						vtemps[ 2 ]	= new Vec3D( x3, y3, height );
						vtemps[ 3 ]	= new Vec3D( x1, y1, height );
						
						for( k=0; k<vtemps.length; k++ )
						{
							vtemp = vtemps[ k ];
							
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
						vtemps[ 1 ]	= new Vec3D( x4, y4, 0 );
						vtemps[ 2 ]	= new Vec3D( x4, y4, height );
						vtemps[ 3 ]	= new Vec3D( x2, y2, height );
						
						for( k=0; k<vtemps.length; k++ )
						{
							vtemp = vtemps[ k ];
							
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
						vtemps[ 1 ]	= new Vec3D( x3, y3, 0 );
						vtemps[ 2 ]	= new Vec3D( x4, y4, 0 );
						vtemps[ 3 ]	= new Vec3D( x2, y2, 0 );
						
						for( k=0; k<vtemps.length; k++ )
						{
							vtemp = vtemps[ k ];
							
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
						vtemps[ 1 ]	= new Vec3D( x3, y3, height );
						vtemps[ 2 ]	= new Vec3D( x4, y4, height );
						vtemps[ 3 ]	= new Vec3D( x2, y2, height );
						
						for( k=0; k<vtemps.length; k++ )
						{
							vtemp = vtemps[ k ];
							
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
				
				if( drawAngles )
				{
					gl.glBegin( GL.GL_LINES );
					
					
					vtemp = new Vec3D( x1, y1, 0 );
					vtemp.normalize();
					vtemp.scaleSelf( 700 );
					
					gl.glVertex3f( 0, 0, 0 );
					gl.glVertex3f
					(
						vtemp.x,
						vtemp.y, 
						vtemp.z
					);
					
					gl.glEnd();
				}
			}
			
			gl.glPopMatrix();
		}
	}	
}