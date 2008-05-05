package com.julapy.noise;

////////////////////////////////////////////////////////////////////////////////
//
// Project: 		Julaps_NoiseField02
// File: 			NoiseField3D.java
// Created by: 		lukaszkarluk, Mar 12, 2008
//
////////////////////////////////////////////////////////////////////////////////

import java.awt.event.MouseEvent;
import java.util.ArrayList;

import javax.media.opengl.GL;

import damkjer.ocd.Camera;

import processing.core.*;
import processing.opengl.*;
import toxi.geom.Vec3D;

public class Julaps_Flocking_01 extends PApplet 
{
	PGraphicsOpenGL pgl;
	GL gl;
	
	int flocksTotal 		= 6;
	int particlesPerFlock 	= 50;
	
	ArrayList<Particle> particles;
	ArrayList<Flock> flocks;
	ArrayList<FlockParticle> flockParticles;
	PImage particleTexture;
	PImage magnetTexture;
	Magnet magnet;
	
	int squareList;
	
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
		size( 1280, 720, OPENGL );
		
		frameRate( 25 );
		
		hint( ENABLE_OPENGL_4X_SMOOTH );
		colorMode( RGB, 1.0f );
		
		pgl = (PGraphicsOpenGL) g;
		gl  = pgl.gl;
		gl.setSwapInterval(1);

		initGL();

		particleTexture = loadImage("particle.png");
		magnetTexture   = loadImage("magnet.png");
		
		cam 		= new Camera( this, width/2, height/2, 1200, width/2, height/2, width/2);
		camVec		= new Vec3D();
		camTarget 	= new Vec3D();
		camPosition = new Vec3D();
		camNormal 	= new Vec3D();

		particles		= new ArrayList<Particle>();
		flocks	  		= new ArrayList<Flock>();
		flockParticles	= new ArrayList<FlockParticle>();

		magnet = new Magnet();
		
		for( int i=0; i<flocksTotal; i++ )
		{
			createFlock();
		}

		registerMouseEvent( this );
	}
	
	private void createFlock ()
	{
		ArrayList<Particle> ps = new ArrayList<Particle>();

		for( int i=0; i<particlesPerFlock; i++)
		{
			Particle p = new Particle();
			
			ps.add( p );
			particles.add( p );
		}
		
		FlockParticle flockTarget = new FlockParticle();
		
		flockParticles.add( flockTarget );
		
		Flock flock = new Flock( ps );
		
		flocks.add( flock );
		
		flock.minDistance 	= 20;
		flock.flockRange 	= 200;
//		flock.centerPull 	= 1000;
		flock.targetPull 	= 0.001f;
		flock.velocityLimit = 20; 
		
		for( int i=0; i<ps.size(); i++)
		{
			Particle p = ps.get(i);
			p.setFLock( flock );
			p.setFlockParticle( flockTarget );
		}	
	}

	public void initGL()
	{
		pgl.beginGL();
		squareList = gl.glGenLists(1);
		gl.glNewList(squareList, GL.GL_COMPILE);
		gl.glBegin(GL.GL_POLYGON);
		gl.glTexCoord2f(0, 0);    gl.glVertex2f(-.5f, -.5f);
		gl.glTexCoord2f(1, 0);    gl.glVertex2f( .5f, -.5f);
		gl.glTexCoord2f(1, 1);    gl.glVertex2f( .5f,  .5f);
		gl.glTexCoord2f(0, 1);    gl.glVertex2f(-.5f,  .5f);
		gl.glEnd();
		gl.glEndList();
		pgl.endGL();
	}
	
	public void mouseEvent (MouseEvent event) 
	{
		switch (event.getID()) {
			case MouseEvent.MOUSE_PRESSED:
				mousePressed(event);
				break;
		}
	}
	
	public void mousePressed (MouseEvent event)
	{
		camDirection *= -1;
		
		for( int i=0; i<flocks.size(); i++)
		{
			flocks.get( i ).setForce( 1 );
		}
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
		
	public void draw() 
	{
		background( 0, 0, 0 );
		
		gl.glDisable( GL.GL_TEXTURE_2D );
		
//		FlockParticle fp1 = flockParticles.get( 0 );
//		
//		Vec3D v0 = fp1.vel.copy().normalize();
//		v0.scaleSelf( 100 );
//		Vec3D v1 = fp1.loc.add( v0 );
		
		Particle fp1 = particles.get( 0 );
		
		Vec3D v0 = fp1.vel.copy().normalize();
		v0.scaleSelf( -2500 );
		Vec3D v1 = fp1.loc.add( v0 );
		
		camVec.x += (v1.x - camVec.x) * 0.01f;
		camVec.y += (v1.y - camVec.y) * 0.01f;
		camVec.z += (v1.z - camVec.z) * 0.01f;
		
//		FlockParticle fp2 = null;
//		for( int i=0; i<flockParticles.size(); i++)
//		{
//			FlockParticle fp = flockParticles.get( i );
//			
//			if( i == 0 )
//			{
//				fp2 = fp;
//			}
//			else
//			{
//				if( fp.loc.distanceTo( fp1.loc ) < fp2.loc.distanceTo( fp1.loc ) )
//				{
//					if ( fp != fp1.flockTarget )
//					{
//						fp2 = fp;
//					}
//				}
//			}
//		}
		
		cam.jump( camVec.x, camVec.y, camVec.z );
		cam.aim( fp1.loc.x, fp1.loc.y, fp1.loc.z );
		
//		cam.jump( fp1.loc.x, fp1.loc.y, fp1.loc.z );
//		cam.aim( width/2, height/2, width/2 );
		
//		cam.circle( radians( ( 1 * camDirection ) ) );
		cam.feed();

		camPosition.set( cam.position()[0], cam.position()[1], cam.position()[2] );
		camTarget.set( cam.target()[0], cam.target()[1], cam.target()[2] );
		camNormal = camPosition.sub(camTarget).normalize();
		
		gl.glDepthMask(false);
		gl.glEnable( GL.GL_DEPTH_TEST );
		gl.glEnable( GL.GL_BLEND );
		gl.glEnable( GL.GL_TEXTURE_2D );
		gl.glBlendFunc(GL.GL_SRC_ALPHA,GL.GL_ONE);
		  
		noiseDetail(2, 2.5f);

		for( int i=0; i<flocks.size(); i++)
		{
			flocks.get( i ).update();
		}
		
		noiseDetail(4, 0.95f); // my favourite result so far. very erratic.
		
		pgl.beginGL();
		
		for( int i=0; i<particles.size(); i++)
		{
			particles.get(i).update();
//			particles.get(i).render();
			particles.get(i).renderTrail();
		}
		
		magnet.update();
//		magnet.render();
		
		pgl.endGL();
		
		if(isRecording) save("export/image" + imageCount++ + ".png");
	}
	
	class Particle implements IParticle
	{
		Vec3D 	loc;
		Vec3D 	vel;
		Vec3D[] locs;
		float 	size = 40;
		
		Flock flock;
		FlockParticle flockTarget;

		float noiseXCount;
		float noiseYCount;
		float noiseZCount;
		float noiseSpeed = 0.01f;
		float noiseScale = 0.005f;
		
		float implodeRadii = 300;
        float implodeVecScale = 1;
        float noiseVecScale = 1;

		public Particle( )
		{
			loc = new Vec3D( random(width), random(height), random(width) );
			
			init();
		}
		
		public Particle( float x, float y, float z )
		{
			loc = new Vec3D( x, y, z) ;
			
			init();
		}
		
		private void init ()
		{
			vel = new Vec3D( random(-4f, 4f), random(-4f, 4f), random(-4f, 4f) );
			
			noiseXCount = random(100);
			noiseYCount = random(100);
			noiseZCount = random(100);
			
			locs = new Vec3D[50];
			for( int i=0; i<locs.length; i++ )
			{
				locs[i] = new Vec3D( loc );
		    }
		}
		
		public Vec3D loc()
		{
			return loc;
		}
		
		public Vec3D vel()
		{
			return vel;
		}
		
		public void setFLock ( Flock flock )
		{
			this.flock = flock;
		}
		
		public void setFlockParticle ( FlockParticle flockTarget )
		{
			this.flockTarget = flockTarget;
		}
		
		public void update()
		{
			calcVectorRatios();

	        Vec3D oldLoc = loc.copy();
	        Vec3D newLoc = loc.copy();
	        
	        newLoc.addSelf( flock.getFlockVector( this ) );
	        newLoc.addSelf( flock.getMagnetVector( this ) );
	        newLoc.addSelf( getFlockTargetVector() );
//	        newLoc.addSelf( getImplodeVector() );
	        newLoc.addSelf( getNoiseVector() );

	        vel = new Vec3D( newLoc.x - oldLoc.x, newLoc.y - oldLoc.y, newLoc.z - oldLoc.z);
//	        vel.scaleSelf( 1.2f );
			loc.addSelf( vel );
			
		    for( int i=locs.length-1; i>0; i-- )
		    {
		    	locs[i].set( locs[i-1] );
		    }
		    locs[0].set( loc );
		}
		
		public void render ()
		{
			gl.glEnable( GL.GL_TEXTURE_2D );
			
			pgl.bindTexture( particleTexture );

			textureMode( NORMALIZED );
			
			gl.glPushMatrix();
			gl.glTranslatef( loc.x, loc.y, loc.z );
	        
			// face texture to camera.
			float deltaX   = camTarget.x - camPosition.x; 
			float deltaY   = camTarget.y - camPosition.y; 
			float deltaZ   = camTarget.z - camPosition.z;

			float angleZ   = atan2( deltaY, deltaX );
			float hyp      = sqrt( sq( deltaX ) + sq( deltaY ) ); 
			float angleY   = atan2( hyp, deltaZ ); 
		 
			gl.glRotatef( degrees( angleZ ), 0, 0,    1.0f );
			gl.glRotatef( degrees( angleY ), 0, 1.0f, 0    );
	        
	        gl.glScalef( size, size, size );
	        gl.glColor4f( red(0xFFFFFF), green(0xFFFFFF), blue(0xFFFFFF), 1 );
	        gl.glCallList( squareList );
	        
			gl.glPopMatrix();
		}
		
		// Trails, Initial discourse.
		// http://processing.org/discourse/yabb_beta/YaBB.cgi?board=OpenGL;action=display;num=1206925856
		
		public void renderTrail ()
		{
			gl.glDisable( GL.GL_TEXTURE_2D );
			
			float xp, yp, zp;
		    float xOff, yOff, zOff;
		    
		    gl.glBegin( GL.GL_QUAD_STRIP );
		    
		    for ( int i=0; i<locs.length - 1; i++ )
		    {
		    	float per	= 1.0f - (float)i/(float)(locs.length-1);
		    	xp			= locs[i].x;
		    	yp			= locs[i].y;
		    	zp			= locs[i].z;

		    	if ( i < locs.length - 2 )
		    	{
		    		Vec3D v1 = locs[i].sub( locs[i+1] ).normalize(); 	// direction vector.
		    		Vec3D v2 = Vec3D.Y_AXIS.cross( v1 );				// up vector.
		    		Vec3D v3 = v1.cross( v2 ).normalize();				// right vector.

		    		xOff	= v3.x * 4;
		    		yOff	= v3.y * 4;
		    		zOff	= v3.z * 4;
		    		
					float r		= 1;					// red
					float g		= 1 - per;				// green
					float b		= per * 0.2f;			// blue
					float a		= per * 0.8f;			// alpha
					gl.glColor4f( r, g, b, a );

		    		gl.glVertex3f( xp - xOff, yp - yOff, zp - zOff );
		    		gl.glVertex3f( xp + xOff, yp + yOff, zp + zOff );
		    	}
		    }

		    gl.glEnd();
		}
		
		private void calcVectorRatios ()
		{
//			float vecRatio = (implodeRadii - loc.distanceTo( new Vec3D(width/2, width/2, width/2) ) ) / implodeRadii;
//			if(vecRatio < 0) vecRatio = 0;
//			
//	        implodeVecScale = 1 - vecRatio;
//	        noiseVecScale   = vecRatio;

			if( loc.distanceTo( new Vec3D(width/2, height/2, width/2) ) < implodeRadii )
			{
				implodeVecScale = 0;
				noiseVecScale = 1;
			}
			else
			{
				implodeVecScale = 1;
				noiseVecScale = 0;
			}
			
		}
		
		private Vec3D getImplodeVector ()
		{
			Vec3D center  = new Vec3D(width/2, height/2, width/2);
			Vec3D implode = center.sub( loc ).normalize();
			implode.scaleSelf( implodeVecScale * 10 );
//			implode.scaleSelf( implodeVecScale * 50 );
			
			return implode;
		}
		
		private Vec3D getNoiseVector ()
		{
			float noiseXVal = noise( loc.x * noiseScale, noiseXCount ) - 0.9f;
			float noiseYVal = noise( loc.y * noiseScale, noiseYCount ) - 0.9f;
			float noiseZVal = noise( loc.z * noiseScale, noiseZCount ) - 0.9f;
			
			noiseXCount += noiseSpeed;
			noiseYCount += noiseSpeed;
			noiseZCount += noiseSpeed;
			
			Vec3D noise = new Vec3D( noiseXVal, noiseYVal, noiseZVal); 
			noise.scaleSelf( noiseVecScale * 2 );
//			noise.scaleSelf( noiseVecScale * 10 );
			
			return noise;
		}
		
		public Vec3D getFlockTargetVector ()
		{
			Vec3D tv = flockTarget.loc().sub( loc ).normalize();
			tv.scaleSelf( 1 );
//			tv.scaleSelf( 5 );
			
			return tv;
		}
	}

	class FlockParticle implements IParticle
	{
		Vec3D loc;
		Vec3D vel;
		float size = 40;
		
		float noiseXCount;
		float noiseYCount;
		float noiseZCount;
		float noiseSpeed = 0.02f;
		float noiseScale = 0.005f;
		
		float implodeRadii = 600;
        float implodeVecScale = 1;
        float noiseVecScale = 1;
        
		public FlockParticle( )
		{
			loc = new Vec3D( random(width), random(height), random(width) );
			
			init();
		}
		
		public FlockParticle( float x, float y, float z )
		{
			loc = new Vec3D( x, y, z) ;
			
			init();
		}
		
		private void init ()
		{
			vel = new Vec3D( random(-4f, 4f), random(-4f, 4f), random(-4f, 4f) );
			
			noiseXCount = random(100);
			noiseYCount = random(100);
			noiseZCount = random(100);
		}
		
		public Vec3D loc()
		{
			return loc;
		}
		
		public Vec3D vel()
		{
			return vel;
		}
		
		public void update()
		{
			calcVectorRatios();

	        Vec3D oldLoc = loc.copy();
	        Vec3D newLoc = loc.copy();
	        
	        newLoc.addSelf( getImplodeVector() );
	        newLoc.addSelf( getNoiseVector() );

	        vel = new Vec3D( newLoc.x - oldLoc.x, newLoc.y - oldLoc.y, newLoc.z - oldLoc.z);
			loc.addSelf( vel );
		}
		
		public void render ()
		{
			pgl.bindTexture( magnetTexture );

			textureMode( NORMALIZED );
			
			gl.glPushMatrix();
			gl.glTranslatef( loc.x, loc.y, loc.z );
	        
			// face texture to camera.
			float deltaX   = camTarget.x - camPosition.x;
			float deltaY   = camTarget.y - camPosition.y; 
			float deltaZ   = camTarget.z - camPosition.z;

			float angleZ   = atan2( deltaY, deltaX ); 
			float hyp      = sqrt( sq( deltaX ) + sq( deltaY ) ); 
			float angleY   = atan2( hyp, deltaZ ); 
		 
			gl.glRotatef( degrees( angleZ ), 0, 0,    1.0f );
			gl.glRotatef( degrees( angleY ), 0, 1.0f, 0    );
	        
	        gl.glScalef( 100, 100, 100 );
	        gl.glCallList( squareList );
	        
			gl.glPopMatrix();
		}
		
		private void calcVectorRatios ()
		{
			float vecRatio = (implodeRadii - loc.distanceTo( new Vec3D(width/2, height/2, width/2) ) ) / implodeRadii;
			if(vecRatio < 0) vecRatio = 0;
			
	        implodeVecScale = 1 - vecRatio;
	        noiseVecScale   = vecRatio;
		}
		
		private Vec3D getImplodeVector ()
		{
			Vec3D center  = new Vec3D(width/2, height/2, width/2);
			Vec3D implode = center.sub( loc ).normalize();
			implode.scaleSelf( implodeVecScale * 10 );
			
			return implode;
		}
		
		private Vec3D getNoiseVector ()
		{
			float noiseXVal = noise( loc.x * noiseScale, noiseXCount ) - 0.9f;
			float noiseYVal = noise( loc.y * noiseScale, noiseYCount ) - 0.9f;
			float noiseZVal = noise( loc.z * noiseScale, noiseZCount ) - 0.9f;
			
			noiseXCount += noiseSpeed;
			noiseYCount += noiseSpeed;
			noiseZCount += noiseSpeed;
			
			Vec3D noise = new Vec3D( noiseXVal, noiseYVal, noiseZVal); 
			noise.scaleSelf( noiseVecScale * 40 );
			
			return noise;
		}
	}
	
	public class Magnet implements IParticle
	{
		Vec3D loc;
		Vec3D vel;
		float force 	= 1;
		float decayRate = 0.9f;
		float range		= 400;
		
		public Magnet ()
		{
			loc = new Vec3D( width/2, height/2, width/2 );
			vel = new Vec3D( );
		}
		
		public Vec3D loc()
		{
			return loc;
		}
		
		public Vec3D vel()
		{
			return vel;
		}
		
		public Vec3D getMagnetVector ( IParticle particle )
		{
			float dist = particle.loc().distanceTo( loc );
			if( dist < range )
			{
				float fs = dist / range;		// linear relationship between distance and force of magnet. fc = force scale.
				
				Vec3D mv = particle.loc().sub( loc ).normalize(); // magnet vector.
				mv.scaleSelf( force * fs * 40 );
				
				return mv;
			}
			else
			{
				return new Vec3D();
			}
		}

		public void setForce (  float force )
		{
			this.force = force;
		}

		public void update ()
		{
			force *= decayRate;
		}
		
		public void render ()
		{
			pgl.bindTexture( magnetTexture );

			textureMode( NORMALIZED );
			
			gl.glPushMatrix();
			gl.glTranslatef( loc.x, loc.y, loc.z );
	        
			// face texture to camera.
			float deltaX   = camTarget.x - camPosition.x; 
			float deltaY   = camTarget.y - camPosition.y; 
			float deltaZ   = camTarget.z - camPosition.z;

			float angleZ   = atan2( deltaY, deltaX ); 
			float hyp      = sqrt( sq( deltaX ) + sq( deltaY ) ); 
			float angleY   = atan2( hyp, deltaZ ); 
		 
			gl.glRotatef( degrees( angleZ ), 0, 0,    1.0f );
			gl.glRotatef( degrees( angleY ), 0, 1.0f, 0    );
	        
	        gl.glScalef( 100, 100, 100 );
	        gl.glCallList( squareList );
	        
			gl.glPopMatrix();
		}
	}
}