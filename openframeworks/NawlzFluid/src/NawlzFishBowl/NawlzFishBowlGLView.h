//
//  NawlzFishBowlGLView.h
//  Nawlz
//
//  Created by lukasz karluk on 20/02/11.
//  Copyright 2011 com.nawlz. All rights reserved.
//

#include "NawlzGLView.h"
#include "NawlzFishBowl.h"
#include "NawlzImage.h"

@interface NawlzFishBowlGLView : NawlzGLView 
{
    NawlzFishBowl*  app;
    NawlzImage*     img;
}

@end