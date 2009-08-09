/*
 *  SphereBg.cpp
 *  openFrameworks
 *
 *  Created by lukasz karluk on 8/08/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "SphereBg.h"

SphereBg :: SphereBg()
{
	//
}

SphereBg :: ~SphereBg()
{
	//
}

void SphereBg :: init()
{
	sphereSize = 800;
	
	quadratic = gluNewQuadric();					// Create A Pointer To The Quadric Object
	gluQuadricTexture( quadratic, GL_TRUE );		// Create Texture Coords
	gluQuadricNormals( quadratic, GLU_SMOOTH );		// Create Smooth Normals
	
//	float right[] = {  1,  0,  0 };		// north / south pole pointing along Y axis.
//	float up[]    = {  0,  0, -1 };
//	float out[]   = {  0,  1,  0 };

	float right[] = {  1,  0,  0 };		// north / south pole pointing along Z axis.
	float up[]    = {  0,  1,  0 };
	float out[]   = {  0,  0,  1 };
	
	sphereOrientation[ 0 ]  = right[ 0 ];
	sphereOrientation[ 1 ]  = right[ 1 ];
	sphereOrientation[ 2 ]  = right[ 2 ];
	sphereOrientation[ 3 ]  = 0;
	sphereOrientation[ 4 ]  = up[ 0 ];
	sphereOrientation[ 5 ]  = up[ 1 ];
	sphereOrientation[ 6 ]  = up[ 2 ];
	sphereOrientation[ 7 ]  = 0;
	sphereOrientation[ 8 ]  = out[ 0 ];
	sphereOrientation[ 9 ]  = out[ 1 ];
	sphereOrientation[ 10 ] = out[ 2 ];
	sphereOrientation[ 11 ] = 0;
	sphereOrientation[ 12 ] = 0;
	sphereOrientation[ 13 ] = 0;
	sphereOrientation[ 14 ] = 0;
	sphereOrientation[ 15 ] = 1;
	
	sphereImage.loadImage( "sphereBg_02.jpg" );
}

void SphereBg :: update()
{

}

void SphereBg :: draw()
{
	glPushMatrix();
	glScalef( sphereSize, sphereSize, sphereSize );
	glMultMatrixf( &sphereOrientation[ 0 ] );
	glColor4f( 1, 1, 1, 1 );
	
	sphereImage.getTextureReference().bind();
	gluSphere( quadratic, 1, 50, 50 );
	sphereImage.getTextureReference().unbind();
	
	glPopMatrix();
}