#pragma once

#include "ofMain.h"
#include "ofxMultiTouch.h"
#include "ofxAccelerometer.h"
#include "trackball.h"

class testApp : public ofSimpleApp, public ofxMultiTouchListener  {
	
public:
	void setup();
	void update();
	void draw();
	void exit();
	
	void initTrackball();
	void mouseTrackballDown( int x, int y );
	void mouseTrackballUp( int x, int y );
	void mouseTrackballMove( int x, int y );
	
	void createSphere( int PointRows, int PointsPerRow );
	void drawTriangle();
	void drawCube();
	void drawSphere();
	
	void keyPressed(int key) {}
	void keyReleased(int key)  {}
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased();
	void mouseReleased(int x, int y, int button );
	
	void touchDown(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL);
	void touchMoved(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL);
	void touchUp(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL);
	void touchDoubleTap(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL);
	
	struct SVertex
	{
		GLfloat position[ 3 ];
		GLubyte color[ 4 ];
	};
	
	SVertex	*Vertices;
	int		NumVertices;		//size of the vertex array
	vector <GLuint> IndexVect;  //we first put the indices into this vector, then copy them to the array below
	GLubyte	*Indices;
	int		NumIndices;			//size of the index array
	
	GLfloat ChangeY;	//Indicates how much the y values of the highest and deepest vertex
	bool	PointsOnly;	//Indicated, if the triangles or only points are drawn
	
    GLuint    vertexBuffer;
    GLuint    indexBuffer;
	
	// trackball.
	GLfloat gTrackBallRotation[ 4 ];
	GLfloat gWorldRotation[ 4 ];
	bool gTrackBall;
};

