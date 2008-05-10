package com.julapy;

////////////////////////////////////////////////////////////////////////////////
//
// Project: 		Julaps_Star
// File: 			Julaps_Star.java
// Created by: 		lukaszkarluk, May 8, 2008
//
////////////////////////////////////////////////////////////////////////////////

import java.nio.ByteBuffer;
import java.util.ArrayList;

import javax.media.opengl.GL;

import com.sun.opengl.util.BufferUtil;

import damkjer.ocd.Camera;
import processing.opengl.PGraphicsOpenGL;
import processing.core.*;
import toxi.geom.Vec3D;

public class Julaps_Star extends PApplet 
{
	PGraphicsOpenGL pgl;
	GL gl;
	
	float sinLUT[];
	float cosLUT[];	
	float SINCOS_PRECISION=1.0f;
	int SINCOS_LENGTH= (int)(360.0/SINCOS_PRECISION);
	
	Camera cam;
	Vec3D camVec;
	Vec3D camTarget;
	Vec3D camPosition;
	Vec3D camNormal;
	
	boolean isRecording = false;
	ArrayList<Shockwave> shockwaves;
	ArrayList<Star> stars;
	SphericalParticle cameraParticle;
	
	int[] textures;
	int cylds;
	Vec3D[][] cylvert;
	
	float fogColor[] = {0.0f, 0.0f, 0.0f, 1.0f};		// Fog Color               ( new )	
	
	public void setup() 
	{
		size(1280, 720, OPENGL);

		frameRate(50);

		hint(ENABLE_OPENGL_4X_SMOOTH);
		colorMode(RGB, 1.0f);

		pgl = (PGraphicsOpenGL) g;
		gl = pgl.gl;

		gl.glShadeModel(GL.GL_SMOOTH);              				// Enable Smooth Shading
		gl.glClearColor(0.0f, 0.0f, 0.0f, 0.5f);    				// Black Background
		gl.glClearDepth(1.0f);                      				// Depth Buffer Setup
		gl.glEnable(GL.GL_DEPTH_TEST);								// Enables Depth Testing
		gl.glDepthFunc(GL.GL_LEQUAL);								// The Type Of Depth Testing To Do
		gl.glHint(GL.GL_PERSPECTIVE_CORRECTION_HINT, GL.GL_NICEST);	// Really Nice Perspective Calculations
		gl.glEnable(GL.GL_TEXTURE_2D);
		
		// load texture.
		PImage ray = loadImage("data/ray01.png");
		
		glGenTextures( gl );
		
		gl.glBindTexture(GL.GL_TEXTURE_2D, textures[0]);
		glTexImage2D( ray, true );
		gl.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_MIN_FILTER, GL.GL_LINEAR);
		gl.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_MAG_FILTER, GL.GL_LINEAR);
        
//		gl.glFogi(GL.GL_FOG_MODE, GL.GL_LINEAR);				// Fog Mode
//		gl.glFogfv(GL.GL_FOG_COLOR, fogColor, 0);				// Set Fog Color
//		gl.glFogf(GL.GL_FOG_DENSITY, 0.1f);						// How Dense Will The Fog Be
//		gl.glHint(GL.GL_FOG_HINT, GL.GL_DONT_CARE);				// Fog Hint Value
//		gl.glFogf(GL.GL_FOG_START, 100.0f);						// Fog Start Depth
//		gl.glFogf(GL.GL_FOG_END, 300.0f);						// Fog End Depth
//		gl.glEnable(GL.GL_FOG);									// Enables GL.GL_FOG
		
//		gl.glTexParameterf( GL.GL_TEXTURE_2D, GL.GL_TEXTURE_MAG_FILTER, GL.GL_LINEAR );
//		gl.glTexParameterf( GL.GL_TEXTURE_2D, GL.GL_TEXTURE_MIN_FILTER, GL.GL_LINEAR );
//		gl.glTexEnvf( GL.GL_TEXTURE_ENV, GL.GL_TEXTURE_ENV_MODE, GL.GL_MODULATE );
		
		// additive blending.
		gl.glDisable( GL.GL_DEPTH_TEST );
		gl.glEnable( GL.GL_BLEND );
		gl.glBlendFunc( GL.GL_SRC_ALPHA, GL.GL_ONE );
		gl.glBlendFunc( GL.GL_SRC_ALPHA, GL.GL_ONE_MINUS_SRC_COLOR );
		
		//__________________________________________________________ trig
		
		sinLUT = new float[SINCOS_LENGTH];
		cosLUT = new float[SINCOS_LENGTH];
		for (int i = 0; i < SINCOS_LENGTH; i++)
		{
			sinLUT[i]= (float)Math.sin( i * DEG_TO_RAD * SINCOS_PRECISION );
			cosLUT[i]= (float)Math.cos( i * DEG_TO_RAD * SINCOS_PRECISION );
		}
		
		//__________________________________________________________ camera.		
		
		cam 		= new Camera( this );
		camVec		= new Vec3D();
		camTarget 	= new Vec3D();
		camPosition = new Vec3D();
		camNormal 	= new Vec3D();

		cam.jump( 0, 250, 300 );
		cam.aim( 0, 0, 0 );
		cam.feed();
		
		//__________________________________________________________ init.
		
		initCylinderVertices( 6 );
		initCylinderDisplayList();
		
		shockwaves = new ArrayList<Shockwave>();
		shockwaves.add( new Shockwave() );
		
		stars = new ArrayList<Star>();
		stars.add( new Star( new Vec3D( 0, 0, 0 ), 4 ) );
		
		cameraParticle = new SphericalParticle( 300, 0.01f, 0.01f );
	}
	
    private void glGenTextures (GL gl) {
    	textures = new int[2];
        gl.glGenTextures(2, textures, 0);
    }
	
    private void glTexImage2D( PImage img, Boolean useAlphaChannel )
    {
    	if( useAlphaChannel )
    	{
    		gl.glTexImage2D(GL.GL_TEXTURE_2D, 0, GL.GL_RGBA, img.width, img.height, 0, GL.GL_RGBA, GL.GL_UNSIGNED_BYTE, getTextureByteBuffer( img, true ) );
    	}
    	else
    	{
    		gl.glTexImage2D(GL.GL_TEXTURE_2D, 0, GL.GL_RGB, img.width, img.height, 0, GL.GL_RGB, GL.GL_UNSIGNED_BYTE, getTextureByteBuffer( img, false ) );
    	}
    }
    
    private ByteBuffer getTextureByteBuffer ( PImage img, Boolean useAlphaChannel )
    {
    	int bytesPerPixel = useAlphaChannel ? 4 : 3;
    	
        ByteBuffer unpackedPixels = BufferUtil.newByteBuffer( img.pixels.length * bytesPerPixel );

        for (int row = img.height - 1; row >= 0; row--) {
            for (int col = 0; col < img.width; col++) {
                int packedPixel = img.pixels[row * img.width + col];
                unpackedPixels.put((byte) ((packedPixel >> 16) & 0xFF));
                unpackedPixels.put((byte) ((packedPixel >> 8) & 0xFF));
                unpackedPixels.put((byte) ((packedPixel >> 0) & 0xFF));
                if ( useAlphaChannel ) {
                    unpackedPixels.put((byte) ((packedPixel >> 24) & 0xFF));
                }
            }
        }

        unpackedPixels.flip();
        
        return  unpackedPixels;
    }

	public void draw() 
	{
		background(0);
		
		float camX = (float)mouseX/width * width - width/2;
		float camY = (float)mouseY/height * height - height/2;
		
		updateCameraParticle();
		
		cam.jump( cameraParticle.loc.x, cameraParticle.loc.y, cameraParticle.loc.z );
//		cam.jump( camX, camY, 300 );
		cam.aim( 0, 0, 0 );
		cam.feed();
		
		if (mousePressed == true)
		{
			shockwaves.add( new Shockwave() );
		}
		
		pgl.beginGL();
		
		renderShockwaves();
		renderStars();
		
//		gl.glPushMatrix();
//		gl.glScalef( 10, -100, 10 );
//		gl.glEnable( GL.GL_TEXTURE_2D );
//		gl.glBindTexture( GL.GL_TEXTURE_2D, textures[0] );
//		gl.glCallList( cylds );
//		gl.glPopMatrix();
		
		
		pgl.endGL();
		
		if(isRecording) save("export/image" + frameCount++ + ".png");
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
	
	private void initCylinderVertices ( int detail )
	{
		cylvert = new Vec3D[2][detail+1];
		
		for (int i=0; i<2; i++)
		{
			float angle = 0;
			for(int j=0; j<=detail; j++)
			{
				cylvert[i][j] = new Vec3D();
				cylvert[i][j].x = cos(radians(angle));
				cylvert[i][j].y = i;
				cylvert[i][j].z = sin(radians(angle));
				
				angle += 360.0 / detail;
			}
		}
	}
	
	private void initCylinderDisplayList ()
	{
		cylds = gl.glGenLists( 1 );
		gl.glNewList( cylds, GL.GL_COMPILE );
		
			gl.glBegin( GL.GL_QUAD_STRIP );
			gl.glColor4f( 1, 1, 1, 1 );
			for(int j=0; j<cylvert[0].length; j++)
			{
				gl.glTexCoord2f( j / ( cylvert[0].length - 1 ), 0 );
				gl.glVertex3f( cylvert[0][j].x, cylvert[0][j].y, cylvert[0][j].z );
				gl.glTexCoord2f( j/ (float)( cylvert[1].length - 1 ), 1 );
				gl.glVertex3f( cylvert[1][j].x, cylvert[1][j].y, cylvert[1][j].z );
			}
			gl.glEnd();
			
		gl.glEndList();
	}

	private void renderShockwaves ( )
	{
		gl.glDisable( GL.GL_TEXTURE_2D );
		
		for( int i=0; i<shockwaves.size(); i++ )
		{
			Shockwave sw = shockwaves.get( i ); 
			
			sw.update();
			if( sw.alive ) 
			{
				sw.render();
			}
			else
			{
				shockwaves.remove( i );
				--i;
			}
		}
	}
	
	private void renderStars ()
	{
		gl.glEnable( GL.GL_TEXTURE_2D );
		gl.glBindTexture( GL.GL_TEXTURE_2D, textures[0] );
		
		for( int i=0; i<stars.size(); i++ )
		{
			stars.get( i ).update();
			stars.get( i ).render();
		}
	}
	
	private void updateCameraParticle ()
	{
		cameraParticle.update();
	}
	
	public class Shockwave 
	{
		float x;	// x coord.
		float y;	// y coord.
		float w;	// width of arc.
		float r;	// radius from arc center.
		float c;	// arc circumference in degress.
		float a;	// alpha.
		
		Boolean alive = true;
		
		public Shockwave ()
		{
			x 	= 0;
			y	= 0;
			c	= 360;
			r	= 20;
			w	= 10;
			a	= 1;
		}
		
		public void update ()
		{
			r += 6;
			a -= 0.01;
			
			if( a < 0 ) alive = false;
		}
		
		public void render ( )
		{
			gl.glPushMatrix();
			gl.glRotatef( 90, 1, 0, 0 );
			gl.glBegin( GL.GL_QUAD_STRIP );
			
			int ang = (int)min (c/SINCOS_PRECISION,SINCOS_LENGTH-1);
			
			for (int i = 0; i < ang; i++) 
			{
				float p = 1.0f - (float)i/(float)(ang-1);

				gl.glNormal3f( 0, 1, 0 );
				gl.glColor4f( 1, 1, 1, 0 );
				gl.glVertex3f( cosLUT[i]*(r)+x, sinLUT[i]*(r)+y, 0 );
				gl.glNormal3f( 0, 1, 0 );
				gl.glColor4f( 1, 1, 1, a );
				gl.glVertex3f( cosLUT[i]*(r+w)+x, sinLUT[i]*(r+w)+y, 0 );
				
				if( i == ang - 1 )
				{
					gl.glNormal3f( 0, 1, 0 );
					gl.glColor4f( 1, 1, 1, 0 );
					gl.glVertex3f( cosLUT[0]*(r)+x, sinLUT[0]*(r)+y, 0 );
					gl.glNormal3f( 0, 1, 0 );
					gl.glColor4f( 1, 1, 1, a );
					gl.glVertex3f( cosLUT[0]*(r+w)+x, sinLUT[0]*(r+w)+y, 0 );
				}
			}
			
			gl.glEnd();
			gl.glPopMatrix();
		}
	}
	
	public class Star 
	{
		Vec3D loc;
		int detail;
		ArrayList<RayLight> rays;
		
		public Star ( Vec3D loc, int detail )
		{
			this.loc = loc;
			this.detail = detail;

			float phi = 0.0f;
			float theta = 0.0f;
			float step = 360 / detail;
			float rayWidth = 5.0f;
			float rayHeight = 100.0f;
			
			rays = new ArrayList<RayLight>();
			for( int i=0; i<detail; i++ )
			{
				theta = step * i;
				
				for( int j=0; j<=detail/2; j++ )		// TODO: if detail is not perfectly divisable by 2 this could be a problem.
				{
					phi = step * j;
					
					if( phi == 0 )  // only create one ray for the north pole.
					{
						if( theta == 0 ) rays.add( new RayLight( theta, phi, rayWidth, rayHeight )  );
					}
					else if( phi == 180 ) // only create one ray for the south pole.
					{
						if( theta == 0 ) rays.add( new RayLight( theta, phi, rayWidth, rayHeight )  );
					}
					else
					{
						rays.add( new RayLight( theta, phi, rayWidth, rayHeight )  );
					}
				}
			}
		}
		
		public void update ()
		{
			for( int i=0; i<rays.size(); i++ )
			{
				rays.get( i ).update();
			}
		}
		
		public void render ()
		{
			gl.glPushMatrix();
			gl.glTranslatef( loc.x, loc.y, loc.z );

			for( int i=0; i<rays.size(); i++ )
			{
				rays.get( i ).render();
			}
			
			gl.glPopMatrix();
		}
	}
	
	public class RayLight 
	{
		float phi;
		float theta;
		float w;		// width
		float h;		// height
		
		public RayLight ( float theta, float phi, float rayWidth, float rayHeight )
		{
			println("theta :: " + theta + " , phi :: " + phi );
			
			this.phi = phi;
			this.theta = theta;
			w = rayWidth;
			h = rayHeight;
		}
		
		public void update ()
		{
			
		}
		
		public void render ()
		{
//			float x = cos(sqrt(phi))*cos(theta) * h * 4;
//			float y = cos(sqrt(phi))*sin(theta) * h * 4;
//			float z = sin(sqrt(phi)) * h * 4;
			
//			gl.glBegin( GL.GL_LINES );
//			gl.glColor4f( 1, 1, 1, 1 );
//			gl.glVertex3f( 0, 0, 0 );
//			gl.glVertex3f( 0, h*4, 0 );
//			gl.glEnd();
			
//			// face texture to camera.
//			float deltaX   = camTarget.x - camPosition.x; 
//			float deltaY   = camTarget.y - camPosition.y; 
//			float deltaZ   = camTarget.z - camPosition.z;
//
//			float angleZ   = atan2( deltaY, deltaX );
//			float hyp      = sqrt( sq( deltaX ) + sq( deltaY ) ); 
//			float angleY   = atan2( hyp, deltaZ ); 
//		 
//			gl.glRotatef( degrees( angleZ ), 0, 0,    1.0f );
//			gl.glRotatef( degrees( angleY ), 0, 1.0f, 0    );

			gl.glPushMatrix();
			gl.glRotatef( theta, 0, 1, 0 );
			gl.glRotatef( phi, 1, 0, 0 );
			gl.glScalef( w, h, 0 );
			
			gl.glBegin( GL.GL_QUADS );
			gl.glColor4f( 1, 1, 1, 1 );
			gl.glNormal3f( 0, 0, 1 );

//			gl.glCallList( cylds );
			
			gl.glTexCoord2f( 0.0f, 0.0f );		gl.glVertex3f( -0.5f, 0.0f, 0.0f );
			gl.glTexCoord2f( 0.0f, 1.0f );		gl.glVertex3f( -0.5f, 1.0f, 0.0f );
			gl.glTexCoord2f( 1.0f, 1.0f );		gl.glVertex3f(  0.5f, 1.0f, 0.0f );
			gl.glTexCoord2f( 1.0f, 0.0f );		gl.glVertex3f(  0.5f, 0.0f, 0.0f );
			gl.glEnd();
			
			gl.glPopMatrix();
		}
		
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