package com.julapy;

////////////////////////////////////////////////////////////////////////////////
//
// Project: 		Julaps_ParticleSpings
// File: 			Julaps_ParticleSpings.java
// Created by: 		lukaszkarluk, Jun 7, 2008
//
////////////////////////////////////////////////////////////////////////////////

import java.util.ArrayList;

import javax.media.opengl.GL;

import com.julapy.opengl.TextureLoader;

import krister.Ess.AudioChannel;
import krister.Ess.Ess;
import krister.Ess.FFT;
import damkjer.ocd.Camera;
import processing.opengl.PGraphicsOpenGL;
import processing.core.PApplet;
import processing.core.PImage;
import toxi.geom.Vec3D;

public class Julaps_ParticleSpings extends PApplet 
{
	PGraphicsOpenGL pgl;
	GL gl;
	TextureLoader texLoader;
	
	Camera cam;
	Vec3D camTarget;
	Vec3D camPosition;
	Vec3D camNormal;
	
	AudioChannel channel;
	FFT fft;
	
	int[] textures;
	
	ArrayList<Particle> particles;
	ArrayList<Spring> springs;
	int nparticles = 0;
	double gravitational = 0;
	double viscousdrag = 0.1;
	int particleCloudSize = 500;
	
	int frameNumber = 0;
	int framesPerSec = 25;
	boolean isRecording = false;	
	
	public void setup() 
	{
		size( 1280, 720, OPENGL );

		frameRate( framesPerSec );
		
		colorMode( RGB, 1.0f );
		
		hint(ENABLE_OPENGL_4X_SMOOTH);

		//__________________________________________________________ opengl.
		
		initGL();
		
		//__________________________________________________________ camera.		
		
		cam 		= new Camera( this );
		camTarget 	= new Vec3D();
		camPosition = new Vec3D();
		camNormal 	= new Vec3D();

		cam.jump( 0, 250, 300 );
		cam.aim( 0, 0, 0 );
		cam.feed();
		
		//__________________________________________________________ ess.
		
		initEss();
		
		//__________________________________________________________ psys.
		
		initParticles();
	}
	
	public void initGL ()
	{
		pgl = (PGraphicsOpenGL) g;
		gl = pgl.gl;

		gl.glShadeModel(GL.GL_SMOOTH);              				// Enable Smooth Shading
		gl.glClearColor(0.0f, 0.0f, 0.0f, 0.5f);    				// Black Background
		gl.glClearDepth(1.0f);                      				// Depth Buffer Setup
		gl.glEnable(GL.GL_DEPTH_TEST);								// Enables Depth Testing
		gl.glDepthFunc(GL.GL_LEQUAL);								// The Type Of Depth Testing To Do
		gl.glHint(GL.GL_PERSPECTIVE_CORRECTION_HINT, GL.GL_NICEST);	// Really Nice Perspective Calculations
		gl.glEnable(GL.GL_TEXTURE_2D);
		
		gl.glEnable( GL.GL_BLEND );
		gl.glDisable( GL.GL_DEPTH_TEST );
		gl.glBlendFunc( GL.GL_SRC_ALPHA, GL.GL_ONE );
		gl.glBlendFunc( GL.GL_SRC_ALPHA, GL.GL_ONE_MINUS_SRC_COLOR );

		texLoader = new TextureLoader( gl );
		texLoader.init();
		texLoader.loadTexture( loadImage( "data/texture/p_01.png" ), true );
	}
	
	public void draw() 
	{
		background(0);
		
		//__________________________________________________________ camera.
		
		float camX = (float)mouseX/width * width - width/2;
		float camY = (float)mouseY/height * height - height/2;
		
		cam.jump( camX, camY, 500 );
		cam.aim( 0, 0, 0 );
		cam.feed();
		cam.up();

		camPosition.set( cam.position()[0], cam.position()[1], cam.position()[2] );
		camTarget.set( cam.target()[0], cam.target()[1], cam.target()[2] );
		camNormal = camPosition.sub(camTarget).normalize();
		
		//__________________________________________________________ ess.
		
		if( isRecording )
		{
			int offset = (int)(frameNumber * channel.sampleRate / framesPerSec);
			fft.getSpectrum( channel.samples, offset );
		}
		else
		{
			fft.getSpectrum( channel );
		}
		
		reactParticles();
		
		//__________________________________________________________ psys.
		
//		addMagnetVector( new Vec3D() );
		updateParticles( 0.1, 1 );
		
		//__________________________________________________________ render.
		   
		pgl.beginGL();

		int i, j, p1, p2;
		int bezDetail = 20;
		float bt;
		Point3D[] bezPoints;
		Point3D bezPoint;
		Particle pp1, pp2;
		
		gl.glEnable(GL.GL_TEXTURE_2D);
		gl.glDepthMask( false );
		gl.glBindTexture( GL.GL_TEXTURE_2D, texLoader.getTexture( 0 ) );
		for ( i=0; i<particles.size(); i++ )
			particles.get( i ).render();

		gl.glDisable(GL.GL_TEXTURE_2D);
		gl.glDepthMask( true );
		for ( i=0; i<springs.size(); i++ ) {
			p1 	= springs.get( i ).from;
			p2 	= springs.get( i ).to;
			pp1	= particles.get( p1 );
			pp2 = particles.get( p2 );
			bezPoints = new Point3D[4];
			bezPoints[0] = new Point3D( pp1.loc.x, pp1.loc.y, pp1.loc.z );
			bezPoints[1] = new Point3D( 0, 0, 0 );
			bezPoints[2] = new Point3D( 0, 0, 0 );
			bezPoints[3] = new Point3D( pp2.loc.x, pp2.loc.y, pp2.loc.z );

			gl.glBegin( GL.GL_LINE_STRIP );
			gl.glColor4f( 1, 1, 1, 0.1f );
            for ( j=0; j<=bezDetail; j++) {
            	bt 			= ((float) j) / ((float) bezDetail);			// percent of bezier.
            	bezPoint 	= Point3D.bernstein( bt, bezPoints );		// generate new point.
            	gl.glVertex3d( bezPoint.x, bezPoint.y, bezPoint.z );	// draw vertex for point.
            }
//			gl.glVertex3f( pp1.loc.x, pp1.loc.y, pp1.loc.z );
//			gl.glVertex3f( pp2.loc.x, pp2.loc.y, pp2.loc.z );
			gl.glEnd();
		}

		pgl.endGL();
		
		if( isRecording ) save("data/export/export"+ frameNumber++ +".png");
	}
	
	public void mousePressed ()
	{
		addParticle( );
	}
	
	public void initEss ()
	{
		Ess.start( this );

		channel = new AudioChannel( "audio/cell.aif" );
		
		if( !isRecording )
			channel.play( );
		
		fft = new FFT( 512 );
		fft.equalizer( true );
		fft.smooth = true;
//		fft.damp( 0.5f );
	}
	
	public void initParticles ()
	{
		int i;
		nparticles	= 30;

		particles	= new ArrayList<Particle>();
		springs		= new ArrayList<Spring>();

		/* Add particles */
		for ( i=0; i<nparticles; i++ ) {
			addParticle();
		}
	}

	public void addParticle ( )
	{
		Particle p	= new Particle();
		p.id		= particles.size();
		p.mass 		= 1;
		p.fixed 	= false;
		p.loc.x		= random(particleCloudSize) - particleCloudSize/2;
		p.loc.y		= random(particleCloudSize) - particleCloudSize/2;
		p.loc.z		= random(particleCloudSize) - particleCloudSize/2;

		int i;
		for( i=0; i<particles.size(); i++ ) {
			Particle pp = particles.get( i );
			Spring s			= new Spring();
//			s.springconstant	= 0.1f;
//			s.dampingconstant	= 0.05f;
			s.springconstant	= 0.5f;
			s.dampingconstant	= 0.5f;
			s.restlength 		= 300;
			s.from				= p.id;
			s.to				= pp.id;
			springs.add( s );
		}
		
		particles.add( p );
	}
	
	public void reactParticles ()
	{
		int i, si;
		float di;
		Spring s;
		for( i=0; i<particles.size(); i++ )
		{
			di 		= i / (float)particles.size();
			si 		= (int)(fft.spectrum.length * di);
			s  		= springs.get( i );
			s.restlength = max( 1, fft.spectrum[si] * 20000 );
		}
	}
	
	public void updateParticles ( double dt, int method )
	{
		int i;
		Particle[] ptmp;
		Particle p;
		Derivatives[] deriv = new Derivatives[particles.size()];

		switch ( method )
		{
			case 0:                                   /* Euler */
				calculateForces(  );
				calculateDerivatives( deriv );
				for ( i=0; i<particles.size(); i++ ) 
				{
					p		= particles.get( i );
					p.loc.x += deriv[i].dpdt.x * dt;
					p.loc.y += deriv[i].dpdt.y * dt;
					p.loc.z += deriv[i].dpdt.z * dt;
					p.vel.x += deriv[i].dvdt.x * dt;
					p.vel.y += deriv[i].dvdt.y * dt;
					p.vel.z += deriv[i].dvdt.z * dt;
				}
			break;
			
			case 1:                                   /* Midpoint */
				calculateForces( );
				calculateDerivatives( deriv );
				ptmp = new Particle[particles.size()];
				for ( i=0; i<particles.size(); i++ ) 
				{
					ptmp[i] = particles.get( i ).copy();
					ptmp[i].loc.x += deriv[i].dpdt.x * dt / 2;
					ptmp[i].loc.y += deriv[i].dpdt.y * dt / 2;
					ptmp[i].loc.z += deriv[i].dpdt.z * dt / 2;
					ptmp[i].loc.x += deriv[i].dvdt.x * dt / 2;
					ptmp[i].loc.y += deriv[i].dvdt.y * dt / 2;
					ptmp[i].loc.z += deriv[i].dvdt.z * dt / 2;
				}
				calculateForces( );
				calculateDerivatives( deriv );
				for ( i=0; i<particles.size(); i++ ) 
				{
					p		= particles.get( i );
					p.loc.x += deriv[i].dpdt.x * dt;
					p.loc.y += deriv[i].dpdt.y * dt;
					p.loc.z += deriv[i].dpdt.z * dt;
					p.vel.x += deriv[i].dvdt.x * dt;
					p.vel.y += deriv[i].dvdt.y * dt;
					p.vel.z += deriv[i].dvdt.z * dt;
				}
			break;
		}
	}

	public void calculateForces ( )
	{
		int i = 0, p1 = 0, p2 = 0;
		float len, dx, dy, dz;
		Vec3D down	= new Vec3D( 0, 0, -1 );
		Vec3D force = new Vec3D( );
		Particle p, pp1, pp2;
		Spring s;

		for ( i=0; i<particles.size(); i++ ) 
		{
			p		= particles.get( i );
			p.force = new Vec3D();
			if ( p.fixed )
				continue;

			/* Gravitation */
			p.force.x += gravitational * p.mass * down.x;
			p.force.y += gravitational * p.mass * down.y;
			p.force.z += gravitational * p.mass * down.z;

			/* Viscous drag */
			p.force.x -= viscousdrag * p.vel.x;
			p.force.y -= viscousdrag * p.vel.y;
			p.force.z -= viscousdrag * p.vel.z;
		}

		/* Handle the spring interaction */
		for ( i=0; i<springs.size(); i++ ) 
		{
			s	= springs.get( i );
			p1 = s.from;
			p2 = s.to;

			pp1 = particles.get( p1 );
			pp2 = particles.get( p2 );
			
			dx = pp1.loc.x - pp2.loc.x;
			dy = pp1.loc.y - pp2.loc.y;
			dz = pp1.loc.z - pp2.loc.z;
			len = sqrt( dx*dx + dy*dy + dz*dz );
			force.x  = s.springconstant  * (len - s.restlength);
			force.x += s.dampingconstant * (pp1.vel.x - pp2.vel.x) * dx / len;
			force.x *= - dx / len;
			force.y  = s.springconstant  * (len - s.restlength);
			force.y += s.dampingconstant * (pp1.vel.y - pp2.vel.y) * dy / len;
			force.y *= - dy / len;
			force.z  = s.springconstant  * (len - s.restlength);
			force.z += s.dampingconstant * (pp1.vel.z - pp2.vel.z) * dz / len;
			force.z *= - dz / len;
			if (!pp1.fixed) 
			{
				pp1.force.x += force.x;
				pp1.force.y += force.y;
				pp1.force.z += force.z;
			}
			if (!pp2.fixed) 
			{
				pp2.force.x -= force.x;
				pp2.force.y -= force.y;
				pp2.force.z -= force.z;
			}
		}
	}
	
	public void calculateDerivatives ( Derivatives[] deriv )
	{
		int i;
		Particle p;

		for (i=0;i<particles.size();i++) 
		{
			p				= particles.get( i );
			deriv[i]		= new Derivatives();
			deriv[i].dpdt.x = p.vel.x;
			deriv[i].dpdt.y = p.vel.y;
			deriv[i].dpdt.z = p.vel.z;
			deriv[i].dvdt.x = p.force.x / p.mass;
			deriv[i].dvdt.y = p.force.y / p.mass;
			deriv[i].dvdt.z = p.force.z / p.mass;
		}
	}
	
	public void addMagnetVector ( Vec3D v )
	{
		Particle p;
		Vec3D magnet;
		int i;
		for ( i=0; i<particles.size(); i++ ) 
		{
			p		= particles.get( i );
			magnet	= p.loc.sub( v ).normalize();
			p.vel.addSelf( magnet );
		}
	}
	
	class Particle
	{
		int id;
		float renderSize = 20;
		float mass;
		Vec3D loc;
		Vec3D vel;
		Vec3D force;
		Boolean fixed;
		
		public Particle()
		{
			loc		= new Vec3D();
			vel		= new Vec3D();
			force	= new Vec3D();
		}
		
		public void update ()
		{
			
		}
		
		public void render ()
		{
			float deltaX   = camTarget.x - camPosition.x;
			float deltaY   = camTarget.y - camPosition.y;
			float deltaZ   = camTarget.z - camPosition.z;

			float angleZ   = atan2( deltaY,deltaX ); 
			float hyp      = sqrt( sq( deltaX ) + sq( deltaY ) ); 
			float angleY   = atan2( hyp,deltaZ );
			
			gl.glPushMatrix();
			gl.glTranslatef( loc.x, loc.y, loc.z );
			gl.glRotatef( degrees( angleZ ), 0, 0, 1 );
			gl.glRotatef( degrees( angleY ), 0, 1, 0 );
			gl.glScalef( renderSize, renderSize, 0 );
			gl.glBegin( GL.GL_QUADS );
			
			gl.glColor4f( 1, 1, 1, 1 );
			gl.glTexCoord2f( 0, 0 );	gl.glVertex3f( -0.5f, -0.5f, 0 );
			gl.glTexCoord2f( 0, 1 );	gl.glVertex3f( -0.5f,  0.5f, 0 );
			gl.glTexCoord2f( 1, 1 );	gl.glVertex3f(  0.5f,  0.5f, 0 );
			gl.glTexCoord2f( 1, 0 );	gl.glVertex3f(  0.5f, -0.5f, 0 );
			
			gl.glEnd();
			gl.glPopMatrix();
		}
		
		public Particle copy()
		{
			Particle p 		= new Particle();
			p.renderSize	= renderSize;
			p.mass 			= mass;
			p.loc 			= p.loc.copy();
			p.vel 			= p.vel.copy();
			p.force 		= p.force.copy();
			p.fixed 		= fixed;;
			return p;
		}
		
		public String toString()
		{
			return "particle " + id + " - loc :: " + loc.toString() + " - vel :: " + vel.toString();
		}
	}
	
	class Spring
	{
		int from;
		int to;
		float springconstant;
		float dampingconstant;   
		float restlength;
		
		public Spring()
		{
			
		}
	}
	
	class Derivatives
	{
		Vec3D dpdt;
		Vec3D dvdt;
		
		public Derivatives()
		{
			dpdt = new Vec3D();
			dvdt = new Vec3D();
		}
	}
	
    private static class Point3D {			// Structure for a 3-dimensional point (NEW)
        public double x, y, z;

        public Point3D() {
        }

        public Point3D(double x, double y, double z) {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        public Point3D add(Point3D q) {
            return new Point3D(x + q.x, y + q.y, z + q.z);
        }

        public Point3D scale(double c) {
            return new Point3D(x * c, y * c, z * c);
        }

        /**
         * Calculates 3rd degree polynomial based on array of 4 points
         * and a single variable (u) which is generally between 0 and 1
         */
        public static Point3D bernstein( float u, Point3D[] p ) {
            Point3D a = p[0].scale(Math.pow(u, 3));
            Point3D b = p[1].scale(3 * Math.pow(u, 2) * (1 - u));
            Point3D c = p[2].scale(3 * u * Math.pow((1 - u), 2));
            Point3D d = p[3].scale(Math.pow((1 - u), 3));

            return a.add(b).add(c).add(d);
        }
    }
}