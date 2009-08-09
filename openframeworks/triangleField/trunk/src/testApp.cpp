#include "testApp.h"

//////////////////////////////////////////////
//	SETUP.
//////////////////////////////////////////////

void testApp :: setup()
{
	ofSetFrameRate( 30 );
	ofSetVerticalSync( true );
	ofSetBackgroundAuto( true );
	ofEnableSmoothing();
	ofEnableAlphaBlending();
	
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	
	tileSaver.init( 10, 0, true );
	
	initColor();
	initFields();
	initDebug();
}

void testApp :: initDebug()
{
	dbInc      = 1;
	frameCount = 0;
}

//////////////////////////////////////////////
//	COLORS.
//////////////////////////////////////////////

void testApp :: initColor()
{
	colorsMax   = 100;
	colorsTotal = 0;
	colors = new float *[ colorsMax ];
	for( int i=0; i<colorsMax; i++ )
	{
		colors[ i ] = new float[ 4 ];
	}
	
	addColor(   0,   0,   0, 255 );		// black.
	addColor(  25, 191,  71, 255 );		// becks green.
	addColor( 210,   2, 169, 255 );		// fuschia.
	addColor( 213, 213,   0, 255 );		// yellow.
	addColor( 168, 255,   0, 255 );		// lime.
	addColor( 255, 132,   9, 255 );		// orange.
	addColor( 156,  56,  56, 255 );		// redish.
}

void testApp :: addColor( int r, int g, int b, int a )
{
	colors[ colorsTotal ][ 0 ] = r;
	colors[ colorsTotal ][ 1 ] = g;
	colors[ colorsTotal ][ 2 ] = b;
	colors[ colorsTotal ][ 3 ] = a;
	colorsTotal += 1;
}

//////////////////////////////////////////////
//	FIELDS.
//////////////////////////////////////////////

void testApp :: initFields()
{
	fieldIndex  = 0;
	fieldsTotal = 4;
	fields = new TriangleField[ fieldsTotal ];
	
	for( int i=0; i<fieldsTotal; i++ )
	{
		fields[ i ].scale  = 1;
		fields[ i ].cutoff = 1;
		fields[ i ].noiseX = 0;
		fields[ i ].noiseY = 0;
		fields[ i ].noiseZ = 0;
		fields[ i ].noiseXRes = 0.003;
		fields[ i ].noiseYRes = 0.003;
		fields[ i ].noiseZRes = 0.003;
		fields[ i ].noiseXVel = 0;
		fields[ i ].noiseYVel = 0;
		fields[ i ].noiseZVel = 0;
	}
	
	fields[ 0 ].sColor    = &colors[ 0 ][ 0 ];
	fields[ 0 ].eColor    = &colors[ 1 ][ 0 ];
	fields[ 0 ].scale     = 16;
	fields[ 0 ].cutoff    = 1.0;
	fields[ 0 ].noiseXVel = 0.01;
	fields[ 0 ].noiseYVel = 0.01;

	fields[ 1 ].sColor    = &colors[ 0 ][ 0 ];
	fields[ 1 ].eColor    = &colors[ 2 ][ 0 ];
	fields[ 1 ].scale     = 8;
	fields[ 1 ].cutoff    = 0.4;
	fields[ 1 ].noiseXVel = 0.02;
	fields[ 1 ].noiseYVel = 0.02;

	fields[ 2 ].sColor    = &colors[ 0 ][ 0 ];
	fields[ 2 ].eColor    = &colors[ 1 ][ 0 ];
	fields[ 2 ].scale     = 4;
	fields[ 2 ].cutoff    = 0.5;
	fields[ 2 ].noiseXVel = 0.03;
	fields[ 2 ].noiseYVel = 0.03;
	
	fields[ 3 ].sColor    = &colors[ 0 ][ 0 ];
	fields[ 3 ].eColor    = &colors[ 2 ][ 0 ];
	fields[ 3 ].scale     = 2;
	fields[ 3 ].cutoff    = 0.3;
	fields[ 3 ].noiseXVel = 0.04;
	fields[ 3 ].noiseYVel = 0.04;
}

//////////////////////////////////////////////
//	UPDATE.
//////////////////////////////////////////////

void testApp :: update()
{
	if( tileSaver.bGoTiling )
		return;

	++frameCount;
}

//////////////////////////////////////////////
//	DRAW.
//////////////////////////////////////////////

void testApp :: draw()
{
	ofBackground( 0, 0, 0 );
	
	glDepthMask( GL_FALSE );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );
	
	tileSaver.begin();
	
//	drawSquareNoise();
//	drawTraingleStatic();

	for( int i=0; i<fieldsTotal; i++ )
		drawTriangleNoise( &fields[ i ] );
	
	tileSaver.end();
	
	if( screenGrabUtil.isRecording() )
	{
		screenGrabUtil.save();
	}
	
	glDepthMask( GL_TRUE );
	glBlendFunc( GL_ONE, GL_ZERO );

	drawDebug();
}

void testApp :: drawSquareNoise ( TriangleField *field )
{
	float size = 40.0;
	
	int cy = ceil( ofGetHeight() / (float)size );
	int cx = ceil( ofGetWidth() / (float)size );
	
	field->noiseX = frameCount * field->noiseXVel;
	field->noiseY = frameCount * field->noiseYVel;
	field->noiseZ = frameCount * field->noiseZVel;
	
	int c;
	float n;
	
	for( int y=0; y<cy; y++ )
	{
		field->noiseY += field->noiseYRes * size;
		
		for( int x=0; x<cx; x++ )
		{
			field->noiseX += field->noiseXRes * size;
			
			n = ( noise.noise( field->noiseX, field->noiseY, field->noiseZ ) + 1.0 ) * 0.5;
			c = (int)( n * 255 );
			
			ofSetColor( c, c, c );
			ofFill();
			ofRect( x * size, y * size, size, size );
			ofNoFill();
			ofRect( x * size, y * size, size, size );
		}
	}
}

void testApp :: drawTraingleStatic ()
{
	float size = 40.0;
	
	int cy = ceil( ofGetHeight() / (float)size );
	int cx = ceil( ofGetWidth() / (float)size );
	
	int c;
	
	for( int y=0; y<cy; y++ )
	{
		for( int x=0; x<cx; x++ )
		{
			int orientation = 1;
			if( x % 2 == 1 )
				orientation *= -1;
			
			if( y % 2 == 1 )
				orientation *= -1;
			
			if( orientation == 1 )
			{
				c = (int)( ofRandom( 10, 240 ) );
				ofSetColor( c, c, c );
				ofTriangle( x * size, y * size, x * size, ( y + 1 ) * size, ( x + 1 ) * size, ( y + 1 ) * size );
				ofNoFill();
				ofTriangle( x * size, y * size, x * size, ( y + 1 ) * size, ( x + 1 ) * size, ( y + 1 ) * size );
				
				c = (int)( ofRandom( 10, 240 ) );
				ofSetColor( c, c, c );
				ofFill();
				ofTriangle( x * size, y * size, ( x + 1 ) * size, y * size, ( x + 1 ) * size, ( y + 1 ) * size );
				ofNoFill();
				ofTriangle( x * size, y * size, ( x + 1 ) * size, y * size, ( x + 1 ) * size, ( y + 1 ) * size );
			}
			else
			{
				c = (int)( ofRandom( 10, 240 ) );
				ofSetColor( c, c, c );
				ofFill();
				ofTriangle( x * size, y * size, ( x + 1 ) * size, y * size, x * size, ( y + 1 ) * size );
				ofNoFill();
				ofTriangle( x * size, y * size, ( x + 1 ) * size, y * size, x * size, ( y + 1 ) * size );
				
				c = (int)( ofRandom( 10, 240 ) );
				ofSetColor( c, c, c );
				ofFill();
				ofTriangle( x * size, ( y + 1 ) * size, ( x + 1 ) * size, ( y + 1 ) * size, ( x + 1 ) * size, y * size );
				ofNoFill();
				ofTriangle( x * size, ( y + 1 ) * size, ( x + 1 ) * size, ( y + 1 ) * size, ( x + 1 ) * size, y * size );
			}
		}
	}
}

void testApp :: drawTriangleNoise ( TriangleField *field )
{
	float size = 10.0 * field->scale;
	
	int cy = ceil( ofGetHeight() / (float)size );
	int cx = ceil( ofGetWidth() / (float)size );
	
	field->noiseX = frameCount * field->noiseXVel;
	field->noiseY = frameCount * field->noiseYVel;
	field->noiseZ = frameCount * field->noiseZVel;
	
	int cr, cg, cb, ca;
	float n, nx, ny;
	
	for( int y=0; y<cy; y++ )
	{
		field->noiseY += field->noiseYRes * size;
		
		for( int x=0; x<cx; x++ )
		{
			field->noiseX += field->noiseXRes * size;
			
			int orientation = 1;
			if( x % 2 == 1 )
				orientation *= -1;
			
			if( y % 2 == 1 )
				orientation *= -1;
			
			if( orientation == 1 )
			{
				nx = field->noiseX +  0.25 * size * field->noiseXRes;
				ny = field->noiseY +  0.75 * size * field->noiseYRes;
				n  = ( noise.noise( nx, ny, field->noiseZ ) + 1.0 ) * 0.5;
				
				if( n < field->cutoff )
				{
					n  /= field->cutoff;
					cr = (int)( n * ( field->eColor[ 0 ] - field->sColor[ 0 ] ) + field->sColor[ 0 ] );
					cg = (int)( n * ( field->eColor[ 1 ] - field->sColor[ 1 ] ) + field->sColor[ 1 ] );
					cb = (int)( n * ( field->eColor[ 2 ] - field->sColor[ 2 ] ) + field->sColor[ 2 ] );
					ca = (int)( n * ( field->eColor[ 3 ] - field->sColor[ 3 ] ) + field->sColor[ 3 ] );
				
					ofSetColor( cr, cg, cb, ca );
					ofFill();
					ofTriangle( x * size, y * size, x * size, ( y + 1 ) * size, ( x + 1 ) * size, ( y + 1 ) * size );
					ofNoFill();
					ofTriangle( x * size, y * size, x * size, ( y + 1 ) * size, ( x + 1 ) * size, ( y + 1 ) * size );
				}
				
				nx = field->noiseX +  0.75 * size * field->noiseXRes;
				ny = field->noiseY +  0.25 * size * field->noiseYRes;
				n  = ( noise.noise( nx, ny, field->noiseZ ) + 1.0 ) * 0.5;

				if( n < field->cutoff )
				{
					n  /= field->cutoff;
					cr = (int)( n * ( field->eColor[ 0 ] - field->sColor[ 0 ] ) + field->sColor[ 0 ] );
					cg = (int)( n * ( field->eColor[ 1 ] - field->sColor[ 1 ] ) + field->sColor[ 1 ] );
					cb = (int)( n * ( field->eColor[ 2 ] - field->sColor[ 2 ] ) + field->sColor[ 2 ] );
					ca = (int)( n * ( field->eColor[ 3 ] - field->sColor[ 3 ] ) + field->sColor[ 3 ] );
					
					ofSetColor( cr, cg, cb, ca );
					ofFill();
					ofTriangle( x * size, y * size, ( x + 1 ) * size, y * size, ( x + 1 ) * size, ( y + 1 ) * size );
					ofNoFill();
					ofTriangle( x * size, y * size, ( x + 1 ) * size, y * size, ( x + 1 ) * size, ( y + 1 ) * size );
				}
			}
			else
			{
				nx = field->noiseX +  0.25 * size * field->noiseXRes;
				ny = field->noiseY +  0.25 * size * field->noiseYRes;
				n  = ( noise.noise( nx, ny, field->noiseZ ) + 1.0 ) * 0.5;

				if( n < field->cutoff )
				{
					n  /= field->cutoff;
					cr = (int)( n * ( field->eColor[ 0 ] - field->sColor[ 0 ] ) + field->sColor[ 0 ] );
					cg = (int)( n * ( field->eColor[ 1 ] - field->sColor[ 1 ] ) + field->sColor[ 1 ] );
					cb = (int)( n * ( field->eColor[ 2 ] - field->sColor[ 2 ] ) + field->sColor[ 2 ] );
					ca = (int)( n * ( field->eColor[ 3 ] - field->sColor[ 3 ] ) + field->sColor[ 3 ] );
					
					ofSetColor( cr, cg, cb, ca );
					ofFill();
					ofTriangle( x * size, y * size, ( x + 1 ) * size, y * size, x * size, ( y + 1 ) * size );
					ofNoFill();
					ofTriangle( x * size, y * size, ( x + 1 ) * size, y * size, x * size, ( y + 1 ) * size );
				}
				
				nx = field->noiseX +  0.75 * size * field->noiseXRes;
				ny = field->noiseY +  0.75 * size * field->noiseYRes;
				n  = ( noise.noise( nx, ny, field->noiseZ ) + 1.0 ) * 0.5;
				
				if( n < field->cutoff )
				{
					n  /= field->cutoff;
					cr = (int)( n * ( field->eColor[ 0 ] - field->sColor[ 0 ] ) + field->sColor[ 0 ] );
					cg = (int)( n * ( field->eColor[ 1 ] - field->sColor[ 1 ] ) + field->sColor[ 1 ] );
					cb = (int)( n * ( field->eColor[ 2 ] - field->sColor[ 2 ] ) + field->sColor[ 2 ] );
					ca = (int)( n * ( field->eColor[ 3 ] - field->sColor[ 3 ] ) + field->sColor[ 3 ] );
					
					ofSetColor( cr, cg, cb, ca );
					ofFill();
					ofTriangle( x * size, ( y + 1 ) * size, ( x + 1 ) * size, ( y + 1 ) * size, ( x + 1 ) * size, y * size );
					ofNoFill();
					ofTriangle( x * size, ( y + 1 ) * size, ( x + 1 ) * size, ( y + 1 ) * size, ( x + 1 ) * size, y * size );
				}
			}
		}
	}
}

void testApp :: drawDebug()
{
	ofSetColor( 0xFFFFFF );
	ofDrawBitmapString
	(
		"fps        ::                     :: " + ofToString( ofGetFrameRate(), 2 ) + "\n\n" +
		"fieldIndex :: press 1, 2 or 3     :: " + ofToString( fieldIndex, 0 ) + "\n\n" +
		"noiseRes   :: press q to change   :: " + ofToString( fields[ fieldIndex ].noiseXRes, 5 ) + "\n\n" +
		"noiseXVel  :: press w to change   :: " + ofToString( fields[ fieldIndex ].noiseXVel, 5 ) + "\n\n" +
		"noiseYVel  :: press e to change   :: " + ofToString( fields[ fieldIndex ].noiseYVel, 5 ) + "\n\n" +
		"noiseZVel  :: press r to change   :: " + ofToString( fields[ fieldIndex ].noiseZVel, 5 ) + "\n\n" +
		"dbInc      :: press -/+ to change :: " + ofToString( dbInc, 5 ) + "\n\n",
		20,
		20
	);
}

//////////////////////////////////////////////
//	HANDLERS.
//////////////////////////////////////////////

void testApp :: keyPressed( int key )
{
	if( key == '-' )
		dbInc = -1;
	
	if( key == '=' )
		dbInc = 1;
	
	if( key == '1' )
		fieldIndex = 0;

	if( key == '2' )
		fieldIndex = 1;
	
	if( key == '3' )
		fieldIndex = 2;
	
	if( key == 'q' )
	{
		fields[ fieldIndex ].noiseXRes += 0.001 * dbInc;
		fields[ fieldIndex ].noiseYRes += 0.001 * dbInc;
		fields[ fieldIndex ].noiseZRes += 0.001 * dbInc;
	}
	
	if( key == 'w' )
		fields[ fieldIndex ].noiseXVel += 0.01 * dbInc;
	
	if( key == 'e' )
		fields[ fieldIndex ].noiseYVel += 0.01 * dbInc;
	
	if( key == 'r' )
		fields[ fieldIndex ].noiseZVel += 0.01 * dbInc;
}

void testApp :: keyReleased( int key )
{
	if( key == 's' )
	{
		char str[255];
		sprintf( str, "tf_%02d%02d%02d_%02d%02d%02d.png", ofGetYear() % 1000, ofGetMonth(), ofGetDay(), ofGetHours(), ofGetMinutes(), ofGetSeconds() );
		
		tileSaver.finish( str, false );
	}
	
	if( key == 'm' )
	{
		if( screenGrabUtil.isRecording() )
		{
			screenGrabUtil.stop();
		}
		else
		{
			screenGrabUtil.start( "movie/trigfield" );
		}
	}
}

void testApp :: mouseMoved( int x, int y )
{

}

void testApp :: mouseDragged( int x, int y, int button )
{

}

void testApp :: mousePressed( int x, int y, int button )
{

}

void testApp :: mouseReleased( int x, int y, int button )
{

}

void testApp :: windowResized( int w, int h )
{

}

