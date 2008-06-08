package com.julapy;

////////////////////////////////////////////////////////////////////////////////
//
// Project: 		Julaps_SphericalParticles
// File: 			SphericalParticles.java
// Created by: 		lukaszkarluk, May 5, 2008
//
////////////////////////////////////////////////////////////////////////////////

import processing.core.*;
import processing.opengl.PGraphicsOpenGL;
import javax.media.opengl.GL;

import damkjer.ocd.Camera;

import java.util.ArrayList;
import toxi.geom.Vec3D;

public class SphereParticles extends PApplet {

	PGraphicsOpenGL pgl;
	GL gl;
	
	ArrayList<SphericalParticle> sp;
	
	SphericalParticle cameraParticle;
	
	Camera cam;
	Vec3D camVec;
	Vec3D camTarget;
	Vec3D camPosition;
	Vec3D camNormal;
	int camDirection = 1;
	
	boolean isRecording = false;
	int imageCount;

	public void setup() 
	{
		size(1280, 720, OPENGL);

		frameRate(25);

		hint(ENABLE_OPENGL_4X_SMOOTH);
		colorMode(RGB, 1.0f);

		pgl = (PGraphicsOpenGL) g;
		gl = pgl.gl;
		gl.setSwapInterval(1);
		
		cam 		= new Camera( this );
		camVec		= new Vec3D();
		camTarget 	= new Vec3D();
		camPosition = new Vec3D();
		camNormal 	= new Vec3D();
		
        initSphericalParticles();
	}

	public void draw() 
	{
		background(0);
		
		updateCameraParticle();
		
		cam.jump( cameraParticle.loc.x, cameraParticle.loc.y, cameraParticle.loc.z );
		cam.aim( 0, 0, 0 );
		cam.feed();

		gl.glDisable( GL.GL_TEXTURE_2D );
		gl.glDepthMask(false);
		gl.glEnable( GL.GL_DEPTH_TEST );
		gl.glEnable( GL.GL_BLEND );
		gl.glEnable( GL.GL_TEXTURE_2D );
		gl.glBlendFunc(GL.GL_SRC_ALPHA,GL.GL_ONE);

		pgl.beginGL();
		
		updateSphericalParticles();

		pgl.endGL();
		
		if(isRecording) save("export/image" + imageCount++ + ".png");
	}
	
	public void keyPressed()
	{
		if(key == 'r')
		{
			isRecording = !isRecording;
			
			if(isRecording) println("started recording.");
			if(!isRecording) println("stopped recording.");
		}
	}
	
    private void initSphericalParticles ()
    {
    	cameraParticle = new SphericalParticle( 500, 0.01f, 0.01f );
    	
    	sp = new ArrayList<SphericalParticle>();
    	for( int i=0; i<500; i++ )
    	{
    		float ts = random( 0.03f, 0.08f );
    		float ps = random( 0.03f, 0.08f );
    		
    		sp.add( new SphericalParticle( 200, ts, ps ) );
    	}
    }
    
	private void updateSphericalParticles ()
	{
		for( int i=0; i<sp.size(); i++ )
		{
			sp.get( i ).update();
			sp.get( i ).render();
		}
	}
	
	private void updateCameraParticle ()
	{
		cameraParticle.update();
	}
	
	public class SphericalParticle
	{
		float radius;
		float theta;
		float phi;
		float thetaSpeed = 0;
		float phiSpeed = 0;
		float itemSize = 0;
		int thetaDir;
		int phiDir;
		
		Vec3D loc;
		Vec3D[] locs;
		
		public SphericalParticle ( float r, float ts, float ps )
		{
			radius		= r;
			itemSize 	= random( 5 );
			theta		= random( 2 * PI );
			phi			= random( 2 * PI );
			thetaSpeed 	= ts;
			phiSpeed 	= ps;
			thetaDir	= random( 0, 1 ) > 0.5f ? 1 : -1;
			phiDir		= random( 0, 1 ) > 0.5f ? 1 : -1;
			
			loc = new Vec3D();
			
			locs = new Vec3D[20];
			for( int i=0; i<locs.length; i++ )
			{
				locs[i] = new Vec3D( loc );
		    }
		}
		
		public void update ()
		{
			theta += thetaSpeed * thetaDir;
			phi += phiSpeed * phiDir;
			
			loc.x = cos(theta) * sin(phi) * radius;
			loc.y = sin(theta) * sin(phi) * radius;
			loc.z = cos(phi) * radius;
			
		    for( int i=locs.length-1; i>0; i-- )
		    {
		    	locs[i].set( locs[i-1] );
		    }
		    locs[0].set( loc );
		}
		
		public void render ()
		{
		    gl.glBegin( GL.GL_QUAD_STRIP );
		    
			float xp, yp, zp;
		    float xOff, yOff, zOff;
		    
		    for ( int i=0; i<locs.length - 1; i++ )
		    {
		    	float per	= 1.0f - (float)i/(float)(locs.length-1);
		    	xp			= locs[i].x;
		    	yp			= locs[i].y;
		    	zp			= locs[i].z;

		    	if ( i < locs.length - 2 )
		    	{
		    		// TODO: rotate strip so the face is always facing the center.
		    		
		    		Vec3D v1 = locs[i].sub( locs[i+1] ).normalize(); 	// direction vector.
		    		Vec3D v2 = Vec3D.Y_AXIS.cross( v1 );				// up vector.
		    		Vec3D v3 = v1.cross( v2 ).normalize();				// right vector.

		    		xOff	= v3.x * 4;
		    		yOff	= v3.y * 4;
		    		zOff	= v3.z * 4;
		    		
					float r		= per * 0.04f;
					float g		= 0.0f;
					float b		= 0.1f;
					float a		= 1.0f;
					
					gl.glColor4f( r, g, b, a );

		    		gl.glVertex3f( xp - xOff, yp - yOff, zp - zOff );
		    		gl.glVertex3f( xp + xOff, yp + yOff, zp + zOff );
		    	}
		    }

		    gl.glEnd();
		}
	}
	
}