package com.processinghacks.arcball;

import processing.core.PApplet;
import toxi.geom.Matrix4x4;
import toxi.geom.Quaternion;
import toxi.geom.Vec3D;

import java.awt.event.MouseEvent;

import com.julapy.math.TrigUtil;

public class ArcBall2 
{
	PApplet parent;
  
	float cx, cy, cz, radius;
	
	Vec3D downVec, dragVec;
	Quaternion nowQuat, downQuat, dragQuat;

	public ArcBall2( float cx, float cy, float cz, float radius, PApplet parent )
	{
		this.parent = parent;

		parent.registerMouseEvent( this );

		this.cx	= cx;
		this.cy	= cy;
		this.cz	= cz;
		this.radius		= radius;

		downVec		= new Vec3D();
		dragVec		= new Vec3D();

		nowQuat		= new Quaternion( 1, new Vec3D( 0, 0, 0 ) );
		downQuat	= new Quaternion( 1, new Vec3D( 0, 0, 0 ) );
		dragQuat	= new Quaternion( 1, new Vec3D( 0, 0, 0 ) );
  }

	public void mouseEvent(MouseEvent event) 
	{
		int id = event.getID();
		if( id == MouseEvent.MOUSE_DRAGGED )
		{
			mouseDragged();
		} 
		else if( id == MouseEvent.MOUSE_PRESSED ) 
		{
			mousePressed();
		}
	}

	public void mousePressed() 
	{
		downVec = mouseToSphere( parent.mouseX, parent.mouseY );
		
		PApplet.println( "downVec :: " + downVec );
		
		downQuat.set( nowQuat );
		dragQuat.reset();
	}

	public void mouseDragged() 
	{
		dragVec = mouseToSphere( parent.mouseX, parent.mouseY );
		
		PApplet.println( "dragVec :: " + dragVec );
		
		dragQuat.set( downVec.dot( dragVec ), downVec.cross( dragVec ) );
	}
	
	public void update() 
	{
		Matrix4x4 m;

		parent.translate( cx, cy, cz );
		
		dragVec = mouseToSphere( 0, 0 );
		dragQuat.set( downVec.dot( dragVec ), downVec.cross( dragVec ) );
		
		nowQuat = dragQuat.multiply( downQuat );
		m		= nowQuat.getMatrix();
		
		parent.applyMatrix
		(
			(float)m.matrix[0][0], (float)m.matrix[0][1], (float)m.matrix[0][2], (float)m.matrix[0][3],
			(float)m.matrix[1][0], (float)m.matrix[1][1], (float)m.matrix[1][2], (float)m.matrix[1][3],
			(float)m.matrix[2][0], (float)m.matrix[2][1], (float)m.matrix[2][2], (float)m.matrix[2][3],
			(float)m.matrix[3][0], (float)m.matrix[3][1], (float)m.matrix[3][2], (float)m.matrix[3][3]
		);
		
		parent.translate( -cx, -cy, -cz );
	}
	
	public void update( float angleX, float angleY )
	{
		PApplet.println( angleX );
		
//		Matrix4x4 m;
//
//		parent.translate( cx, cy, cz );
//
//		Vec3D xaxis, yaxis;
//		Vec3D xrot;
//		Vec3D yrot;
//		Quaternion xrotQuat, yrotQuat;
//		
//		xaxis	= Vec3D.X_AXIS;
//		yaxis	= Vec3D.Y_AXIS;
//		
//		PApplet.println( angleDegreesX );
//
//		xrot = xaxis.copy().rotateX( angleDegreesX * TrigUtil.DEGTORAD );
//		yrot = yaxis.copy().rotateY( angleDegreesY * TrigUtil.DEGTORAD );
//		
//		PApplet.println( xrot );
//		
//		xrotQuat = new Quaternion( xaxis.copy().dot( xrot ), xaxis.copy().cross( xrot ) );
//		yrotQuat = new Quaternion( xaxis.copy().dot( yrot ), xaxis.copy().cross( yrot ) );
//		
//		nowQuat = xrotQuat.multiply( yrotQuat );
//		m		= nowQuat.getMatrix();
//		
//		parent.applyMatrix
//		(
//			(float)m.matrix[0][0], (float)m.matrix[0][1], (float)m.matrix[0][2], (float)m.matrix[0][3],
//			(float)m.matrix[1][0], (float)m.matrix[1][1], (float)m.matrix[1][2], (float)m.matrix[1][3],
//			(float)m.matrix[2][0], (float)m.matrix[2][1], (float)m.matrix[2][2], (float)m.matrix[2][3],
//			(float)m.matrix[3][0], (float)m.matrix[3][1], (float)m.matrix[3][2], (float)m.matrix[3][3]
//		);
//		
//		parent.translate( -cx, -cy, -cz );
	}

	Vec3D mouseToSphere( float x, float y )
	{
		Vec3D v = new Vec3D
		(
			x - cx / 360,
			y - cy / 360,
			0
		);

		if( v.magSquared() > 1.0f ) 
		{
			v.normalize();
		}
		else 
		{
			v.z = PApplet.sqrt( 1.0f - v.magSquared() );
		}

		return v;
	}
}
