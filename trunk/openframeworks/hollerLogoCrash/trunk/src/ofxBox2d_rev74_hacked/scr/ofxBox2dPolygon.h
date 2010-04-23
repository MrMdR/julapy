#pragma once
#include "ofMain.h"
#include "ofxBox2dBaseShape.h"

class ofxBox2dPolygon : public ofxBox2dBaseShape {
	
public:
	
	b2PolygonDef		poly;
	int					vertexCount;
	vector	<ofPoint>	vertices;
	
	//------------------------------------------------
	ofxBox2dPolygon() {
	}
	
	//------------------------------------------------ 
	void addVertex(float x, float y) {
		vertices.push_back(ofPoint(x, y));
	}
	void addVertex(ofPoint pt) {
		vertices.push_back(pt);
	}
	
	//------------------------------------------------
	void createShape(b2World * b2dworld, float x, float y) {
		
		if(b2dworld == NULL) {
			ofLog(OF_LOG_NOTICE, "- must have a valid world -");
			return;
		}
		if((int)vertices.size() <= 0) {
			ofLog(OF_LOG_NOTICE, "- please add vertex points -");
			return;
		}
		
		world			  = b2dworld;
		vertexCount		  = (int)vertices.size();
		poly.density	  = mass;
		poly.restitution  = bounce;
		poly.friction	  = friction;
		poly.vertexCount  = vertexCount;
		printf("- num vertices: %i -\n", poly.vertexCount);
		
		if(vertexCount > b2_maxPolygonVertices) {
			vertexCount = b2_maxPolygonVertices;
			ofLog(OF_LOG_WARNING, "- to many vertices -");
		}
		
		/*
		 
		 
		need to write something to test for bad polygons - ohh crap! 
		 
		 
		 */
		
		for(int i=0; i<vertexCount; i++) {
			poly.vertices[i].Set(vertices[i].x/OFX_BOX2D_SCALE, vertices[i].y/OFX_BOX2D_SCALE);
		}
		
		
		//Build Body for shape
		bodyDef.position.Set(x/OFX_BOX2D_SCALE, y/OFX_BOX2D_SCALE);
		body = world->CreateBody(&bodyDef);
		body->CreateShape(&poly);
		body->SetMassFromShapes();
		
		
		printf("--- We built the shape \n---");
		
		// anything that you need called
		init();
	}
	
	//------------------------------------------------
	void draw() {
		if(body != NULL ) {
			for(b2Shape* s=body->GetShapeList(); s; s=s->GetNext()) {
				const b2XForm& xf = body->GetXForm();		
				b2PolygonShape* poly = (b2PolygonShape*)s;
				int count = poly->GetVertexCount();
				const b2Vec2* verts = poly->GetVertices();
				
				ofEnableAlphaBlending();
				ofFill();
				ofSetColor(255, 0, 255, 100);
				ofBeginShape();
				for(int j=0; j<count; j++) {
					b2Vec2 pt = b2Mul(xf, verts[j]);
					ofVertex(pt.x*OFX_BOX2D_SCALE, pt.y*OFX_BOX2D_SCALE);
				}
				ofEndShape();
				
				ofNoFill();
				ofSetColor(255, 255, 255);
				ofBeginShape();
				for(int j=0; j<count; j++) {
					b2Vec2 pt = b2Mul(xf, verts[j]);
					ofVertex(pt.x*OFX_BOX2D_SCALE, pt.y*OFX_BOX2D_SCALE);
				}
				ofEndShape(true);
				
				ofDisableAlphaBlending();
				
			}

		}
	}
};











