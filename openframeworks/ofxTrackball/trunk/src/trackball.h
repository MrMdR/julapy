
#ifndef __trackball_h__
#define __trackball_h__

#ifdef __cplusplus
extern "C" {
#endif

void startTrackball (long x, long y, long originX, long originY, long width, long height);
void rollToTrackball (long x, long y, float rot [4]); // rot is output rotation angle
void addToRotationTrackball (float * dA, float * A);
	
void rollToTrackballSingle( float center[ 2 ], float mouseCurr[ 2 ], float mousePrev[ 2 ], float rot[ 4 ] );

#ifdef __cplusplus
}
#endif

#endif