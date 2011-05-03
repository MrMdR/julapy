//
//  NawlzFluidGLView.mm
//  Nawlz
//
//  Created by lukasz karluk on 17/12/10.
//  Copyright 2010 com.nawlz. All rights reserved.
//

#import "NawlzDandelionGLView.h"

@implementation NawlzDandelionGLView

- (void) appInit
{
    app = new NawlzDandelion();
    
    img = new NawlzImage();
    [ self loadImage : @"dandelion_bg.png" : img ];
    app->createBackgroundTexture( img->pixels, img->width, img->height, img->glType );
    delete img;
    
    img = new NawlzImage();
    [ self loadImage : @"dandelion_fat_lady.png" : img ];
    app->createFatLadyTexture( img->pixels, img->width, img->height, img->glType, 345, 135 );
    delete img;
    
    img = new NawlzImage();
    [ self loadImage : @"dandelion_particle.png" : img ];
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
    //
}

- (void) appTouchUp     : (int) x : (int) y : (int) button
{
    //
}

@end