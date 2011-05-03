//
//  NawlzBlahGLView.h
//  Nawlz
//
//  Created by lukasz karluk on 20/02/11.
//  Copyright 2011 com.nawlz. All rights reserved.
//

#include "NawlzGLView.h"
#include "NawlzBlah.h"
#include "NawlzImage.h"

@interface NawlzBlahGLView : NawlzGLView 
{
    NawlzBlah*  app;
    NawlzImage* img;
}

@end