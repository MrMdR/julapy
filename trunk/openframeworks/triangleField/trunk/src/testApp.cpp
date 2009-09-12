#include "testApp.h"

//////////////////////////////////////////////
//	SETUP.
//////////////////////////////////////////////

void testApp :: setup()
{
	ofSetFrameRate( 30 );
	ofSetVerticalSync( true );
	
	smoothing = false;
	
	initRenderArea();
	initFieldConfig();
	initFields();
	initDebug();
	initBlendModes();
	initGui();
	initAudio();
	initTriggers();
	
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
//	FIELDS.
//////////////////////////////////////////////

void testApp :: initFieldConfig()
{
	fieldConfigTotal	= 3;
	fieldConfigIndex	= fieldConfigIndex2 = 1;
	fieldConfig			= new TriangleFieldConfig *[ fieldConfigTotal ];
	fieldConfig[ 0 ]	= new TriangleFieldConfig01();
	fieldConfig[ 1 ]	= new TriangleFieldConfig02();
	fieldConfig[ 2 ]	= new TriangleFieldConfig03();
}

void testApp :: initFields()
{
	fieldIndex  = 0;
	fieldsTotal = 3;
	fields = new TriangleField[ fieldsTotal ];
	
	fieldConfig[ fieldConfigIndex ]->copyTo( fields, fieldsTotal, true );
}

//////////////////////////////////////////////
//	BLEND MODES.
//////////////////////////////////////////////

void testApp :: initBlendModes()
{
	blendModesTotal	= 0;
	blendModeIndex	= 1;
	blendModes		= new GLuint[ 10 ];
	
	addBlendMode( GL_ONE, GL_ONE );
	addBlendMode( GL_SRC_ALPHA, GL_ONE );
	addBlendMode( GL_ONE, GL_ONE_MINUS_DST_ALPHA );
	addBlendMode( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
}

void testApp :: addBlendMode( GLuint srcBlend, GLuint dstBlend )
{
	blendModes[ blendModesTotal * 2 + 0 ] = srcBlend;
	blendModes[ blendModesTotal * 2 + 1 ] = dstBlend;

	blendModesTotal += 1;
}

//////////////////////////////////////////////
//	UI.
//////////////////////////////////////////////

void testApp :: initGui()
{
	colorPickers = new ColorPicker[ fieldsTotal * 2 ];
	
	int colorWheelWidth  = 100;
	int colorWheelHeight = 100;
	
	int py = 86;
	int px = 280;
	int sx = 30;
	
	for( int i=0; i<fieldsTotal; i++ )
	{
		Color c1, c2;
		
		c1.r = fields[ i ].sColor[ 0 ];
		c1.g = fields[ i ].sColor[ 1 ];
		c1.b = fields[ i ].sColor[ 2 ];
		
		c2.r = fields[ i ].eColor[ 0 ];
		c2.g = fields[ i ].eColor[ 1 ];
		c2.b = fields[ i ].eColor[ 2 ];
		
		colorPickers[ i * 2 + 0 ].setSize( px, py, colorWheelWidth, colorWheelHeight );
		colorPickers[ i * 2 + 0 ].setMode( COLOR_PICKER_MODE_MOUSE);
		colorPickers[ i * 2 + 0 ].setColor( &c1 );
		
		px += colorWheelWidth + sx;
		
		colorPickers[ i * 2 + 1 ].setSize( px, py, colorWheelWidth, colorWheelHeight );
		colorPickers[ i * 2 + 1 ].setMode( COLOR_PICKER_MODE_MOUSE);
		colorPickers[ i * 2 + 1 ].setColor( &c2 );
		
		px += colorWheelWidth + sx + 20;
		
		gui.addSlider( "scale     ", &fields[ i ].scaleInc,   1.0, 4.0 );
		gui.addSlider( "cut off   ", &fields[ i ].cutoff,     0.0, 1.0 );
		gui.addSlider( "noiseXRes ", &fields[ i ].noiseXRes, -0.1, 0.1 );
		gui.addSlider( "noiseYRes ", &fields[ i ].noiseYRes, -0.1, 0.1 );
		gui.addSlider( "noiseZRes ", &fields[ i ].noiseZRes, -0.1, 0.1 );
		gui.addSlider( "noiseXVel ", &fields[ i ].noiseXVel, -0.1, 0.1 );
		gui.addSlider( "noiseYVel ", &fields[ i ].noiseYVel, -0.1, 0.1 );
		gui.addSlider( "noiseZVel ", &fields[ i ].noiseZVel, -0.1, 0.1 );
		gui.addToggle( "outline   ", &fields[ i ].drawOutline );
		
		if( i < fieldsTotal - 1 )
			gui.addPage( "Settings" );
	}
	
	colorPickers[ 1 ].setMode( COLOR_PICKER_MODE_CIRLCE_ROTATION );
	colorPickers[ 1 ].setCircularSpeed( 0.5 );
	colorPickers[ 1 ].setCircularLowerBounds( 0.4 );
	colorPickers[ 1 ].setCircularUpperBounds( 0.4 );
	
	gui.addPage( "General" );
	gui.addToggle( "smoothing  ", &smoothing );
	gui.addSlider( "blend mode ", &blendModeIndex, 0, blendModesTotal - 1 );
	gui.addSlider( "field config ", &fieldConfigIndex, 0, fieldConfigTotal - 1 );
	
	gui.setPage( 4 );
}	

//////////////////////////////////////////////
//	AUDIO.
//////////////////////////////////////////////

void testApp :: initAudio()
{
//	audio.init( "../../../../_audio/dj_krush_08_decks_athron.mp3" );
//	audio.init( "../../../../_audio/dat_politics_12_nude_Noodle.mp3" );
//	audio.init( "../../../../_audio/autechre_ep7_06_dropp.mp3" );
//	audio.init( "../../../../_audio/radiohead_06_I_Am_Citizen_Insane.mp3" );
//	audio.init( "../../../../_audio/aphex_twin_07_come_to_daddy_mummy_mix.mp3" );
//	audio.init( "../../../../_audio/principles_of_geometry_01_arp_center.mp3" );
//	audio.init( "../../../../_audio/massive_attack_05_exchange.mp3" );
//	audio.init( "../../../../_audio/lukid_06_chord.mp3" );
//	audio.init( "../../../../_audio/gentle_force_5_into_6.mp3" );
//	audio.init( "../../../../_audio/gentle_force_hohner.wav" );
	audio.init( "../../../../_audio/gentle_force_learning_to_forgive.wav" );
//	audio.init( "../../../../_audio/gentle_force_majestic.wav" );
//	audio.init( "../../../../_audio/gentle_force_ode_to_moritz.wav" );
	audio.setPosition( 0.0 );
//	audio.setVolume( 0 );
//	audio.setNoOfBands( 100 );
//	audio.setThreshold( 0.55 );
}

void testApp :: initTriggers()
{
	trigger.setWriteFormat( "trigger" );
	
	trigger.addTrigger( 0.15606,		&fieldConfigIndex, 0 );
	
	trigger.addTrigger( 0.196034029,	&fieldConfigIndex, 2 );
	trigger.addTrigger( 0.196135268,	&fieldConfigIndex, 1 );
	trigger.addTrigger( 0.196506515,	&fieldConfigIndex, 2 );
	trigger.addTrigger( 0.196709007,	&fieldConfigIndex, 1 );
	trigger.addTrigger( 0.197080255,	&fieldConfigIndex, 2 );
	trigger.addTrigger( 0.197181493,	&fieldConfigIndex, 1 );
	trigger.addTrigger( 0.197653994,	&fieldConfigIndex, 2 );
	trigger.addTrigger( 0.198328972,	&fieldConfigIndex, 0 );
	
	trigger.addTrigger( 0.200826421,	&fieldConfigIndex, 2 );
	trigger.addTrigger( 0.201028913,	&fieldConfigIndex, 1 );
	trigger.addTrigger( 0.201501414,	&fieldConfigIndex, 2 );
	trigger.addTrigger( 0.201602653,	&fieldConfigIndex, 1 );
	trigger.addTrigger( 0.202007651,	&fieldConfigIndex, 2 );
	trigger.addTrigger( 0.202176392,	&fieldConfigIndex, 1 );
	trigger.addTrigger( 0.202581391,	&fieldConfigIndex, 2 );
	trigger.addTrigger( 0.202851385,	&fieldConfigIndex, 0 );
		
	trigger.addTrigger( 0.206023812,	&fieldConfigIndex, 2 );
	trigger.addTrigger( 0.206125066,	&fieldConfigIndex, 1 );
	trigger.addTrigger( 0.206496298,	&fieldConfigIndex, 2 );	
	trigger.addTrigger( 0.206800044,	&fieldConfigIndex, 1 );
	trigger.addTrigger( 0.20700255,		&fieldConfigIndex, 2 );
	trigger.addTrigger( 0.207272545,	&fieldConfigIndex, 1 );
	trigger.addTrigger( 0.207576275,	&fieldConfigIndex, 2 );
	trigger.addTrigger( 0.207778782,	&fieldConfigIndex, 1 );
	trigger.addTrigger( 0.208150014,	&fieldConfigIndex, 2 );
	trigger.addTrigger( 0.209196255,	&fieldConfigIndex, 0 );
	
	trigger.addTrigger( 0.211052462,	&fieldConfigIndex, 2 );
	trigger.addTrigger( 0.211322457,	&fieldConfigIndex, 1 );
	trigger.addTrigger( 0.211592451,	&fieldConfigIndex, 2 );
	trigger.addTrigger( 0.211794943,	&fieldConfigIndex, 1 );
	trigger.addTrigger( 0.21216619,		&fieldConfigIndex, 2 );
	trigger.addTrigger( 0.212267429,	&fieldConfigIndex, 1 );
	trigger.addTrigger( 0.212942421,	&fieldConfigIndex, 0 );
}

//////////////////////////////////////////////
//	UPDATE.
//////////////////////////////////////////////

void testApp :: update()
{
	if( tileSaver.bGoTiling )
		return;

	audio.update();
	
	trigger.update( audio.getPosition() );
	
	fieldConfig[ fieldConfigIndex ]->setAudioNorm( audio.getAveragePeakNorm() );
	fieldConfig[ fieldConfigIndex ]->update();
	fieldConfig[ fieldConfigIndex ]->copyChangesTo( fields, fieldsTotal );
	
	updateFields();

	++frameCount;
}

void testApp :: updateFields()
{
	if( fieldConfigIndex2 != fieldConfigIndex )
	{
		fieldConfigIndex2 = fieldConfigIndex;
		
		fieldConfig[ fieldConfigIndex ]->copyTo( fields, fieldsTotal );
		
		for( int i=0; i<fieldsTotal; i++ )
		{
			Color c1, c2;
			
			c1.r = fields[ i ].sColor[ 0 ];
			c1.g = fields[ i ].sColor[ 1 ];
			c1.b = fields[ i ].sColor[ 2 ];
			c1.a = fields[ i ].sColor[ 3 ];
			
			c2.r = fields[ i ].eColor[ 0 ];
			c2.g = fields[ i ].eColor[ 1 ];
			c2.b = fields[ i ].eColor[ 2 ];
			c2.a = fields[ i ].eColor[ 3 ];
			
			colorPickers[ i * 2 + 0 ].setColor( &c1 );
			colorPickers[ i * 2 + 1 ].setColor( &c2 );
		}
	}
	
	for( int i=0; i<fieldsTotal; i++ )
	{
		Color sColor;
		Color eColor;
		colorPickers[ i * 2 + 0 ].getColor( &sColor );
		colorPickers[ i * 2 + 1 ].getColor( &eColor );
		
		fields[ i ].sColor[ 0 ] = sColor.r;			// fill.
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
	ofBackground( 20, 20, 20 );
	ofEnableAlphaBlending();
	
	if( smoothing )
		ofEnableSmoothing();
	else
		ofDisableSmoothing();
	
	glBlendFunc( blendModes[ blendModeIndex * 2 + 0 ], blendModes[ blendModeIndex * 2 + 1 ] );
	
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
	
//	field->noiseXInit = frameCount * field->noiseXVel;
//	field->noiseYInit = frameCount * field->noiseYVel;
//	field->noiseZInit = frameCount * field->noiseZVel;

	field->noiseXInit += field->noiseXVel;
	field->noiseYInit += field->noiseYVel;
	field->noiseZInit += field->noiseZVel;
	
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
	ofSetColor( 0x000000 );
	ofDrawBitmapString
	(
		"audio position :: " + ofToString( audio.getPosition(), 5 ) + "\n\n"
		,
		800,
		renderArea.height - 300
	);
	
	for( int i=0; i<fieldsTotal; i++ )
	{
		Color sColor;
		Color eColor;
		colorPickers[ i * 2 + 0 ].draw();
		colorPickers[ i * 2 + 1 ].draw();
	}		
	
	ofSetColor( 0xFFFFFF );
	glPushMatrix();
	glTranslatef( 270, renderArea.height - 210, 0 );
	audio.draw( 512, 200 );
	glPopMatrix();
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
	if( key == '1' )
	{
		fieldConfigIndex = 0;
		trigger.addTrigger( audio.getPosition(), &fieldConfigIndex, fieldConfigIndex, true, "fieldConfigIndex" );
	}

	if( key == '2' )
	{
		fieldConfigIndex = 1;
		trigger.addTrigger( audio.getPosition(), &fieldConfigIndex, fieldConfigIndex, true, "fieldConfigIndex" );
	}
	
	if( key == '3' )
	{
		fieldConfigIndex = 2;
		trigger.addTrigger( audio.getPosition(), &fieldConfigIndex, fieldConfigIndex, true, "fieldConfigIndex" );
	}
	
	if( key == 'c' )
	{
//		for( int i=0; i<fieldsTotal; i++ )
//		{
//			colorPickers[ i * 2 + 0 ].setRandomColor();
//			colorPickers[ i * 2 + 1 ].setRandomColor();
//		}

		colorPickers[ 0 ].setRandomColor();
		colorPickers[ 1 ].setRandomColor();
		
		Color c1, c2;
		colorPickers[ 0 ].getColor( &c1 );
		colorPickers[ 1 ].getColor( &c2 );
		
		colorPickers[ 2 ].setColor( &c1 );
		colorPickers[ 3 ].setColor( &c2 );

		colorPickers[ 4 ].setColor( &c1 );
		colorPickers[ 5 ].setColor( &c2 );
	}
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
			
			audio.setFrameRateSync( false );
		}
		else
		{
			screenGrabUtil.start();
			
			audio.setFrameRateSync( true );
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
	
	if( key == 't' )
		trigger.writeToFile( "trigger_data.txt" );
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

