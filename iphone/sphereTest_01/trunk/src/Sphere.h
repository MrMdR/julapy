/*
 *  Sphere.h
 *  iPhone Touch+Accel Example
 *
 *  Created by lukasz karluk on 8/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"

/* ErrorCode */
#define GLU_INVALID_ENUM                   100900
#define GLU_INVALID_VALUE                  100901
#define GLU_OUT_OF_MEMORY                  100902
#define GLU_INCOMPATIBLE_GL_VERSION        100903
#define GLU_INVALID_OPERATION              100904

/* QuadricDrawStyle */
#define GLU_POINT                          100010
#define GLU_LINE                           100011
#define GLU_FILL                           100012
#define GLU_SILHOUETTE                     100013

/* QuadricCallback */
#define GLU_ERROR                          100103

/* QuadricNormal */
#define GLU_SMOOTH                         100000
#define GLU_FLAT                           100001
#define GLU_NONE                           100002

/* QuadricOrientation */
#define GLU_OUTSIDE                        100020
#define GLU_INSIDE                         100021

class Sphere
{
public :
	
	Sphere();
	~Sphere();
	
	void drawSphere( GLfloat radius, GLint slices, GLint stacks );
	void setSphereDrawStyle( int value );
	
private :
	bool	useSphereTextureCoords;
	int		sphereShadeMode;
	int		sphereOrientation;
	int		sphereDrawStyle;
};