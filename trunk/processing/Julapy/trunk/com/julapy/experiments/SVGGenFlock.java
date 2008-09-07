package com.julapy.experiments;

////////////////////////////////////////////////////////////////////////////////
//
// Project: 		Julapy
// File: 			SVGGenFlock.java
// Created by: 		lukaszkarluk, Aug 31, 2008
//
////////////////////////////////////////////////////////////////////////////////

import java.util.ArrayList;

import javax.media.opengl.GL;

import com.julapy.experiments.RibbonsVideo.FlockParticle;
import com.julapy.experiments.RibbonsVideo.Particle;
import com.julapy.utils.TextFileUtil;

import processing.candy.SVG;
import processing.core.*;
import toxi.geom.Vec3D;

public class SVGGenFlock extends PApplet 
{
	String[] svgFileList = { "svg/shape_01.svg" };
	SVG svgList[];
	
	int particlesPerFlock 	= 10;
	FlockParticle flockParticle;
	Particle[] particles;
	
	PImage srcImage;
	
	float noiseXCount;
	float noiseYCount;
	float noiseZCount;
	float noiseSpeed = 0.01f;
	
	boolean drawFromFile = true;
	
	public void setup() 
	{
		size( 1200, 780, "prosvg.SVGOut" );
//		size( 1200, 780 );

		smooth( );
		frameRate( 25 );
		background( 255 );
		
		srcImage = loadImage( "image/startape.jpg" );
		
		if( drawFromFile )
		{
			noLoop( );
			writeInPosition( );
		}
		else
		{
			initSVG( );
			initParticles( );
			initNoiseField( );
		}
	}

	///////////////////////////////////////////////////
	//
	// INIT
	//
	///////////////////////////////////////////////////
	
	public void initSVG ()
	{
		SVG svg;
		int i;
		
		svgList = new SVG[ svgFileList.length ];
		
		for( i=0; i<svgFileList.length; i++ )
		{
			svg = new SVG( this, svgFileList[ i ] );
			svg.drawMode( CENTER );
			svgList[ i ] = svg;
		}
	}

	private void initParticles ( )
	{
		flockParticle	= new FlockParticle( width * 0.5f, height * 0.5f, 0 );
		particles		= new Particle[ particlesPerFlock ];
		
		for( int i=0; i<particlesPerFlock; i++ )
			particles[ i ] = new Particle();
	}
	
	private void initNoiseField ( )
	{
		noiseXCount = random(100);
		noiseYCount = random(100);
		noiseZCount = random(100);
	}

	///////////////////////////////////////////////////
	//
	// EVENTS
	//
	///////////////////////////////////////////////////
	
	public void keyPressed ( )
	{
		if( key == 's' )
		{
			writeOutPositions( );
			save( "data/export/svgTest.svg" );
		}
	}
	
	///////////////////////////////////////////////////
	//
	// DRAW
	//
	///////////////////////////////////////////////////
	
	public void draw() 
	{
//		background( 0 );
		
		if( !drawFromFile )
		{
			for( int i=0; i<particles.length; i++ )
			{
				particles[ i ].update();
				particles[ i ].render();
			}
		}
	}

	///////////////////////////////////////////////////
	//
	// WRITE OUT
	//
	///////////////////////////////////////////////////
	
	public void writeOutPositions ()
	{
		TextFileUtil tfu = new TextFileUtil( this );
		tfu.createWriter( "data/line_positions.txt" );
		for( int i=0; i<1; i++ ) 
		{
			Particle p = particles[ i ];
			
			for( int j=0; j<p.locs.size(); j++ )
			{
				float[] pp = new float[2];
				pp[0] = p.locs.get( j ).x;
				pp[1] = p.locs.get( j ).y;
				tfu.writeFloatLine( pp );
			}
		}
		tfu.closeWriter();
	}
	
	public void writeInPosition ()
	{
		TextFileUtil tfu = new TextFileUtil( this );
		tfu.readDataFromFile( "data/line_positions.txt" );
		
		noFill();
		stroke( 0, 100 );
		strokeWeight( 1 );
		strokeJoin( ROUND );
		strokeCap( SQUARE );
		
		beginShape( );
		
		float[] pp;
		float px, py;
		int c;
		
		while( tfu.hasNextLine() )
		{
			pp = tfu.readFloatLine();
			
			px = pp[ 0 ];
			py = pp[ 1 ];
			
			// doesn't work for entire line
			c = srcImage.get( (int)( px / width * srcImage.width ), (int)( py / height * srcImage.height ) );
			stroke( c, 100 );

			curveVertex( px, py );
		}
		
		endShape( );
		
		save( "data/export/svgTest.svg" );
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
		
		ArrayList<Vec3D> locs;

		Vec3D oldLoc;
        Vec3D newLoc;
		
		float rule1scale	= 0.01f;
		float rule2scale	= 0.2f;
		float rule3scale	= 0.0125f;
		
		int minDistance		= 50;
		float velocityLimit	= 10;
		
		float r1Damping = 0.5f;
		float r2Damping = 0.2f;
		float r3Damping = 1.0f;
		
		float force 	= 0;
		float decayRate = 0.85f;
		float range		= 400;
		
		float implodeRadii 	= width * 0.4f;
        int implodeSwitch 	= 1;
        int noiseSwitch 	= 1;
        int mouseSwitch		= 0;
        float implodeScale	= 2;
        float noiseScale 	= 0.3f;
        float mouseScale	= 0.5f;

		public Particle( )
		{
			loc = new Vec3D( random(width), random(height), 0 );
			
			init();
		}
		
		public Particle( float x, float y, float z )
		{
			loc = new Vec3D( x, y, z );
			
			init();
		}
		
		private void init ()
		{
			vel = new Vec3D( random(-4f, 4f), random(-4f, 4f), 0 );
			
			locs = new ArrayList<Vec3D>();
			locs.add( loc );
		}
		
		public void update()
		{
			calcVectorRatios();

			if( mousePressed )
				mouseSwitch = 1;
			else
				mouseSwitch = 0;
			
	        oldLoc = loc.copy();
	        newLoc = loc.copy();
	        
	        newLoc.addSelf( getFlockVector( ) );
//	        newLoc.addSelf( getMagnetVector( ) );
//	        newLoc.addSelf( getFlockTargetVector( ) );
	        newLoc.addSelf( getImplodeVector( ) );
	        newLoc.addSelf( getNoiseVector( ) );
	        newLoc.addSelf( getMouseVector( ) );

	        vel = new Vec3D( newLoc.x - oldLoc.x, newLoc.y - oldLoc.y, 0 );
	        
			limitVelocity( vel );
	        
			loc.addSelf( vel );
			
			locs.add( loc.copy() );
			
		    force *= decayRate;
		}
		
		public void render ()
		{
			int r = (int)( loc.x / width * 255 );
			int g = (int)( loc.y / width * 255 );
			int b = (int)( loc.x / width * 255 );
			int c = srcImage.get( (int)( loc.x / width * srcImage.width ), (int)( loc.y / height * srcImage.height ) );
			
			pushMatrix();
//			translate( loc.x, loc.y );
//			rotate( random(360) );
//			scale( random( 0.5f ), random( 0.5f ) );
			
			stroke( c, 100 );
			strokeWeight( 1 );
			strokeJoin( ROUND );
			strokeCap( SQUARE );

			if( locs.size() >= 4 )
			{
				beginShape( );
				for( int i=locs.size()-4; i<=locs.size()-1; i++ )
				{
					curveVertex( locs.get( i ).x, locs.get( i ).y );
				}
				endShape( );
			}

//			beginShape( LINES );			
//			vertex( oldLoc.x, oldLoc.y );
//			vertex( newLoc.x, newLoc.y );
//			endShape( );
			
//			line( oldLoc.x, oldLoc.y, newLoc.x, newLoc.y );
			
//			svgList[ 0 ].ignoreStyles( );
//			fill( r, g, b, 255 );
//			fill( c, 255 );
//			noStroke( );
		
//			if( frameCount % 3 == 0 )
//				svgList[ 0 ].draw( 0, 0 );
			
			popMatrix();

//			noStroke();
//			fill( 255 );
//			ellipse( loc.x, loc.y, 10, 10 );
//			noFill();
		}

		///////////////////////////////////////////////////
		// FLOCKING
		///////////////////////////////////////////////////
	
		public Vec3D getFlockVector ( )
		{
			Vec3D v1 = rule1();		// pull to the center.
			Vec3D v2 = rule2();		// avoid collision with other particles.
			Vec3D v3 = rule3();		// head to flock average
			
			Vec3D v = vel.copy();
			v.addSelf( v1.scaleSelf( r1Damping ) );
			v.addSelf( v2.scaleSelf( r2Damping ) );
			v.addSelf( v3.scaleSelf( r3Damping ) );
			
			return v;
		}
		
		public Vec3D rule1 ()	// pull to the center.
		{
			Vec3D v 	= new Vec3D();
			
			for (int i=0; i < particles.length; i++)
			{
				if ( this != particles[ i ] )
					v.addSelf( particles[ i ].loc );
			}

			v.scaleSelf( 1 / (float)( particles.length - 1 ) );
			v.subSelf( loc );
			v.scaleSelf( rule1scale );
			
			return v;
		}
		
		public Vec3D rule2()	// avoid collision with other particles.
		{
			Vec3D v = new Vec3D();
			
			for (int i=0; i < particles.length; i++)
			{
				Particle p = particles[ i ];
				
				if ( this != p )
				{
					if ( loc.distanceTo( p.loc ) < minDistance )
					{
						v.subSelf( p.loc.sub( loc ) );
						v.scaleSelf( rule2scale );
					}
				}
			}

			return v;
		}
		
		public Vec3D rule3()	// head to flock average
		{
			Vec3D v = new Vec3D();
			
			for (int i=0; i < particles.length; i++)
			{
				if ( this != particles[ i ] )
					v.addSelf( particles[ i ].vel );
			}
			
			v.scaleSelf( 1 / (float)( particles.length - 1 ) );
			v.subSelf( vel );
			v.scaleSelf( rule3scale );
			
			return v;
		}
		
		public Vec3D limitVelocity (Vec3D v)
		{
		    if ( v.magnitude() > velocityLimit)
		    {
		    	v.normalize( );
		    	v.scaleSelf( velocityLimit );
		    }
		    
		    return v;
		}
		

		private void calcVectorRatios ()
		{
//			float vecRatio = (implodeRadii - loc.distanceTo( new Vec3D(width/2, width/2, width/2) ) ) / implodeRadii;
//			if(vecRatio < 0) vecRatio = 0;
//			
//	        implodeVecScale = 1 - vecRatio;
//	        noiseVecScale   = vecRatio;

			if( loc.x < 0 || loc.x > width || loc.y < 0 || loc.y > height )
			{
				implodeSwitch 	= 1;
				noiseSwitch		= 0;
			}
			else
			{
				implodeSwitch	= 0;
				noiseSwitch		= 1;
			}
			
		}
		
		private Vec3D getImplodeVector ( )
		{
			Vec3D center  = new Vec3D( width/2, height/2, 0 );
			Vec3D implode = center.sub( loc ).normalize();
			implode.scaleSelf( implodeSwitch * implodeScale );
			
			return implode;
		}
		
		public Vec3D getMagnetVector ( )
		{
			Vec3D fc = new Vec3D();		// flock center.
			
			for (int i=0; i < particles.length; i++)
				fc.addSelf( particles[ i ].loc );
			
			fc.x /= particles.length;
			fc.y /= particles.length;
			fc.z /= particles.length;
			
			
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
		
		private Vec3D getNoiseVector ( )
		{
			float noiseXVal = noise( loc.x * 0.05f, noiseXCount ) - 0.9f;
			float noiseYVal = noise( loc.y * 0.05f, noiseYCount ) - 0.9f;
			float noiseZVal = noise( loc.z * 0.05f, noiseZCount ) - 0.9f;
			
			noiseXCount += noiseSpeed;
			noiseYCount += noiseSpeed;
			noiseZCount += noiseSpeed;
			
			Vec3D noise = new Vec3D( noiseXVal, noiseYVal, noiseZVal );
			noise.scaleSelf( noiseSwitch * noiseScale );
			
			return noise;
		}
		
		public Vec3D getFlockTargetVector ( )
		{
			Vec3D v = flockParticle.loc.sub( this.loc ).normalize();
			v.scaleSelf( 1 );
			
			return v;
		}
		
		public Vec3D getMouseVector ( )
		{
			Vec3D v = new Vec3D( mouseX, mouseY, 0 );
			v.subSelf( loc ).normalize();
			v.scaleSelf( mouseSwitch * mouseScale );
			
			return v;
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