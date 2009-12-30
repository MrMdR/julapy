/*
 *  ofxShapes.cpp
 *  julapy_projectionMapping
 *
 *  Created by lukasz karluk on 29/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxShapes.h"

ofxShapes :: ofxShapes()
{
	bShapeSelected		= false;
	selectedShapeIndex	= 0;
	
	enableDraw( true );
	clearShape();
}

ofxShapes :: ~ofxShapes()
{
	
}

//////////////////////////////////////////////////////
//	UPDATE.
//////////////////////////////////////////////////////

void ofxShapes :: update()
{
	if( bShapeSelected )
		resizeShape( shapes[ selectedShapeIndex ] );
}

//////////////////////////////////////////////////////
//	DRAW MAIN.
//////////////////////////////////////////////////////

void ofxShapes :: draw()
{
	//-- draw current shape.
//	drawShapeFill( &currentShape );
	drawShapeStroke( &currentShape );
	drawShapeAnchors( &currentShape );
	
	//-- draw stored shapes.
	for( int i=0; i<shapes.size(); i++ )
	{
		if( i == selectedShapeIndex )
		{
//			drawSelectedShapeFill( shapes[ i ] );
			drawSelectedShapeStroke( shapes[ i ] );
			drawShapeAnchors( shapes[ i ] );
		}
		else
		{
//			drawShapeFill( shapes[ i ] );
			drawShapeStroke( shapes[ i ] );
			drawShapeAnchors( shapes[ i ] );
		}
	}
}

//////////////////////////////////////////////////////
//	ADD VERTEX.
//////////////////////////////////////////////////////

void ofxShapes :: addVertex( int x, int y )
{
	if( bDraw )
	{
		if( bShapeSelected )
			return;
		
		checkEndShape();
		
		if( bEndShape )		// end shape.
		{
			double* point = new double[ 3 ];
			point[ 0 ] = currentShape.vertices[ endAnchorIndex ][ 0 ];
			point[ 1 ] = currentShape.vertices[ endAnchorIndex ][ 1 ];
			point[ 2 ] = 0;
			currentShape.vertices.push_back( point );
			
			//			bShapeSelected = true;	// DEBUG.
			
			storeShape();
			clearShape();
		}
		else				// add new point.
		{
			addVertexToShape( &currentShape, x, y );
			addAnchorToShape( &currentShape, x, y );
		}
	}
}

void ofxShapes :: addVertexToShape ( ofxShapesItem *shape, int x, int y )
{
	double* point = new double[ 3 ];
	point[ 0 ] = x;
	point[ 1 ] = y;
	point[ 2 ] = 0;
	shape->vertices.push_back( point );
}

void ofxShapes :: addAnchorToShape ( ofxShapesItem *shape, int x, int y )
{
	int s = OFX_SHAPES_ANCHOR_SIZE;
	ofxMSAInteractiveObject *anchor;
	anchor = new ofxMSAInteractiveObject();
	anchor->setPos( x - (int)( s * 0.5 ), y - (int)( s * 0.5 ) );
	anchor->setSize( s, s );
	anchor->enableMouseEvents();
	shape->anchors.push_back( anchor );
}

//////////////////////////////////////////////////////
//	ENABLE.
//////////////////////////////////////////////////////

void ofxShapes :: enableDraw( bool value )
{
	bDraw = value;
}

//////////////////////////////////////////////////////
//	CLEAR.
//////////////////////////////////////////////////////

void ofxShapes :: clear()
{
	shapes.clear();
	
	clearShape();
	
	bShapeSelected		= false;
	selectedShapeIndex	= 0;
}

//////////////////////////////////////////////////////
//	WRITE / READ.
//////////////////////////////////////////////////////

void ofxShapes :: writeToFile ( string fileName )
{
	ofstream fout;
	fout.open( ofToDataPath( fileName ).c_str() );
	
	for( int i=0; i<shapes.size(); i++ )
	{
		string shapeDataStr;
		
		int vlen;
		vlen = shapes[ i ]->vertices.size();
		
		for( int j=0; j<vlen; j++ )
		{
			shapeDataStr += ofToString( shapes[ i ]->vertices[ j ][ 0 ], 5 );
			shapeDataStr += "-";
			shapeDataStr += ofToString( shapes[ i ]->vertices[ j ][ 1 ], 5 );
			shapeDataStr += "-";
			shapeDataStr += ofToString( shapes[ i ]->vertices[ j ][ 2 ], 5 );
			
			if( j == vlen - 1 )
				break;
			
			shapeDataStr += " ";
		}
		
		fout << shapeDataStr << endl;
	}
	
	fout.close();
}

void ofxShapes :: loadFromFile ( string fileName )
{
	clear();
	
	ifstream	shapeDataFile;
	string		shapeDataStr;
	
	shapeDataFile.open( ofToDataPath( fileName ).c_str() );
	
	if( shapeDataFile.is_open() )
	{
		while( !shapeDataFile.eof() )
		{
			getline( shapeDataFile, shapeDataStr );
			
			if( shapeDataStr == "" )
				continue;
			
			ofxShapesItem *shape;
			shape = new ofxShapesItem();
			
			vector<string> shapeData;
			shapeData = ofSplitString( shapeDataStr, " " );
			
			for( int i=0; i<shapeData.size(); i++ )
			{
				vector<string> pointData;
				pointData = ofSplitString( shapeData[ i ], "-" );
				
				ofPoint p;
				
				p.x = atof( pointData[ 0 ].c_str() );
				p.y = atof( pointData[ 1 ].c_str() );
				p.z = atof( pointData[ 2 ].c_str() );
				
				addVertexToShape( shape, p.x, p.y );
				
				if( i == shapeData.size() - 1 )
					break;
				
				addAnchorToShape( shape, p.x, p.y );
			}
			
			shapes.push_back( shape );
		}
	}
	
	shapeDataFile.close();
}

//////////////////////////////////////////////////////
//	GET SHAPES.
//////////////////////////////////////////////////////

int ofxShapes :: getShapesNum ()
{
	return shapes.size();
}

vector <double*> ofxShapes :: getShapeAt ( int index )
{
	index = MAX( MIN( index, shapes.size() - 1 ), 0 );
	
	return shapes[ index ]->vertices;
}


//////////////////////////////////////////////////////
//	SHAPE STUFF.
//////////////////////////////////////////////////////

void ofxShapes :: checkEndShape()
{
	bool bAnchorOver = false;
	
	for( int i=0; i<currentShape.anchors.size(); i++ )
	{
		if( currentShape.anchors[ i ]->isMouseOver() )
		{
			bAnchorOver		= true;
			endAnchorIndex	= i;
			break;
		}
	}
	
	if( bAnchorOver )
	{
		int shapeAnchorLength;
		shapeAnchorLength = currentShape.anchors.size() - endAnchorIndex;
		
		if( shapeAnchorLength > 2 )
		{
			bEndShape = true;
		}
	}
}

void ofxShapes :: clearShape()
{
	bEndShape		= false;
	endAnchorIndex	= 0;
	
	currentShape.clear();
}

void ofxShapes :: storeShape()
{
	ofxShapesItem *shape;
	shape = new ofxShapesItem();
	
	for( int i=endAnchorIndex; i<currentShape.vertices.size(); i++ )
	{
		double* point = new double[ 3 ];
		point[ 0 ] = currentShape.vertices[ i ][ 0 ];
		point[ 1 ] = currentShape.vertices[ i ][ 1 ];
		point[ 2 ] = 0;
		
		shape->vertices.push_back( point );
	}
	
	for( int i=endAnchorIndex; i<currentShape.anchors.size(); i++ )
	{
		ofxMSAInteractiveObject *anchor;
		anchor = new ofxMSAInteractiveObject();
		anchor->setPos(  currentShape.anchors[ i ]->x,     currentShape.anchors[ i ]->y );
		anchor->setSize( currentShape.anchors[ i ]->width, currentShape.anchors[ i ]->height );
		anchor->enableMouseEvents();
		
		shape->anchors.push_back( anchor );
	}
	
	shapes.push_back( shape );
}

//////////////////////////////////////////////////////
//	RESIZE SHAPES.
//////////////////////////////////////////////////////

void ofxShapes :: checkShapeSelected()
{
	// check here is shape is selected.
}

void ofxShapes :: resizeShape( ofxShapesItem *shape )
{
	for( int i=0; i<shapes.size(); i++ )
	{
		for( int j=0; j<shapes[ i ]->anchors.size(); j++ )
		{
			ofxMSAInteractiveObject *anchor;
			anchor = shapes[ i ]->anchors[ j ];
			
			if( anchor->isMouseDown() )		// drag anchor.
			{
				int mouseX = anchor->getMouseX();
				int mouseY = anchor->getMouseY();
				
				double *point;
				point = shapes[ i ]->vertices[ j ];
				point[ 0 ] = mouseX;
				point[ 1 ] = mouseY;
				
				if( j == 0 )
				{
					int k = shapes[ i ]->vertices.size() - 1;
					point = shapes[ i ]->vertices[ k ];
					point[ 0 ] = mouseX;
					point[ 1 ] = mouseY;
				}
				
				int s = OFX_SHAPES_ANCHOR_SIZE;
				anchor->setPos( mouseX - (int)( s * 0.5 ), mouseY - (int)( s * 0.5 ) );
			}
		}
	}
}

//////////////////////////////////////////////////////
//	DRAW.
//////////////////////////////////////////////////////

void ofxShapes :: drawShape( ofxShapesItem *shape )
{
	ofBeginShape();
	for( int i=0; i<shape->vertices.size(); i++ )
	{
		ofVertex( shape->vertices[ i ][ 0 ], shape->vertices[ i ][ 1 ] );
	}
	ofEndShape( false );
}

void ofxShapes :: drawShapeFill( ofxShapesItem *shape )
{
	ofFill();
	ofSetColor( 200, 200, 200 );
	drawShape( shape );
}

void ofxShapes :: drawShapeStroke( ofxShapesItem *shape )
{
	ofNoFill();
	ofSetColor( 255, 0, 255 );
	drawShape( shape );
}

void ofxShapes :: drawSelectedShapeFill( ofxShapesItem *shape )
{
	ofFill();
	ofSetColor( 200, 200, 200 );
	drawShape( shape );
}

void ofxShapes :: drawSelectedShapeStroke( ofxShapesItem *shape )
{
	ofNoFill();
	ofSetColor( 0, 0, 255 );
	drawShape( shape );
}

void ofxShapes :: drawShapeAnchors( ofxShapesItem *shape )
{
	ofFill();
	
	for( int i=0; i<shape->anchors.size(); i++ )
	{
		if( shape->anchors[ i ]->isMouseOver() )
		{
			ofSetColor( 255, 0, 0 );
		}
		else
		{
			ofSetColor( 0, 0, 0 );
		}
		
		ofRect( shape->anchors[ i ]->x, shape->anchors[ i ]->y, shape->anchors[ i ]->width, shape->anchors[ i ]->height );
	}
}
