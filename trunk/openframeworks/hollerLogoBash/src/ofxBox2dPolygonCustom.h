/*
 *  ofxBox2dPolygonCustom.h
 *  emptyExample
 *
 *  Created by Lukasz Karluk on 28/04/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"
#include "ofxBox2dBaseShape.h"

class ofxBox2dPolygonCustom : public ofxBox2dPolygon
{

public :
	
	int color;
	
	virtual void setPhysicsClean( float mass, float bounce, float friction )
	{
		for(b2Shape* s=body->GetShapeList(); s; s=s->GetNext()) {
			body->DestroyShape(s);
		}
		
		ofPoint pos;
		pos.x = getPosition().x;
		pos.y = getPosition().y;
		
		poly.density	  = mass;
		poly.restitution  = bounce;
		poly.friction	  = friction;
		
		bodyDef.position.Set( pos.x / OFX_BOX2D_SCALE, pos.y / OFX_BOX2D_SCALE );
		body = world->CreateBody( &bodyDef );
		body->CreateShape( &poly );
		body->SetMassFromShapes();
	}
	
	virtual void draw() 
	{
		if(body != NULL ) {
			for(b2Shape* s=body->GetShapeList(); s; s=s->GetNext()) {
				const b2XForm& xf = body->GetXForm();		
				b2PolygonShape* poly = (b2PolygonShape*)s;
				int count = poly->GetVertexCount();
				const b2Vec2* verts = poly->GetVertices();

				ofEnableSmoothing();
//				ofEnableAlphaBlending();
				
				ofFill();
				ofSetColor( color );
				ofBeginShape();
				for(int j=0; j<count; j++) {
					b2Vec2 pt = b2Mul(xf, verts[j]);
					ofVertex(pt.x*OFX_BOX2D_SCALE, pt.y*OFX_BOX2D_SCALE);
				}
				ofEndShape(true);
				
				ofNoFill();
				ofSetColor( color );
				ofBeginShape();
				for(int j=0; j<count; j++)
				{
					b2Vec2 pt = b2Mul( xf, verts[ j ] );
					ofVertex( pt.x * OFX_BOX2D_SCALE, pt.y * OFX_BOX2D_SCALE );
					
					if( j == count - 1 )
					{
						pt = b2Mul( xf, verts[ 0 ] );
						ofVertex( pt.x * OFX_BOX2D_SCALE, pt.y * OFX_BOX2D_SCALE );
					}
				}
				ofEndShape();

//				ofDisableAlphaBlending();
				ofDisableSmoothing();
			}
			
		}
	}
};