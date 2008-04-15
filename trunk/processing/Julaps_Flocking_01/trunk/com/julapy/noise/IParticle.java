package com.julapy.noise;

import toxi.geom.Vec3D;

public interface IParticle {

	public Vec3D loc();
	public Vec3D vel();
	public void update();
	public void render();
}
