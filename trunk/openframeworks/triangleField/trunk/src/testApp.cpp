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
	
	screenWidth  = ofGetWidth();
	screenHeight = ofGetHeight();
	
	tileSaver.init( 10, 0, true );
	
	initColor();
	initFields();
	initDebug();
}

void testApp :: initDebug()
{
	dbInc      = 1;
	frameCount = 0;
	showDebug  = true;
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
	
	addColor(   0,   0,   0, 255 );		// black alpha.
	addColor(   0,   0,   0,   0 );		// black fill.
	addColor(  25, 191,  71, 255 );		// becks green.
	addColor(   7,  52,  16, 180 );		// dark green.
	addColor( 210,   2, 169, 255 );		// fuschia.
	addColor( 210,   2, 169,  50 );		// fuschia alpha.
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
		fields[ i ].scale    = 1;
		fields[ i ].scaleInc = 1;
		fields[ i ].cutoff   = 1.0;
		
		fields[ i ].drawOutline = true;
		
		fields[ i ].noiseX = 0.0;
		fields[ i ].noiseY = 0.0;
		fields[ i ].noiseZ = 0.0;
		
		fields[ i ].noiseXInit = 0.0;
		fields[ i ].noiseYInit = 0.0;
		fields[ i ].noiseZInit = 0.0;
		
		fields[ i ].noiseXRes = 0.01;
		fields[ i ].noiseYRes = 0.01;
		fields[ i ].noiseZRes = 0.01;
		
		fields[ i ].noiseXVel = 0.0;
		fields[ i ].noiseYVel = 0.0;
		fields[ i ].noiseZVel = 0.005;
		
		fields[ i ].noiseXScl = screenHeight / (float)screenWidth;
		fields[ i ].noiseYScl = 1.0;
	}
	
	fields[ 0 ].sColor    = &colors[ 0 ][ 0 ];
	fields[ 0 ].eColor    = &colors[ 2 ][ 0 ];
	fields[ 0 ].bsColor   = fields[ 0 ].sColor;
	fields[ 0 ].beColor   = fields[ 0 ].eColor;
	fields[ 0 ].scaleInc  = 4;
	fields[ 0 ].scale     = pow( 2, fields[ 0 ].scaleInc );
	fields[ 0 ].cutoff    = 0.0;
	fields[ 0 ].noiseXVel = 0.0;
	fields[ 0 ].noiseYVel = 0.01;

	fields[ 1 ].sColor    = &colors[ 0 ][ 0 ];
	fields[ 1 ].eColor    = &colors[ 4 ][ 0 ];
	fields[ 1 ].bsColor   = fields[ 1 ].sColor;
	fields[ 1 ].beColor   = fields[ 1 ].eColor;
	fields[ 1 ].scaleInc  = 3;
	fields[ 1 ].scale     = pow( 2, fields[ 1 ].scaleInc );
	fields[ 1 ].cutoff    = 0.6;
	fields[ 1 ].noiseXVel = 0.0;
	fields[ 1 ].noiseYVel = -0.01;

	fields[ 2 ].sColor    = &colors[ 1 ][ 0 ];
	fields[ 2 ].eColor    = &colors[ 2 ][ 0 ];
	fields[ 2 ].bsColor   = fields[ 2 ].sColor;
	fields[ 2 ].beColor   = fields[ 2 ].eColor;
	fields[ 2 ].scaleInc  = 2;
	fields[ 2 ].scale     = pow( 2, fields[ 2 ].scaleInc );
	fields[ 2 ].cutoff    = 0.4;
	fields[ 2 ].noiseXVel = 0.0;
	fields[ 2 ].noiseYVel = 0.02;
	
	fields[ 3 ].sColor    = &colors[ 5 ][ 0 ];
	fields[ 3 ].eColor    = &colors[ 4 ][ 0 ];
	fields[ 3 ].bsColor   = fields[ 3 ].sColor;
	fields[ 3 ].beColor   = fields[ 3 ].eColor;
	fields[ 3 ].scaleInc  = 1;
	fields[ 3 ].scale     = pow( 2, fields[ 3 ].scaleInc );
	fields[ 3 ].cutoff    = 0.6;
	fields[ 3 ].noiseXVel = 0.015;
	fields[ 3 ].noiseYVel = 0.015;

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
	ofEnableAlphaBlending();
	
//	glBlendFunc( GL_ONE, GL_ONE );
//	glBlendFunc( GL_ONE, GL_ONE_MINUS_DST_ALPHA );
//	glBlendFunc( GL_SRC_ALPHA, GL_ONE );
//	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	
	tileSaver.begin();
	
//	drawSquareNoise( &fields[ 0 ] );
//	drawTraingleStatic();
//	drawTriangleNoise( &fields[ 0 ] );

	for( int i=0; i<fieldsTotal; i++ )
		drawTriangleNoise( &fields[ i ] );
	
	tileSaver.end();
	
	if( screenGrabUtil.isRecording() )
	{
		screenGrabUtil.save();
	}
	
	glBlendFunc( GL_ONE, GL_ZERO );

	if( showDebug )
		drawDebug();
}

void testApp :: drawSquareNoise ( TriangleField *field )
{
	float size = 10.0 * field->scale;
	
	int cy = ceil( screenHeight / size );
	int cx = ceil( screenWidth / size );
	
	field->noiseXInit = frameCount * field->noiseXVel;
	field->noiseYInit = frameCount * field->noiseYVel;
	field->noiseZInit = frameCount * field->noiseZVel;
	
	field->noiseZ = field->noiseZInit;
	
	int cr, cg, cb, ca;
	float n, nx, ny;
	
	for( int y=0; y<cy; y++ )
	{
		field->noiseY = field->noiseYInit + y * size * field->noiseYRes * field->noiseYScl;
		
		for( int x=0; x<cx; x++ )
		{
			field->noiseX = field->noiseXInit + x * size * field->noiseXRes * field->noiseXScl;

			n = ( noise.noise( field->noiseX, field->noiseY, field->noiseZ ) + 1.0 ) * 0.5;
			
			if( n < field->cutoff )
			{
				n  /= field->cutoff;
				cr = (int)( n * ( field->eColor[ 0 ] - field->sColor[ 0 ] ) + field->sColor[ 0 ] );
				cg = (int)( n * ( field->eColor[ 1 ] - field->sColor[ 1 ] ) + field->sColor[ 1 ] );
				cb = (int)( n * ( field->eColor[ 2 ] - field->sColor[ 2 ] ) + field->sColor[ 2 ] );
				ca = (int)( n * ( field->eColor[ 3 ] - field->sColor[ 3 ] ) + field->sColor[ 3 ] );
				
				ofSetColor( cr, cg, cb, ca );
				ofFill();
				ofRect( x * size, y * size, size, size );
				ofNoFill();
				ofRect( x * size, y * size, size, size );
			}
		}
	}
}

void testApp :: drawTraingleStatic ()
{
	float size = 40.0;
	
	int cy = ceil( screenHeight / size );
	int cx = ceil( screenWidth / size );
	
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
	
	int cy = ceil( screenHeight / size );
	int cx = ceil( screenWidth / size );
	
	field->noiseXInit = frameCount * field->noiseXVel;
	field->noiseYInit = frameCount * field->noiseYVel;
	field->noiseZInit = frameCount * field->noiseZVel;
	
	field->noiseZ = field->noiseZInit + field->noiseZRes;
	
	int cr, cg, cb, ca;
	float n, nx, ny;
	
	for( int y=0; y<cy; y++ )
	{
		field->noiseY = field->noiseYInit + y * field->noiseYRes * field->noiseYScl * size;
		
		for( int x=0; x<cx; x++ )
		{
			field->noiseX = field->noiseXInit + x * field->noiseXRes * field->noiseXScl * size;
			
			int orientation = 1;
			if( x % 2 == 1 )
				orientation *= -1;
			
			if( y % 2 == 1 )
				orientation *= -1;
			
			if( orientation == 1 )
			{
				nx = field->noiseX +  0.25 * size * field->noiseXRes * field->noiseXScl;
				ny = field->noiseY +  0.75 * size * field->noiseYRes * field->noiseYScl;
				n  = ( noise.noise( nx, ny, field->noiseZ ) + 1.0 ) * 0.5;
				
				if( n > field->cutoff )
				{
					n  = ( n - field->cutoff ) / (float)( 1 - field->cutoff );
					
					cr = (int)( n * ( field->eColor[ 0 ] - field->sColor[ 0 ] ) + field->sColor[ 0 ] );
					cg = (int)( n * ( field->eColor[ 1 ] - field->sColor[ 1 ] ) + field->sColor[ 1 ] );
					cb = (int)( n * ( field->eColor[ 2 ] - field->sColor[ 2 ] ) + field->sColor[ 2 ] );
					ca = (int)( n * ( field->eColor[ 3 ] - field->sColor[ 3 ] ) + field->sColor[ 3 ] );
				
					ofSetColor( cr, cg, cb, ca );
					ofFill();
					ofTriangle( x * size, y * size, x * size, ( y + 1 ) * size, ( x + 1 ) * size, ( y + 1 ) * size );
					
					if( fields->drawOutline )
					{
						cr = (int)( n * ( field->beColor[ 0 ] - field->bsColor[ 0 ] ) + field->bsColor[ 0 ] );
						cg = (int)( n * ( field->beColor[ 1 ] - field->bsColor[ 1 ] ) + field->bsColor[ 1 ] );
						cb = (int)( n * ( field->beColor[ 2 ] - field->bsColor[ 2 ] ) + field->bsColor[ 2 ] );
						ca = (int)( n * ( field->beColor[ 3 ] - field->bsColor[ 3 ] ) + field->bsColor[ 3 ] );
						
						ofSetColor( cr, cg, cb, ca );
						ofNoFill();
						ofTriangle( x * size, y * size, x * size, ( y + 1 ) * size, ( x + 1 ) * size, ( y + 1 ) * size );
					}
				}
				
				nx = field->noiseX +  0.75 * size * field->noiseXRes * field->noiseXScl;
				ny = field->noiseY +  0.25 * size * field->noiseYRes * field->noiseYScl;
				n  = ( noise.noise( nx, ny, field->noiseZ ) + 1.0 ) * 0.5;

				if( n > field->cutoff )
				{
					n  = ( n - field->cutoff ) / (float)( 1 - field->cutoff );
					
					cr = (int)( n * ( field->eColor[ 0 ] - field->sColor[ 0 ] ) + field->sColor[ 0 ] );
					cg = (int)( n * ( field->eColor[ 1 ] - field->sColor[ 1 ] ) + field->sColor[ 1 ] );
					cb = (int)( n * ( field->eColor[ 2 ] - field->sColor[ 2 ] ) + field->sColor[ 2 ] );
					ca = (int)( n * ( field->eColor[ 3 ] - field->sColor[ 3 ] ) + field->sColor[ 3 ] );
					
					ofSetColor( cr, cg, cb, ca );
					ofFill();
					ofTriangle( x * size, y * size, ( x + 1 ) * size, y * size, ( x + 1 ) * size, ( y + 1 ) * size );
					
					if( fields->drawOutline )
					{
						cr = (int)( n * ( field->beColor[ 0 ] - field->bsColor[ 0 ] ) + field->bsColor[ 0 ] );
						cg = (int)( n * ( field->beColor[ 1 ] - field->bsColor[ 1 ] ) + field->bsColor[ 1 ] );
						cb = (int)( n * ( field->beColor[ 2 ] - field->bsColor[ 2 ] ) + field->bsColor[ 2 ] );
						ca = (int)( n * ( field->beColor[ 3 ] - field->bsColor[ 3 ] ) + field->bsColor[ 3 ] );
						
						ofSetColor( cr, cg, cb, ca );
						ofNoFill();
						ofTriangle( x * size, y * size, ( x + 1 ) * size, y * size, ( x + 1 ) * size, ( y + 1 ) * size );
					}
				}
			}
			else
			{
				nx = field->noiseX +  0.25 * size * field->noiseXRes * field->noiseXScl;
				ny = field->noiseY +  0.25 * size * field->noiseYRes * field->noiseYScl;
				n  = ( noise.noise( nx, ny, field->noiseZ ) + 1.0 ) * 0.5;

				if( n > field->cutoff )
				{
					n  = ( n - field->cutoff ) / (float)( 1 - field->cutoff );
					
					cr = (int)( n * ( field->eColor[ 0 ] - field->sColor[ 0 ] ) + field->sColor[ 0 ] );
					cg = (int)( n * ( field->eColor[ 1 ] - field->sColor[ 1 ] ) + field->sColor[ 1 ] );
					cb = (int)( n * ( field->eColor[ 2 ] - field->sColor[ 2 ] ) + field->sColor[ 2 ] );
					ca = (int)( n * ( field->eColor[ 3 ] - field->sColor[ 3 ] ) + field->sColor[ 3 ] );
					
					ofSetColor( cr, cg, cb, ca );
					ofFill();
					ofTriangle( x * size, y * size, ( x + 1 ) * size, y * size, x * size, ( y + 1 ) * size );
					
					if( fields->drawOutline )
					{
						cr = (int)( n * ( field->beColor[ 0 ] - field->bsColor[ 0 ] ) + field->bsColor[ 0 ] );
						cg = (int)( n * ( field->beColor[ 1 ] - field->bsColor[ 1 ] ) + field->bsColor[ 1 ] );
						cb = (int)( n * ( field->beColor[ 2 ] - field->bsColor[ 2 ] ) + field->bsColor[ 2 ] );
						ca = (int)( n * ( field->beColor[ 3 ] - field->bsColor[ 3 ] ) + field->bsColor[ 3 ] );
						
						ofSetColor( cr, cg, cb, ca );
						ofNoFill();
						ofTriangle( x * size, y * size, ( x + 1 ) * size, y * size, x * size, ( y + 1 ) * size );
					}
				}
				
				nx = field->noiseX +  0.75 * size * field->noiseXRes * field->noiseXScl;
				ny = field->noiseY +  0.75 * size * field->noiseYRes * field->noiseYScl;
				n  = ( noise.noise( nx, ny, field->noiseZ ) + 1.0 ) * 0.5;
				
				if( n > field->cutoff )
				{
					n  = ( n - field->cutoff ) / (float)( 1 - field->cutoff );
					
					cr = (int)( n * ( field->eColor[ 0 ] - field->sColor[ 0 ] ) + field->sColor[ 0 ] );
					cg = (int)( n * ( field->eColor[ 1 ] - field->sColor[ 1 ] ) + field->sColor[ 1 ] );
					cb = (int)( n * ( field->eColor[ 2 ] - field->sColor[ 2 ] ) + field->sColor[ 2 ] );
					ca = (int)( n * ( field->eColor[ 3 ] - field->sColor[ 3 ] ) + field->sColor[ 3 ] );
					
					ofSetColor( cr, cg, cb, ca );
					ofFill();
					ofTriangle( x * size, ( y + 1 ) * size, ( x + 1 ) * size, ( y + 1 ) * size, ( x + 1 ) * size, y * size );
					
					if( fields->drawOutline )
					{
						cr = (int)( n * ( field->beColor[ 0 ] - field->bsColor[ 0 ] ) + field->bsColor[ 0 ] );
						cg = (int)( n * ( field->beColor[ 1 ] - field->bsColor[ 1 ] ) + field->bsColor[ 1 ] );
						cb = (int)( n * ( field->beColor[ 2 ] - field->bsColor[ 2 ] ) + field->bsColor[ 2 ] );
						ca = (int)( n * ( field->beColor[ 3 ] - field->bsColor[ 3 ] ) + field->bsColor[ 3 ] );
						
						ofSetColor( cr, cg, cb, ca );
						ofNoFill();
						ofTriangle( x * size, ( y + 1 ) * size, ( x + 1 ) * size, ( y + 1 ) * size, ( x + 1 ) * size, y * size );
					}
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
		"\n\n" +
		"fieldIndex :: press 1 - 9         :: " + ofToString( fieldIndex, 0 ) + "\n\n" +
		"noiseRes   :: press q to change   :: " + ofToString( fields[ fieldIndex ].noiseXRes, 5 ) + "\n\n" +
		"noiseXVel  :: press w to change   :: " + ofToString( fields[ fieldIndex ].noiseXVel, 5 ) + "\n\n" +
		"noiseYVel  :: press e to change   :: " + ofToString( fields[ fieldIndex ].noiseYVel, 5 ) + "\n\n" +
		"noiseZVel  :: press r to change   :: " + ofToString( fields[ fieldIndex ].noiseZVel, 5 ) + "\n\n" +
		"scale      :: press t to change   :: " + ofToString( fields[ fieldIndex ].scale, 5 ) + "\n\n" +
		"cutoff     :: press y to change   :: " + ofToString( fields[ fieldIndex ].cutoff, 5 ) + "\n\n" +
		"\n\n" +
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
	
	if( key == '0' )
		fieldIndex = 0;
	
	if( key == '1' )
		fieldIndex = 1;
	
	if( key == '2' )
		fieldIndex = 2;

	if( key == '3' )
		fieldIndex = 3;
	
	if( key == '4' )
		fieldIndex = 4;
	
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

	if( key == 't' )
	{
		fields[ fieldIndex ].scaleInc = MIN( 10, MAX( 0, fields[ fieldIndex ].scaleInc + dbInc ) );
		fields[ fieldIndex ].scale    = pow( 2, fields[ fieldIndex ].scaleInc );
	}
	
	if( key == 'y' )
		fields[ fieldIndex ].cutoff = MIN( 1, MAX( 0, fields[ fieldIndex ].cutoff += 0.01 * dbInc ) );

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
	
	if( key == 'd' )
		showDebug = !showDebug;
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

