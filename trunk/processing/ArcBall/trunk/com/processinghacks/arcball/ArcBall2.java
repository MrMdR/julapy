package com.processinghacks.arcball;

import processing.core.PApplet;
import toxi.geom.Matrix4x4;
import toxi.geom.Quaternion;
import toxi.geom.Vec3D;

import java.awt.event.MouseEvent;

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
		
		downQuat.set( nowQuat );
		dragQuat.reset();
	}

	public void mouseDragged() 
	{
		dragVec = mouseToSphere( parent.mouseX, parent.mouseY );
		
		dragQuat.set( downVec.dot( dragVec ), downVec.cross( dragVec ) );
	}
	
	public void update() 
	{
		Matrix4x4 m;

		parent.translate( cx, cy, cz );
		
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
	
	public void update( float angleZ, float angleY )
	{
		Matrix4x4 m;

		parent.translate( cx, cy, cz );

		Vec3D yrot, zrot;
		Quaternion yrotQuat, zrotQuat;
		
		yrot = Vec3D.X_AXIS.copy().rotateY( angleY );
		zrot = Vec3D.X_AXIS.copy().rotateZ( angleZ );
		
		yrotQuat = new Quaternion( 1, new Vec3D( 0, 0, 0 ) );
		yrotQuat.set( yrot.dot( Vec3D.X_AXIS.copy() ), yrot.cross( Vec3D.X_AXIS.copy() ) );

		zrotQuat = new Quaternion( 1, new Vec3D( 0, 0, 0 ) );
		zrotQuat.set( zrot.dot( Vec3D.X_AXIS.copy() ), zrot.cross( Vec3D.X_AXIS.copy() ) );
		
		nowQuat = yrotQuat.multiply( zrotQuat );
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

	Vec3D mouseToSphere( float x, float y )
	{
		Vec3D v = new Vec3D
		(
			( x - cx ) / radius,
			( y - cy ) / radius,
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
