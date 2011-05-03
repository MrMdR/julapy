//
//  NawlzFluidGLView.h
//  Nawlz
//
//  Created by lukasz karluk on 17/12/10.
//  Copyright 2010 com.nawlz. All rights reserved.
//

#include "NawlzGLView.h"
#include "NawlzDandelion.h"
#include "NawlzImage.h"

@interface NawlzDandelionGLView : NawlzGLView 
{
    NawlzDandelion* app;
    NawlzImage*     img;
}

@end