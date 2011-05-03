//
//  NawlzFluidGLView.h
//  Nawlz
//
//  Created by lukasz karluk on 17/12/10.
//  Copyright 2010 com.nawlz. All rights reserved.
//

#include "NawlzGLView.h"
#include "NawlzFluid.h"
#include "NawlzImage.h"

@interface NawlzFluidGLView : NawlzGLView 
{
    NawlzFluid* app;
    NawlzImage* img;
}

@end