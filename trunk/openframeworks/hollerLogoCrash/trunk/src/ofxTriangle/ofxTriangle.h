/*!
*	Original ofxTriangle code by => kikko.fr
*	-> C++ Triangle warper by Piyush Kumar
*	-> Point inside polygon by Theo
*
*	julapy - removed dependecy to openCV
*	triangulate method now takes a vector of polygon points.
*
*/

#ifndef OFXTRIANGLE_H_INCLUDED
#define OFXTRIANGLE_H_INCLUDED

#include "ofMain.h"
#include <del_interface.hpp>

using namespace tpp;
using namespace std;

typedef struct
{
    ofPoint a;
    ofPoint b;
    ofPoint c;

    float area;

} ofxTriangleData;


class ofxTriangle
{

public :

	 ofxTriangle();
	~ofxTriangle();

	void	triangulate				( vector<ofPoint>& polygon, int resolution = 50 );
	bool	isPointInsidePolygon	( const vector<ofPoint>& polygon, ofPoint p );
	ofPoint getTriangleCenter		( ofPoint *tr );
	
	vector<ofxTriangleData>&		getTriangles();			// const will make sure the data can not be tempered with outside of this class.

	void clear	();
	
	void draw	();
	void draw	( float x, float y );

private :
	
	vector<ofxTriangleData>	triangles;
};

#endif