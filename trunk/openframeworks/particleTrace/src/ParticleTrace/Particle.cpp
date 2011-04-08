/*
 *  Particle.cpp
 *  emptyExample
 *
 *  Created by lukasz karluk on 1/03/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "Particle.h"

Particle :: Particle( PixelFlow* pfImage, PixelFlow* pfTrace )
{
	this->pfImage = pfImage;
	this->pfTrace = pfTrace;
	
	pfSampleRangeX	= 5;
	pfSampleRangeY	= 5;
	
	//---
	
	imageVecScale	= 100;
	traceVecScale	= 100;
	wanderVecScale	= 1;

	velLimit		= 2.0;
	velEase			= 0.2;
	
	//---

	line_ind_max    = PARTICLE_MAX_LENGTH;
	line_ind_total  = 0;
	line_ver_array  = new GLfloat[ line_ind_max * 3 ];
	line_col_array  = new GLfloat[ line_ind_max * 4 ];
    
	strip_ind_max   = PARTICLE_MAX_LENGTH * 2;
	strip_ind_total = 0;
	strip_ver_array = new GLfloat[ strip_ind_max * 3 ];
	strip_col_array = new GLfloat[ strip_ind_max * 4 ];

    //---
    
    line_normals    = new float[ line_ind_max * 3 ];
    line_lengths    = new float[ line_ind_max ];
    
    //---
	
	stripWidth	= 0.1;
	
	headColor.r = 255;
	headColor.g = 0;
	headColor.b = 0;
	headColor.a = 255;
	
	lineColor.r = 255;
	lineColor.g = 255;
	lineColor.b = 255;
	lineColor.a = 128;
	
	colorEase	= 0.05;
	
	traceAlpha	= 1.0;
	
	size		= 2;
	sizeHalf	= size * 0.5;
	
	lifeCount	= 0;
	
    distMoved   = 0;
	minPosDist	= 2.0;
	
	//---
	
	wanderTheta			= ofRandom( 0, TWO_PI );
	wanderRadius		= 16.0;
	wanderDistance		= 60.0;
	wanderChange		= 0.25;
	wanderEase			= 0.2;
	wanderEaseTarget	= wanderEase;
	wanderMaxSpeed		= 2.0;
	
	//---
	
	bounds.x		= 0;
	bounds.y		= 0;
	bounds.width	= ofGetWidth();
	bounds.height	= ofGetHeight();
	bInsideBounds	= true;
	
	//---

	bUseImageColour		= false;
	bUseImageForce		= true;
	bUseTraceForce		= true;
	bUseWanderForce		= true;
	bMarkAsTestParticle	= false;
	bVerbose			= false;
    bPointAdded         = false;
    
    //---
    
    ribbonType          = NULL;
    ribbonPositionX     = 0;
    ribbonCopyIndex     = 0;
    ribbonCopy          = "hello hello hello hello hello hello hello hello hello hello hello hello hello hello hello hello hello hello";
    ribbonFontScale     = 0.045;
    ribbonFontScaleVar  = ofRandom( 0.25, 1.0 );
}

Particle :: ~Particle()
{
	pfImage = NULL;
	pfTrace = NULL;
	
	delete[] line_ver_array;
	delete[] line_col_array;
	line_ver_array = NULL;
	line_col_array = NULL;
	
	delete[] strip_ver_array;
	delete[] strip_col_array;
	strip_ver_array = NULL;
	strip_col_array = NULL;
    
    delete[] line_normals;
    line_normals = NULL;
    
    delete[] line_lengths;
    line_lengths = NULL;
    
    for( int i=0; i<ribbonLetters.size(); i++ )
        delete ribbonLetters[ i ];
    ribbonLetters.clear();
    
    ribbonType = NULL;
}

//////////////////////////////////////////////////
//	SETUP.
//////////////////////////////////////////////////

void Particle :: setInitialPosition	( float x, float y )
{
	posVec.x = x;
	posVec.y = y;
	
	posPrevVec.x = x;
	posPrevVec.y = y;
	
	posLastAdded.x = x;
	posLastAdded.y = y;
    
    posInImg    = posVec;
    posInImg.x -= bounds.x;
    posInImg.y -= bounds.y;
	
	currentColor = pfImage->getColourAt( posInImg );
    
	addToLineVertexArray( posVec, currentColor );
	addToStripVertexArray( posVec, posVec, currentColor, currentColor );
    
    //---
    
    line_normals[ 0 ] = 0;
    line_normals[ 1 ] = 0;
    line_normals[ 2 ] = 0;
    
    line_lengths[ 0 ] = 0;
}

void Particle :: setInitialVelocity	( float x, float y )
{
	velVec.x = x;
	velVec.y = y;
}

void Particle :: setPixelRange ( int x, int y )
{
	pfSampleRangeX = x;
	pfSampleRangeY = y;
}

void Particle :: setBounds ( const ofRectangle& rect )
{
	bounds = rect;
}

void Particle :: setRibbonType ( RibbonType2D* ribbonType, string ribbonCopy )
{
    this->ribbonType = ribbonType;
    this->ribbonCopy = ribbonCopy;
}

void Particle :: setup ()
{
	//
}

//////////////////////////////////////////////////
//	UPDATE.
//////////////////////////////////////////////////

void Particle :: update ()
{
	++lifeCount;
    
	//--
	
	posPrevVec.x = posVec.x;
	posPrevVec.y = posVec.y;
    
    posInImg    = posVec;
    posInImg.x -= bounds.x;
    posInImg.y -= bounds.y;
	
	//--
	
	ofxVec2f totalVec;
	
	imgVec.set( 0, 0 );
	trcVec.set( 0, 0 );
	
	if( bUseImageForce )
	{
		imgVec = pfImage->getVectorAt( posInImg, pfSampleRangeX, pfSampleRangeY );
		imgVec *= imageVecScale;
	
		totalVec += imgVec;
	}
	
	if( bUseTraceForce )
	{
		trcVec = pfTrace->getVectorAt( posInImg, pfSampleRangeX, pfSampleRangeY );
		trcVec *= traceVecScale * -1;
		
		totalVec += trcVec;
	}
	
	if( bUseWanderForce )
	{
		wander();
		
		totalVec += wanderVel * wanderVecScale;
	}
	
	totalVec.limit( velLimit );
	
	velVec.x += ( totalVec.x - velVec.x ) * velEase;
	velVec.y += ( totalVec.y - velVec.y ) * velEase;
	
	posVec.x += velVec.x;
	posVec.y += velVec.y;
	
	//---
    
    resetRibbonType();
    
    //---
	
	distMoved = ofDist( posLastAdded.x, posLastAdded.y, posVec.x, posVec.y );
	if( distMoved >= minPosDist )
	{
		posLastAdded.x = posVec.x;
		posLastAdded.y = posVec.y;
        
        bPointAdded = true;
	}
	else
	{
        bPointAdded = false;
        
		return;
	}
	
	if( bUseImageColour )
	{
		ofColor c = pfImage->getColourAt( posInImg );
		
		currentColor.r += ( c.r - currentColor.r ) * colorEase;
		currentColor.g += ( c.g - currentColor.g ) * colorEase;
		currentColor.b += ( c.b - currentColor.b ) * colorEase;
		currentColor.a += ( c.a - currentColor.a ) * colorEase;
        
        currentColor.r = MAX( currentColor.r, 50 );
        currentColor.g = MAX( currentColor.g, 50 );
        currentColor.b = MAX( currentColor.b, 50 );
        currentColor.a = MAX( currentColor.a, 50 );
	}
	else
	{
		currentColor.r = lineColor.r;
		currentColor.g = lineColor.g;
		currentColor.b = lineColor.b;
		currentColor.a = lineColor.a;
	}

	addToLineVertexArray( posVec, currentColor );
	addToStrip();
    
    addToLineNormals();
    addToLineLengths();
    addToRibbonType();
}

void Particle :: wander()
{
	wanderTheta += ofRandom( -wanderChange, wanderChange );     // Randomly change wander theta
	
	wanderCircle = wanderVel;			// Start with velocity
	wanderCircle.normalize();			// Normalize to get heading
	wanderCircle *= wanderDistance;		// Multiply by distance
	wanderCircle += posVec;				// Make it relative to boid's locationx
	
	bool isInside;
	isInside = checkIsInsideBounds( wanderCircle );
	if( !isInside )
		constrainToBounds( wanderCircle );
	
	wanderEaseTarget = isInside ? 0.2 : 1.0;
	wanderEase		+= ( wanderEaseTarget - wanderEase ) * 0.3;
	
	wanderCircleOffSet	= ofxVec2f( wanderRadius * cos( wanderTheta ), wanderRadius * sin( wanderTheta ) );
	wanderCircleTarget	= wanderCircle + wanderCircleOffSet;
	
	ofxVec2f steerVec;
	steerVec		= wanderCircleTarget - posVec;		// A vector pointing from the location to the target
	steerVec.normalize();
	steerVec *= wanderMaxSpeed;
	
	wanderVel.x += ( steerVec.x - wanderVel.x ) * wanderEase;
	wanderVel.y += ( steerVec.y - wanderVel.y ) * wanderEase;
}

bool Particle :: checkIsInsideBounds ( const ofxVec2f& target )
{
	bool l = target.x >= bounds.x;
	bool t = target.y >= bounds.y;
	bool r = target.x < bounds.x + bounds.width;
	bool b = target.y < bounds.y + bounds.height;
	
	return ( l && t && r && b );
}

void Particle :: constrainToBounds ( const ofxVec2f& target )		// target back to center of bounds.
{
	float cx = bounds.x + bounds.width  * 0.5;
	float cy = bounds.y + bounds.height * 0.5;
	
	float px = target.x - cx;
	float py = target.y - cy;
	
	ofxVec2f vec = ofxVec2f( px, py );
	float ang = vec.angle( ofxVec2f( 0, -1 ) );							// return an angle between -180 and 180.
	
	wanderTheta = ( ( ang + 180 ) / 360.0 ) * TWO_PI + PI * 0.5;		// circle starts at 12 oclock and moves clock wise.
	wanderTheta *= -1;
}

//////////////////////////////////////////////////
//	VERTEX ARRAYS.
//////////////////////////////////////////////////

void Particle :: addToStrip ()
{
	ofxVec2f v1 = posVec - posPrevVec;
	v1.perpendicular();
	v1.normalize();
	v1 *= stripWidth;
	
	stripPoint0.x = posVec.x + v1.x;
	stripPoint0.y = posVec.y + v1.y;
	stripPoint0.z = 0;
	
	stripPoint1.x = posVec.x - v1.x;
	stripPoint1.y = posVec.y - v1.y;
	stripPoint1.z = 0;
	
	addToStripVertexArray( stripPoint0, stripPoint1, currentColor, currentColor );
}

void Particle :: addToLineVertexArray ( const ofPoint& p, const ofColor& c )
{
	if( line_ind_total >= line_ind_max )
	{
		if( bVerbose )
		{
			cout << "particle " << pid << ", PARTICLE_MAX_LENGTH exceeded.";
		}
		return;
	}
	
	int i = line_ind_total * 3;
	int j = line_ind_total * 4;
	
	line_ver_array[ i + 0 ] = p.x;
	line_ver_array[ i + 1 ] = p.y;
	line_ver_array[ i + 2 ] = 0;
	
	line_col_array[ j + 0 ] = c.r / 255.0;
	line_col_array[ j + 1 ] = c.g / 255.0;
	line_col_array[ j + 2 ] = c.b / 255.0;
	line_col_array[ j + 3 ] = c.a / 255.0;
	
	line_ind_total += 1;
}

void Particle :: addToStripVertexArray ( const ofPoint& p1, const ofPoint& p2, const ofColor& c1, const ofColor& c2 )
{
	if( strip_ind_total >= strip_ind_max )
	{
		if( bVerbose )
		{
			cout << "particle " << pid << ", PARTICLE_MAX_LENGTH exceeded.";
		}
		
		return;
	}
	
	int i = strip_ind_total * 3;
	int j = strip_ind_total * 4;
	
	strip_ver_array[ i + 0 ] = p1.x;
	strip_ver_array[ i + 1 ] = p1.y;
	strip_ver_array[ i + 2 ] = 0;
	strip_ver_array[ i + 3 ] = p2.x;
	strip_ver_array[ i + 4 ] = p2.y;
	strip_ver_array[ i + 5 ] = 0;
	
	strip_col_array[ j + 0 ] = c1.r / 255.0;
	strip_col_array[ j + 1 ] = c1.g / 255.0;
	strip_col_array[ j + 2 ] = c1.b / 255.0;
	strip_col_array[ j + 3 ] = c1.a / 255.0;
	strip_col_array[ j + 4 ] = c2.r / 255.0;
	strip_col_array[ j + 5 ] = c2.g / 255.0;
	strip_col_array[ j + 6 ] = c2.b / 255.0;
	strip_col_array[ j + 7 ] = c2.a / 255.0;
	
	strip_ind_total += 2;
}

void Particle :: addToLineNormals ()
{
    ofxVec2f q;
    ofxVec2f p;
    ofxVec2f v;
    int i, j;
    
    //--- left to right.
    
    i = line_ind_total - 1;
    j = line_ind_total - 2;

    q.set( line_ver_array[ i * 3 + 0 ], line_ver_array[ i * 3 + 1 ] );
    p.set( line_ver_array[ j * 3 + 0 ], line_ver_array[ j * 3 + 1 ] );
    v = q - p;
    v.perpendicular();
    v.normalize();
    v *= -1;            // reversed so its always pointing up.
    
    line_normals[ i * 3 + 0 ] = v.x;
    line_normals[ i * 3 + 1 ] = v.y;
    line_normals[ i * 3 + 2 ] = 0;

    //--- right to left.
    
    i = line_ind_total - 2;
    j = line_ind_total - 1;
    
    q.set( line_ver_array[ i * 3 + 0 ], line_ver_array[ i * 3 + 1 ] );
    p.set( line_ver_array[ j * 3 + 0 ], line_ver_array[ j * 3 + 1 ] );
    v = q - p;
    v.perpendicular();
    v.normalize();
    v *= 1;             // doesn't need to be reversed as its already pointing up.
    
    //--- average with previous.
    
    v.x += line_normals[ i * 3 + 0 ];
    v.y += line_normals[ i * 3 + 1 ];
    v.normalize();
    
    line_normals[ i * 3 + 0 ] = v.x;
    line_normals[ i * 3 + 1 ] = v.y;
    line_normals[ i * 3 + 2 ] = 0;
}

void Particle :: addToLineLengths ()
{
    int i = line_ind_total - 1;
    int j = line_ind_total - 2;
    
    line_lengths[ i ] = line_lengths[ j ] + distMoved;
}

void Particle :: setLineColor ( const ofColor& c )
{
	if( bUseImageColour )
		return;
	
	bool b1 = lineColor.r == c.r;
	bool b2 = lineColor.g == c.g;
	bool b3 = lineColor.b == c.b;
	bool b4 = lineColor.a == c.a;
	
	if( b1 && b2 && b3 && b4 )
		return;
	
	lineColor.r = c.r;
	lineColor.g = c.g;
	lineColor.b = c.b;
	lineColor.a = c.a;
	
	for( int i=0; i<line_ind_total; i++ )
	{
		line_col_array[ i * 4 + 0 ] = c.r / 255.0;
		line_col_array[ i * 4 + 1 ] = c.g / 255.0;
		line_col_array[ i * 4 + 2 ] = c.b / 255.0;
		line_col_array[ i * 4 + 3 ] = c.a / 255.0;
	}
	
	for( int i=0; i<strip_ind_total; i++ )
	{
		strip_col_array[ i * 4 + 0 ] = c.r / 255.0;
		strip_col_array[ i * 4 + 1 ] = c.g / 255.0;
		strip_col_array[ i * 4 + 2 ] = c.b / 255.0;
		strip_col_array[ i * 4 + 3 ] = c.a / 255.0;
	}
}

void Particle :: setLineAlpha ( float alpha )
{
	if( bUseImageColour )
		return;
	
	int a = alpha * 255;
	
	if( lineColor.a == a )
		return;
	
	lineColor.a = a;
	
	for( int i=0; i<line_ind_total; i++ )
	{
		line_col_array[ i * 4 + 3 ] = alpha;
	}
	
	for( int i=0; i<strip_ind_total; i++ )
	{
		strip_col_array[ i * 4 + 3 ] = alpha;
	}
}

//////////////////////////////////////////////////
//	RIBBON TYPE
//////////////////////////////////////////////////

void Particle :: resetRibbonType ()
{
    for( int i=0; i<ribbonLetters.size(); i++ )
        delete ribbonLetters[ i ];
    ribbonLetters.clear();
}

void Particle :: addToRibbonType ()
{
    if( !ribbonType )
        return;
    
    ribbonType->setCopy( ribbonCopy );
    ribbonType->setRibbon( line_ver_array, line_ind_total, line_normals, line_col_array, line_lengths );
    ribbonType->setFontScale( ribbonFontScale * ribbonFontScaleVar );
    
    ribbonLetters = ribbonType->generateTypeOnRibbon( ribbonPositionX, ribbonCopyIndex );
}

//////////////////////////////////////////////////
//	DRAW
//////////////////////////////////////////////////

void Particle :: drawHead ()
{
	ofSetColor( headColor.r, headColor.g, headColor.b, headColor.a );
	
	if( bMarkAsTestParticle )
	{
		glColor4f( 0.0, 1.0, 0.0, 1.0 );
	}
	
	int x = posVec.x;
	int y = posVec.y;

	ofRect( x, y, 1, 1 );
//	ofRect( posVec.x - sizeHalf, posVec.y - sizeHalf, size, size );
}

void Particle :: drawLine ()
{
	glEnableClientState( GL_COLOR_ARRAY );
	glColorPointer( 4, GL_FLOAT, 0, line_col_array );
	
	glEnableClientState( GL_VERTEX_ARRAY );		
	glVertexPointer( 3, GL_FLOAT, 0, line_ver_array );
	
	glDrawArrays( GL_LINE_STRIP, 0, line_ind_total );
	
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );
}

void Particle :: drawLineToFBO ()
{
    if( !bPointAdded )
        return;
    
    if( line_ind_total < 2 )
        return;
    
    int t = 2;
    int i = line_ind_total - t;
    
	glEnableClientState( GL_COLOR_ARRAY );
	glColorPointer( 4, GL_FLOAT, 0, &line_col_array[ i * 4 ] );
	
	glEnableClientState( GL_VERTEX_ARRAY );		
	glVertexPointer( 3, GL_FLOAT, 0, &line_ver_array[ i * 3 ] );
	
	glDrawArrays( GL_LINES, 0, t );
	
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );
}

void Particle :: drawStrip ()
{
	glEnableClientState( GL_COLOR_ARRAY );
	glColorPointer( 4, GL_FLOAT, 0, strip_col_array );
	
	glEnableClientState( GL_VERTEX_ARRAY );		
	glVertexPointer( 3, GL_FLOAT, 0, strip_ver_array );
	
	glDrawArrays( GL_QUAD_STRIP, 0, strip_ind_total );
	
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );
}

void Particle :: drawStripToFBO ()
{
    if( !bPointAdded )
        return;
    
    if( strip_ind_total < 4 )
        return;
    
    int t = 4;
    int i = strip_ind_total - t;
    
	glEnableClientState( GL_COLOR_ARRAY );
	glColorPointer( 4, GL_FLOAT, 0, &strip_col_array[ i * 4 ] );
	
	glEnableClientState( GL_VERTEX_ARRAY );		
	glVertexPointer( 3, GL_FLOAT, 0, &strip_ver_array[ i * 3 ] );
	
	glDrawArrays( GL_QUAD_STRIP, 0, t );
	
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );
}

void Particle :: drawTrace ()
{
	bool b1 = ( posVec.x == posPrevVec.x );
	bool b2 = ( posVec.y == posPrevVec.y );
	
	if( b1 && b2  )
		return;
	
	glColor4f
	( 
		currentColor.r / 255.0,
		currentColor.g / 255.0,
		currentColor.b / 255.0,
		currentColor.a / 255.0
	);
	
	glColor4f( 1.0, 1.0, 1.0, traceAlpha );
	
	int x = posVec.x;
	int y = posVec.y;
	
	ofxVec2f dir;
	dir = velVec.getNormalized();
	
	ofFill();
//	ofRect( x, y, 1, 1 );
//	ofLine( posPrevVec.x, posPrevVec.y, posVec.x, posVec.y );
	
	// adding a unit dir vector so line is drawn to point, otherwise it falls short.
	// not sure why, but think its the way lines are drawn to fbo.
	
	ofLine( posPrevVec.x, posPrevVec.y, posVec.x + dir.x, posVec.y + dir.y );
}


void Particle :: drawType ()
{
	glColor4f
	( 
        currentColor.r / 255.0,
        currentColor.g / 255.0,
        currentColor.b / 255.0,
        0.7
    );
    
    for( int i=0; i<ribbonLetters.size(); i++ )
    {
        ribbonLetters[ i ]->drawFill();
        ribbonLetters[ i ]->drawOutline( true );
    }
}