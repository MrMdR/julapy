#include "ofxTriangle.h"

ofxTriangle :: ofxTriangle()
{
	//
}

ofxTriangle :: ~ofxTriangle()
{
	//
}

void ofxTriangle :: triangulate( vector<ofPoint>& polygon, int resolution )
{
	int t;
	t = MIN( polygon.size(), resolution );
	
	vector< Delaunay::Point > v;
	
    for( int i=0; i<t; i++ )
    {
		float p;
		p = i / (float)t;
		
		int j;
		j = (int)( p * polygon.size() );
		
		ofPoint& p1 = polygon.at( j );
		
		v.push_back( Delaunay::Point() );
		Delaunay::Point& p2 = v.back();

		p2[ 0 ] = p1.x;
		p2[ 1 ] = p1.y;
    }
	
	Delaunay* delobject;
	delobject = new Delaunay( v );
	delobject->Triangulate();
	
	Delaunay::fIterator fit;
	for( fit=delobject->fbegin(); fit != delobject->fend(); ++fit )
	{
		int pta = delobject->Org( fit );
		int ptb = delobject->Dest( fit );
		int ptc = delobject->Apex( fit );
		
		int pta_id = (int)( ( pta / (float)resolution ) * polygon.size() );
		int ptb_id = (int)( ( ptb / (float)resolution ) * polygon.size() );
		int ptc_id = (int)( ( ptc / (float)resolution ) * polygon.size() );
		
		ofxTriangleData tr;
		tr.a = ofPoint( polygon.at( pta_id ).x, polygon.at( pta_id ).y );
		tr.b = ofPoint( polygon.at( ptb_id ).x, polygon.at( ptb_id ).y );
		tr.c = ofPoint( polygon.at( ptc_id ).x, polygon.at( ptc_id ).y );
		
		if( isPointInsidePolygon( polygon, getTriangleCenter( tr ) ) )
        {
			triangles.push_back( ofxTriangleData() );
			ofxTriangleData& td = triangles.back();
			
			td.a	= ofPoint( tr.a.x, tr.a.y );
			td.b	= ofPoint( tr.b.x, tr.b.y );
			td.c	= ofPoint( tr.c.x, tr.c.y );
			
			td.area = delobject->area( fit );
		}
	}
	
	delete delobject;
}

vector<ofxTriangleData>& ofxTriangle :: getTriangles()
{
	return triangles;
}

void ofxTriangle :: clear()
{
    triangles.clear();
}

ofPoint ofxTriangle :: getTriangleCenter( const ofxTriangleData& tr )
{
	float cx = ( tr.a.x + tr.b.x + tr.c.x ) / 3.0;
	float cy = ( tr.a.y + tr.b.y + tr.c.y ) / 3.0;

	return ofPoint( cx, cy );
}

bool ofxTriangle :: isPointInsidePolygon( const vector<ofPoint>& polygon, ofPoint p )
{
    int counter;
	counter = 0;
	
	ofPoint p1, p2;
	
	p1 = polygon.at( 0 );
	
	for( int i=1; i<=polygon.size(); i++ )
	{
		p2 = polygon.at( i % polygon.size() );
		
		if( p.y > MIN( p1.y, p2.y ) )
		{
			if( p.y <= MAX( p1.y, p2.y ) )
			{
				if( p.x <= MAX( p1.x, p2.x ) )
				{
                    if( p1.y != p2.y )
					{
                        double xinters;
						xinters = ( p.y - p1.y ) * ( p2.x - p1.x ) / ( p2.y - p1.y ) + p1.x;
						
						if( p1.x == p2.x || p.x <= xinters )
						{
							counter++;
						}
					}
				}
			}
		}
        p1 = p2;
    }
	
	if (counter % 2 == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}


void ofxTriangle :: draw( float x, float y )
{
    ofPushMatrix();
    ofTranslate( x, y, 0 );
        draw();
    ofPopMatrix();
}

void ofxTriangle :: draw()
{
	for( int i=0; i<triangles.size(); i++ )
	{
		ofxTriangleData& tr = triangles.at( i );
		
		ofTriangle
		(
			tr.a.x, tr.a.y,
			tr.b.x, tr.b.y,
			tr.c.x, tr.c.y
		);
	}
}
