package com.julapy.camera;

import processing.core.PApplet;
import toxi.geom.Vec3D;

public class Camera 
{
	public PApplet papp;
	public Vec3D eye, center, up;
	
	public Camera ( PApplet papp )
	{
		this.papp = papp;
		
		eye		= new Vec3D( 0, 100, 1000 );
		center	= new Vec3D( 0, 0, 0 );
		up		= new Vec3D( 0, 1, 0 );
		
		papp.camera( eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z );
	}
	
	public void update ()
	{
		papp.camera( eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z );
	}
	
	public void orbitTo ( float theta, float phi, float radius )
	{
		eye.x = (float)Math.cos( theta ) * (float)Math.sin( phi ) * radius;
		eye.y = (float)Math.sin( theta ) * (float)Math.sin( phi ) * radius;
		eye.z = (float)Math.cos( phi ) * radius;
	}
}
