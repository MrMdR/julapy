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
	
	initRenderArea();
	initColor();
	initFields();
	initDebug();
	initGui();
	
	tileSaver.init( 10, 0, true );
	screenGrabUtil.setup( "movie/trigfield", &renderArea );
}

void testApp :: initRenderArea()
{
	renderAreaWindow.x      = 0;
	renderAreaWindow.y      = 0;
	renderAreaWindow.width  = 1280;
	renderAreaWindow.height = 720;
	
	renderAreaFullScreen.x      = 1440;
	renderAreaFullScreen.y      = 0;
	renderAreaFullScreen.width  = 800;
	renderAreaFullScreen.height = 480;
	
	renderArea.x		= renderAreaWindow.x;
	renderArea.y		= renderAreaWindow.y;
	renderArea.width	= renderAreaWindow.width;
	renderArea.height	= renderAreaWindow.height;
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
	fieldsTotal = 3;
	fields = new TriangleField[ fieldsTotal ];
	
	for( int i=0; i<fieldsTotal; i++ )
	{
		fields[ i ].scale    = 1;
		fields[ i ].scaleInc = 1;
		fields[ i ].cutoff   = 1.0;
		
		fields[ i ].sColor  = new float[ 4 ];
		fields[ i ].eColor  = new float[ 4 ];
		fields[ i ].bsColor = new float[ 4 ];
		fields[ i ].beColor = new float[ 4 ];
		
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
		
		fields[ i ].noiseXScl = renderArea.height / (float)renderArea.width;
		fields[ i ].noiseYScl = 1.0;
	}
	
	fields[ 0 ].scaleInc  = 4;
	fields[ 0 ].scale     = pow( 2, fields[ 0 ].scaleInc );
	fields[ 0 ].cutoff    = 0.0;
	fields[ 0 ].noiseXVel = 0.0;
	fields[ 0 ].noiseYVel = 0.01;

	fields[ 1 ].scaleInc  = 3;
	fields[ 1 ].scale     = pow( 2, fields[ 1 ].scaleInc );
	fields[ 1 ].cutoff    = 0.6;
	fields[ 1 ].noiseXVel = 0.0;
	fields[ 1 ].noiseYVel = -0.01;

	fields[ 2 ].scaleInc  = 2;
	fields[ 2 ].scale     = pow( 2, fields[ 2 ].scaleInc );
	fields[ 2 ].cutoff    = 0.4;
	fields[ 2 ].noiseXVel = 0.0;
	fields[ 2 ].noiseYVel = 0.02;
	
//	fields[ 3 ].sColor    = &colors[ 5 ][ 0 ];
//	fields[ 3 ].eColor    = &colors[ 4 ][ 0 ];
//	fields[ 3 ].bsColor   = fields[ 3 ].sColor;
//	fields[ 3 ].beColor   = fields[ 3 ].eColor;
//	fields[ 3 ].scaleInc  = 1;
//	fields[ 3 ].scale     = pow( 2, fields[ 3 ].scaleInc );
//	fields[ 3 ].cutoff    = 0.6;
//	fields[ 3 ].noiseXVel = 0.015;
//	fields[ 3 ].noiseYVel = 0.015;
}

//////////////////////////////////////////////
//	UI.
//////////////////////////////////////////////

void testApp :: initGui()
{
//	gui.addSlider("fs.viscocity", &fluidSolver.viscocity, 0.0, 0.0002, 0.5);
//	gui.addSlider("fs.colorDiffusion", &fluidSolver.colorDiffusion, 0.0, 0.0003, 0.5); 
//	gui.addSlider("fs.fadeSpeed", &fluidSolver.fadeSpeed, 0.0, 0.1, 0.5); 
//	gui.addSlider("fs.solverIterations", &fluidSolver.solverIterations, 1, 20); 
//	gui.addSlider("fd.drawMode", &fluidDrawer.drawMode, 0, FLUID_DRAW_MODE_COUNT-1); 
//	gui.addToggle("fs.doRGB", &fluidSolver.doRGB); 
//	gui.addToggle("fs.doVorticityConfinement", &fluidSolver.doVorticityConfinement); 
//	gui.addToggle("drawFluid", &drawFluid); 
//	gui.addToggle("renderUsingVA", &renderUsingVA); 
//	
//	gui.addSlider("optical floor",		&opticalField.opticalFlowMin,	0.0f, 10.0f, 0.1f );
//	gui.addSlider("optical ceil",		&opticalField.opticalFlowMax,	0.0f, 10.0f, 0.5f );
//	gui.addSlider("optical scale",		&opticalField.opticalFlowScale, 0.0f, 0.001f, 0.1f );
//	gui.addSlider("fluid color scale",	&fluidColorScale,				0.0, 2.0, 0.5 );
	
	colorPickers = new ColorPicker[ fieldsTotal * 2 ];
	
	int colorWheelWidth  = 100;
	int colorWheelHeight = 100;
	
	int py = 430;
	int px = 30;
	int sx = 30;
	
	for( int i=0; i<fieldsTotal; i++ )
	{
		colorPickers[ i * 2 + 0 ].init( px, py, colorWheelWidth, colorWheelHeight );
		colorPickers[ i * 2 + 0 ].setMode( COLOR_PICKER_MODE_MOUSE);
		
		px += colorWheelWidth + sx;
		
		colorPickers[ i * 2 + 1 ].init( px, py, colorWheelWidth, colorWheelHeight );
		colorPickers[ i * 2 + 1 ].setMode( COLOR_PICKER_MODE_MOUSE);
		
		px += colorWheelWidth + sx + 20;
		
		gui.addSlider( "scale ",   &fields[ i ].scaleInc, 1.0, 4.0 );
		gui.addSlider( "cut off ", &fields[ i ].cutoff,   0.0, 1.0 );

		gui.addPage( "anther page" );
	}
}	

//////////////////////////////////////////////
//	UPDATE.
//////////////////////////////////////////////

void testApp :: update()
{
	if( tileSaver.bGoTiling )
		return;
	
	updateFieldColors();

	++frameCount;
}

void testApp :: updateFieldColors()
{
	for( int i=0; i<fieldsTotal; i++ )
	{
		Color sColor;
		Color eColor;
		colorPickers[ i * 2 + 0 ].getColor( &sColor );
		colorPickers[ i * 2 + 1 ].getColor( &eColor );
		
		fields[ i ].sColor[ 0 ] = sColor.r;
		fields[ i ].sColor[ 1 ] = sColor.g;
		fields[ i ].sColor[ 2 ] = sColor.b;
		fields[ i ].sColor[ 3 ] = sColor.a;

		fields[ i ].eColor[ 0 ] = eColor.r;
		fields[ i ].eColor[ 1 ] = eColor.g;
		fields[ i ].eColor[ 2 ] = eColor.b;
		fields[ i ].eColor[ 3 ] = eColor.a;
		
		fields[ i ].bsColor[ 0 ] = sColor.r;		// broder.
		fields[ i ].bsColor[ 1 ] = sColor.g;
		fields[ i ].bsColor[ 2 ] = sColor.b;
		fields[ i ].bsColor[ 3 ] = sColor.a;

		fields[ i ].beColor[ 0 ] = eColor.r;
		fields[ i ].beColor[ 1 ] = eColor.g;
		fields[ i ].beColor[ 2 ] = eColor.b;
		fields[ i ].beColor[ 3 ] = eColor.a;
		
		fields[ i ].scale = pow( 2, fields[ i ].scaleInc );
	}
}

//////////////////////////////////////////////
//	DRAW.
//////////////////////////////////////////////

void testApp :: draw()
{
	ofBackground( 0, 0, 0 );
	ofEnableAlphaBlending();
	
	glBlendFunc( GL_ONE, GL_ONE );
//	glBlendFunc( GL_ONE, GL_ONE_MINUS_DST_ALPHA );
//	glBlendFunc( GL_SRC_ALPHA, GL_ONE );
//	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	
	tileSaver.begin();
	
	glPushMatrix();
	glTranslatef( renderArea.x, renderArea.y, 0 );

//	drawSquareNoise( &fields[ 0 ] );
//	drawTraingleStatic();
//	drawTriangleNoise( &fields[ 0 ] );
	
	for( int i=0; i<fieldsTotal; i++ )
		drawTriangleNoise( &fields[ i ] );
	
	glPopMatrix();
	
	tileSaver.end();

	if( tileSaver.bGoTiling )
		return;
	
	if( screenGrabUtil.isRecording() )
		screenGrabUtil.save();
	
	glBlendFunc( GL_ONE, GL_ZERO );

	gui.draw();
	
	if( showDebug )
		drawDebug();
}

void testApp :: drawSquareNoise ( TriangleField *field )
{
	float size = 10.0 * field->scale;
	
	int cy = ceil( renderArea.height / size );
	int cx = ceil( renderArea.width / size );
	
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
	
	int cy = ceil( renderArea.height / size );
	int cx = ceil( renderArea.width / size );
	
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
	
	int cy = ceil( renderArea.height / size );
	int cx = ceil( renderArea.width / size );
	
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
		400,
		20
	);
}

//////////////////////////////////////////////
//	UPDATE.
//////////////////////////////////////////////

void testApp :: toggleFullScreen()
{
	ofToggleFullscreen();
	
	if( ofGetWindowMode() == OF_WINDOW )
	{
		renderArea.x		= renderAreaWindow.x;
		renderArea.y		= renderAreaWindow.y;
		renderArea.width	= renderAreaWindow.width;
		renderArea.height	= renderAreaWindow.height;
	}
	
	if( ofGetWindowMode() == OF_FULLSCREEN )
	{
		renderArea.x		= renderAreaFullScreen.x;
		renderArea.y		= renderAreaFullScreen.y;
		renderArea.width	= renderAreaFullScreen.width;
		renderArea.height	= renderAreaFullScreen.height;
	}
	
	screenGrabUtil.setArea( &renderArea );
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
			screenGrabUtil.start();
		}
	}
	
	if( key == 'd' )
	{
		showDebug = !showDebug;
		
		for( int i=0; i<fieldsTotal * 2; i++ )
		{
			colorPickers[ i ].setVisible( showDebug );
		}
		
		gui.toggleDraw();
	}
	
	if( key == 'f' )
		toggleFullScreen();
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

