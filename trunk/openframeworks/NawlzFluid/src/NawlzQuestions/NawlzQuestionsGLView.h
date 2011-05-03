//
//  NawlzSandGLView.h
//  Nawlz
//
//  Created by lukasz karluk on 20/02/11.
//  Copyright 2011 com.nawlz. All rights reserved.
//

#include "NawlzGLView.h"
#include "NawlzQuestions.h"
#include "NawlzImage.h"

@interface NawlzQuestionsGLView : NawlzGLView 
{
    NawlzQuestions* app;
    NawlzImage*     img;
}

@end