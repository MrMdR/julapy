package com.julapy.experiments;

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

public class Ribbons extends PApplet 
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
	Boolean useCamera = true;
	
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
	
		if( !useCamera )
		{
			float cameraZ = ( height / 2.0f ) / PApplet.tan( PApplet.PI * 60 /360.0f );
			gl.glTranslatef( 0, 0, -cameraZ );
		}
		
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
		
		if( useCamera )
		{
			camVec.x += (v1.x - camVec.x) * 0.01f;
			camVec.y += (v1.y - camVec.y) * 0.01f;
			camVec.z += (v1.z - camVec.z) * 0.01f;
		}
		
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
		
		if( useCamera )
		{
			cam.jump( camVec.x, camVec.y, camVec.z );
			cam.aim( fp1.loc.x, fp1.loc.y, fp1.loc.z );
			
//			cam.jump( fp1.loc.x, fp1.loc.y, fp1.loc.z );
//			cam.aim( width/2, height/2, width/2 );
			
//			cam.circle( radians( ( 1 * camDirection ) ) );
			cam.feed();
	
			camPosition.set( cam.position()[0], cam.position()[1], cam.position()[2] );
			camTarget.set( cam.target()[0], cam.target()[1], cam.target()[2] );
			camNormal = camPosition.sub(camTarget).normalize();
		}
		
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
	
	class Particle
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
			
			pgl.texture( particleTexture );

			textureMode( NORMALIZED );
			
			gl.glPushMatrix();
			gl.glTranslatef( loc.x, loc.y, loc.z );
	        
			// face texture to camera.
			float deltaX   = 0; 
			float deltaY   = 0; 
			float deltaZ   = 0;
			if( useCamera )
			{
				deltaX   = camTarget.x - camPosition.x; 
				deltaY   = camTarget.y - camPosition.y; 
				deltaZ   = camTarget.z - camPosition.z;
			}

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

	class FlockParticle
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
			pgl.texture( magnetTexture );

			textureMode( NORMALIZED );
			
			gl.glPushMatrix();
			gl.glTranslatef( loc.x, loc.y, loc.z );
	        
			// face texture to camera.
			float deltaX   = 0;
			float deltaY   = 0; 
			float deltaZ   = 0;
			if( useCamera )
			{
				deltaX   = camTarget.x - camPosition.x;
				deltaY   = camTarget.y - camPosition.y; 
				deltaZ   = camTarget.z - camPosition.z;
			}

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
	
	public class Magnet
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
		
		public Vec3D getMagnetVector ( Particle particle )
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
			pgl.texture( magnetTexture );

			textureMode( NORMALIZED );
			
			gl.glPushMatrix();
			gl.glTranslatef( loc.x, loc.y, loc.z );
	        
			// face texture to camera.
			float deltaX   = 0; 
			float deltaY   = 0; 
			float deltaZ   = 0;
			if( useCamera )
			{
				deltaX   = camTarget.x - camPosition.x; 
				deltaY   = camTarget.y - camPosition.y; 
				deltaZ   = camTarget.z - camPosition.z;
			}

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
	
	public class Flock
	{
		public int 		centerPull		= 120;
		public int 		flockPull 		= 80;
		public int 	 	minDistance		= 10;
		public float	velocityLimit	= 4.0f;
		public int		flockRange		= 10000;
		public float	targetPull		= 0.00125f;
		
		float r1Damping = 0.5f;
		float r2Damping = 0.2f;
		float r3Damping = 1.0f;
		float r4Damping = 1.0f;
		
		float force 	= 0;
		float decayRate = 0.85f;
		float range		= 400;
		
		Vec3D _flockTraget;
		
		ArrayList<Particle> _particles;
		Particle _particle;
		
		public Flock ( ArrayList<Particle> particles )
		{
			_particles 	 = particles;
			_flockTraget = new Vec3D(); 
		}
		
		public void setFlockTarget ( Vec3D target )
		{
			_flockTraget = target;
		}
		
		public void setForce (  float force )
		{
			this.force = force;
		}
		
		public void update ()
		{
			force *= decayRate;
		}
		
		public Vec3D getFlockVector ( Particle particle )
		{
			_particle = particle;
			
			Vec3D v1 = rule1();
			Vec3D v2 = rule2();
			Vec3D v3 = rule3();
			Vec3D v4 = rule4();
			
			Vec3D v = _particle.vel().copy();
			
			v.x += ( r1Damping * v1.x ) + ( r2Damping * v2.x ) + ( r3Damping * v3.x ) + ( r4Damping * v4.x );
			v.y += ( r1Damping * v1.y ) + ( r2Damping * v2.y ) + ( r3Damping * v3.y ) + ( r4Damping * v4.y );
			v.z += ( r1Damping * v1.z ) + ( r2Damping * v2.z ) + ( r3Damping * v3.z ) + ( r4Damping * v4.z );
			
			limitVelocity( v );
			
			return v;
		}
		
		public Vec3D getMagnetVector ( Particle particle )
		{
			Vec3D fc = new Vec3D();		// flock center.
			
			for (int i=0; i < _particles.size(); i++)
			{
				Particle p = (Particle) _particles.get(i);

				fc.addSelf(p.loc());
			}
			
			fc.x /= _particles.size();
			fc.y /= _particles.size();
			fc.z /= _particles.size();
			
			
			float dist = particle.loc().distanceTo( fc );
			if( dist < range )
			{
				float fs = dist / range;		// linear relationship between distance and force of magnet. fc = force scale.
				
				Vec3D mv = particle.loc().sub( fc ).normalize(); // magnet vector.
				mv.scaleSelf( force * fs * 40 );
				
				return mv;
			}
			else
			{
				return new Vec3D();
			}
		}
		
		// pull to the center
		public Vec3D rule1 ()
		{
			Vec3D v 	= new Vec3D();
			float pd	= 0;
			int count	= 0;
			
			for (int i=0; i < _particles.size(); i++)
			{
				Particle p = (Particle) _particles.get(i);

				if (_particle != p)
				{
					pd = _particle.loc().distanceTo(p.loc());
					
					if(pd < flockRange)
					{
						v.addSelf(p.loc());
						
						++count;
					}
				}
			}
			
			if(count > 0)
			{
				v.x /= count;
				v.y /= count;
				v.z /= count;
				
				v.x = (v.x - _particle.loc().x) / centerPull;
				v.y = (v.y - _particle.loc().y) / centerPull;
				v.z = (v.z - _particle.loc().z) / centerPull;
			}
			
			return v;
		}
		
		// avoid collision with other particles.
		public Vec3D rule2()
		{
			Vec3D v = new Vec3D();
			
			for (int i=0; i < _particles.size(); i++)
			{
				Particle p = (Particle) _particles.get(i);
				
				if (_particle != p)
				{
					if ( _particle.loc().distanceTo( p.loc() ) < minDistance )
					{
						v.x -= p.loc().x - _particle.loc().x;
						v.y -= p.loc().y - _particle.loc().y;
						v.z -= p.loc().z - _particle.loc().z;
					}
				}
			}

			return v;
		}
		
		// head to flock average
		public Vec3D rule3()
		{
			Vec3D v = new Vec3D();
			
			for (int i=0; i < _particles.size(); i++)
			{
				Particle p = (Particle) _particles.get(i);
				
				if (_particle != p)
				{
					v.x +=  p.vel().x;
					v.y +=  p.vel().y;
					v.z +=  p.vel().z;
				}
			}
			
			v.x /= (_particles.size() - 1);
			v.y /= (_particles.size() - 1);
			v.z /= (_particles.size() - 1);
			
			v.x = (v.x - _particle.vel().x) / flockPull; //8
			v.y = (v.y - _particle.vel().y) / flockPull; //8
			v.z = (v.z - _particle.vel().z) / flockPull; //8
			
			return v;
		}
		
		// 
		public Vec3D rule4()
		{
			Vec3D v = new Vec3D();
			
			v.x	= (_flockTraget.x - _particle.loc().x) * targetPull;
			v.y	= (_flockTraget.y - _particle.loc().y) * targetPull;
			v.z	= (_flockTraget.z - _particle.loc().z) * targetPull;
			
			return v;
		}

		public Vec3D limitVelocity (Vec3D v)
		{
		    float vel = v.magnitude();

		    if (vel > velocityLimit)
		    {
		    	v.x = ( v.x / vel ) * velocityLimit;
		    	v.y = ( v.y / vel ) * velocityLimit;
		    	v.z = ( v.z / vel ) * velocityLimit;
		    }
		    
		    return v;
		}
		
	}
	
}