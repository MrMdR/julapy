/*
 *  ofxShapes.h
 *  julapy_projectionMapping
 *
 *  Created by lukasz karluk on 29/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _OFX_SHAPES_H_
#define _OFX_SHAPES_H_

#define	OFX_SHAPES_ANCHOR_SIZE	10;

#include "ofMain.h"
#include "ofxMSAInteractiveObject.h"
#include <fstream>

class ofxShapesItem
{

public :

	ofxShapesItem()
	{
		//
	};
	
	~ofxShapesItem()
	{
		clear();
	};

	void clear()
	{
		for( vector<double*>::iterator itr=vertices.begin(); itr!=vertices.end(); ++itr)
		{
			delete [] (*itr);
		}
		vertices.clear();
		
		for( vector<ofxMSAInteractiveObject*>::iterator itr=anchors.begin(); itr!=anchors.end(); ++itr)
		{
			delete (*itr);
		}
		anchors.clear();
	};
	
	vector <double*> vertices;
	vector <ofxMSAInteractiveObject*> anchors;
};


class ofxShapes
{
	
public :
	
	 ofxShapes();
	~ofxShapes();
	
	void update		();
	void draw		();
	void addVertex	( int x, int y );
	void enableDraw	( bool value );
	void clear		();
	
	void writeToFile  ( string fileName = "shapes_data" );
	void loadFromFile ( string fileName = "shapes_data" );
	
	int  getShapesNum ();
	vector <double*> getShapeAt( int index );
	
private :
	
	void addVertexToShape ( ofxShapesItem *shape, int x, int y );
	void addAnchorToShape ( ofxShapesItem *shape, int x, int y );
	
	void checkEndShape();
	void clearShape();
	void storeShape();
	
	void checkShapeSelected();
	void resizeShape( ofxShapesItem *shape );
	
	void drawShape( ofxShapesItem *s );
	void drawShapeFill( ofxShapesItem *s );
	void drawShapeStroke( ofxShapesItem *s );
	void drawSelectedShapeFill( ofxShapesItem *s );
	void drawSelectedShapeStroke( ofxShapesItem *s );
	void drawShapeAnchors( ofxShapesItem *s );
	
	bool	bDraw;
	bool	bShapeSelected;
	
	vector <ofxShapesItem*> shapes;
	ofxShapesItem			currentShape;
	
	bool bEndShape;
	int  endAnchorIndex;
	
	int selectedShapeIndex;
};

#endif
