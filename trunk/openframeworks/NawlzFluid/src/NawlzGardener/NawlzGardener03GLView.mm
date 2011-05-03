//
//  NawlzGardenerGLView.mm
//  Nawlz
//
//  Created by lukasz karluk on 17/12/10.
//  Copyright 2010 com.nawlz. All rights reserved.
//

#import "NawlzGardener03GLView.h"

@implementation NawlzGardener03GLView

- (void) appInit
{
    app = new NawlzGardener( NAWLZ_GARDENER_APP_INDEX_2 );
    
    img = new NawlzImage();
    [ self loadImage : @"gardener_bg_03_1024x768.png" : img ];
    app->createBackgroundTexture( img->pixels, img->width, img->height, img->glType );
    delete img;
    
    img = new NawlzImage();
    [ self loadImage : @"gardener_particle_01.png" : img ];
    app->createParticle01Texture( img->pixels, img->width, img->height, img->glType, 358, 220 );
    delete img;
    
    img = new NawlzImage();
    [ self loadImage : @"gardener_particle_02.png" : img ];
    app->createParticle02Texture( img->pixels, img->width, img->height, img->glType, 358, 220 );
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