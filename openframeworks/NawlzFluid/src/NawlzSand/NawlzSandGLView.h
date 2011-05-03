//
//  NawlzSandGLView.h
//  Nawlz
//
//  Created by lukasz karluk on 20/02/11.
//  Copyright 2011 com.nawlz. All rights reserved.
//

#include "NawlzGLView.h"
#include "NawlzSand.h"
#include "NawlzImage.h"

@interface NawlzSandGLView : NawlzGLView 
{
    NawlzSand*  app;
    NawlzImage* img;
}

@end