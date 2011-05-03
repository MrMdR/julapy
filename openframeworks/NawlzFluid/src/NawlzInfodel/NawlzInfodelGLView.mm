//
//  NawlzSandGLView.m
//  Nawlz
//
//  Created by lukasz karluk on 20/02/11.
//  Copyright 2011 com.nawlz. All rights reserved.
//

#import "NawlzInfodelGLView.h"

@implementation NawlzInfodelGLView

- (void) appInit
{
    app = new NawlzInfodel();
    
    img = new NawlzImage();
    [ self loadImage : @"infodel_bg_clean.png" : img ];
    app->createBackgroundTexture( img->pixels, img->width, img->height, img->glType );
    delete img;
    
    img = new NawlzImage();
    [ self loadImage : @"infodel_harley_0.png" : img ];
    app->createHarleyOneTexture( img->pixels, img->width, img->height, img->glType, 366, 311 );
    delete img;
    
    img = new NawlzImage();
    [ self loadImage : @"infodel_harley_1.png" : img ];
    app->createHarleyTwoTexture( img->pixels, img->width, img->height, img->glType, 438, 366 );
    delete img;
    
    img = new NawlzImage();
    [ self loadImage : @"infodel_particle.png" : img ];
    app->createParticleTexture( img->pixels, img->width, img->height, img->glType );
    delete img;
	
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