//
//  NawlzJackInBox.cpp
//  emptyExample
//
//  Created by lukasz karluk on 21/03/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "NawlzJackInBox.h"


void NawlzJackInBox :: setup ()
{
	box2d.init();
	box2d.setGravity( 0, 0 );
	box2d.setFPS( 30.0 );
	box2d.registerGrabbing();
}

void NawlzJackInBox :: createBackgroundTexture ( unsigned char* pixels, int width, int height, int glType, int x, int y )
{
    backgroundTexture = new ofTexture();
	backgroundTexture->allocate( width, height, glType );
	backgroundTexture->loadData( pixels, width, height, glType );
}

void NawlzJackInBox :: update ()
{
    //
}

void NawlzJackInBox :: draw ()
{
    backgroundTexture->draw( 0, 0 );
}

void NawlzJackInBox :: keyPressed ( int key )
{
    //
}

void NawlzJackInBox :: keyReleased ( int key )
{
    //
}

void NawlzJackInBox :: mouseMoved ( int x, int y )
{
    //
}

void NawlzJackInBox :: mouseDragged ( int x, int y, int button )
{
    //
}

void NawlzJackInBox :: mousePressed ( int x, int y, int button )
{
    //
}

void NawlzJackInBox :: mouseReleased ( int x, int y, int button )
{
    //
}

void NawlzJackInBox :: windowResized ( int w, int h )
{
    //
}
