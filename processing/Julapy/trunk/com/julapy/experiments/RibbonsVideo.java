package com.julapy.experiments;

////////////////////////////////////////////////////////////////////////////////
//
// Project: 		RibbonsVideo
// File: 			RibbonsVideo.java
// Created by: 		lukaszkarluk, Aug 26, 2008
//
////////////////////////////////////////////////////////////////////////////////

import java.util.ArrayList;

import javax.media.opengl.GL;

import com.julapy.camera.CameraOrbit;
import com.julapy.opengl.TextureLoader;

import processing.core.*;
import processing.opengl.*;
import processing.video.Movie;
import toxi.geom.Vec3D;

public class RibbonsVideo extends PApplet
{
	PGraphicsOpenGL pgl;
	GL gl;

	TextureLoader texLoader;
	int textureIndex = 0;
	
	Movie mov;
	int movWidth		= 0;
	int movHeight		= 0;
	int movFrameRate	= 25;
	int movIndex		= 0;
	PImage[] frameBuffer;
	
	int particlesPerFlock 	= 250;
	FlockParticle flockParticle;
	ArrayList<Particle> particles;
	
	CameraOrbit camOrbit;
	
	boolean isRecording = false;
	int imageCount;
	
	public void setup() 
	{
		size( 1280, 720, OPENGL );
		
		frameRate( 25 );
		
		pgl = (PGraphicsOpenGL) g;
		gl  = pgl.gl;

		camOrbit = new CameraOrbit( this, 500, 0, 0, random( PI * 0.004f, PI * 0.006f ), random( PI * 0.004f, PI * 0.006f ) );

		initGL( );
		initVideo( );
		initParticles( );
	}
	
	///////////////////////////////////////////////////
	//
	// INIT
	//
	///////////////////////////////////////////////////

	public void initGL ()
	{
		pgl = (PGraphicsOpenGL) g;
		gl = pgl.gl;

		/* opengl init */
		gl.glShadeModel( GL.GL_SMOOTH );              					// Enable Smooth Shading
		gl.glClearColor( 0, 0, 0, 0.5f );		    					// Black Background
		gl.glClearDepth( 1 );	                      					// Depth Buffer Setup
		gl.glEnable( GL.GL_DEPTH_TEST );								// Enables Depth Testing
		gl.glDepthFunc( GL.GL_LEQUAL );									// The Type Of Depth Testing To Do
		gl.glHint( GL.GL_PERSPECTIVE_CORRECTION_HINT, GL.GL_NICEST );	// Really Nice Perspective Calculations
		gl.glEnable( GL.GL_TEXTURE_2D );
	}

	public void initVideo ()
	{
		mov = new Movie( this, "video/jellyfishsky_debug.mov", movFrameRate );
		mov.loop();
		mov.speed( 0 );
		mov.read();

		movWidth = mov.width;
		movHeight = mov.height;

		frameBuffer = new PImage[ (int)(25 * mov.duration()) ];
		
		println( "frameBuffer length :: " + frameBuffer.length + ", movWidth :: " + movWidth + ", movHeight :: " + movHeight );
		
		for( int i=0; i<frameBuffer.length; i++ )
		{
			mov.jump( i/(float)25 );
			mov.read();
			
			frameBuffer[ i ] = mov.get();
			
			if( i % 50 == 0 && i != 0 ) println( i + " frames added to buffer" ); 
		}
		
		mov.dispose();
		mov = null;
		
		texLoader = new TextureLoader( gl, frameBuffer.length );
		
		for( int j=0; j<frameBuffer.length; j++ )
		{
			texLoader.loadTexture( frameBuffer[ j ], false );
		}
		
		frameBuffer = null;
	}
	
	private void initParticles ( )
	{
		particles		= new ArrayList<Particle>();
		flockParticle	= new FlockParticle( 0, 0, 0 );
		
		Particle p;
		for( int i=0; i<particlesPerFlock; i++)
		{
			p = new Particle();
			p.minDistance 	= 20;
			p.flockRange 	= 200;
			p.targetPull 	= 0.001f;
			p.velocityLimit = 20;
			
			particles.add( p );
		}
		
	}
	
	///////////////////////////////////////////////////
	//
	// EVENTS
	//
	///////////////////////////////////////////////////
	
	public void mousePressed ( )
	{
		for( int i=0; i<particles.size(); i++)
			particles.get( i ).force = 1;
	}
	
	public void keyPressed ( )
	{
		if(key == 'r')
		{
			isRecording = !isRecording;
			
			if(isRecording) println("started recording.");
			if(!isRecording) println("stopped recording.");
		}
	}
	
	///////////////////////////////////////////////////
	//
	// DRAW
	//
	///////////////////////////////////////////////////
		
	public void draw() 
	{
		background( 0 );

//		camera( 0, 0, -500, 0, 0, 0, 0, 1, 0 );
		camOrbit.update();
		
		pgl.beginGL();

		gl.glEnable( GL.GL_TEXTURE_2D );
		gl.glBindTexture( GL.GL_TEXTURE_2D, texLoader.getTexture( textureIndex ) );
		
		if( ++textureIndex > texLoader.getTextureLength() - 1 )
			textureIndex = 0;
		
//		gl.glDepthMask(false);				// additive blending
//		gl.glEnable( GL.GL_DEPTH_TEST );
//		gl.glEnable( GL.GL_BLEND );
//		gl.glBlendFunc(GL.GL_SRC_ALPHA,GL.GL_ONE);
		  
		noiseDetail( 4, 0.95f ); // my favourite result so far. very erratic.
		
		for( int i=0; i<particles.size(); i++)
		{
			particles.get(i).update();
			particles.get(i).renderTrail();
		}
		
		pgl.endGL();
		
		if(isRecording) save("export/image" + imageCount++ + ".png");
	}

	///////////////////////////////////////////////////
	//
	// PARTICLE CLASS
	//
	///////////////////////////////////////////////////
	
	class Particle
	{
		Vec3D 	loc;
		Vec3D 	vel;
		Vec3D[] locs;
		float 	size = 40;
		float	trailSize = 20;
		
		int centerPull		= 120;
		int flockPull 		= 80;
		int minDistance		= 10;
		float velocityLimit	= 4.0f;
		int	flockRange		= 10000;
		float targetPull	= 0.00125f;
		
		float r1Damping = 0.5f;
		float r2Damping = 0.2f;
		float r3Damping = 1.0f;
		float r4Damping = 1.0f;
		
		float force 	= 0;
		float decayRate = 0.85f;
		float range		= 400;
		
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
		
		public void update()
		{
			calcVectorRatios();

	        Vec3D oldLoc = loc.copy();
	        Vec3D newLoc = loc.copy();
	        
	        newLoc.addSelf( getFlockVector( ) );
	        newLoc.addSelf( getMagnetVector( ) );
	        newLoc.addSelf( getFlockTargetVector( ) );
//	        newLoc.addSelf( getImplodeVector( ) );
	        newLoc.addSelf( getNoiseVector( ) );

	        vel = new Vec3D( newLoc.x - oldLoc.x, newLoc.y - oldLoc.y, newLoc.z - oldLoc.z);
//	        vel.scaleSelf( 1.2f );
			loc.addSelf( vel );

		    for( int i=locs.length-1; i>0; i-- )
		    	locs[i].set( locs[i-1] );
		    locs[0].set( loc );

		    force *= decayRate;
		}

		///////////////////////////////////////////////////
		//
		// FLOCKING
		//
		///////////////////////////////////////////////////
	
		public Vec3D getFlockVector ( )
		{
			Vec3D v1 = rule1();
			Vec3D v2 = rule2();
			Vec3D v3 = rule3();
			Vec3D v4 = rule4();
			
			Vec3D v = vel.copy();
			
			v.x += ( r1Damping * v1.x ) + ( r2Damping * v2.x ) + ( r3Damping * v3.x ) + ( r4Damping * v4.x );
			v.y += ( r1Damping * v1.y ) + ( r2Damping * v2.y ) + ( r3Damping * v3.y ) + ( r4Damping * v4.y );
			v.z += ( r1Damping * v1.z ) + ( r2Damping * v2.z ) + ( r3Damping * v3.z ) + ( r4Damping * v4.z );
			
			limitVelocity( v );
			
			return v;
		}
		
		public Vec3D getMagnetVector ( )
		{
			Vec3D fc = new Vec3D();		// flock center.
			
			for (int i=0; i < particles.size(); i++)
				fc.addSelf( particles.get(i).loc );
			
			fc.x /= particles.size();
			fc.y /= particles.size();
			fc.z /= particles.size();
			
			
			float dist = loc.distanceTo( fc );
			if( dist < range )
			{
				float fs = dist / range;		// linear relationship between distance and force of magnet. fc = force scale.
				
				Vec3D mv = loc.sub( fc ).normalize(); // magnet vector.
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
			
			for (int i=0; i < particles.size(); i++)
			{
				Particle p = (Particle) particles.get(i);

				if ( this != p)
				{
					pd = loc.distanceTo( p.loc );
					
					if(pd < flockRange)
					{
						v.addSelf( p.loc );
						
						++count;
					}
				}
			}
			
			if(count > 0)
			{
				v.x /= count;
				v.y /= count;
				v.z /= count;
				
				v.x = ( v.x - loc.x ) / centerPull;
				v.y = ( v.y - loc.y ) / centerPull;
				v.z = ( v.z - loc.z ) / centerPull;
			}
			
			return v;
		}
		
		// avoid collision with other particles.
		public Vec3D rule2()
		{
			Vec3D v = new Vec3D();
			
			for (int i=0; i < particles.size(); i++)
			{
				Particle p = (Particle) particles.get(i);
				
				if ( this != p )
				{
					if ( loc.distanceTo( p.loc ) < minDistance )
					{
						v.x -= p.loc.x - loc.x;
						v.y -= p.loc.y - loc.y;
						v.z -= p.loc.z - loc.z;
					}
				}
			}

			return v;
		}
		
		// head to flock average
		public Vec3D rule3()
		{
			Vec3D v = new Vec3D();
			
			for (int i=0; i < particles.size(); i++)
			{
				Particle p = (Particle) particles.get(i);
				
				if ( this != p )
				{
					v.x +=  p.vel.x;
					v.y +=  p.vel.y;
					v.z +=  p.vel.z;
				}
			}
			
			v.x /= ( particles.size() - 1 );
			v.y /= ( particles.size() - 1 );
			v.z /= ( particles.size() - 1 );
			
			v.x = ( v.x - vel.x ) / flockPull; //8
			v.y = ( v.y - vel.y ) / flockPull; //8
			v.z = ( v.z - vel.z ) / flockPull; //8
			
			return v;
		}
		
		// 
		public Vec3D rule4()
		{
			Vec3D v = new Vec3D();
			
			v.x	= ( flockParticle.loc.x - loc.x ) * targetPull;
			v.y	= ( flockParticle.loc.y - loc.y ) * targetPull;
			v.z	= ( flockParticle.loc.z - loc.z ) * targetPull;
			
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
		

		///////////////////////////////////////////////////
		//
		// RENDER
		//
		// Trails, Initial discourse.
		// http://processing.org/discourse/yabb_beta/YaBB.cgi?board=OpenGL;action=display;num=1206925856
		//
		///////////////////////////////////////////////////
		
		public void renderTrail ()
		{
			float r, g, b, a, p;
		    float xOff, yOff, zOff;
		    Vec3D v1, v2, v3;
		    Vec3D p1 = new Vec3D();
		    Vec3D p2 = new Vec3D();
		    Vec3D p3 = new Vec3D();
		    Vec3D p4 = new Vec3D();
		    
		    for ( int i=0; i<locs.length - 2; i++ )
		    {
	    		v1 = locs[ i ].sub( locs[ i+1 ] ).normalize(); 	// direction vector.
	    		v2 = Vec3D.Y_AXIS.cross( v1 );					// up vector.
	    		v3 = v1.cross( v2 ).normalize();				// right vector.

	    		xOff = v3.x * trailSize;
	    		yOff = v3.y * trailSize;
	    		zOff = v3.z * trailSize;
	    		
	    		p = 1.0f - (float)i/(float)(locs.length-1);
				r = 1;
				g = 1 - p;
				b = p * 0.2f;
				a = p * 0.3f;
				
				if( i == 0 )
				{
					p3.x = locs[i].x - xOff;
					p3.y = locs[i].y - yOff;
					p3.z = locs[i].z - zOff;
					
					p4.x = locs[i].x + xOff;
					p4.y = locs[i].y + yOff;
					p4.z = locs[i].z + zOff;
				}
				else
				{
					p1 = p3.copy();
					p2 = p4.copy();

					p3.x = locs[i].x - xOff;
					p3.y = locs[i].y - yOff;
					p3.z = locs[i].z - zOff;
					
					p4.x = locs[i].x + xOff;
					p4.y = locs[i].y + yOff;
					p4.z = locs[i].z + zOff;
					
					gl.glColor4f( 1, 1, 1, a );

					float vdiv = 300;
					
			        gl.glBegin( GL.GL_POLYGON );
					gl.glTexCoord2f( ( p2.x + 200 ) / vdiv, ( p2.y + 200 ) / vdiv );		gl.glVertex3f( p2.x, p2.y, p2.z );
					gl.glTexCoord2f( ( p1.x + 200 ) / vdiv, ( p1.y + 200 ) / vdiv );		gl.glVertex3f( p1.x, p1.y, p1.z );
					gl.glTexCoord2f( ( p3.x + 200 ) / vdiv, ( p3.y + 200 ) / vdiv );		gl.glVertex3f( p3.x, p3.y, p3.z );
					gl.glTexCoord2f( ( p4.x + 200 ) / vdiv, ( p4.y + 200 ) / vdiv );		gl.glVertex3f( p4.x, p4.y, p4.z );
					gl.glEnd();
				}
		    }
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
		
		private Vec3D getImplodeVector ( )
		{
			Vec3D center  = new Vec3D(width/2, height/2, width/2);
			Vec3D implode = center.sub( loc ).normalize();
			implode.scaleSelf( implodeVecScale * 10 );
//			implode.scaleSelf( implodeVecScale * 50 );
			
			return implode;
		}
		
		private Vec3D getNoiseVector ( )
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
		
		public Vec3D getFlockTargetVector ( )
		{
			Vec3D tv = flockParticle.loc.sub( this.loc ).normalize();
			tv.scaleSelf( 1 );
//			tv.scaleSelf( 5 );
			
			return tv;
		}
	}
	
	///////////////////////////////////////////////////
	//
	// FLOCK PARTICLE CLASS
	//
	///////////////////////////////////////////////////

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

}