package com.julapy.bats;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import javax.media.opengl.GL;

import com.julapy.camera.Camera;
import com.julapy.math.TrigUtil;
import com.julapy.opengl.Primitive;
import com.julapy.opengl.TextureLoader;

import processing.core.PApplet;
import processing.opengl.PGraphicsOpenGL;
import toxi.geom.Quaternion;
import toxi.geom.Vec3D;
import toxi.math.MathUtils;

public class BatModel extends PApplet
{
	PGraphicsOpenGL pgl;
	GL gl;
	
	TextureLoader texLoader;
	int wingLCallList;
	int wingRCallList;
	Camera cam;
	
	Bat bat;
	
	public void setup()
	{
		size( 1280, 720, OPENGL );
		frameRate( 30 );
		colorMode( RGB, 1.0f );
		background( 0.7f );
		
		initGL();
		initTextureList();
		
		cam = new Camera( this );
		
		bat = new Bat();
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
		texLoader.loadTexture( loadImage( "data/bat.png" ), true );
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
	
	//////////////////////////////////////////////
	// DRAW.
	//////////////////////////////////////////////
	
	public void draw ()
	{
		// clear.
		background( 0.6f );
		
//		updateCamera();
		
		// render.
		pgl.beginGL();
		
		gl.glDisable( GL.GL_DEPTH_TEST );
		gl.glEnable( GL.GL_TEXTURE_2D );
		gl.glBindTexture( GL.GL_TEXTURE_2D, texLoader.getTexture( 0 ) );
		
		bat.render( );
		bat.update( );
		
		pgl.endGL();
	}
	
	private void updateCamera ()
	{
		float dx, dy, theta, phi, radius;
		
		dx		= ( mouseX / (float)width - 0.5f ) * 2;
		dy		= ( mouseY / (float)height - 0.5f ) * 2;
		
		theta	= dx * PI * 0.5f;
		phi		= dy * PI * 0.5f;
		if( dy < 0 )
			phi = PI * 2 - phi;
		radius	= -1000;
		
		cam.orbitTo( theta, phi, radius );
		cam.update( );
	}
	
	//////////////////////////////////////////////
	// BAT CLASS.
	//////////////////////////////////////////////
	
	public class Bat
	{
		float count		= 0;
		float countInc	= 0.05f;
		float countDir	= 1;

		Vec3D[] locs;
		Vec3D loc;
		Vec3D vel;
		
		float wingW = 400;
		float wingH	= 200;
		
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
			
			locs = new Vec3D[ 50 ];
			for( int i=0; i<locs.length; i++ )
				locs[i] = new Vec3D( loc );
		}

		public void render ()
		{
			Vec3D out, up, right;
			
			up		= new Vec3D( 0, 1, 0 );
			out		= locs[ 0 ].sub( locs[ 1 ] ).normalize();
			up		= up.cross( out ).normalize();
			right	= out.cross( up ).normalize();
			
			float[] sp;
			float theta, phi, radius;
			
			sp		= TrigUtil.cartesianToSpherical( out.x, out.y, out.z );
			radius	= sp[ 0 ];
			theta	= sp[ 1 ] * TrigUtil.RADTODEG;
			phi		= sp[ 2 ] * TrigUtil.RADTODEG;
			
//			float rotX, rotY, rotZ;
//			
//			rotX	= out.headingYZ() * TrigUtil.RADTODEG;
//			rotY	= out.headingXZ() * TrigUtil.RADTODEG - 90;
//			rotZ	= out.headingXY() * TrigUtil.RADTODEG;
			
			
			float[] m;
//			m = new float[]
//			            {
//							right.x, up.x, out.x, 0,
//							right.y, up.y, out.y, 0,
//							right.z, up.z, out.z, 0,
//							0,	     0,	   0,	  1
//						};

			m = TrigUtil.eulerToMatrix( 0, phi, theta );
//			m = TrigUtil.quaternionToMatrix( out.x, out.y, out.z, 30 * TrigUtil.DEGTORAD );
			
			FloatBuffer rotMatrix;
			rotMatrix = ByteBuffer.allocateDirect(4 * 4 * 4).order(ByteOrder.nativeOrder()).asFloatBuffer();
			rotMatrix.put( m );
			rotMatrix.flip();

			gl.glPushMatrix();
			
			gl.glColor4f( 0, 0, 0, 1 );
			gl.glTranslatef( loc.x, loc.y, loc.z );
			gl.glMultMatrixf( rotMatrix );
			
			gl.glPushMatrix();
			gl.glRotatef( 90, 1, 0, 0 );
			gl.glRotatef( wingLAngle, 0, 1, 0 );
			gl.glScalef( wingW, wingH, 0 );
			gl.glCallList( wingLCallList );
			gl.glPopMatrix();

			gl.glPushMatrix();
			gl.glRotatef( 90, 1, 0, 0 );
			gl.glRotatef( wingRAngle, 0, 1, 0 );
			gl.glScalef( wingW, wingH, 0 );
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
			orbitHorz();
//			orbitVert();
			
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
		
		private void savePosition ()
		{
		    for( int i=locs.length-1; i>0; i-- )
		    	locs[ i ].set( locs[ i-1 ] );
		    locs[ 0 ].set( loc );
		}
	}
	
	static public void main( String args[] )
	{
		PApplet.main( new String[] { "com.julapy.bats.Main" } );
	}
}
