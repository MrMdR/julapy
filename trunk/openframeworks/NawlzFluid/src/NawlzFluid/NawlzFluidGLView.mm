//
//  NawlzFluidGLView.mm
//  Nawlz
//
//  Created by lukasz karluk on 17/12/10.
//  Copyright 2010 com.nawlz. All rights reserved.
//

#import "NawlzFluidGLView.h"

@implementation NawlzFluidGLView

- (void) appInit
{
    app = new NawlzFluid();
    
    img = new NawlzImage();
    [ self loadImage : @"whale_bg_1024x768.png" : img ];
    app->createBackgroundTexture( img->pixels, img->width, img->height, img->glType );
    delete img;
    
    img = new NawlzImage();
    [ self loadImage : @"whale_477x223.png" : img ];
    app->createImageTexture( img->pixels, img->width, img->height, img->glType, 358, 220 );
    delete img;
    
//    img = new NawlzImage();
//    [ self loadImage : @"particle_4x4.png" : img ];
//    app->createParticleTexture( img->pixels, img->width, img->height, img->glType );
//    delete img;
    
//    app->useCircleMotion		= true;
	app->bDrawMeshGrid		= false;
	app->bDrawMeshQuads		= true;
	app->bDrawParticles		= true;
	
	app->setup();
}

- (void) appKill
{
	if( app )
	{
		delete app;
		app = NULL;
	}
}

- (void) appUpdate
{
    app->update();
}

- (void) appDraw
{
    app->draw();
}

- (void) appTouchDown   : (int) x : (int) y : (int) button
{
    //
}

- (void) appTouchMoved  : (int) x : (int) y : (int) button
{
    app->mouseMoved( x, y );
}

- (void) appTouchUp     : (int) x : (int) y : (int) button
{
    //
}

@end