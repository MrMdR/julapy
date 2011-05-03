//
//  NawlzInfodelGLView.h
//  Nawlz
//
//  Created by lukasz karluk on 20/02/11.
//  Copyright 2011 com.nawlz. All rights reserved.
//

#include "NawlzGLView.h"
#include "NawlzInfodel.h"
#include "NawlzImage.h"

@interface NawlzInfodelGLView : NawlzGLView 
{
    NawlzInfodel*   app;
    NawlzImage*     img;
}

@end