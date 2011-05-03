//
//  NawlzBlahGLView.m
//  Nawlz
//
//  Created by lukasz karluk on 20/02/11.
//  Copyright 2011 com.nawlz. All rights reserved.
//

#import "NawlzBlahGLView.h"

@implementation NawlzBlahGLView

- (void) appInit
{
    app = new NawlzBlah();
    
    img = new NawlzImage();
    [ self loadImage : @"blah_bg.png" : img ];
    app->createBackgroundTexture( img->pixels, img->width, img->height, img->glType );
    delete img;
    
    img = new NawlzImage();
    [ self loadImage : @"blah_particle_2.png" : img ];
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
