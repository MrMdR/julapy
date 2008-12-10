package com.julapy.bats;

import java.awt.Dimension;
import java.awt.Toolkit;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import javax.media.opengl.GL;

import com.julapy.audio.AudioInputAnalysis;
import com.julapy.blob.BlobDetect;
import com.julapy.camera.Camera;
import com.julapy.math.TrigUtil;
import com.julapy.opengl.TextureLoader;
import com.julapy.steering.Flock;
import com.julapy.steering.NoiseField;
import com.julapy.steering.Particle;

import processing.core.PApplet;
import processing.core.PFont;
import processing.core.PImage;
import processing.opengl.PGraphicsOpenGL;
import processing.video.Capture;
import toxi.geom.Vec3D;

public class BatFlock extends PApplet
{
	PGraphicsOpenGL pgl;
	GL gl;
	
	TextureLoader texLoader;
	int wingLCallList;
	int wingRCallList;
	Camera cam;
	
	int capWidth	= 200;
	int capHeight	= 150;
	Capture cap;
	BlobDetect bd;
	
	AudioInputAnalysis audioAnalysis;
	
	PFont font;
	float debugInc = 0.1f;
	
	Bat[] 	bats;
	Flock[] batFlocks;
	NoiseField noiseField_01;
	NoiseField noiseField_02;
	Bat flockTarget;
	Vec3D flockTargetLoc;
	
	PImage moon;
	float moonTintAlpha = 0;
	
	boolean isRecording = false;
	int imageCount;
	
	boolean debug = false;
	
	static public void main( String args[] )
	{
		PApplet.main( 	new String[]
	                    {
							"--display=1",
							"--present",
							"--bgcolor=#000000",
							"--present-stop-color=#000000", 
							"com.julapy.bats.BatFlock" 
						} 
		);
	}
	
	public void setup()
	{
//		Dimension screen = Toolkit.getDefaultToolkit().getScreenSize();
//		size( screen.width, screen.height, OPENGL );
		size( 800, 600, OPENGL );
		frameRate( 30 );
		colorMode( RGB, 1.0f );
		background( 0.7f );
		
		initGL();
		initTextureList();
		initCamera();
		initBlobDetect();
		initAudioAnalysis();
		initDebugText();
		initScene();
		initBats();
	}
	
	//////////////////////////////////////////////
	// INIT.
	//////////////////////////////////////////////
	
	public void initGL ()
	{
		pgl = (PGraphicsOpenGL) g;
		gl = pgl.gl;

		/* opengl init */
		gl.glShadeModel( GL.GL_SMOOTH );              					// Enable Smooth Shading
		gl.glClearColor( 0, 0, 0, 0.5f );		    					// Black Background
		gl.glClearDepth( 1 );	                      					// Depth Buffer Setup
		gl.glDepthFunc( GL.GL_LEQUAL );									// The Type Of Depth Testing To Do
		gl.glHint( GL.GL_PERSPECTIVE_CORRECTION_HINT, GL.GL_NICEST );	// Really Nice Perspective Calculations
		gl.glEnable( GL.GL_TEXTURE_2D );
		gl.glDisable( GL.GL_DEPTH_TEST );								// Enable Depth Testing
		gl.glEnable( GL.GL_BLEND );
		
        /* load texture */
		texLoader = new TextureLoader( gl, 1 );
		texLoader.loadTexture( loadImage( "data/bat_02.png" ), true );
	}
	
	private void initTextureList ()
	{
		wingLCallList = gl.glGenLists( 1 );
		gl.glNewList( wingLCallList, GL.GL_COMPILE );
			gl.glBegin( GL.GL_QUADS );
			gl.glNormal3f( 0, 0, 1 );
			gl.glTexCoord2f( 0.0f, 0.0f );	gl.glVertex3f( -1, -0.5f, 0 );
			gl.glTexCoord2f( 0.0f, 1.0f );	gl.glVertex3f( -1,  0.5f, 0 );
			gl.glTexCoord2f( 0.5f, 1.0f );	gl.glVertex3f(  0,  0.5f, 0 );
			gl.glTexCoord2f( 0.5f, 0.0f );	gl.glVertex3f(  0, -0.5f, 0 );
			gl.glEnd();
		gl.glEndList();

		wingRCallList = gl.glGenLists( 1 );
		gl.glNewList( wingRCallList, GL.GL_COMPILE );
			gl.glBegin( GL.GL_QUADS );
			gl.glNormal3f( 0, 0, 1 );
			gl.glTexCoord2f( 0.5f, 0.0f );	gl.glVertex3f( 0, -0.5f, 0 );
			gl.glTexCoord2f( 0.5f, 1.0f );	gl.glVertex3f( 0,  0.5f, 0 );
			gl.glTexCoord2f( 1.0f, 1.0f );	gl.glVertex3f( 1,  0.5f, 0 );
			gl.glTexCoord2f( 1.0f, 0.0f );	gl.glVertex3f( 1, -0.5f, 0 );
			gl.glEnd();
		gl.glEndList();
	}
	
	private void initCamera ()
	{
		cam = new Camera( this );
	}
	
	private void initBlobDetect ()
	{
		// The name of the capture device is dependent those
		// plugged into the computer. To get a list of the 
		// choices, uncomment the following line 
		println( Capture.list() );
		
		cap	= new Capture( this, capWidth, capHeight, "USB Video Class Video", 25 );
		bd	= new BlobDetect( this, cap );
	}
	
	private void initAudioAnalysis ()
	{
		audioAnalysis = new AudioInputAnalysis( this );
	}
	
	private void initDebugText ()
	{
		font = createFont( "Courier", 10 );
	}
	
	private void initScene ()
	{
		moon = loadImage( "moonbg.jpg" );
	}
	
	private void initBats ()
	{
		Vec3D loc, vel;
		int i;
		
		bats = new Bat[ 200 ];
		for( i=0; i<bats.length; i++ )
		{
			loc			= new Vec3D( width * ( random( 2 ) - 1 ), width * ( random( 2 ) - 1 ), 0 );
			vel			= Vec3D.randomVector().scaleSelf( random( 30 ) + 1 );
			
			bats[ i ]	= new Bat( loc, vel );
		}
		
		flockTarget		= new Bat();
		flockTargetLoc	= new Vec3D();
		Bat[] batSingle;
		batSingle		= new Bat[ 1 ];
		batSingle[ 0 ]	= flockTarget;
		
		batFlocks		= new Flock[ 1 ];
		batFlocks[ 0 ]	= new Flock( bats, flockTargetLoc );
		noiseField_01	= new NoiseField( bats, 1200 );
		noiseField_02	= new NoiseField( batSingle, width * 0.6f );
	}
	
	//////////////////////////////////////////////
	// DRAW.
	//////////////////////////////////////////////
	
	public void draw ()
	{
		background( 0 );
		tint( 1, 1-moonTintAlpha, 1-moonTintAlpha );
		image( moon, 0, 0 );
		noTint();
		
		updateBlobDetect();
		updateAudioAnalysis();
		
		updateNoiseField();
		updateFlock();
		updateBats();
		
		updateCamera();
		
		// render.
		pgl.beginGL();

		/* enable additive blending */
//		gl.glEnable( GL.GL_BLEND );
//		gl.glBlendFunc( GL.GL_SRC_ALPHA, GL.GL_ONE );
//		gl.glBlendFunc( GL.GL_SRC_COLOR, GL.GL_ONE );
		
		gl.glDisable( GL.GL_DEPTH_TEST );
		gl.glEnable( GL.GL_TEXTURE_2D );
		gl.glBindTexture( GL.GL_TEXTURE_2D, texLoader.getTexture( 0 ) );
		
		renderBats();

		/* disable additive blending */
//		gl.glDisable( GL.GL_BLEND );
		
		pgl.endGL();

		resetCamera();
		if( debug )
		{
			drawBlobDetect();
			drawAudioAnalysis();
			drawDebugText();
		}

		saveImage();
	}
	
	private void updateBats ()
	{
		int i;
		
		for( i=0; i<bats.length; i++ )
		{
			bats[ i ].update();
		}
	}
	
	private void renderBats ()
	{
		int i;
		
		for( i=0; i<bats.length; i++ )
		{
			bats[ i ].render();
		}
	}
	
	private void updateCamera ()
	{
//		float dx, dy, theta, phi, radius;
//		
//		dx		= ( mouseX / (float)width - 0.5f ) * 2;
//		dy		= ( mouseY / (float)height - 0.5f ) * 2;
//		
//		theta	= dx * PI * 0.5f;
//		phi		= dy * PI * 0.5f;
//		if( dy < 0 )
//			phi = PI * 2 - phi;
//		radius	= -1000;
//		
//		cam.orbitTo( theta, phi, radius );
		cam.update( );
	}
	
	private void resetCamera ()
	{
		camera();
	}
	
	private void updateFlock ()
	{
		int i;
		float dx, dy;
		float audioLevel;
		
		dx	= ( mouseX / (float)width - 0.5f ) * 2;
		dy	= ( mouseY / (float)height - 0.5f ) * 2;
		
		audioLevel = audioAnalysis.getLevel(); 
		
		noiseField_02.update();
		
		flockTarget.update();
		flockTargetLoc.set( flockTarget.loc );
		flockTargetLoc.x *= 1 - audioLevel;
		flockTargetLoc.y *= 1 - audioLevel;
		flockTargetLoc.z += audioLevel * 1000;
		
		moonTintAlpha += ( audioLevel - moonTintAlpha ) * 0.3f;
		
		for( i=0; i<batFlocks.length; i++ )
		{
			batFlocks[ i ].velocityIncrease = audioLevel * 20;
			batFlocks[ i ].update();
		}
	}
	
	private void updateNoiseField ()
	{
		noiseField_01.update();
	}
	
	private void updateBlobDetect ()
	{
		bd.update();
	}
	
	private void drawBlobDetect ()
	{
		bd.draw();
	}
	
	private void updateAudioAnalysis ()
	{
		audioAnalysis.update();
		audioAnalysis.getLevel();
	}
	
	private void drawAudioAnalysis ()
	{
		audioAnalysis.draw();
	}
	
	private void drawDebugText ()
	{
		fill( 1, 0, 0 );
		
		textFont( font );
		
		text( "centerPullScale (a/z)          :: " + batFlocks[ 0 ].centerPullScale,			width - 250, 10 );
		text( "collisionAvoidanceScale (s/x)  :: " + batFlocks[ 0 ].collisionAvoidanceScale,	width - 250, 25 );
		text( "minDistance (d/c)              :: " + batFlocks[ 0 ].minDistance,				width - 250, 40 );
		text( "targetPullScale (f/v)          :: " + batFlocks[ 0 ].targetPullScale,			width - 250, 55 );
		text( "velocityLimit (g/b)            :: " + batFlocks[ 0 ].velocityLimit,				width - 250, 70 );
		text( "flockAverageScale (h/n)        :: " + batFlocks[ 0 ].flockAverageScale,			width - 250, 85 );
		text( "noiseVecScale (j/m)            :: " + noiseField_01.noiseVecScale,					width - 250, 100 );
	}
	
	private void saveImage ()
	{
		if( isRecording )
		{
			save("export/image" + imageCount++ + ".png");		
		}
	}
	
	//////////////////////////////////////////////
	// BAT CLASS.
	//////////////////////////////////////////////
	
	public class Bat extends Particle
	{
		float count		= random( 1 );
		float countInc	= 0.2f;
		float countDir	= 1;

		Vec3D[] locs;
		
		float wingW = 80;
		float wingH	= 40;
		
		float wingMaxAngle		= 40;
		float wingOffsetAngle	= 30;
		float wingLAngle		= 0;
		float wingRAngle		= 0;
		
		float orbitAngle		= 0;
		float orbitAngleInc		= 1;
		float orbitRadius		= 500;
		
		public Bat()
		{
			loc = new Vec3D( );
			vel = new Vec3D( );
			
			init();
		}

		public Bat( Vec3D loc )
		{
			this.loc = loc;
			vel = new Vec3D( );
			
			init();
		}

		public Bat( Vec3D loc, Vec3D vel )
		{
			this.loc = loc;
			this.vel = vel;
			
			init();
		}
		
		private void init ()
		{
			locs = new Vec3D[ 50 ];
			for( int i=0; i<locs.length; i++ )
				locs[i] = new Vec3D( loc );
			
			size = random( 0.3f ) + 0.7f;
		}
		
		public void render ()
		{
			gl.glPushMatrix();
			
			gl.glColor4f( 1, 0, 0, 1 );
			gl.glTranslatef( loc.x, loc.y, loc.z );
			
			gl.glPushMatrix();
			gl.glRotatef( 180, 0, 0, 1 );
			gl.glRotatef( wingLAngle, 0, 1, 0 );
			gl.glScalef( size * wingW, size * wingH, 0 );
			gl.glCallList( wingLCallList );
			gl.glPopMatrix();

			gl.glPushMatrix();
			gl.glRotatef( 180, 0, 0, 1 );
			gl.glRotatef( wingRAngle, 0, 1, 0 );
			gl.glScalef( size * wingW, size * wingH, 0 );
			gl.glCallList( wingRCallList );
			gl.glPopMatrix();
			
			gl.glPopMatrix();
		}
		
		public void update ()
		{
			countBounce();
			calcWingAngle();
			
//			wander();
//			pullToPoint( new Vec3D( 0, 0, 0 ) );
//			orbitHorz();
//			orbitVert();
			
			updatePosition();
			savePosition();
		}
		
		private void countBounce ()
		{
			count += countInc * countDir;
			
			if( count < 0 || count > 1 )
			{
				if( countDir == 1 )
				{
					count = 2 - count;
				}
				
				if( countDir == -1 )
				{
					count = -count;
				}
				
				countDir *= -1;
			}
		}
		
		private void countLinear ()
		{
			count += countInc;
			
			if( count > 1 )
			{
				count = count - 1;
			}
		}
		
		private void calcWingAngle()
		{
			wingLAngle = cos( count * PI ) * wingMaxAngle + wingOffsetAngle;
			wingRAngle = cos( count * PI ) * -wingMaxAngle - wingOffsetAngle;
		}
		
		private void wander ()
		{
			float rndRange;
			
			rndRange = 10;
			
			vel.x += random( rndRange ) - rndRange * 0.5f;
			vel.y += random( rndRange ) - rndRange * 0.5f;
			vel.z += random( rndRange ) - rndRange * 0.5f;
			
			loc.addSelf( vel );
		}
		
		private void pullToPoint ( Vec3D p )
		{
			Vec3D v;

			v = p.sub( loc ).normalize();
			v.scaleSelf( loc.distanceTo( new Vec3D() ) / 20 );
			
			loc.addSelf( v );
		}
		
		private void orbitHorz ()
		{
			float p[];
			
			p = TrigUtil.getPointOnCircle( orbitAngle, 500 );
			
			loc.x = p[ 0 ];
			loc.z = p[ 1 ];
			
			orbitAngle += orbitAngleInc;
		}
		
		private void orbitVert ()
		{
			float p[];
			
			p = TrigUtil.getPointOnCircle( orbitAngle, 500 );
			
			loc.x = p[ 0 ];
			loc.y = p[ 1 ];
			
			orbitAngle += orbitAngleInc;
		}
		
		private void updatePosition ()
		{
			loc.addSelf( vel );
		}
		
		private void savePosition ()
		{
		    for( int i=locs.length-1; i>0; i-- )
		    	locs[ i ].set( locs[ i-1 ] );
		    locs[ 0 ].set( loc );
		}
	}
	
	//////////////////////////////////////////////
	// EVENTS.
	//////////////////////////////////////////////
	
	public void keyPressed()
	{
		if(key == 'r')
		{
			isRecording = !isRecording;
			
			if(isRecording) println("started recording.");
			if(!isRecording) println("stopped recording.");
		}

		/*__________BREAK___________*/
		if( key == '0' )
		{
			debugInc = 1;
		}
		if( key == '1' )
		{
			debugInc = 0.1f;
		}
		if( key == '2' )
		{
			debugInc = 0.01f;
		}
		if( key == '3' )
		{
			debugInc = 0.001f;
		}
		if( key == '4' )
		{
			debugInc = 0.0001f;
		}
		if( key == '5' )
		{
			debugInc = 0.00001f;
		}
		
		/*__________BREAK___________*/
		if( key == 'a' )
		{
			batFlocks[ 0 ].centerPullScale += debugInc;
		}
		if( key == 'z' )
		{
			batFlocks[ 0 ].centerPullScale -= debugInc;
		}

		/*__________BREAK___________*/
		if( key == 's' )
		{
			batFlocks[ 0 ].collisionAvoidanceScale += debugInc;
		}
		if( key == 'x' )
		{
			batFlocks[ 0 ].collisionAvoidanceScale -= debugInc;
		}
		
		/*__________BREAK___________*/
		if( key == 'd' )
		{
			batFlocks[ 0 ].flockAverageScale += debugInc;
		}
		if( key == 'c' )
		{
			batFlocks[ 0 ].flockAverageScale -= debugInc;
		}

		/*__________BREAK___________*/
		if( key == 'f' )
		{
			batFlocks[ 0 ].targetPullScale += debugInc;
		}
		if( key == 'v' )
		{
			batFlocks[ 0 ].targetPullScale -= debugInc;
		}

		/*__________BREAK___________*/
		if( key == 'g' )
		{
			batFlocks[ 0 ].velocityLimit += debugInc;
		}
		if( key == 'b' )
		{
			batFlocks[ 0 ].velocityLimit -= debugInc;
		}
		
		/*__________BREAK___________*/
		if( key == 'h' )
		{
			batFlocks[ 0 ].minDistance += debugInc;
		}
		if( key == 'n' )
		{
			batFlocks[ 0 ].minDistance -= debugInc;
		}

		/*__________BREAK___________*/
		if( key == 'j' )
		{
			noiseField_01.noiseVecScale += debugInc;
		}
		if( key == 'm' )
		{
			noiseField_01.noiseVecScale -= debugInc;
		}
	}
}
