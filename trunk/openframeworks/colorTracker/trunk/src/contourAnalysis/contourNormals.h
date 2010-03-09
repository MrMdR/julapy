#pragma once



#include "ofMain.h"
#include "ofxVectorMath.h"
#include "ofxOpenCv.h"

class contourNormals{

	public:
		contourNormals(){

		}

		//------------------------------
		void makeNormals(vector <ofxPoint2f> &contourIn, vector <ofxPoint2f> &normalsOut, int direction = 1){
			int length = contourIn.size();

			normalsOut.clear();
			normalsOut.assign(length, ofxPoint2f() );

			ofxVec2f normal;
			ofxVec2f delta;
			for(int i = 1; i < length; i++){
                delta = contourIn[i] - contourIn[i-1];
                delta.normalize();

                if(direction){
                    normal.set(delta.y, -delta.x);
                }else{
                    normal.set(-delta.y, delta.x);
                }

                normalsOut[i] = normal;
			}

		}


};
