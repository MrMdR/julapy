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
	
	virtual void draw() 
	{
		if(body != NULL ) {
			for(b2Shape* s=body->GetShapeList(); s; s=s->GetNext()) {
				const b2XForm& xf = body->GetXForm();		
				b2PolygonShape* poly = (b2PolygonShape*)s;
				int count = poly->GetVertexCount();
				const b2Vec2* verts = poly->GetVertices();

				ofEnableSmoothing();
				ofEnableAlphaBlending();
				
				ofFill();
				ofSetColor( 255, 255, 255, 220 );
				ofBeginShape();
				for(int j=0; j<count; j++) {
					b2Vec2 pt = b2Mul(xf, verts[j]);
					ofVertex(pt.x*OFX_BOX2D_SCALE, pt.y*OFX_BOX2D_SCALE);
				}
				ofEndShape(true);
				
				ofNoFill();
				ofSetColor( 0, 0, 0, 100 );
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

				ofDisableAlphaBlending();
				ofDisableSmoothing();
			}
			
		}
	}
};