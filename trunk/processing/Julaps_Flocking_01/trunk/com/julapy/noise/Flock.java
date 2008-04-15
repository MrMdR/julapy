package com.julapy.noise;

import processing.core.PApplet;
import java.util.ArrayList;

import com.julapy.noise.NoiseField3D.Particle;

import toxi.geom.Vec3D;

public class Flock
{
	PApplet _papp;
	
	public int 		centerPull		= 120;
	public int 		flockPull 		= 80;
	public int 	 	minDistance		= 10;
	public float	velocityLimit	= 4.0f;
	public int		flockRange		= 10000;
	public float	targetPull		= 0.00125f;
	
	float r1Damping = 0.5f;
	float r2Damping = 0.2f;
	float r3Damping = 1.0f;
	float r4Damping = 1.0f;
	
	float force 	= 0;
	float decayRate = 0.85f;
	float range		= 400;
	
	Vec3D _flockTraget;
	
	ArrayList<Particle> _particles;
	IParticle _particle;
	
	public Flock ( ArrayList<Particle> particles )
	{
		_particles 	 = particles;
		_flockTraget = new Vec3D(); 
	}
	
	public void setFlockTarget ( Vec3D target )
	{
		_flockTraget = target;
	}
	
	public void setForce (  float force )
	{
		this.force = force;
	}
	
	public void update ()
	{
		force *= decayRate;
	}
	
	public Vec3D getFlockVector (IParticle particle)
	{
		_particle = particle;
		
		Vec3D v1 = rule1();
		Vec3D v2 = rule2();
		Vec3D v3 = rule3();
		Vec3D v4 = rule4();
		
		Vec3D v = _particle.vel().copy();
		
		v.x += ( r1Damping * v1.x ) + ( r2Damping * v2.x ) + ( r3Damping * v3.x ) + ( r4Damping * v4.x );
		v.y += ( r1Damping * v1.y ) + ( r2Damping * v2.y ) + ( r3Damping * v3.y ) + ( r4Damping * v4.y );
		v.z += ( r1Damping * v1.z ) + ( r2Damping * v2.z ) + ( r3Damping * v3.z ) + ( r4Damping * v4.z );
		
		limitVelocity( v );
		
		return v;
	}
	
	public Vec3D getMagnetVector ( IParticle particle )
	{
		Vec3D fc = new Vec3D();		// flock center.
		
		for (int i=0; i < _particles.size(); i++)
		{
			IParticle p = (IParticle) _particles.get(i);

			fc.addSelf(p.loc());
		}
		
		fc.x /= _particles.size();
		fc.y /= _particles.size();
		fc.z /= _particles.size();
		
		
		float dist = particle.loc().distanceTo( fc );
		if( dist < range )
		{
			float fs = dist / range;		// linear relationship between distance and force of magnet. fc = force scale.
			
			Vec3D mv = particle.loc().sub( fc ).normalize(); // magnet vector.
			mv.scaleSelf( force * fs * 40 );
			
			return mv;
		}
		else
		{
			return new Vec3D();
		}
	}
	
	// pull to the center
	public Vec3D rule1 ()
	{
		Vec3D v 	= new Vec3D();
		float pd	= 0;
		int count	= 0;
		
		for (int i=0; i < _particles.size(); i++)
		{
			IParticle p = (IParticle) _particles.get(i);

			if (_particle != p)
			{
				pd = _particle.loc().distanceTo(p.loc());
				
				if(pd < flockRange)
				{
					v.addSelf(p.loc());
					
					++count;
				}
			}
		}
		
		if(count > 0)
		{
			v.x /= count;
			v.y /= count;
			v.z /= count;
			
			v.x = (v.x - _particle.loc().x) / centerPull;
			v.y = (v.y - _particle.loc().y) / centerPull;
			v.z = (v.z - _particle.loc().z) / centerPull;
		}
		
		return v;
	}
	
	// avoid collision with other particles.
	public Vec3D rule2()
	{
		Vec3D v = new Vec3D();
		
		for (int i=0; i < _particles.size(); i++)
		{
			IParticle p = (IParticle) _particles.get(i);
			
			if (_particle != p)
			{
				if ( _particle.loc().distanceTo( p.loc() ) < minDistance )
				{
					v.x -= p.loc().x - _particle.loc().x;
					v.y -= p.loc().y - _particle.loc().y;
					v.z -= p.loc().z - _particle.loc().z;
				}
			}
		}

		return v;
	}
	
	// head to flock average
	public Vec3D rule3()
	{
		Vec3D v = new Vec3D();
		
		for (int i=0; i < _particles.size(); i++)
		{
			IParticle p = (IParticle) _particles.get(i);
			
			if (_particle != p)
			{
				v.x +=  p.vel().x;
				v.y +=  p.vel().y;
				v.z +=  p.vel().z;
			}
		}
		
		v.x /= (_particles.size() - 1);
		v.y /= (_particles.size() - 1);
		v.z /= (_particles.size() - 1);
		
		v.x = (v.x - _particle.vel().x) / flockPull; //8
		v.y = (v.y - _particle.vel().y) / flockPull; //8
		v.z = (v.z - _particle.vel().z) / flockPull; //8
		
		return v;
	}
	
	// 
	public Vec3D rule4()
	{
		Vec3D v = new Vec3D();
		
		v.x	= (_flockTraget.x - _particle.loc().x) * targetPull;
		v.y	= (_flockTraget.y - _particle.loc().y) * targetPull;
		v.z	= (_flockTraget.z - _particle.loc().z) * targetPull;
		
		return v;
	}

	public Vec3D limitVelocity (Vec3D v)
	{
	    float vel = v.magnitude();

	    if (vel > velocityLimit)
	    {
	    	v.x = ( v.x / vel ) * velocityLimit;
	    	v.y = ( v.y / vel ) * velocityLimit;
	    	v.z = ( v.z / vel ) * velocityLimit;
	    }
	    
	    return v;
	}
	
}
