#include "testApp.h"

///////////////////////////////////////////
//	INIT.
///////////////////////////////////////////

void testApp::setup()
{
	ofSetVerticalSync( true );
	ofSetFrameRate( 25 );
	
	screenGrab.setup( "movies/" );
//	screenGrab.setPause( false );
	
	logo.loadImage( "holler_logo_med.png" );
	logo.setImageType( OF_IMAGE_GRAYSCALE );
	
	logoPixles = logo.getPixels();
	
	int logoPixelsTotal;
	logoPixelsTotal = logo.width * logo.height;
	
	float pixelThreshold;
	pixelThreshold = 0.5;
	
	logoBWPixles = new unsigned char[ logoPixelsTotal ];
	
	for( int i=0; i<logoPixelsTotal; i++ )		// clean up image.
	{
		if( logoPixles[ i ] > (int)( pixelThreshold * 255 ) )
		{
			logoBWPixles[ i ] = 255;
		}
		else 
		{
			logoBWPixles[ i ] = 0;
		}
	}
	
	logoBW.allocate( logo.width, logo.height );
	logoBW.setFromPixels( logoBWPixles, logo.width, logo.height );
	
	bShowImage = true;
	
	initContourAnalysis();
	computeContourAnalysis();
	parseLogoShapes();

	initBox2d();
	updateTriangles();
	updateBox2dTriangles();
}

///////////////////////////////////////////
//	contour analysis.
///////////////////////////////////////////

void testApp :: initContourAnalysis ()
{
	maxContoursToFind		= 20;
	
	cdata.nBlobs			= 0;
	cdata.box				= new CvBox2D32f[ maxContoursToFind ];
	cdata.blobAngle			= new float[ maxContoursToFind ];
	cdata.contourReg		= new vector<ofxPoint2f>[ maxContoursToFind ];
	cdata.contourSmooth		= new vector<ofxPoint2f>[ maxContoursToFind ];
	cdata.contourSimple		= new vector<ofxPoint2f>[ maxContoursToFind ];
	cdata.contourHull		= new vector<ofxPoint2f>[ maxContoursToFind ];
	
	smoothPct		= 0.13f;
	tolerance		= 20.0f;
	
	cfMinArea		= 0.001;	// do not go to zero, shit will get weird.
	cfDetail		= 0;
	
	bShowRect		= true;
	bShowBox		= true;
	bShowEllipse	= false;
	bShowAngle		= false;
	bShowLines		= false;
}

void testApp :: computeContourAnalysis ()
{
	int maxArea;
	maxArea = logoBW.width * logoBW.height;
	
	int runningBlobs;
	runningBlobs = contourFinder.findContours
	(
		logoBW,							// image to be used.
		(int)( maxArea * cfMinArea ),	// min area.
		maxArea,						// max area.
		maxContoursToFind,				// max number of contours to find.
		true,							// find holes.
		false							// use approximation.
	);
	
	if( runningBlobs == 0 )
	{
		cdata.nBlobs = 0;
		
		return;
	}
	
	int nBlobs;
	nBlobs = contourFinder.nBlobs;
	
	cdata.nBlobs = nBlobs;
	
	cdata.blobArea.clear();
	cdata.blobArea.assign( nBlobs, 1.0f );
	cdata.blobBoundingRect.clear();
	cdata.blobBoundingRect.assign( nBlobs, ofRectangle() );
	cdata.blobCentroid.clear();
	cdata.blobCentroid.assign( nBlobs, ofPoint() );
	
	for( int j=0; j<nBlobs; j++ )
	{
		cdata.blobArea.at( j )					= contourFinder.blobs[ j ].area;
		
		cdata.blobBoundingRect.at( j ).x		= contourFinder.blobs[ j ].boundingRect.x;
		cdata.blobBoundingRect.at( j ).y		= contourFinder.blobs[ j ].boundingRect.y;
		cdata.blobBoundingRect.at( j ).width	= contourFinder.blobs[ j ].boundingRect.width;
		cdata.blobBoundingRect.at( j ).height	= contourFinder.blobs[ j ].boundingRect.height;
		
		cdata.blobCentroid.at( j ).x			= contourFinder.blobs[ j ].centroid.x;
		cdata.blobCentroid.at( j ).y			= contourFinder.blobs[ j ].centroid.y;
		
		int contourLength;
		contourLength = contourFinder.blobs[ j ].pts.size();
		
//		fitEllipse
//		(
//			contourFinder.blobs[ j ].pts,
//			cdata.box[ j ]
//		);
		
//		cdata.blobAngle[ j ] = getOrientation( contourFinder.blobs[ j ].pts );
		
//		findLines
//		(
//			contourFinder.blobs[ j ].pts,	// blob points.
//			cdata.geomLines,			// lines vector.
//			30,								// angle threshold.
//			40,								// minimum length.
//			30								// resolution.
//		);
		
		cdata.contourReg[ j ].clear();
		cdata.contourReg[ j ].assign( contourLength, ofxPoint2f() );
		cdata.contourSmooth[ j ].clear();
		cdata.contourSmooth[ j ].assign( contourLength, ofxPoint2f() );
		cdata.contourSimple[ j ].clear();
		cdata.contourHull[ j ].clear();
		
		for( int k=0; k<contourLength; k++ )
		{
			cdata.contourReg[ j ].at( k ) = contourFinder.blobs[ j ].pts[ k ];
		}
		
		contourS.smooth( cdata.contourReg[ j ], cdata.contourSmooth[ j ], smoothPct );
		contourS.simplify( cdata.contourSmooth[ j ], cdata.contourSimple[ j ], tolerance );
		contourS.convexHull( cdata.contourSmooth[ j ], cdata.contourHull[ j ] );
	}
}

void testApp :: drawContourAnalysis()
{
	for( int j=0; j<cdata.nBlobs; j++ )
	{
		if( cfDetail == 0 )
		{
			ofSetColor( 0xFF0000 );
			ofNoFill();
			ofBeginShape();
			for( int k=0; k<cdata.contourReg[ j ].size(); k++ )
			{
				ofVertex
				(
					cdata.contourReg[ j ].at( k ).x,
					cdata.contourReg[ j ].at( k ).y
				);
			}
			ofEndShape( true );
		}
		else if( cfDetail == 1 )
		{
			ofSetColor( 0xFF0000 );
			ofNoFill();
			ofBeginShape();
			for( int k=0; k<cdata.contourSmooth[ j ].size(); k++ )
			{
				ofVertex
				(
					cdata.contourSmooth[ j ].at( k ).x,
					cdata.contourSmooth[ j ].at( k ).y
				);
			}
			ofEndShape(true);
		}
		else if( cfDetail == 2 )
		{
			ofSetColor( 0xFF0000 );
			ofNoFill();
			ofBeginShape();
			for( int k=0; k<cdata.contourSimple[ j ].size(); k++ )
			{
				ofVertex
				(
					cdata.contourSimple[ j ].at( k ).x,
					cdata.contourSimple[ j ].at( k ).y
				);
			}
			ofEndShape(true);
		}
		else if( cfDetail == 3 )
		{
			ofSetColor( 0xFF0000 );
			ofNoFill();
			ofBeginShape();
			for( int k=0; k<cdata.contourHull[ j ].size(); k++ )
			{
				ofVertex
				(
					cdata.contourHull[ j ].at( k ).x,
					cdata.contourHull[ j ].at( k ).y
				);
			}
			ofEndShape(true);
			
			for( int k=0; k<cdata.contourHull[ j ].size(); k++ )
			{
				ofCircle
				(
					cdata.contourHull[ j ].at( k ).x,
					cdata.contourHull[ j ].at( k ).y,
					2
				);
			}
		}
		
		if( bShowRect )
		{
			ofNoFill();
			ofSetColor( 0xFF00FF );
			
			ofRect
			(
				cdata.blobBoundingRect[ j ].x,
				cdata.blobBoundingRect[ j ].y,
				cdata.blobBoundingRect[ j ].width,
				cdata.blobBoundingRect[ j ].height
			);
		}
		
		continue;
		
		if( bShowBox )
		{
			ofSetRectMode( OF_RECTMODE_CENTER );
			
			ofNoFill();
			ofSetColor( 0xFFFFFF );
			
			glPushMatrix();
			glTranslatef
			(
				cdata.box[ j ].center.x,
				cdata.box[ j ].center.y,
				0
			);
			glRotatef( cdata.box[ j ].angle, 0, 0, 1 );
			ofRect
			(
				0,
				0,
				cdata.box[ j ].size.width,
				cdata.box[ j ].size.height
			);
			glPopMatrix();
			
			ofSetRectMode( OF_RECTMODE_CORNER );
		}
		
		if( bShowEllipse )
		{
			ofNoFill();
			ofSetColor( 0xFFFFFF );
			
			glPushMatrix();
			glTranslatef
			(
				cdata.box[ j ].center.x,
				cdata.box[ j ].center.y,
				0
			);
			glRotatef( cdata.box[ j ].angle, 0, 0, 1 );
			ofEllipse
			(
				0,
				0,
				cdata.box[ j ].size.width  * 0.5,
				cdata.box[ j ].size.height * 0.5
			);
			glPopMatrix();
		}
		
		if( bShowAngle )
		{
			ofSetColor( 0xFFFFFF );
			
			float x1,y1,x2,y2;
			
			x1 = cdata.blobCentroid[ j ].x + 25 * cos( cdata.blobAngle[ j ] );
			y1 = cdata.blobCentroid[ j ].y + 25 * sin( cdata.blobAngle[ j ] );
			x2 = cdata.blobCentroid[ j ].x - 25 * cos( cdata.blobAngle[ j ] );
			y2 = cdata.blobCentroid[ j ].y - 25 * sin( cdata.blobAngle[ j ] );
			
			glPushMatrix();
			glScalef(0.5,0.5,0.0);
			ofLine(x1*2,y1*2,x2*2,y2*2);
			glPopMatrix();
			
			x1 = cdata.blobCentroid[ j ].x + 10 * cos( cdata.blobAngle[ j ] + HALF_PI );
			y1 = cdata.blobCentroid[ j ].y + 10 * sin( cdata.blobAngle[ j ] + HALF_PI );
			x2 = cdata.blobCentroid[ j ].x - 10 * cos( cdata.blobAngle[ j ] + HALF_PI );
			y2 = cdata.blobCentroid[ j ].y - 10 * sin( cdata.blobAngle[ j ] + HALF_PI );
			glPushMatrix();
			glScalef(0.5,0.5,0.0);
			ofLine(x1*2,y1*2,x2*2,y2*2);
			glPopMatrix();
		}
		
		if( bShowLines )
		{
			ofSetColor( 0x8aff00 );
			ofNoFill();
			for( int k=0; k<cdata.geomLines.size(); k++ )
			{
				ofLine
				(
					cdata.geomLines[ k ].x,
					cdata.geomLines[ k ].y,
					cdata.geomLines[ k ].z,
					cdata.geomLines[ k ].w
				);
				ofCircle
				(
					cdata.geomLines[ k ].x,
					cdata.geomLines[ k ].y,
					3
				);
				ofCircle
				(
					cdata.geomLines[ k ].z,
					cdata.geomLines[ k ].w,
					3
				);
			}
		}
	}
}

///////////////////////////////////////////
//	contours to shapes.
///////////////////////////////////////////

void testApp :: parseLogoShapes ()
{
	int t;
	t = cdata.nBlobs;
	
	for( int i=0; i<t; i++ )				// initialise shapes.
	{
		shapes.push_back( LogoShape() );
		
		LogoShape& s	= shapes.back();
		s.parent		= -1;
		s.child			= -1;
		s.noPolys		= 1;
	}
	
	for( int i=0; i<t; i++ )				// check if shapes are embedded - left to right.
	{
		ofRectangle r1;
		r1.x		= cdata.blobBoundingRect[ i ].x;
		r1.y		= cdata.blobBoundingRect[ i ].y;
		r1.width	= cdata.blobBoundingRect[ i ].width;
		r1.height	= cdata.blobBoundingRect[ i ].height;
		
		LogoShape& s1 = shapes.at( i );
		
		int j = 0;
		while( j++ < t - 1 )
		{
			int k;
			k = ( i + j ) % t;
			
			ofRectangle r2;
			r2.x		= cdata.blobBoundingRect[ k ].x;
			r2.y		= cdata.blobBoundingRect[ k ].y;
			r2.width	= cdata.blobBoundingRect[ k ].width;
			r2.height	= cdata.blobBoundingRect[ k ].height;
			
			LogoShape& s2 = shapes.at( k );
			
			bool embedded = false;
			embedded = checkEmbeddedRectangles( r1, r2 );
			
			if( embedded )
			{
				s1.child	= k;
				s2.parent	= i;
			}
		}
	}

	for( int i=t-1; i>=0; i-- )				// check if shapes are embedded - right to left.
	{
		ofRectangle r1;
		r1.x		= cdata.blobBoundingRect[ i ].x;
		r1.y		= cdata.blobBoundingRect[ i ].y;
		r1.width	= cdata.blobBoundingRect[ i ].width;
		r1.height	= cdata.blobBoundingRect[ i ].height;
		
		LogoShape& s1 = shapes.at( i );
		
		int j = 0;
		while( j++ < t - 1 )
		{
			int k;
			k = ( i - j );
			
			if( k < 0 )
				k = t + k;
			
			ofRectangle r2;
			r2.x		= cdata.blobBoundingRect[ k ].x;
			r2.y		= cdata.blobBoundingRect[ k ].y;
			r2.width	= cdata.blobBoundingRect[ k ].width;
			r2.height	= cdata.blobBoundingRect[ k ].height;
			
			LogoShape& s2 = shapes.at( k );
			
			bool embedded;
			embedded = checkEmbeddedRectangles( r1, r2 );
			
			if( embedded )
			{
				s1.child	= k;
				s2.parent	= i;
			}
		}
	}
	
	for( int i=0; i<t; i++ )			// copy child data into parent.
	{
		LogoShape& s1 = shapes.at( i );
		
		bool hasChild;
		hasChild = ( s1.child > -1 );
		
		if( hasChild )
		{
			s1.noPolys += 1;
		}
		
		s1.polyPoints = new vector<ofPoint>[ s1.noPolys ];
		
		copyPolygonData( cdata.contourReg[ i ], s1.polyPoints[ 0 ] );
		
		if( hasChild )
		{
//			copyPolygonData( cdata.contourReg[ s1.child ], s1.polyPoints[ 1 ] );
			copyPolygonData( cdata.contourReg[ s1.child ], s1.polyPoints[ 0 ] );
		}
	}
	
	for( int i=0; i<t; i++ )						// remove child contours with a parent, as they have already been copied to parent.
	{
		LogoShape& s1 = shapes.at( i );
		
		if( s1.parent != -1 )						// has parent. remove it.
		{
//			delete s1.polyPoints;					// causing an error... ?
			
			shapes.erase( shapes.begin() + i );		// remove from array, step back counter after removed.
			
			--i;
			--t;
		}
	}
}

bool testApp :: checkEmbeddedRectangles( const ofRectangle& r1, const ofRectangle& r2 )
{
	bool p1, p2, p3, p4;
	
	p1 = ( ( r2.x >= r1.x ) && ( r2.y >= r1.y ) );
	p2 = ( ( r2.x + r2.width <= r1.x + r1.width ) && ( r2.y >= r1.y ) );
	p3 = ( ( r2.x + r2.width <= r1.x + r1.width ) && ( r2.y + r2.height <= r1.y + r1.height ) );
	p4 = ( ( r2.x >= r1.x ) && ( r2.y + r2.height <= r1.y + r1.height ) );
	
	return ( p1 && p2 && p3 && p4 );
}

void testApp :: copyPolygonData ( const vector<ofxPoint2f>& p1, vector<ofPoint>& p2 )
{
	for( int i=0; i<p1.size(); i++ )
	{
		p2.push_back( ofPoint() );
		
		ofPoint& p = p2.back();
		p.x = p1.at( i ).x;
		p.y = p1.at( i ).y;
	}
}

void testApp :: drawLogoShapes ()
{
	ofFill();
	ofSetColor( 0xFFFFFF );
	
	for( int i=0; i<shapes.size(); i++ )
	{
		LogoShape& s1 = shapes.at( i );
		
		ofBeginShape();
		
		for( int j=0; j<1; j++ )
		{
			for( int k=0; k<s1.polyPoints[ j ].size(); k++ )
			{
				ofPoint& p1 = s1.polyPoints[ j ].at( k );
				
				ofVertex( p1.x, p1.y );
			}
		}
		
		ofEndShape( true );
	}
}

///////////////////////////////////////////
//	triangles.
///////////////////////////////////////////

void testApp :: updateTriangles ()
{
	triangle.clear();
	
	for( int i=0; i<shapes.size(); i++ )
	{
		int resolution;
		resolution = MAX( 3.0, shapes.at( i ).polyPoints[ 0 ].size() * 0.1 );
//		resolution = MAX( 3.0, shapes.at( i ).polyPoints[ 0 ].size() * 0.2 );
//		resolution = MAX( 3.0, shapes.at( i ).polyPoints[ 0 ].size() * 0.3 );
		
		triangle.triangulate( shapes.at( i ).polyPoints[ 0 ], resolution );
		
//		addBody( triangle.getTriangles() );
	}
}

void testApp :: updateBox2dTriangles ()
{
	box2dTriangles.clear();
	
	for( int i=0; i<triangle.getTriangles().size(); i++ )
	{
		ofxTriangleData& triData = triangle.getTriangles().at( i );
		
		ofPoint triPos;
		triPos = triangle.getTriangleCenter( triData );

		ofPoint ta, tb, tc;
		ta = triData.a - triPos;
		tb = triData.b - triPos;
		tc = triData.c - triPos;
		
		ofxBox2dPolygonCustom tri;
		tri.setPhysics( 0, 0, 0 );			// fixed.
//		tri.setPhysics
//		(
//			1.0,							// mass.
//			ofRandom( 0.5, 0.8 ),			// bouce.
//			ofRandom( 0.1, 0.7 )			// friction.
//		);
		tri.addVertex( ta );
		tri.addVertex( tb );
		tri.addVertex( tc );
		tri.createShape( box2d.getWorld(), triPos.x, triPos.y );
		
//		ofxVec2f vel;
//		vel.x = triPos.x - ofGetWidth()  * 0.5;
//		vel.y = triPos.y - ofGetHeight() * 0.5;
//		vel.normalize();
//		vel		*= ofRandom(  0.3, 0.8 );
//		vel.x	+= ofRandom( -0.5, 0.5 );
//		vel.y	+= ofRandom( -0.5, 0.5 );
//		vel.y	*= 0.5;
//		
//		tri.setVelocity( vel.x, vel.y );
		
		box2dTriangles.push_back( tri );
	}
}

void testApp :: drawTriangles ()
{
	ofFill();
	ofSetColor( 0xFFFFFF );
	
	vector<ofxTriangleData>& triangles = triangle.getTriangles();
	
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
	
	triangle.draw();
}

void testApp :: drawBox2dTriangles ()
{
	for( int i=0; i<box2dTriangles.size(); i++ )
	{
		box2dTriangles[ i ].draw();
	}
}

///////////////////////////////////////////
//	BOX2D BODIES.
///////////////////////////////////////////

void testApp :: addBox2dCircle ( ofPoint& position, ofPoint& velocity, float size, float mass, float bounce, float friction )
{
	ofxBox2dCircleCustom circle;
	
	circle.setPhysics( 1000, 0, 0 );
	circle.setup( box2d.getWorld(), position.x, position.y, size, false );
	circle.setVelocity( velocity.x, velocity.y );
	
	box2dCircles.push_back( circle );
}

void testApp :: drawBox2dCircles ()
{
	for( int i=0; i<box2dCircles.size(); i++ )
	{
		box2dCircles[ i ].draw();
	}
}

///////////////////////////////////////////
//	BOX2D BODIES.
///////////////////////////////////////////

void testApp :: initBox2d ()
{
	box2d.init();
	box2d.setGravity( 0, 0 );
	box2d.setFPS( 20 );
	
	int thick;
	thick = 1;
	
	float wallMass		= 1.0;
	float wallBounce	= 0.5;
	float wallFriction	= 0.3;
	
	ofxBox2dRect floor;
	floor.setPhysics( wallMass, wallBounce, wallFriction );
	floor.setup( box2d.getWorld(), 0, ofGetHeight(), ofGetWidth(), thick, true );
	
	ofxBox2dRect ceil;
	ceil.setPhysics( wallMass, wallBounce, wallFriction );
	ceil.setup( box2d.getWorld(), 0, 0, ofGetWidth(), thick, true );
	
	ofxBox2dRect left;
	left.setPhysics( wallMass, wallBounce, wallFriction );
	left.setup( box2d.getWorld(), 0, 0, thick, ofGetHeight(), true );
	
	ofxBox2dRect right;
	right.setPhysics( wallMass, wallBounce, wallFriction );
	right.setup( box2d.getWorld(), ofGetWidth(), 0, thick, ofGetHeight(), true );
	
	ofPoint p, v;
	p.x = 40;
	p.y = ofGetHeight() * 0.5;
	v.x = 10;
	v.y = 0;
	addBox2dCircle( p, v, 20, 1000 );
}

void testApp :: addBody( const vector<ofxTriangleData>& triangles )
{
	b2BodyDef* bodyDef = new b2BodyDef();
	bodyDef->position.Set( 360 / OFX_BOX2D_SCALE, 20 / OFX_BOX2D_SCALE );
	bodyDef->linearDamping	= 0.25;
	bodyDef->angularDamping = 0.25;
	
	b2Body* body = box2d.getWorld()->CreateBody( bodyDef );
	makeComplexBody( triangles, body );
	bodies.push_back( body );
}

//-- http://www.psyked.co.uk/box2d/simple-box2d-custom-polygon-creation.htm

void testApp :: makeComplexBody( const vector<ofxTriangleData>& triangles, b2Body* body )
{
	int shapeCnt = 0;
	
	for( int i=0; i<triangles.size(); i++ )
	{
		b2PolygonDef* shapeDef = new b2PolygonDef();
		shapeDef->density		= 1;
		shapeDef->friction		= 5;
		shapeDef->vertexCount	= 3;
		
		shapeDef->vertices[ 0 ].Set( triangles[ i ].a.x / OFX_BOX2D_SCALE, triangles[ i ].a.y / OFX_BOX2D_SCALE );
		shapeDef->vertices[ 1 ].Set( triangles[ i ].b.x / OFX_BOX2D_SCALE, triangles[ i ].b.y / OFX_BOX2D_SCALE );
		shapeDef->vertices[ 2 ].Set( triangles[ i ].c.x / OFX_BOX2D_SCALE, triangles[ i ].c.y / OFX_BOX2D_SCALE );
		
		body->CreateShape( shapeDef );
		shapeCnt++;
	}
	
	shapeCnts.push_back(shapeCnt);
	body->SetMassFromShapes();
}

void testApp :: updateBodies ()
{
	//
}

void testApp :: drawBodies ()
{
	for( int i=0; i<bodies.size(); i++ )
	{
		b2Body*	body				= bodies[i];
		b2Shape* s					= body->GetShapeList();
		const b2XForm& xf			= body->GetXForm();
		b2PolygonShape* poly		= (b2PolygonShape*)s;
		int count					= poly->GetVertexCount();
		const b2Vec2* localVertices = poly->GetVertices();
		vector<b2Vec2> verts;
		
		for(int j = 0; j < count; j++)
		{
			verts.push_back(b2Mul(xf, localVertices[j]));
		}
		
		int shapeCnt = shapeCnts[i];			
		for (int j = 0; j < shapeCnt-1; j++)
		{
			b2Shape* next = s->GetNext();
			s = next;
			b2PolygonShape* poly = (b2PolygonShape*)next;
			int count = poly->GetVertexCount();
			const b2Vec2* localVertices = poly->GetVertices();
			for(int k = 0; k < count; k++)
			{
				verts.push_back(b2Mul(xf, localVertices[k]));
			}					
		}
		
		ofEnableAlphaBlending();
		ofSetColor( 0xFF00FF );
		glBegin(GL_TRIANGLES); 
		for(int j = 0; j < verts.size(); j++) {
			glVertex3f(verts[j].x*OFX_BOX2D_SCALE, verts[j].y*OFX_BOX2D_SCALE, 0);
		}
		glEnd();			
		ofDisableAlphaBlending();		
	}
}

///////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////

void testApp::update()
{
	box2dCircles[ 0 ].setVelocity( 10, 0 );
	
	box2d.update();
}

///////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////

void testApp::draw()
{
	ofFill();
	ofSetColor( 0x000000 );
	ofRect( 0, 0, ofGetWidth(), ofGetHeight() );
	
	ofSetColor( 0xFFFFFF );
	logo.draw( 0, 0 );
	
//	drawContourAnalysis();
	
//	drawLogoShapes();

//	drawTriangles();
	
	drawBox2dTriangles();
	drawBox2dCircles();
//	drawBodies();
	
	screenGrab.save();
}

///////////////////////////////////////////
//	HANDLERS.
///////////////////////////////////////////

void testApp::keyPressed(int key)
{
	if( key == 'i' )
	{
		bShowImage = !bShowImage;
	}
	
	if( key == 's')
	{
		screenGrab.togglePause();
	}
}

void testApp::keyReleased(int key)
{

}

void testApp::mouseMoved(int x, int y )
{

}

void testApp::mouseDragged(int x, int y, int button)
{

}

void testApp::mousePressed(int x, int y, int button)
{

}

void testApp::mouseReleased(int x, int y, int button)
{

}

void testApp::windowResized(int w, int h)
{

}

