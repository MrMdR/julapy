/***********************************************************************
 
 This class is for drawing a fluidsolver using the openFrameworks texture
 
 /***********************************************************************
 
 Copyright (c) 2008, 2009, Memo Akten, www.memo.tv
 *** The Mega Super Awesome Visuals Company ***
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of MSA Visuals nor the names of its contributors 
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE. 
 *
 * ***********************************************************************/ 


#include "ofxMSAFluidDrawer.h"

ofxMSAFluidDrawer :: ofxMSAFluidDrawer()
{
	_fluidSolver = NULL;
}

ofxMSAFluidDrawer :: ~ofxMSAFluidDrawer()
{
	//
}

ofxMSAFluidSolver* ofxMSAFluidDrawer :: setup( ofxMSAFluidSolver* f, int renderWidth, int renderHeight )
{
	_fluidSolver = f;

	imageLrgWidth	= renderWidth;
	imageLrgHeight	= renderHeight;
	
	imageSmlWidth	= _fluidSolver->getWidth()  - 2;
	imageSmlHeight	= _fluidSolver->getHeight() - 2;
	
	imagePixels		= new unsigned char[ imageSmlWidth * imageSmlHeight ];
	
	imageSml.allocate( imageSmlWidth, imageSmlHeight );
	imageLrg.allocate( imageLrgWidth, imageLrgHeight );
	
	return _fluidSolver;
}

ofxMSAFluidSolver* ofxMSAFluidDrawer :: getFluidSolver()
{
	return _fluidSolver;
}


void ofxMSAFluidDrawer::update()
{
	int fw = _fluidSolver->getWidth();
	int fh = _fluidSolver->getHeight();
	
	ofPoint vel;
	ofPoint color;
	
	int index = 0;
	for( int j=1; j < fh-1; j++ )
	{
		for( int i=1; i < fw-1; i++ )
		{
			_fluidSolver->getInfoAtCell( i, j, &vel, &color );
			
			imagePixels[ index++ ] = MIN( color.x * 255, 255 );
			
//			int r = colorRgbPixels[index++] = MIN(color.x * 255 * alpha, 255);
//			int g = colorRgbPixels[index++] = MIN(color.y * 255 * alpha, 255);		
//			int b = colorRgbPixels[index++] = MIN(color.z * 255 * alpha, 255);	
		}
	}  
	
	imageSml.setFromPixels( imagePixels, imageSmlWidth, imageSmlHeight );
	imageLrg.scaleIntoMe( imageSml, CV_INTER_CUBIC );
//	imageLrg.blurGaussian( 5 );
}

unsigned char* ofxMSAFluidDrawer :: getFluidPixels()
{
	return imageLrg.getPixels();
}

void ofxMSAFluidDrawer :: draw( float x, float y, float renderWidth, float renderHeight )
{
	imageLrg.draw( x, y, renderWidth, renderHeight );
}
