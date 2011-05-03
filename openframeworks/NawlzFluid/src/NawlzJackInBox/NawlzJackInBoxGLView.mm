//
//  NawlzJackInBoxGLView.m
//  Nawlz
//
//  Created by lukasz karluk on 20/02/11.
//  Copyright 2011 com.nawlz. All rights reserved.
//

#import "NawlzJackInBoxGLView.h"

@implementation NawlzJackInBoxGLView

- (void) appInit
{
    app = new NawlzJackInBox();
    
    img = new NawlzImage();
    [ self loadImage : @"jackinbox_bg.png" : img ];
    app->createBackgroundTexture( img->pixels, img->width, img->height, img->glType );
    delete img;
    
    //--- parts.
    
    img = new NawlzImage();
    [ self loadImage : @"jackinbox_body.png" : img ];
    app->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 629, 247 ), ofPoint( 0, 0 ), 0, 0, false );
    delete img;
    
    img = new NawlzImage();
    [ self loadImage : @"jackinbox_tongue.png" : img ];
    app->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 650, 534 ), ofPoint( 0, 0 ), 0, 0, false );
    delete img;
    
    img = new NawlzImage();
    [ self loadImage : @"jackinbox_lighter.png" : img ];
    app->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 759, 330 ), ofPoint( -5, -95 ), 120, 70, true );
    delete img;
    
    img = new NawlzImage();
    [ self loadImage : @"jackinbox_bomb_holder.png" : img ];
    app->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 768, 325 ), ofPoint( -8, -47 ), 70, 80, true );
    delete img;
    
    img = new NawlzImage();
    [ self loadImage : @"jackinbox_rocket.png" : img ];
    app->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 645, 326 ), ofPoint( -130, -47 ), 70, 270, true );
    delete img;
    
    img = new NawlzImage();
    [ self loadImage : @"jackinbox_arm_right.png" : img ];
    app->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 751, 425 ), ofPoint( -8, -95 ), 90, 75, true );
    delete img;
    
    img = new NawlzImage();
    [ self loadImage : @"jackinbox_arm_left.png" : img ];
    app->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 646, 426 ), ofPoint( -115, -73 ), 80, 270, true );
    delete img;
    
    img = new NawlzImage();
    [ self loadImage : @"jackinbox_bomb.png" : img ];
    app->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 723, 310 ), ofPoint( -6, -65 ), 105, 75, true );
    delete img;
    
    //--- ears right.
    
    img = new NawlzImage();
    [ self loadImage : @"jackinbox_ears_right_3.png" : img ];
    app->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 725, 183 ), ofPoint( -4, -13 ), 50, 100, true );
    delete img;
    
    img = new NawlzImage();
    [ self loadImage : @"jackinbox_ears_right_1.png" : img ];
    app->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 724, 180 ), ofPoint( -3, -33 ), 55, 70, true );
    delete img;
    
    img = new NawlzImage();
    [ self loadImage : @"jackinbox_ears_right_2.png" : img ];
    app->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 726, 182 ), ofPoint( -5, -25 ), 70, 90, true );
    delete img;
    
    //--- brain.
    
    img = new NawlzImage();
    [ self loadImage : @"jackinbox_arrow.png" : img ];
    app->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 710, 157 ), ofPoint( -37, -63 ), 35, 0, true );
    delete img;
    
    img = new NawlzImage();
    [ self loadImage : @"jackinbox_brain.png" : img ];
    app->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 714, 138 ), ofPoint( -23, -71 ), 55, 10, true );
    delete img;
    
    //--- head.
    
    img = new NawlzImage();
    [ self loadImage : @"jackinbox_head_solo.png" : img ];
    app->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 667, 134 ), ofPoint( 0, 0 ), 0, 0, false );
    delete img;
    
    img = new NawlzImage();
    [ self loadImage : @"jackinbox_head_spine.png" : img ];
    app->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 695, 205 ), ofPoint( 0, 0 ), 0, 0, false );
    delete img;
    
    //--- eyes left.
    
    img = new NawlzImage();
    [ self loadImage : @"jackinbox_eyes_left_L.png" : img ];
    app->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 698, 150 ), ofPoint( -69, -36 ), 60, -70, true );
    delete img;
    
    img = new NawlzImage();
    [ self loadImage : @"jackinbox_eyes_left_M.png" : img ];
    app->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 703, 147 ), ofPoint( -74, -66 ), 60, -45, true );
    delete img;
    
    img = new NawlzImage();
    [ self loadImage : @"jackinbox_eyes_left_R.png" : img ];
    app->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 708, 141 ), ofPoint( -30, -54 ), 45, -20, true );
    delete img;
    
    //--- eyes right.
    
    img = new NawlzImage();
    [ self loadImage : @"jackinbox_eyes_right_R.png" : img ];
    app->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 741, 150 ), ofPoint( -9, -55 ), 80, 60, true );
    delete img;
    
    img = new NawlzImage();
    [ self loadImage : @"jackinbox_eyes_right_M.png" : img ];
    app->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 739, 149 ), ofPoint( -7, -61 ), 55, 36, true );
    delete img;
    
    img = new NawlzImage();
    [ self loadImage : @"jackinbox_eyes_right_L.png" : img ];
    app->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 735, 147 ), ofPoint( -3, -64 ), 55, 13, true );
    delete img;
    
    //--- ears left.
    
    img = new NawlzImage();
    [ self loadImage : @"jackinbox_ears_left_3.png" : img ];
    app->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 679, 177 ), ofPoint( -78, -5 ), 75, -125, true );
    delete img;
    
    img = new NawlzImage();
    [ self loadImage : @"jackinbox_ears_left_2.png" : img ];
    app->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 678, 178 ), ofPoint( -69, -13 ), 45, -110, true );
    delete img;
    
    img = new NawlzImage();
    [ self loadImage : @"jackinbox_ears_left_1.png" : img ];
    app->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 677, 174 ), ofPoint( -76, -44 ), 70, -75, true );
    delete img;
    
    //--- mouth.
    
    img = new NawlzImage();
    [ self loadImage : @"jackinbox_mouth_base.png" : img ];
    app->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 699, 157 ), ofPoint( 0, 0 ), 0, 0, false );
    delete img;
    
    img = new NawlzImage();
    [ self loadImage : @"jackinbox_mouth_tongue.png" : img ];
    app->createBionicPart( img->pixels, img->width, img->height, img->glType, ofPoint( 752, 211 ), ofPoint( -12, -7 ), 20, 135, true );
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