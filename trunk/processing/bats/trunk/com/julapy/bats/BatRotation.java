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
import toxi.geom.Matrix4x4;
import toxi.geom.Quaternion;
import toxi.geom.Vec3D;
import toxi.math.MathUtils;

public class BatRotation extends PApplet
{
	PGraphicsOpenGL pgl;
	GL gl;
	
	TextureLoader texLoader;
	Primitive prim;
	int wingLCallList;
	int wingRCallList;
	
	Bat bat;
	float angleDegreesX		= 0;
	float angleDegreesY		= 0;
	float angleDegreesZ		= 0;
	
	public void setup()
	{
		size( 900, 720, OPENGL );
		frameRate( 30 );
		colorMode( RGB, 1.0f );
		background( 0.7f );
		
		initGL();
		initTextureList();
		
		camera
		(
			0, 200, 1000,		// eye
			0, 0, 0, 		// center
			0, 1, 0			// up
		);
		
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
		gl.glEnable( GL.GL_DEPTH_TEST );								// Enable Depth Testing
		gl.glEnable( GL.GL_BLEND );
		
        /* load texture */
		texLoader = new TextureLoader( gl, 1 );
		texLoader.loadTexture( loadImage( "data/bat_01.png" ), true );
		
		prim = new Primitive( gl );
	}
	
	private void initTextureList ()
	{
		wingLCallList = gl.glGenLists( 1 );
		gl.glNewList( wingLCallList, GL.GL_COMPILE );
			gl.glBegin( GL.GL_QUADS );
			gl.glNormal3f( 0, 0, 1 );
			gl.glTexCoord2f( 0.0f, 0.0f );	gl.glVertex3f( -1, 0, -0.5f );
			gl.glTexCoord2f( 0.0f, 1.0f );	gl.glVertex3f( -1,  0, 0.5f );
			gl.glTexCoord2f( 0.5f, 1.0f );	gl.glVertex3f(  0,  0, 0.5f );
			gl.glTexCoord2f( 0.5f, 0.0f );	gl.glVertex3f(  0, 0, -0.5f );
			gl.glEnd();
		gl.glEndList();

		wingRCallList = gl.glGenLists( 1 );
		gl.glNewList( wingRCallList, GL.GL_COMPILE );
			gl.glBegin( GL.GL_QUADS );
			gl.glNormal3f( 0, 0, 1 );
			gl.glTexCoord2f( 0.5f, 0.0f );	gl.glVertex3f( 0, 0, -0.5f );
			gl.glTexCoord2f( 0.5f, 1.0f );	gl.glVertex3f( 0,  0, 0.5f );
			gl.glTexCoord2f( 1.0f, 1.0f );	gl.glVertex3f( 1,  0, 0.5f );
			gl.glTexCoord2f( 1.0f, 0.0f );	gl.glVertex3f( 1, 0, -0.5f );
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
		
		// render.
		pgl.beginGL();
		
		bat.renderBat( );
		bat.updateBat( );
		
		pgl.endGL();
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
		// previous direction
		Vec3D prevDir;
		
		float wingMaxAngle		= 40;
		float wingOffsetAngle	= 30;
		float wingLAngle		= 0;
		float wingRAngle		= 0;
		
		float orbitAngle		= 0;
		float orbitAngleInc		= 3;
		float orbitRadius		= 500;
		
		public Bat()
		{
			loc = new Vec3D( );
			vel = new Vec3D( );
			
			locs = new Vec3D[ 50 ];
			for( int i=0; i<locs.length; i++ )
				locs[i] = new Vec3D( loc );
		}

		public void renderBat ()
		{
			Vec3D xrot, yrot, zrot;
			Quaternion xrotQuat, yrotQuat, zrotQuat, rotQuat;
			Matrix4x4 m;
			float angleX, angleY, angleZ;
			
			angleX = angleDegreesX * TrigUtil.DEGTORAD;
			angleY = angleDegreesY * TrigUtil.DEGTORAD;
			angleZ = angleDegreesZ * TrigUtil.DEGTORAD;

			xrot = Vec3D.Z_AXIS.copy().rotateX( angleX );
			yrot = Vec3D.X_AXIS.copy().rotateY( angleY );
			zrot = Vec3D.X_AXIS.copy().rotateZ( angleZ );

			xrotQuat = new Quaternion( 1, new Vec3D( 0, 0, 0 ) );
			xrotQuat.set( xrot.dot( Vec3D.Z_AXIS.copy() ), xrot.cross( Vec3D.Z_AXIS.copy() ) );
			
			yrotQuat = new Quaternion( 1, new Vec3D( 0, 0, 0 ) );
			yrotQuat.set( yrot.dot( Vec3D.X_AXIS.copy() ), yrot.cross( Vec3D.X_AXIS.copy() ) );

			zrotQuat = new Quaternion( 1, new Vec3D( 0, 0, 0 ) );
			zrotQuat.set( zrot.dot( Vec3D.X_AXIS.copy() ), zrot.cross( Vec3D.X_AXIS.copy() ) );
			
			rotQuat = xrotQuat.multiply( yrotQuat ).multiply( zrotQuat );
			m		= rotQuat.getMatrix();

			FloatBuffer rotMatrix;
			rotMatrix = ByteBuffer.allocateDirect(4 * 4 * 4).order(ByteOrder.nativeOrder()).asFloatBuffer();
			rotMatrix.put
			(
				new float[]
				{
					(float)m.matrix[0][0], (float)m.matrix[0][1], (float)m.matrix[0][2], (float)m.matrix[0][3],
					(float)m.matrix[1][0], (float)m.matrix[1][1], (float)m.matrix[1][2], (float)m.matrix[1][3],
					(float)m.matrix[2][0], (float)m.matrix[2][1], (float)m.matrix[2][2], (float)m.matrix[2][3],
					(float)m.matrix[3][0], (float)m.matrix[3][1], (float)m.matrix[3][2], (float)m.matrix[3][3]
				}
			);
			rotMatrix.flip();

			gl.glPushMatrix();

			gl.glTranslatef( loc.x, loc.y, loc.z );
			gl.glMultMatrixf( rotMatrix );
			gl.glTranslatef( -loc.x, -loc.y, -loc.z );
			
			// draw bounding box.
			gl.glPushMatrix();
			gl.glScalef( 300, 300, 300 );
			gl.glBegin( GL.GL_LINES );
			prim.drawCube();
			gl.glEnd();
			gl.glPopMatrix();
			
			// draw wings.
//			gl.glDisable( GL.GL_DEPTH_TEST );
			gl.glEnable( GL.GL_TEXTURE_2D );
			gl.glBindTexture( GL.GL_TEXTURE_2D, texLoader.getTexture( 0 ) );
			
			gl.glPushMatrix();
			gl.glRotatef( wingLAngle, 0, 0, 1 );
			gl.glScalef( 400, 0, 200 );
			gl.glCallList( wingLCallList );
			gl.glPopMatrix();

			gl.glPushMatrix();
			gl.glRotatef( wingRAngle, 0, 0, 1 );
			gl.glScalef( 400, 0, 200 );
			gl.glCallList( wingRCallList );
			gl.glPopMatrix();
			
			gl.glDisable( GL.GL_TEXTURE_2D );
			
			gl.glPopMatrix();
		}
		
		public void updateBat ()
		{
			countBounce();
			calcWingAngle();
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
	}
	
	//////////////////////////////////////////////
	// KEY PRESS HANDLER.
	//////////////////////////////////////////////
	
	public void keyPressed()
	{
		if( keyCode == 37 )	// left.
		{
			angleDegreesY -= 1.0f;
		}

		if( keyCode == 39 )	// right
		{
			angleDegreesY += 1.0f;
		}
		
		if( keyCode == 38 )	// up
		{
			angleDegreesX += 1.0f;
		}
		
		if( keyCode == 40 )	// down
		{
			angleDegreesX -= 1.0f;
		}

		if( keyCode == 44 )	//
		{
			angleDegreesZ -= 1.0f;
		}
		
		if( keyCode == 46 ) //
		{
			angleDegreesZ += 1.0f;
		}
	}
	
	static public void main( String args[] )
	{
		PApplet.main( new String[] { "com.julapy.bats.BatModel" } ); 
	}
}
