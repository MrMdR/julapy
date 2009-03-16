package com.julapy.cilindricoCollapse;

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
import com.julapy.utils.TrigLookUpTable;

import processing.opengl.PGraphicsOpenGL;
import processing.core.*;
import toxi.geom.Matrix4x4;
import toxi.geom.Quaternion;
import toxi.geom.Vec3D;
import toxi.math.MathUtils;

public class CilindricoCollapse extends PApplet 
{
	PGraphicsOpenGL pgl;
	GL gl;

	Boolean lightingEnabled	= true;
	float[] lightAmbient	= { 0.1f, 0.1f, 0.1f, 1.0f };
	float[] lightDiffuse	= { 0.3f, 0.3f, 0.3f, 1.0f };
	float[] lightSpecular	= { 0.5f, 0.5f, 0.5f, 1.0f };
	float[] lightPosition	= { -1.5f, 1.0f, -4.0f, 1.0f };
	
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
//		wfColour,		// yellow.
//		{ 0.90f, 0.18f, 0.04f, 1.00f },		// red.
//		{ 0.88f, 0.44f, 0.13f, 1.00f },		// orange.
//		{ 0.74f, 0.33f, 0.05f, 1.00f },		// orange dark.
//		{ 0.42f, 0.45f, 0.03f, 1.00f }		// olive dark.
//	};

//	float[][] colorPalette = 
//	{ 
//		{ 0.13f, 0.21f, 0.23f, 1.00f },		// dark green 01.
//		{ 0.20f, 0.25f, 0.26f, 1.00f },		// dark green 02.
//		{ 0.18f, 0.30f, 0.33f, 1.00f },		// green.
//		{ 0.32f, 0.42f, 0.45f, 1.00f },		// green / grey.
//		{ 0.28f, 0.28f, 0.26f, 1.00f },		// brown.
//		{ 0.75f, 0.67f, 0.56f, 1.00f },		// light brown.
//	};
	
//	float[][] colorPalette = 
//	{
//		{ 0.38f, 0.40f, 0.38f, 1.00f },		// sepia green.
//		{ 0.31f, 0.27f, 0.29f, 1.00f },		// sepia brown.
//		{ 0.68f, 0.62f, 0.47f, 1.00f },		// sepia yellow.
//		{ 0.62f, 0.47f, 0.40f, 1.00f },		// sepia red.
//		{ 0.58f, 0.65f, 0.71f, 1.00f },		// sepia light blue.
//		{ 0.39f, 0.49f, 0.70f, 1.00f },		// sepia dark blue.
//		{ 0.76f, 0.64f, 0.53f, 1.00f }		// sepia light brown.
//	};

	float[][] colorPalette = 
	{ 
		{ 0.95f, 0.95f, 0.95f, 1.00f },
		{ 0.92f, 0.92f, 0.92f, 1.00f },
		{ 0.89f, 0.89f, 0.89f, 1.00f },
		{ 0.86f, 0.86f, 0.86f, 1.00f },
		{ 0.83f, 0.83f, 0.83f, 1.00f },
		{ 0.96f, 0.00f, 0.64f, 1.00f }
	};
	
	float [] bgColorLight	= { 0.95f, 0.95f, 0.95f };	// almost white
	float [] bgColorDark	= { 0.50f, 0.50f, 0.50f };	// greyish.
	
//	float [] bgColorLight	= { 0.77f, 0.72f, 0.70f };	// olive.
//	float [] bgColorDark	= { 0.60f, 0.56f, 0.52f };	// dark olive.
	
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

		float rotInc = 1;
		
		if( keyCode == 37 )	// left.
		{
			sceneRotZ -= rotInc * DEG_TO_RAD;
		}

		if( keyCode == 39 )	// right
		{
			sceneRotZ += rotInc * DEG_TO_RAD;
		}
		
		if( keyCode == 38 )	// up
		{
			sceneRotX += rotInc * DEG_TO_RAD;
		}
		
		if( keyCode == 40 )	// down
		{
			sceneRotX -= rotInc * DEG_TO_RAD;
		}
	}
	
	////////////////////////////////////////////
	// INIT.
	////////////////////////////////////////////
	
	private void initLookUpTables ()
	{
		TrigLookUpTable.initLookUpTabel();
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
		int arcsTotal		= 100;
		float arcHeight		= 6;
		float arcLocZInc	= 0;
		float arcLocZ 		= -(int)( ( arcsTotal * ( arcHeight + arcLocZInc ) ) * 0.5f );
		float arcAngleMin	= 100;
		float arcAngleMax	= 270;
		
		float arcRadiusMin	= 50;
		float arcRadiusMax	= 200;
		float arcWidthMin	= 200;
		float arcWidthMax	= 400;
		float arcDispRange	= 100;
		float arcCosMult	= 0;
		int arcColorIndex	= 0;
		float[] arcColors	= {};
		ArcBar arcBar;
		int i;

		arcBars = new ArcBar[ arcsTotal ];
		
		pgl.beginGL();
		gl.glGenLists( arcsTotal * 2 );
		
		for( i=0; i<arcsTotal; i++ )
		{
			arcCosMult		= -(float)Math.cos( ( i / (float)( arcsTotal - 1 ) ) * Math.PI + Math.PI * 0.5f );
			
			arcBars[ i ] 		= arcBar = new ArcBar( );
			arcBar.id			= i * 2 + 1;
			arcBar.loc.z 		= arcLocZ;
			arcBar.height		= arcHeight;
			arcBar.radius		= random( arcRadiusMin, arcRadiusMax ) * arcCosMult;
			arcBar.width		= random( arcWidthMin, arcWidthMax ) * arcCosMult;
			arcBar.angle		= random( arcAngleMin, arcAngleMax ) * ( 1 - arcCosMult );
			arcBar.rz			= random( 360 );
			arcBar.rInc			= random( 1.0f ) - 0.5f;
			arcBar.dx			= random( arcDispRange ) - arcDispRange * 0.5f;
			arcBar.dy			= random( arcDispRange ) - arcDispRange * 0.5f;
			arcBar.dz			= random( arcDispRange ) - arcDispRange * 0.5f;
			arcBar.wireframePad	= MathUtils.random( 20.0f ) + 10.0f;
			
			arcColorIndex	= (int)( random( colorPalette.length ) );
			arcColors		= colorPalette[ arcColorIndex ];
//			arcBar.colour	= arcColors;
			arcBar.colour[ 0 ]	= arcColors[ 0 ];
			arcBar.colour[ 1 ]	= arcColors[ 1 ];
			arcBar.colour[ 2 ]	= arcColors[ 2 ];
			arcBar.colour[ 3 ]	= 1f;
			
			arcBar.gl	= gl;
			
			arcBar.init();
			arcBar.createSolidModel();
			arcBar.createWireframe();
			
			arcLocZ += ( arcHeight + arcLocZInc );
		}
		
		pgl.endGL();
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
        	gl.glDisable( GL.GL_LIGHT1 );
        }

		// blend mode.
//		gl.glDepthMask( false );
//		gl.glEnable( GL.GL_BLEND );
//		gl.glBlendFunc( GL.GL_SRC_ALPHA, GL.GL_ONE );
		
		// START DRAW.
		pgl.beginGL();
			drawBackground();
			gl.glPushMatrix();
			gl.glTranslatef( sceneCenterX, sceneCenterY, sceneCenterZ );
				applySceneRotation();
				drawArcBars();
			gl.glPopMatrix();
		pgl.endGL();
		
		tiler.post();
		if( !tiler.checkStatus() && isTiling )
		{
			isTiling = false;
		}
		
		if(isRecording) save("export/image" + frameCount + ".png");
	}
	
	private void drawBackground ()
	{
    	gl.glDisable( GL.GL_LIGHTING );
    	gl.glDisable( GL.GL_LIGHT1 );
		gl.glDisable( GL.GL_DEPTH_TEST );
		gl.glPushMatrix();
		gl.glTranslatef( sceneCenterX, sceneCenterY, -2000 );
		gl.glScalef( 1500, 1500, 0 );
			gl.glBegin( GL.GL_QUADS );
				gl.glNormal3f( 0, 0, 0 );
				gl.glColor3fv( bgColorDark, 0 );	// top.
				gl.glVertex2f(  1.0f, -1.0f );
				gl.glVertex2f( -1.0f, -1.0f );
				gl.glColor3fv( bgColorLight, 0 );	// middle.
				gl.glVertex2f( -1.0f,  0.0f );
				gl.glVertex2f(  1.0f,  0.0f );
				gl.glColor3fv( bgColorLight, 0 );	// middle.
				gl.glVertex2f(  1.0f,  0.0f );
				gl.glVertex2f( -1.0f,  0.0f );
				gl.glColor3fv( bgColorDark, 0 );	// bottom.
				gl.glVertex2f( -1.0f,  1.0f );
				gl.glVertex2f(  1.0f,  1.0f );
			gl.glEnd();
		gl.glPopMatrix();
		gl.glEnable( GL.GL_LIGHTING );
    	gl.glEnable( GL.GL_LIGHT1 );
		gl.glEnable( GL.GL_DEPTH_TEST );
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
//			arcBars[ i ].render();
			arcBars[ i ].isTiling = isTiling;
			arcBars[ i ].renderSolidModel();
			arcBars[ i ].renderWireframe();
		}
	}
	

	////////////////////////////////////////////
	// ARC BAR CLASS.
	////////////////////////////////////////////
	
}