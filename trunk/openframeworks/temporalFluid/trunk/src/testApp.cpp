#include "testApp.h"

////////////////////////////////////////////////////////////////
// INIT.
////////////////////////////////////////////////////////////////

void testApp :: setup()
{
	ofBackground( 30, 30, 30 );
	
	showVideoSource			= true;
	showFluid				= true;
	showTemporalDistortion	= true;
	showMotionDetection		= true;
	
	if( showVideoSource )
	{
		initVideoPlayer();
	}

	if( showMotionDetection )
	{
		initMotionDetection();
	}
	
	if( showFluid )
	{
		initFluid();
	}
	
	if( showTemporalDistortion )
	{
		initTemporalDistortion();
	}
}

void testApp :: initVideoPlayer ()
{
	int i;
	int j;
	unsigned char * vpPixels;
	
//	vp.loadMovie("movies/onthebalcony_30fps_450x300_h264.mov");
	vp.loadMovie("movies/decompose_450x248.mov");
	vp.setPaused( true );
	
	vpWidth				= vp.getWidth();
	vpHeight			= vp.getHeight();
	vpPixelsPerFrame	= vpWidth * vpHeight * 3;
	vpTexturePixels		= new unsigned char[ vpPixelsPerFrame ];
	
	frameBufferTotal		= vp.getTotalNumFrames();
	frameBufferPlayIndex	= 0;
	frameBuffer				= new unsigned char[ vpPixelsPerFrame * frameBufferTotal ];
	
	for( i=0; i<frameBufferTotal; i++ )
	{
		vp.setPosition( (float)i / (float)( frameBufferTotal - 1 ) );
		vpPixels = vp.getPixels();
		
		for( j=0; j<vpPixelsPerFrame; j++ )
		{
			frameBuffer[ i * vpPixelsPerFrame + j ] = vpPixels[ j ];
		}
	}
	
	vpTexture.allocate( vpWidth, vpHeight, GL_RGB );
}

void testApp :: initFluid ()
{
	fluidWidth		= vpWidth;
	fluidHeight		= vpHeight;
	fluidNX			= 120;
	fluidNY			= fluidNX * fluidHeight / fluidWidth;
	fluidInvWidth	= 1.0f / fluidWidth;
	fluidInvHeight	= 1.0f / fluidHeight;
	fluidVisc		= 0.00001f;
	fluidDT			= 2.0f;
	fluidFadeSpeed	= 0.01f;
	fluidInvNX		= 1.0f / ( fluidNX + 2 );
	fluidInvNY		= 1.0f / ( fluidNY + 2 );
	
	mouseX	= 0;
	mouseY	= 0;
	pmouseX	= 0;
	pmouseY	= 0;
	
	fluid.setup( fluidNX, fluidNY );
	fluid.enableRGB( true );
//	fluid.setFadeSpeed( 0.01f );
//	fluid.setDeltaT( 2.0f );
//	fluid.setVisc( 0.00001f );
	fluid.setFadeSpeed( 0.005f );
	fluid.setDeltaT( 0.5f );
	fluid.setVisc( 0.0001f );
	
	fluidPixels = new unsigned char[ fluidNX * fluidNY * 3 ];
	
	fluidTexture.allocate( fluidNX, fluidNY, GL_RGBA);
	
//	fluid.init
//	(
//		fluidWidth,
//		fluidHeight,
//		fluidNX,
//		fluidNY,
//		fluidDT,
//		fluidVisc,
//		fluidFadeSpeed
//	 );
}

void testApp :: initTemporalDistortion()
{
	tdTexturePixels		= new unsigned char[ vpPixelsPerFrame ];
	tdWidth				= vpWidth;
	tdHeight			= vpHeight;

	tdTexture.allocate( tdWidth, tdHeight, GL_RGB );
	tdScreenImage.allocate( tdWidth, tdHeight, GL_RGB );
}

void testApp :: initMotionDetection ()
{
	motionDetect.init( 320, 240 );
}

////////////////////////////////////////////////////////////////
// UPDATE.
////////////////////////////////////////////////////////////////

void testApp::update()
{
	if( showVideoSource )
	{
		updateVideoPlayer();
	}

	if( showMotionDetection )
	{
		updateMotionDetection();
	}
	
	if( showFluid )
	{
		updateFluid();
	}
	
	if( showTemporalDistortion )
	{
		updateTemporalDistortion();
	}
}

void testApp :: updateVideoPlayer ()
{
	int i;
	
	for( i=0; i<vpPixelsPerFrame; i++ )
	{
		vpTexturePixels[ i ] = frameBuffer[ frameBufferPlayIndex * vpPixelsPerFrame + i ];
	}
	
	vpTexture.loadData( vpTexturePixels, vpWidth, vpHeight, GL_RGB );
	
	++frameBufferPlayIndex;
	
	if( frameBufferPlayIndex > frameBufferTotal - 1 )
	{
		frameBufferPlayIndex = 0;
	}
}

void testApp :: updateFluid ()
{
	fluid.update();
}

void testApp :: updateTemporalDistortion ()
{
	unsigned char * pixels;
	int i;
	int j;
	int r;
	int g;
	int b;
	float c;

	tdScreenImage.grabScreen( 10, vpHeight + 20, fluidWidth, fluidHeight );
	pixels = tdScreenImage.getPixels();
	
	for( i=0; i<vpPixelsPerFrame; i+=3 )
	{
		r = pixels[ i + 0 ];
		g = pixels[ i + 1 ];
		b = pixels[ i + 2 ];
		c = ( r + g + b ) / (float)765.0f;
		
		tdTexturePixels[ i + 0 ] = frameBuffer[ (int)( c * frameBufferTotal ) * vpPixelsPerFrame + ( i + 0 ) ];
		tdTexturePixels[ i + 1 ] = frameBuffer[ (int)( c * frameBufferTotal ) * vpPixelsPerFrame + ( i + 1 ) ];
		tdTexturePixels[ i + 2 ] = frameBuffer[ (int)( c * frameBufferTotal ) * vpPixelsPerFrame + ( i + 2 ) ];
	}
	
	tdTexture.loadData( tdTexturePixels, tdWidth, tdHeight, GL_RGB );
}

void testApp :: updateMotionDetection ()
{
	motionDetect.update();
	
	if( motionDetect.newFrame ) 
	{
		int fx, fy;			// fluid coordinates
		float dx, dy;		// get cam velocity into these vars
		float scrX, scrY;	// will store screen coordinates
		
		float speed2;		// absolute speed squared
		
		float max	= 0;
		
		for ( fx=0; fx<fluidNX; fx+=1 )
		{
			scrX = fx * fluidInvNX;
			
			for ( fy=0; fy<fluidNY; fy+=1 ) 
			{
				scrY = fy * fluidInvNY;
				
				motionDetect.getVelAtNorm
				(
					scrX,
					scrY,
					&dx,
					&dy
				);
				
				speed2	= dx * dx + dy * dy;
				max		= MAX( speed2, max );
				
				fluid.addForceAtPos
				(
					scrX,
					scrY,
					dx,
					dy
				);
				
//				fluid.addAtPixel
//				(
//					fx,
//					fy,
//					dx,
//					dy,
//					0.00001f,	
//					0.0025f,		
//					1.0f, 
//					1.0f,
//					1.0f,
//					speed2 * speed2
//				 );
			}
		}
		
	}
}

////////////////////////////////////////////////////////////////
// DRAW.
////////////////////////////////////////////////////////////////

void testApp::draw()
{
	if( showVideoSource )
	{
		drawVideoPlayer();
	}

	if( showMotionDetection )
	{
		drawMotionDetection();
	}
	
	if( showFluid )
	{
		drawFluid();
	}
	
	if( showTemporalDistortion )
	{
		drawTemporalDistortion();
	}
}

void testApp :: drawVideoPlayer ()
{
	ofSetColor( 0xFFFFFF );
	vpTexture.draw( vpWidth + 20, 10 );
}

void testApp :: drawFluid ()
{
	int i = 0;
	int j = 0;
	int d = 2;
	int r;
	int g;
	int b;
	int m = 0;
	
//	ofSetColor( 0xFFFFFF );
//	fluid.predraw();
//	fluid.draw( 10, vpHeight + 20, fluidWidth, fluidHeight );

//	for(int i=0, j=0; i<fluid.getNumCells(); i++, j+=3 )
//	{
//		r = (int)( fluid.getR()[ i ] * 255 );
//		g = (int)( fluid.getG()[ i ] * 255 );
//		b = (int)( fluid.getB()[ i ] * 255 );
//		
//		fluidPixels[ j + 0 ] = MIN( r, 255 );
//		fluidPixels[ j + 1 ] = MIN( g, 255 );
//		fluidPixels[ j + 2 ] = MIN( b, 255 );
//		
//		if( m < r)
//		{
//			m = r;
//		}
//	}

	ofPoint vel;
	ofPoint color;
	int index = 0;
	float alpha	= 1;
	bool withAlpha = false;
	
	for(int j=1; j <fluidNX+1; j++) 
	{
		for(int i=1; i < fluidNY+1; i++) 
		{
			fluid.getInfoAtCell(i, j, &vel, &color);
			float speed2 = fabs(vel.x) * fluidNX + fabs(vel.y) * fluidNY;
			int speed = MIN(speed2 * 255 * alpha, 255);
			int r = fluidPixels[index++] = MIN(color.x * 255 * alpha, 255);
			int g = fluidPixels[index++] = MIN(color.y * 255 * alpha, 255);		
			int b = fluidPixels[index++] = MIN(color.z * 255 * alpha, 255);	
			int a = fluidPixels[index++] = withAlpha ? MAX(b, MAX(r, g)) : 255;
		}
	}  
	
	fluidTexture.loadData( fluidPixels, fluidNX, fluidNY, GL_RGBA );
	fluidTexture.draw( 10, vpHeight + 20, fluidWidth, fluidHeight );
}

void testApp :: drawTemporalDistortion ()
{
	ofSetColor( 0xFFFFFF );
	tdTexture.draw( 10, 10, tdWidth, tdHeight );
}

void testApp :: drawMotionDetection ()
{
	motionDetect.draw( vpWidth + 20, vpHeight + 20, vpWidth, vpHeight );
}

////////////////////////////////////////////////////////////////
// HANDLERS.
////////////////////////////////////////////////////////////////

void testApp::keyPressed(int key)
{
	switch (key)
	{
		case ' ' :
			motionDetect.takeBgSnapshot();
		break;
	}
}

void testApp::keyReleased(int key)
{

}

void testApp::mouseMoved(int x, int y )
{
	mouseX	= x + 10;
	mouseY	= y + 10;
	
	if( mouseX > vpWidth )
	{
		mouseX = vpWidth;
	}
	
	if( mouseY > vpHeight )
	{
		mouseY = vpHeight;
	}
	
	float mouseNormX	= mouseX * fluidInvWidth;
	float mouseNormY	= mouseY * fluidInvHeight;
	float mouseVelX		= ( mouseX - pmouseX ) * fluidInvWidth;
	float mouseVelY		= ( mouseY - pmouseY ) * fluidInvHeight;
	
	pmouseX	= mouseX;
	pmouseY	= mouseY;
	
	if( showFluid )
	{
		fluid.addForceAtPos
		(
			mouseNormX,
			mouseNormY,
			mouseVelX,
			mouseVelY
		);
		 
//		fluid.addAtNorm
//		(
//			mouseNormX,
//			mouseNormY,
//			mouseVelX,
//			mouseVelY,
//			50000000000.0f,	
//			5.0f,				
//			1.0,	// r
//			1.0,	// g
//			1.0		// b
//		 );
	}
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

