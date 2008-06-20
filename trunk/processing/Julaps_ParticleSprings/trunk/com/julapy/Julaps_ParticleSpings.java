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

import com.julapy.opengl.Primitive;
import com.julapy.opengl.TextureLoader;
import com.julapy.utils.TextFileUtil;

import krister.Ess.AudioChannel;
import krister.Ess.Ess;
import krister.Ess.FFT;
import damkjer.ocd.Camera;
import processing.opengl.PGraphicsOpenGL;
import processing.core.PApplet;
import toxi.geom.Vec3D;

public class Julaps_ParticleSpings extends PApplet 
{
	PGraphicsOpenGL pgl;
	GL gl;
	TextureLoader texLoader;
	Primitive prim;
	int particleCallList;
    float[] lightAmbient =	{ 0.5f, 1.0f, 0.2f, 1.0f }; // DOESN'T WORK
    float[] lightDiffuse =	{ 1.0f, 0.7f, 0.5f, 1.0f }; // DOESN'T WORK
    float[] lightPosition =	{ 0.0f, 0.0f, 2.0f, 1.0f }; // DOESN'T WORK
	
	Camera cam;
	Vec3D camTarget;
	Vec3D camPosition;
	Vec3D camNormal;
	
	AudioChannel channel;
	FFT fft;
	
	int[] textures;
	
	ArrayList<Particle> particles;
	ArrayList<Spring> springs;
	ArrayList<CurveHop> curvehops;
	CurveHop camCurve;
	
	int nparticles = 0;
	double gravitational = 0;
	double viscousdrag = 0.1;
	int particleCloudSize = 1;
	float centerPush = 200;
	float centerPull = 1;
	
	int frameNumber = 0;
	int framesPerSec = 25;
	boolean isRecording = false;
	boolean readFromFile = true;
	
	public void setup()
	{
		size( 1280, 720, OPENGL );

		frameRate( framesPerSec );
		
		colorMode( RGB, 1.0f );
		
		hint(ENABLE_OPENGL_4X_SMOOTH);

		//__________________________________________________________ init.
		
		nparticles	= 300;

		particles	= new ArrayList<Particle>();
		springs		= new ArrayList<Spring>();
		curvehops	= new ArrayList<CurveHop>();
		
		//__________________________________________________________ opengl.
		
		initGL();
		initTextureList();
		
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
		
		for( int i=0; i<1; i++ ) {
			addCurveHop( );
		}
		camCurve = curvehops.get( 0 );
	}
	
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
		
		/* define blend mode */
		gl.glEnable( GL.GL_BLEND );
		gl.glDisable( GL.GL_DEPTH_TEST );
		gl.glBlendFunc( GL.GL_SRC_ALPHA, GL.GL_ONE );
		gl.glBlendFunc( GL.GL_SRC_ALPHA, GL.GL_ONE_MINUS_SRC_COLOR );
		
		/* turn on lighting */
        gl.glLightfv( GL.GL_LIGHT0, GL.GL_AMBIENT, lightAmbient, 0 );
        gl.glLightfv( GL.GL_LIGHT0, GL.GL_DIFFUSE, lightDiffuse, 0 );
        gl.glLightfv( GL.GL_LIGHT0, GL.GL_POSITION, lightPosition, 0 );
        gl.glEnable( GL.GL_LIGHT0 );
        gl.glEnable( GL.GL_LIGHTING );
        
        /* load texture */
		texLoader = new TextureLoader( gl );
		texLoader.init();
		texLoader.loadTexture( loadImage( "data/texture/p_03.png" ), true );
		
		prim = new Primitive( gl );
	}
	
	private void initTextureList ()
	{
		particleCallList = gl.glGenLists( 1 );
		gl.glNewList( particleCallList, GL.GL_COMPILE );
			gl.glBegin( GL.GL_QUADS );
			gl.glNormal3f( 0, 0, 1 );
			gl.glTexCoord2f( 0, 0 );	gl.glVertex3f( -0.5f, -0.5f, 0 );
			gl.glTexCoord2f( 0, 1 );	gl.glVertex3f( -0.5f,  0.5f, 0 );
			gl.glTexCoord2f( 1, 1 );	gl.glVertex3f(  0.5f,  0.5f, 0 );
			gl.glTexCoord2f( 1, 0 );	gl.glVertex3f(  0.5f, -0.5f, 0 );
			gl.glEnd();
		gl.glEndList();
	}
	
	public void draw() 
	{
		background(0);
		
		//__________________________________________________________ camera.
		
		float camX = (float)mouseX/width * width - width/2;
		float camY = (float)mouseY/height * height - height/2;
		
		Vec3D c;
		c = camCurve.loc.copy().normalize().scaleSelf( 350 );
		cam.jump( c.x, c.y, c.z );
		cam.aim( 0, 0, 0 );
		
//		cam.jump( camX, camY, 500 );
//		cam.aim( 0, 0, 0 );
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
		
//		reactParticles();
		
		//__________________________________________________________ psys.
		
//		if( mousePressed )
//			addMagnetVector( new Vec3D() );
		
		centerPush = max( 200, centerPush *= 0.9f );

		changeSpringLength();
		
		updateParticles( 0.1, 0 );
		
		//__________________________________________________________ render.
		   
		pgl.beginGL();

		int i, j, p1, p2;
		int bezDetail = 20;
		int sphereSize = 130;
		float bt;
		Point3D[] bezPoints;
		Point3D bezPoint;
		Particle p, pp1, pp2;
		
		/* draw sphere */
		gl.glPushMatrix();
		gl.glScalef( sphereSize, sphereSize, sphereSize );
		gl.glColor4f( 0, 0, 0, 1 );
		prim.drawSphere( 30 );
		gl.glPopMatrix();
		
		/* draw particles */
		gl.glEnable(GL.GL_TEXTURE_2D);
		gl.glDepthMask( false );
		gl.glBindTexture( GL.GL_TEXTURE_2D, texLoader.getTexture( 0 ) );
		for ( i=0; i<particles.size(); i++ ) {
			p = particles.get( i );
			p.render();
		}

		/* draw lines between particles and center */
//		gl.glDisable(GL.GL_TEXTURE_2D);
//		for ( i=0; i<particles.size(); i++ ) {
//			p = particles.get( i );
//			gl.glBegin( GL.GL_LINES );
//			gl.glColor4f( 1, 1, 1, 0.1f );
//			gl.glVertex3f( p.loc.x, p.loc.y, p.loc.z );
//			gl.glVertex3f( 0, 0, 0 );
//			gl.glEnd();
//		}
		
		/* draw bezier spring connections */
//		gl.glDisable(GL.GL_TEXTURE_2D);
//		gl.glDepthMask( true );
//		for ( i=0; i<springs.size(); i++ ) {
//			p1 	= springs.get( i ).from;
//			p2 	= springs.get( i ).to;
//			pp1	= particles.get( p1 );
//			pp2 = particles.get( p2 );
//			bezPoints = new Point3D[4];
//			bezPoints[0] = new Point3D( pp1.loc.x, pp1.loc.y, pp1.loc.z );
//			bezPoints[1] = new Point3D( 0, 0, 0 );
//			bezPoints[2] = new Point3D( 0, 0, 0 );
//			bezPoints[3] = new Point3D( pp2.loc.x, pp2.loc.y, pp2.loc.z );
//
//			gl.glBegin( GL.GL_LINE_STRIP );
//			gl.glColor4f( 1, 1, 1, 0.1f );
//            for ( j=0; j<=bezDetail; j++) {								// draw bezier connections.
//            	bt 			= ((float) j) / ((float) bezDetail);		// percent of bezier.
//            	bezPoint 	= Point3D.bernstein( bt, bezPoints );		// generate new point.
//            	gl.glVertex3d( bezPoint.x, bezPoint.y, bezPoint.z );	// draw vertex for point.
//            }
//			gl.glEnd();
//		}

		/* draw straight spring connections */
//		gl.glDisable(GL.GL_TEXTURE_2D);
//		gl.glDepthMask( true );
//		for ( i=0; i<springs.size(); i++ ) {
//			p1 	= springs.get( i ).from;
//			p2 	= springs.get( i ).to;
//			pp1	= particles.get( p1 );
//			pp2 = particles.get( p2 );
//			gl.glVertex3f( pp1.loc.x, pp1.loc.y, pp1.loc.z );
//			gl.glVertex3f( pp2.loc.x, pp2.loc.y, pp2.loc.z );
//			gl.glEnd();
//		}
		
		/* draw curve hops */
		gl.glDisable(GL.GL_TEXTURE_2D);
		gl.glDepthMask( true );
		CurveHop curvehop;
		for ( i=0; i<curvehops.size(); i++ ) 
		{
			curvehop = curvehops.get( i );

			if( curvehop.playedOut ) 
				curvehops.remove( i-- );

			if( !curvehop.playedIn )
			{
				curvehop.playInStep();
			}
			else
			{
				curvehop.playOutStep();
				if( curvehop.spawn )
					curvehop.spawn();
			}
				
			curvehop.render();
		}
		
		pgl.endGL();
		
		if( isRecording ) save("data/export/export"+ frameNumber++ +".png");
	}
	
	public void mousePressed ()
	{
//		addParticle( );
		
//		centerPush = 500;
		
		/* add curve */
		int i;
		for( i=0; i<4; i++ ) {
			addCurveHop( );
		}
	}
	
	public void keyPressed ()
	{
		if( key == 's' )
		{
			TextFileUtil tfu = new TextFileUtil( this );
			tfu.createWriter( "data/particle_positions.txt" );
			for( int i=0; i<particles.size(); i++ ) {
				Particle p = particles.get( i );
				float[] pp = new float[3];
				pp[0] = p.loc.x;
				pp[1] = p.loc.y;
				pp[2] = p.loc.z;
				tfu.writeFloatLine( pp );
			}
			tfu.closeWriter();
		}
		
		if( key == 'r' )
		{
			isRecording = !isRecording;
		}
	}
	
	public void initEss ()
	{
		Ess.start( this );

		channel = new AudioChannel( "audio/cell.aif" );
		
		if( !isRecording )
			channel.play( Ess.FOREVER );
		
		fft = new FFT( 512 );
		fft.equalizer( true );
		fft.smooth = true;
//		fft.damp( 0.5f );
	}
	
	public void initParticles ()
	{
		int i, j;

		/* center particle */
		Particle p	= new Particle();
		p.id		= particles.size();
		p.mass 		= 1;
		p.fixed 	= true;
		particles.add( p );
		
		/* add particles */
		for ( i=0; i<nparticles; i++ ) {
			addParticle();
		}

		/* read particle postions from file */
		if( readFromFile )
		{
			TextFileUtil tfu = new TextFileUtil( this );
			tfu.readDataFromFile( "data/particle_positions.txt" );
			for( i=0; i<particles.size(); i++ ) {
				float[] pp = tfu.readFloatLine();
				p = particles.get( i );
				p.loc.x = pp[0];
				p.loc.y = pp[1];
				p.loc.z = pp[2];
			}
		}
		
		/* work out particle neighbours */
		Particle p1;
		Particle p2;
		for ( i=0; i<particles.size(); i++ ) {
			p1 = particles.get( i );
			for ( j=0; j<particles.size(); j++ ) {
				p2 = particles.get( j );
				if( p1 != p2 )
				{
					float d = p1.loc.distanceTo( p2.loc );
					if( d < 70 )
						p1.neighbours.add( p2 );
				}
			}
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
			s.from				= p.id;
			s.to				= pp.id;
			if( s.to == 0 )
			{
				s.springconstant	= 0.1f;
				s.dampingconstant	= 0.05f;
				s.restlength 		= 180;
			}
			else
			{
				s.springconstant	= 0.1f;
				s.dampingconstant	= 0.05f;
				s.restlength 		= 200;
			}
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
		Vec3D center;

		switch ( method )
		{
			case 0:                                   /* Euler */
				calculateSpringForces(  );
				for ( i=0; i<particles.size(); i++ ) 
				{
					p		= particles.get( i );
					p.loc.x += p.vel.x * dt;
					p.loc.y += p.vel.y * dt;
					p.loc.z += p.vel.z * dt;
					p.vel.x += p.force.x / p.mass * dt;
					p.vel.y += p.force.y / p.mass * dt;
					p.vel.z += p.force.z / p.mass * dt;
				}
			break;
			
			case 1:                                   /* Midpoint */
				calculateSpringForces( );
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
				calculateSpringForces( );
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

	public void calculateSpringForces ( )
	{
		int i = 0, p1 = 0, p2 = 0;
		float len, dx, dy, dz;
		Vec3D down	= new Vec3D( 0, 0, -1 );
		Vec3D center = new Vec3D( );
		Vec3D force = new Vec3D( );
		Vec3D out = new Vec3D();
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
			
			if( !pp1.fixed )
			{
				pp1.force.x += force.x;
				pp1.force.y += force.y;
				pp1.force.z += force.z;
			}
			if( !pp2.fixed ) 
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
	
	public void changeSpringLength ()
	{
		int i;
		Spring s;
		for( i=0; i<springs.size(); i++ ) {
			s = springs.get( i );
			if( s.to == 0 )
			{
				s.springconstant	= 0.1f;
				s.dampingconstant	= 0.05f;
				s.restlength 		= centerPush * 0.9f;
			}
			else
			{
				s.springconstant	= 0.1f;
				s.dampingconstant	= 0.05f;
				s.restlength 		= centerPush;
			}
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
			magnet.scaleSelf( centerPush );
			p.vel.addSelf( magnet );
		}
	}
	
	public void addCurveHop ( )
	{
		Particle p1, p2;
		Vec3D c1, c2;
		
		p1 = particles.get( (int)(random(particles.size()-1)) );
		p2 = p1.neighbours.get( (int)(random(p1.neighbours.size()-1)) );
		c1 = p1.loc.copy().scaleSelf( 2 );
		c2 = p2.loc.copy().scaleSelf( 2 );
		curvehops.add( new CurveHop( p1, c1, c2, p2, 3 ));
	}
	
	class Particle
	{
		int id;
		float renderSize = 20;
		float mass;
		Vec3D loc;
		Vec3D vel;
		Vec3D force;
		Vec3D center;
		Boolean fixed;
		float r, g, b;
		ArrayList<Particle> neighbours;
		
		public Particle()
		{
			loc		= new Vec3D();
			vel		= new Vec3D();
			force	= new Vec3D();
			
			neighbours = new ArrayList<Particle>();
			
			r = g = b = 1;
		}
		
		public void update ()
		{
			
		}
		
		public void render ()
		{
			/* face particle to camera */
//			float deltaX   = camTarget.x - camPosition.x;
//			float deltaY   = camTarget.y - camPosition.y;
//			float deltaZ   = camTarget.z - camPosition.z;

			/* face particle outwards from center */
			float deltaX   = loc.x;
			float deltaY   = loc.y;
			float deltaZ   = loc.z;

			float angleZ   = atan2( deltaY, deltaX ); 
			float angleY   = atan2( sqrt( sq( deltaX ) + sq( deltaY ) ), deltaZ );

//			float r = mouseX / (float)width;
//			float g = mouseY / (float)height;
//			float b = max( 0, sin( (frameCount / (float)720) * 2 * PI  ) );
			
			// r = 0-1;
			// g = 0.3;
			// b = 0.42;
			
			gl.glPushMatrix();
			gl.glTranslatef( loc.x, loc.y, loc.z );
			gl.glRotatef( degrees( angleZ ), 0, 0, 1 );
			gl.glRotatef( degrees( angleY ), 0, 1, 0 );
			
			gl.glColor4f( r, g, b, 1 );
			gl.glScalef( renderSize, renderSize, 0 );
			gl.glCallList( particleCallList );
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
	
	class CurveHop
	{
		Particle p, p1, p2;
		Vec3D c, c1, c2;
		Vec3D loc;
		float progressIn = 0;
		float progressOut = 0;
		float progressStep = 0.05f;
		int bezierDetail = 50;
		boolean playedIn = false;
		boolean playedOut = false;
		boolean spawn = false;
		float r, g, b;
		int lifetime;
		int lifetimeMax = 3;
		
		public CurveHop( Particle p1, Vec3D c1, Vec3D c2, Particle p2, int lifetime )
		{
			this.p1 = p1;
			this.p2 = p2;
			this.c1 = c1;
			this.c2 = c2;
			
			this.lifetime = lifetime;
			
			progressStep += random( 0, 0.02f );
			
			loc = p1.loc.copy();
			
			r = g = b = random( 0.001f, 0.005f );
		}
		
		public void spawn ()
		{
			if( lifetime > 0 )
			{
				int i;

				ArrayList<Particle> neighbours = new ArrayList<Particle>();	// probably a better way of copying an ArrayList
				for( i=0; i<p2.neighbours.size(); i++ ) {
					neighbours.add( p2.neighbours.get(i) );
				}
				neighbours.remove( p1 );
				
				p = neighbours.get( (int)( random( neighbours.size() - 1 ) ) );
				c = p.loc.copy().scaleSelf( lifetime / (float)lifetimeMax + 1 );
				neighbours.remove( p );
				
				if( lifetime == lifetimeMax )
					curvehops.add( camCurve = new CurveHop( p2, c2, c, p, lifetimeMax ) );
				else
					curvehops.add( new CurveHop( p2, c2, c, p, lifetime-1 ) );

				lifetime--;
				
				for( i=0; i<neighbours.size(); i++ ) {
					if( random(1) < 0.3 )
					{
						p = neighbours.get( 0 );
						c = p.loc.copy().scaleSelf( lifetime / (float)lifetimeMax + 1 );
						curvehops.add( new CurveHop( p2, c2, c, p, lifetime ) );
					}
				}
			}
			
			spawn = false;
		}
		
		public void playInStep ()
		{
			progressOut += progressStep;
			
			p1.r -= r * progressStep;
			p1.g -= g * progressStep;
			p1.b -= b * progressStep;
			
			if( progressOut >= 1 )
			{
				playedIn = true;
				spawn = true;
			}
			
		}
		
		public void playOutStep ()
		{
			progressIn += progressStep;
			if( progressIn >= 1 )
				playedOut = true;
		}
		
		public void render ()
		{
			int j, j1, j2;
			float dt;
			Point3D[] bezPoints;
			Point3D prevPoint = new Point3D();
			Point3D nextPoint = new Point3D();
			Vec3D prev, next, up, right, look;
			
			bezPoints = new Point3D[4];
			bezPoints[0] = new Point3D( p2.loc.x, p2.loc.y, p2.loc.z );
			bezPoints[1] = new Point3D( c2.x, c2.y, c2.z );
			bezPoints[2] = new Point3D( c1.x, c1.y, c1.z );
			bezPoints[3] = new Point3D( p1.loc.x, p1.loc.y, p1.loc.z );

			gl.glBegin( GL.GL_QUAD_STRIP );
			
			j1 = (int)( bezierDetail * progressIn );
			j2 = (int)( bezierDetail * progressOut );
            for ( j=j1; j<=j2; j++) {									// draw bezier connections.
            	dt = ((float) j) / ((float) bezierDetail);				// percent of bezier.
            	if( j == j1 ) 
            		prevPoint = Point3D.bernstein( dt, bezPoints );			// generate new point.
            	else
            	{
            		nextPoint = Point3D.bernstein( dt, bezPoints );			// generate new point.
            		
            		// cennect prev point with next point.
            		prev	= new Vec3D( (float)prevPoint.x, (float)prevPoint.y, (float)prevPoint.z );
            		next	= new Vec3D( (float)nextPoint.x, (float)nextPoint.y, (float)nextPoint.z );
            		up		= prev.sub( next );
            		right	= up.cross( prev ).normalize();
            		look	= up.cross( right ).normalize();
            		right	= up.cross( look ).normalize();
  
            		float ss	= sin( dt * PI );		// sine sale.
            		float xOff	= right.x * 6 * ss;
            		float yOff	= right.y * 6 * ss;
            		float zOff	= right.z * 6 * ss;
        	        
        	        gl.glColor4f( 1, 0.75f * ss, 0.23f * ss, max( 0.2f, ss ) );
        	        gl.glVertex3f( prev.x - xOff, prev.y - yOff, prev.z - zOff );
        	        gl.glVertex3f( prev.x + xOff, prev.y + yOff, prev.z + zOff );
            		
        	        /* draw line */
//            		gl.glVertex3d( nextPoint.x, nextPoint.y, nextPoint.z );	// draw vertex for point.
            		
            		prevPoint 	= nextPoint;
            		loc 		= next.copy();
            	}
            }
			gl.glEnd();
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