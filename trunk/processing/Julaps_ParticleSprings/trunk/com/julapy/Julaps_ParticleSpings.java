package com.julapy;

////////////////////////////////////////////////////////////////////////////////
//
// Project: 		Julaps_ParticleSpings
// File: 			Julaps_ParticleSpings.java
// Created by: 		lukaszkarluk, Jun 7, 2008
//
////////////////////////////////////////////////////////////////////////////////

import javax.media.opengl.GL;
import damkjer.ocd.Camera;
import processing.opengl.PGraphicsOpenGL;
import processing.core.*;
import toxi.geom.Vec3D;

public class Julaps_ParticleSpings extends PApplet 
{
	PGraphicsOpenGL pgl;
	GL gl;
	
	Camera cam;
	Vec3D camTarget;
	Vec3D camPosition;
	Vec3D camNormal;

	Particle[] particles;
	Spring[] springs;
	int nparticles = 0;
	int nsprings = 0;
	double gravitational = 0;
	double viscousdrag = 0.1;
	int particleCloudSize = 500;
	
	public void setup() 
	{
		size( 1280, 720, OPENGL );

		frameRate( 25 );
		
		colorMode( RGB, 1.0f );
		
		hint(ENABLE_OPENGL_4X_SMOOTH);

		pgl = (PGraphicsOpenGL) g;
		gl = pgl.gl;
		
		//__________________________________________________________ camera.		
		
		cam 		= new Camera( this );
		camTarget 	= new Vec3D();
		camPosition = new Vec3D();
		camNormal 	= new Vec3D();

		cam.jump( 0, 250, 300 );
		cam.aim( 0, 0, 0 );
		cam.feed();
		
		//__________________________________________________________ psys.
		
		initParticles();
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

		camPosition.set( cam.position()[0], cam.position()[1], cam.position()[2] );
		camTarget.set( cam.target()[0], cam.target()[1], cam.target()[2] );
		camNormal = camPosition.sub(camTarget).normalize();
		
		//__________________________________________________________ psys.
		
		updateParticles( 0.1, 1 );
		
		//__________________________________________________________ render.
		   
		pgl.beginGL();

		int i,p1,p2;
		for (i=0;i<nparticles;i++)
			particles[i].render();

		for (i=0;i<nsprings;i++) 
		{
			p1 = springs[i].from;
			p2 = springs[i].to;
			gl.glBegin( GL.GL_LINES );
			gl.glColor4f( 1, 1, 1, 1 );
			gl.glVertex3f( particles[p1].loc.x, particles[p1].loc.y, particles[p1].loc.z );
			gl.glVertex3f( particles[p2].loc.x, particles[p2].loc.y, particles[p2].loc.z );
			gl.glEnd();
		}

		pgl.endGL();
	}
	
	public void initParticles ()
	{
		int i;
		nparticles	= 5;
		nsprings	= 9;

		particles	= new Particle[nparticles];
		springs		= new Spring[nsprings];

		/* Random positions */
		for ( i=0; i<nparticles; i++ ) 
		{
			particles[i] 		= new Particle();
			particles[i].mass 	= 1;
			particles[i].fixed 	= false;
			particles[i].loc.x	= random(particleCloudSize) - particleCloudSize/2;
			particles[i].loc.y	= random(particleCloudSize) - particleCloudSize/2;
			particles[i].loc.z	= random(particleCloudSize) - particleCloudSize/2;
		}

		for ( i=0; i<nsprings; i++ ) 
		{
			springs[i]					= new Spring();
			springs[i].springconstant	= 0.1f;
			springs[i].dampingconstant	= 0.01f;
			springs[i].restlength 		= 400;
		}
		
		/* Edges */
		springs[0].from  = 0; springs[0].to  = 1;
		springs[1].from  = 0; springs[1].to  = 2;
		springs[2].from  = 0; springs[2].to  = 3;
		springs[3].from  = 4; springs[3].to  = 1;
		springs[4].from  = 4; springs[4].to  = 2;
		springs[5].from  = 4; springs[5].to  = 3;
		springs[6].from  = 1; springs[6].to  = 2;
		springs[7].from  = 2; springs[7].to  = 3;
		springs[8].from  = 3; springs[8].to  = 1;
	}

	public void updateParticles ( double dt, int method )
	{
		int i;
		Particle[] ptmp;
		Derivatives[] deriv = new Derivatives[nparticles];

		switch ( method )
		{
			case 0:                                   /* Euler */
				calculateForces(  );
				calculateDerivatives( deriv );
				for ( i=0; i<nparticles; i++ ) 
				{
					particles[i].loc.x += deriv[i].dpdt.x * dt;
					particles[i].loc.y += deriv[i].dpdt.y * dt;
					particles[i].loc.z += deriv[i].dpdt.z * dt;
					particles[i].vel.x += deriv[i].dvdt.x * dt;
					particles[i].vel.y += deriv[i].dvdt.y * dt;
					particles[i].vel.z += deriv[i].dvdt.z * dt;
				}
			break;
			
			case 1:                                   /* Midpoint */
				calculateForces( );
				calculateDerivatives( deriv );
				ptmp = new Particle[nparticles];
				for ( i=0; i<nparticles; i++ ) 
				{
					ptmp[i] = particles[i].copy();
					ptmp[i].loc.x += deriv[i].dpdt.x * dt / 2;
					ptmp[i].loc.y += deriv[i].dpdt.y * dt / 2;
					ptmp[i].loc.z += deriv[i].dpdt.z * dt / 2;
					ptmp[i].loc.x += deriv[i].dvdt.x * dt / 2;
					ptmp[i].loc.y += deriv[i].dvdt.y * dt / 2;
					ptmp[i].loc.z += deriv[i].dvdt.z * dt / 2;
				}
				calculateForces( );
				calculateDerivatives( deriv );
				for ( i=0; i<nparticles; i++ ) 
				{
					particles[i].loc.x += deriv[i].dpdt.x * dt;
					particles[i].loc.y += deriv[i].dpdt.y * dt;
					particles[i].loc.z += deriv[i].dpdt.z * dt;
					particles[i].vel.x += deriv[i].dvdt.x * dt;
					particles[i].vel.y += deriv[i].dvdt.y * dt;
					particles[i].vel.z += deriv[i].dvdt.z * dt;
				}
			break;
		}
	}

	public void calculateForces ( )
	{
		int i, p1, p2;
		float len, dx, dy, dz;
		Vec3D down	= new Vec3D( 0, 0, -1 );
		Vec3D force = new Vec3D( );

		for ( i=0; i<nparticles; i++ ) 
		{
			particles[i].force = new Vec3D();
			if (particles[i].fixed)
				continue;

			/* Gravitation */
			particles[i].force.x += gravitational * particles[i].mass * down.x;
			particles[i].force.y += gravitational * particles[i].mass * down.y;
			particles[i].force.z += gravitational * particles[i].mass * down.z;

			/* Viscous drag */
			particles[i].force.x -= viscousdrag * particles[i].vel.x;
			particles[i].force.y -= viscousdrag * particles[i].vel.y;
			particles[i].force.z -= viscousdrag * particles[i].vel.z;
		}

		/* Handle the spring interaction */
		for ( i=0; i<nsprings; i++ ) 
		{
			p1 = springs[i].from;
			p2 = springs[i].to;
			dx = particles[p1].loc.x - particles[p2].loc.x;
			dy = particles[p1].loc.y - particles[p2].loc.y;
			dz = particles[p1].loc.z - particles[p2].loc.z;
			len = sqrt( dx*dx + dy*dy + dz*dz );
			force.x  = springs[i].springconstant  * (len - springs[i].restlength);
			force.x += springs[i].dampingconstant * (particles[p1].vel.x - particles[p2].vel.x) * dx / len;
			force.x *= - dx / len;
			force.y  = springs[i].springconstant  * (len - springs[i].restlength);
			force.y += springs[i].dampingconstant * (particles[p1].vel.y - particles[p2].vel.y) * dy / len;
			force.y *= - dy / len;
			force.z  = springs[i].springconstant  * (len - springs[i].restlength);
			force.z += springs[i].dampingconstant * (particles[p1].vel.z - particles[p2].vel.z) * dz / len;
			force.z *= - dz / len;
			if (!particles[p1].fixed) 
			{
				particles[p1].force.x += force.x;
				particles[p1].force.y += force.y;
				particles[p1].force.z += force.z;
			}
			if (!particles[p2].fixed) 
			{
				particles[p2].force.x -= force.x;
				particles[p2].force.y -= force.y;
				particles[p2].force.z -= force.z;
			}
		}
	}
	
	public void calculateDerivatives ( Derivatives[] deriv )
	{
		int i;

		for (i=0;i<nparticles;i++) 
		{
			deriv[i]		= new Derivatives();
			deriv[i].dpdt.x = particles[i].vel.x;
			deriv[i].dpdt.y = particles[i].vel.y;
			deriv[i].dpdt.z = particles[i].vel.z;
			deriv[i].dvdt.x = particles[i].force.x / particles[i].mass;
			deriv[i].dvdt.y = particles[i].force.y / particles[i].mass;
			deriv[i].dvdt.z = particles[i].force.z / particles[i].mass;
		}
	}
	
	class Particle
	{
		float renderSize = 10;
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
			gl.glRotatef( degrees( angleZ ), 0, 0,    1.0f );
			gl.glRotatef( degrees( angleY ), 0, 1.0f, 0    );
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
}