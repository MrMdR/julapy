
#pragma once
#include "ofMain.h"
#include "Box2D.h"

class ofxBox2dRayCast : public b2RayCastCallback
{
	
public :

	b2World*	world;
	
	b2Fixture*	m_fixture;
	b2Vec2		m_point;
	b2Vec2		m_normal;
	float32		m_fraction;
	
	ofxBox2dRayCast()
	{
		world		= NULL;
		m_fixture	= NULL;
	}

	void setWorld ( b2World* world )
	{
		this->world = world;
	}
	
	void rayCast ( ofPoint& p1, ofPoint& p2 )
	{
		if( world == NULL )
			return;
		
		b2Vec2 point1( p1.x, p1.y );
		b2Vec2 point2( p2.y, p2.y );
		
		world->RayCast( this, point1, point2 );
	}
	
	//--
	
	float32 ReportFixture( b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction )
	{
		m_fixture	= fixture;
		m_point		= point;
		m_normal	= normal;
		m_fraction	= fraction;
		
		return fraction;
	}
};



