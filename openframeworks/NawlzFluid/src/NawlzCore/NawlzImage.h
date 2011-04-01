//
//  NawlzImage.h
//  Nawlz
//
//  Created by lukasz karluk on 1/04/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

class NawlzImage
{

public:

    NawlzImage()
    {
        pixels		= NULL;
        width		= 0;
        height		= 0;
        pixelDepth	= 0;
        glType		= GL_LUMINANCE;
    };

    ~NawlzImage()
    {
        if( pixels )
        {
            delete[] pixels;
            pixels = NULL;
        }
    }

    unsigned char* pixels;
    int width;
    int height;
    int pixelDepth;
    int glType;
    
};