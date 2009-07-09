/*
 * SGI FREE SOFTWARE LICENSE B (Version 2.0, Sept. 18, 2008)
 * Copyright (C) 1991-2000 Silicon Graphics, Inc. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice including the dates of first publication and
 * either this permission notice or a reference to
 * http://oss.sgi.com/projects/FreeB/
 * shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * SILICON GRAPHICS, INC. BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
 * OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Except as contained in this notice, the name of Silicon Graphics, Inc.
 * shall not be used in advertising or otherwise to promote the sale, use or
 * other dealings in this Software without prior written authorization from
 * Silicon Graphics, Inc.
 *
 * OpenGL ES CM 1.0 port of part of GLU by Mike Gorchak <mike@malva.ua>
 */

//#ifndef _OF_IPHONE_SPHERE_
//#define _OF_IPHONE_SPHERE_

/* ErrorCode */
//#define GLU_INVALID_ENUM                   100900
//#define GLU_INVALID_VALUE                  100901
//#define GLU_OUT_OF_MEMORY                  100902
//#define GLU_INCOMPATIBLE_GL_VERSION        100903
//#define GLU_INVALID_OPERATION              100904
//
///* QuadricDrawStyle */
//#define GLU_POINT                          100010
//#define GLU_LINE                           100011
//#define GLU_FILL                           100012
//#define GLU_SILHOUETTE                     100013
//
///* QuadricCallback */
//#define GLU_ERROR                          100103
//
///* QuadricNormal */
//#define GLU_SMOOTH                         100000
//#define GLU_FLAT                           100001
//#define GLU_NONE                           100002
//
///* QuadricOrientation */
//#define GLU_OUTSIDE                        100020
//#define GLU_INSIDE                         100021

class Sphere
{

public :
	void drawSphere( GLfloat radius, GLint slices, GLint stacks );

};


//#endif /* __GLUES_QUAD_H__ */
