package com.julapy.camera;

import processing.core.PApplet;
import toxi.geom.Vec3D;

public class CameraOrbit 
{
	public PApplet papp;
	public float radius;
	public float theta;
	public float phi;
	public float thetaSpeed = 0;
	public float phiSpeed = 0;
	public int thetaDir = 1;
	public int phiDir = 1;
	public Vec3D eye, center, up;

	public CameraOrbit ( PApplet papp, float radius, float theta, float phi, float thetaSpeed, float phiSpeed )
	{
		this.papp		= papp;
		this.radius		= radius;
		this.theta		= theta;
		this.phi		= phi;
		this.thetaSpeed = thetaSpeed;
		this.phiSpeed 	= phiSpeed;

		eye		= new Vec3D();
		center	= new Vec3D();
		up		= new Vec3D();
	}
	
	public void update ()
	{
//		center = eye.copy();
		
		theta 	+= thetaSpeed * thetaDir;
		phi 	+= phiSpeed * phiDir;
		
		eye.x = (float)Math.cos(theta) * (float)Math.sin(phi) * radius;
		eye.y = (float)Math.sin(theta) * (float)Math.sin(phi) * radius;
		eye.z = (float)Math.cos(phi) * radius;
		
//		up = eye.copy().normalize();
		up = new Vec3D( 0, 0, 1 );
		
		papp.camera( eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z );
	}
	
}
