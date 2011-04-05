//
//  Letter.h
//  emptyExample
//
//  Created by lukasz karluk on 4/04/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"

class Letter
{

public :
    
    Letter ()
    {
        shapeIndex              = NULL;
        shapePointsLength       = NULL;
        shapePoints             = NULL;
        shapeVBOs               = NULL;
        
        shapeFillIndex          = NULL;
        shapeFillTypes          = NULL;
        shapeFillPointsLength   = NULL;
        shapeFillPoints         = NULL;
        shapeFillVBOs           = NULL;
        
        bFillVBO                = false;
        bOutlineVBO             = false;
    }
    
    ~Letter ()
    {
        clean();
    }
    
    void clean ()
    {
        if( bFillVBO )
        {
            glDeleteBuffersARB( shapeFillTotal, &shapeFillVBOs[ 0 ] );
        }
        
        if( bOutlineVBO )
        {
            glDeleteBuffersARB( shapesTotal, &shapeVBOs[ 0 ] );
        }
        
        if( shapeIndex )
        {
            delete[] shapeIndex;
            shapeIndex = NULL;
        }
        
        if( shapePointsLength )     
        {
            delete[] shapePointsLength;
            shapePointsLength = NULL;
        }
        
        if( shapePoints )           
        {
            delete[] shapePoints;
            shapePoints = NULL;
        }
        
        if( shapeVBOs )             
        {
            delete[] shapeVBOs;
            shapeVBOs = NULL;
        }
        
        //----
        
        if( shapeFillIndex )        
        {
            delete[] shapeFillIndex;
            shapeFillIndex = NULL;
        }
        
        if( shapeFillTypes )        
        {
            delete[] shapeFillTypes;
            shapeFillTypes = NULL;
        }
        
        if( shapeFillPointsLength ) 
        {
            delete[] shapeFillPointsLength;
            shapeFillPointsLength = NULL;
        }
        if( shapeFillPoints )       
        {
            delete[] shapeFillPoints;
            shapeFillPoints = NULL;
        }
        
        if( shapeFillVBOs )         
        {
            delete[] shapeFillVBOs;
            shapeFillVBOs = NULL;
        }
    }
    
    void createFillVBO ()
    {
        glGenBuffersARB( shapeFillTotal, &shapeFillVBOs[ 0 ] );
        
		for( int j=0; j<shapeFillTotal; j++ )
		{
			glBindBufferARB( GL_ARRAY_BUFFER_ARB, shapeFillVBOs[ j ] );
			glBufferDataARB( GL_ARRAY_BUFFER_ARB, shapeFillPointsLength[ j ] * sizeof( float ), &shapeFillPoints[ shapeFillIndex[ j ] ], GL_STATIC_DRAW_ARB );
            glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
		}
        
        bFillVBO = true;
    }
    
    void createOutlineVBO ()
    {
        glGenBuffersARB( shapesTotal, &shapeVBOs[ 0 ] );
        
		for( int i=0; i<shapesTotal; i++ )
		{
			glBindBufferARB( GL_ARRAY_BUFFER_ARB, shapeVBOs[ i ] );
			glBufferDataARB( GL_ARRAY_BUFFER_ARB, shapePointsLength[ i ] * sizeof( float ), &shapePoints[ shapeIndex[ i ] ], GL_STATIC_DRAW_ARB );
            glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
		}
        
        bOutlineVBO = true;
    }
    
    void drawFill ()
    {
        if( !bFillVBO )
            return;
        
        for( int i=0; i<shapeFillTotal; i++ )
        {
            int vboIndex	= shapeFillVBOs[ i ];
            int vboType		= shapeFillTypes[ i ];
            int vboLength	= shapeFillPointsLength[ i ] / 3;
            
            glEnableClientState( GL_VERTEX_ARRAY );
            
            glBindBufferARB( GL_ARRAY_BUFFER_ARB, vboIndex );
            glVertexPointer( 3, GL_FLOAT, 0, 0 );
            
            glDrawArrays( vboType, 0, vboLength );
            
            glDisableClientState( GL_VERTEX_ARRAY );
            
            glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
        }
    }
    
    void drawOutline ( bool bSmooth = false )
    {
        if( !bOutlineVBO )
            return;
        
        if( bSmooth )
        {
            glEnable(GL_BLEND);										// line smoothing.
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_LINE_SMOOTH);
        }
        
        for( int i=0; i<shapesTotal; i++ )
        {
            int vboIndex	= shapeVBOs[ i ];
            int vboLength   = shapePointsLength[ i ] / 3;
            
            glEnableClientState( GL_VERTEX_ARRAY );
            
            glBindBufferARB( GL_ARRAY_BUFFER_ARB, vboIndex );
            glVertexPointer( 3, GL_FLOAT, 0, 0 );
            
            glDrawArrays( GL_LINE_STRIP, 0, vboLength );
            
            glDisableClientState( GL_VERTEX_ARRAY );
            
            glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
        }

        if( bSmooth )
        {
            ofDisableSmoothing();
        }
    }
    
    bool        bOutlineVBO;
    bool        bFillVBO;
    
    ofRectangle rect;
    
	int         shapesTotal;
	int         shapePointsTotal;
	int*        shapeIndex;
	int*        shapePointsLength;
	float*      shapePoints;
	GLuint*     shapeVBOs;
    
	int         shapeFillTotal;
	int         shapeFillPointsTotal;
	int*        shapeFillIndex;
	int*        shapeFillTypes;
	int*        shapeFillPointsLength;
	float*      shapeFillPoints;
	GLuint*     shapeFillVBOs;
};
