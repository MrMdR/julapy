/*
 *  ArcBar.cpp
 *  julapy_cilindricoCollapse
 *
 *  Created by lukasz karluk on 12/12/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ArcBar.h"

ArcBar :: ArcBar()
{
	rot.x = random() * 2 * PI;
	rot.y = random() * 2 * PI;
	rot.z = random() * 2 * PI;
	
	float c;
	c = ofRandom( 0, 1 );
	
	colour[ 0 ] = c;
	colour[ 1 ] = c;
	colour[ 2 ] = c;
	colour[ 3 ] = 1;
	
	wfColour[ 0 ] = 0.4;
	wfColour[ 1 ] = 0.4;
	wfColour[ 2 ] = 0.4;
	wfColour[ 3 ] = 0.8;
	
	id				= 0;
	angle			= 200;
	angleStep		= (int)( angle / ofxTrigLUT :: precision );
	radius			= 300;
	width			= 50;
	height			= 50;
	scale			= 1;
	wireframePad	= 10;
	
	normLength		= 20;
	
	drawQuads		= true;
	drawNormals		= false;
	drawAngles		= false;
}

ArcBar :: ~ArcBar()
{
	//
}

void ArcBar :: createSolidModel ()
{
	float x1, y1, x2, y2, x3, y3, x4, y4;
	float fx1, fy1, fx2, fy2;
	float ps1, ps2;
	ofxVec3f ntemp, n1, n2;
	int i, j;
	
	x1 = y1 = x2 = y2 = x3 = y3 = x4 = y4 = 0;
	fx1 = fy1 = fx2 = fy2 = 0;
	ps1 = 0;
	ps2 = 0;
	
	glNewList( id, GL_COMPILE );
	
	for( i = 0; i<angleStep; i++ ) 
	{
		j = ( i < angleStep - 1 ) ? ( i + 1 ) : 0;
		
//		ps1 = ps.perlinSteps[ i ];
//		ps2 = ps.perlinSteps[ j ];
		
		ps1 = 1.0;
		ps2 = 1.0;
		
		x1	= ofxTrigLUT :: cosLUT[ j ] * ( radius );
		y1	= ofxTrigLUT :: sinLUT[ j ] * ( radius );
		x2	= ofxTrigLUT :: cosLUT[ j ] * ( radius + ( width * ps1 ) );
		y2	= ofxTrigLUT :: sinLUT[ j ] * ( radius + ( width * ps1 ) );
		
		x3	= ofxTrigLUT :: cosLUT[ i ] * ( radius );
		y3	= ofxTrigLUT :: sinLUT[ i ] * ( radius );
		x4	= ofxTrigLUT :: cosLUT[ i ] * ( radius + ( width * ps1 ) );
		y4	= ofxTrigLUT :: sinLUT[ i ] * ( radius + ( width * ps1 ) );
		
		ofxVec3f zaxis;
		zaxis.set( 0, 0, 1 );
		
		// normal pointing inwards.
		n1.set( 1, 0, 0 );
		n1.rotate( j * DEG_TO_RAD, zaxis );
		
		n2.set( 1, 0, 0 );
		n2.rotate( i * DEG_TO_RAD, zaxis );
		
		glBegin( GL_QUADS );
		
		if( ps1 != ps2 )
		{
			// draw edge faces.
			ntemp.set( 0, -1, 0 );
			
			if( ps1 < ps2  )	// facing left.
			{
				fx1	= ofxTrigLUT :: cosLUT[ j ] * ( radius + ( width * ps1 ) );
				fy1	= ofxTrigLUT :: sinLUT[ j ] * ( radius + ( width * ps1 ) );
				fx2	= ofxTrigLUT :: cosLUT[ j ] * ( radius + ( width * ps2 ) );
				fy2	= ofxTrigLUT :: sinLUT[ j ] * ( radius + ( width * ps2 ) );
				
				ntemp.rotate( j * DEG_TO_RAD, zaxis );
			}
			else				// facing right.
			{
				fx1	= ofxTrigLUT :: cosLUT[ j ] * ( radius + ( width * ps2 ) );
				fy1	= ofxTrigLUT :: sinLUT[ j ] * ( radius + ( width * ps2 ) );
				fx2	= ofxTrigLUT :: cosLUT[ j ] * ( radius + ( width * ps1 ) );
				fy2	= ofxTrigLUT :: sinLUT[ j ] * ( radius + ( width * ps1 ) );
				
				ntemp.rotate( i * DEG_TO_RAD, zaxis );
				ntemp *= -1;
			}
			
			glNormal3f( ntemp.x, ntemp.y, ntemp.z );
			glVertex3f( fx1, fy1, 0 );
			glVertex3f( fx1, fy1, height );
			glVertex3f( fx2, fy2, height );
			glVertex3f( fx2, fy2, 0 );
		}
		
		if( ( ps1 > 0 ) || ( ps2 > 0 ) && ( ps1 != 0 ) )
		{
			// draw inner faces.
			ntemp.set( n1 );
			ntemp *= -1;
			glNormal3f( ntemp.x, ntemp.y, ntemp.z );
			glVertex3f( x1, y1, 0 );
			
			ntemp.set( n2 );
			ntemp *= -1;
			glNormal3f( ntemp.x, ntemp.y, ntemp.z );
			glVertex3f( x3, y3, 0 );
			glVertex3f( x3, y3, height );
			
			ntemp.set( n1 );
			ntemp *= -1;
			glNormal3f( ntemp.x, ntemp.y, ntemp.z );
			glVertex3f( x1, y1, height );
			
			// draw outer faces.
			ntemp.set( n1 );
			glNormal3f( ntemp.x, ntemp.y, ntemp.z );
			glVertex3f( x2, y2, 0 );
			
			ntemp.set( n2 );
			glNormal3f( ntemp.x, ntemp.y, ntemp.z );
			glVertex3f( x4, y4, 0 );
			glVertex3f( x4, y4, height );
			
			ntemp.set( n1 );
			glNormal3f( ntemp.x, ntemp.y, ntemp.z );
			glVertex3f( x2, y2, height );
			
			// draw bottom faces.
			glNormal3f( 0, 0, -1 );
			glVertex3f( x1, y1, 0 );
			glVertex3f( x3, y3, 0 );
			glVertex3f( x4, y4, 0 );
			glVertex3f( x2, y2, 0 );
			
			// draw top faces.
			glNormal3f( 0, 0, 1 );
			glVertex3f( x1, y1, height );
			glVertex3f( x3, y3, height );
			glVertex3f( x4, y4, height );
			glVertex3f( x2, y2, height );
		}
		
		glEnd();
	}
	
	glEndList();
}

void ArcBar :: createWireframe ()
{
	float x1, y1, x2, y2, x3, y3, x4, y4;
	float fx1, fy1, fx2, fy2;
	float ps1, ps2;
	int i, j;
	
	x1 = y1 = x2 = y2 = x3 = y3 = x4 = y4 = 0;
	fx1 = fy1 = fx2 = fy2 = 0;
	ps1 = 0;
	ps2 = 0;
	
	glNewList( id + 1, GL_COMPILE );
	glNormal3f( 0, 0, 0 );
	
	for( i = 0; i<angleStep; i++ ) 
	{
		j = ( i < angleStep - 1 ) ? ( i + 1 ) : 0;
		
//		ps1 = ps.perlinSteps[ i ];
//		ps2 = ps.perlinSteps[ j ];

		ps1 = 1.0;
		ps2 = 1.0;
		
		x1	= ofxTrigLUT :: cosLUT[ j ] * ( radius - wireframePad );
		y1	= ofxTrigLUT :: sinLUT[ j ] * ( radius - wireframePad );
		x2	= ofxTrigLUT :: cosLUT[ j ] * ( radius + wireframePad + ( width * ps1 ) );
		y2	= ofxTrigLUT :: sinLUT[ j ] * ( radius + wireframePad + ( width * ps1 ) );
		
		x3	= ofxTrigLUT :: cosLUT[ i ] * ( radius - wireframePad );
		y3	= ofxTrigLUT :: sinLUT[ i ] * ( radius - wireframePad );
		x4	= ofxTrigLUT :: cosLUT[ i ] * ( radius + wireframePad + ( width * ps1 ) );
		y4	= ofxTrigLUT :: sinLUT[ i ] * ( radius + wireframePad + ( width * ps1 ) );
		
//				if( ps1 != ps2 )
//				{
//					if( ps1 == 0 )
//					{
//						fx1	= cosLUT[ j ] * ( radius - wireframePad );
//						fy1	= sinLUT[ j ] * ( radius - wireframePad );
//						fx2	= cosLUT[ j ] * ( radius + wireframePad + ( width * ps2 ) );
//						fy2	= sinLUT[ j ] * ( radius + wireframePad + ( width * ps2 ) );
//					}
//					else if( ps2 == 0 )
//					{
//						fx1	= cosLUT[ j ] * ( radius + wireframePad + ( width * ps1 ) );
//						fy1	= sinLUT[ j ] * ( radius + wireframePad + ( width * ps1 ) );
//						fx2	= cosLUT[ j ] * ( radius - wireframePad );
//						fy2	= sinLUT[ j ] * ( radius - wireframePad );
//					}
//					
//					gl.glBegin( GL.GL_LINE_STRIP );		// draw side faces.
//						gl.glVertex3f( fx1, fy1, 0 - wireframePad );
//						gl.glVertex3f( fx1, fy1, height + wireframePad );
//						gl.glVertex3f( fx2, fy2, height + wireframePad );
//						gl.glVertex3f( fx2, fy2, 0 - wireframePad );
//						gl.glVertex3f( fx1, fy1, 0 - wireframePad );
//					gl.glEnd();
//				}
		
		if( ( ps1 > 0 ) || ( ( ps2 > 0 ) && ( ps1 > 0 ) ) )
		{
			glBegin( GL_LINE_STRIP );		// draw inner faces.
			glVertex3f( x1, y1, 0 - wireframePad );
			glVertex3f( x3, y3, 0 - wireframePad );
			glVertex3f( x3, y3, height + wireframePad );
			glVertex3f( x1, y1, height + wireframePad );
			glVertex3f( x1, y1, 0 - wireframePad );
			glEnd();
			
			glBegin( GL_LINE_STRIP );		// draw outer faces.
			glVertex3f( x2, y2, 0 - wireframePad );
			glVertex3f( x4, y4, 0 - wireframePad );
			glVertex3f( x4, y4, height + wireframePad );
			glVertex3f( x2, y2, height + wireframePad );
			glVertex3f( x2, y2, 0 - wireframePad );
			glEnd();
			
			glBegin( GL_LINE_STRIP );		// draw bottom faces.
			glVertex3f( x1, y1, 0 - wireframePad );
			glVertex3f( x3, y3, 0 - wireframePad );
			glVertex3f( x4, y4, 0 - wireframePad );
			glVertex3f( x2, y2, 0 - wireframePad );
			glVertex3f( x1, y1, 0 - wireframePad );
			glEnd();	
			
			glBegin( GL_LINE_STRIP );		// draw top faces.
			glVertex3f( x1, y1, height + wireframePad );
			glVertex3f( x3, y3, height + wireframePad );
			glVertex3f( x4, y4, height + wireframePad );
			glVertex3f( x2, y2, height + wireframePad );
			glVertex3f( x1, y1, height + wireframePad );
			glEnd();
		}
	}
	
	glEndList();
}

void ArcBar :: update ()
{
	rot += rInc;
}

void ArcBar :: renderSolidModel ()
{
	glPushMatrix();
//	glRotatef( 90, 1, 0, 0 );
	glTranslatef( loc.x, loc.y, loc.z );
	glTranslatef( dis.x, dis.y, dis.z );
	glRotatef( rot.z, 0, 0, 1 );
//	glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, &colour[ 0 ] );
	ofSetColor( (int)( colour[ 0 ] * 255 ), (int)( colour[ 1 ] * 255 ), (int)( colour[ 2 ] * 255 ) );
	glCallList( id );
	glPopMatrix();
}

void ArcBar :: renderWireframe ()
{
	glPushMatrix();
//	glRotatef( 90, 1, 0, 0 );
	glTranslatef( loc.x, loc.y, loc.z );
	glRotatef( rot.z, 0, 0, 1 );
//	glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, &wfColour[ 0 ] );
	ofSetColor( (int)( colour[ 0 ] * 255 ), (int)( colour[ 1 ] * 255 ), (int)( colour[ 2 ] * 255 ) );
	glLineWidth( 0.5f );
	glCallList( id + 1 );
	glPopMatrix();
}

void ArcBar :: render ()
{
	float x1, y1, x2, y2, x3, y3, x4, y4;
	float fx1, fy1, fx2, fy2;
	float ps1, ps2;
	ofxVec3f vtemp, ntemp, n1, n2;
	ofxVec3f vtemps[ 4 ];
	ofxVec3f zaxis;
	int i, j, k;
	
	x1 = y1 = x2 = y2 = x3 = y3 = x4 = y4 = 0;
	fx1 = fy1 = fx2 = fy2 = 0;
	ps1 = 0;
	ps2 = 0;
	
	zaxis.set( 0, 0, 1 );
	
	glPushMatrix();
	
	glRotatef( 90, 1, 0, 0 );
	glTranslatef( loc.x, loc.y, loc.z );
	glRotatef( rot.z, 0, 0, 1 );
//	glRotatef( 90, 0, 1, 0 );
//	glRotatef( 90, 1, 0, 0 );
	
	for( i = 0; i<angleStep; i++ ) 
	{
		j = ( i < angleStep - 1 ) ? ( i + 1 ) : 0;
		
//		ps1 = ps.perlinSteps[ i ];
//		ps2 = ps.perlinSteps[ j ];

		ps1 = 1.0;
		ps2 = 1.0;
		
		x1	= ofxTrigLUT :: cosLUT[ j ] * ( radius );
		y1	= ofxTrigLUT :: sinLUT[ j ] * ( radius );
		x2	= ofxTrigLUT :: cosLUT[ j ] * ( radius + ( width * ps1 ) );
		y2	= ofxTrigLUT :: sinLUT[ j ] * ( radius + ( width * ps1 ) );
		
		x3	= ofxTrigLUT :: cosLUT[ i ] * ( radius );
		y3	= ofxTrigLUT :: sinLUT[ i ] * ( radius );
		x4	= ofxTrigLUT :: cosLUT[ i ] * ( radius + ( width * ps1 ) );
		y4	= ofxTrigLUT :: sinLUT[ i ] * ( radius + ( width * ps1 ) );
		
		// normal pointing inwards.
		n1.set( 1, 0, 0 );
		n1.rotate( j * DEG_TO_RAD, zaxis );
		
		n2.set( 1, 0, 0 );
		n2.rotate( i * DEG_TO_RAD, zaxis );
		
		if( drawQuads )
		{
			glBegin( GL_QUADS );
			
			glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, &colour[ 0 ] );
			
			if( ps1 != ps2 )
			{
				// draw edge faces.
				ntemp.set( 0, -1, 0 );
				
				if( ps1 < ps2  )	// facing left.
				{
					fx1	= ofxTrigLUT :: cosLUT[ j ] * ( radius + ( width * ps1 ) );
					fy1	= ofxTrigLUT :: sinLUT[ j ] * ( radius + ( width * ps1 ) );
					fx2	= ofxTrigLUT :: cosLUT[ j ] * ( radius + ( width * ps2 ) );
					fy2	= ofxTrigLUT :: sinLUT[ j ] * ( radius + ( width * ps2 ) );
					
					ntemp.rotate( j * DEG_TO_RAD, zaxis );
				}
				else				// facing right.
				{
					fx1	= ofxTrigLUT :: cosLUT[ j ] * ( radius + ( width * ps2 ) );
					fy1	= ofxTrigLUT :: sinLUT[ j ] * ( radius + ( width * ps2 ) );
					fx2	= ofxTrigLUT :: cosLUT[ j ] * ( radius + ( width * ps1 ) );
					fy2	= ofxTrigLUT :: sinLUT[ j ] * ( radius + ( width * ps1 ) );
					
					ntemp.rotate( i * DEG_TO_RAD, zaxis );
					ntemp *= -1;
				}
				
				glNormal3f( ntemp.x, ntemp.y, ntemp.z );
				glVertex3f( fx1, fy1, 0 );
				glVertex3f( fx1, fy1, height );
				glVertex3f( fx2, fy2, height );
				glVertex3f( fx2, fy2, 0 );
			}
			
			if( ( ps1 > 0 ) || ( ps2 > 0 ) && ( ps1 != 0 ) )
			{
				// draw inner faces.
				ntemp.set( n1 );
				ntemp *= -1;
				glNormal3f( ntemp.x, ntemp.y, ntemp.z );
				glVertex3f( x1, y1, 0 );
				
				ntemp.set( n2 );
				ntemp *= -1;
				glNormal3f( ntemp.x, ntemp.y, ntemp.z );
				glVertex3f( x3, y3, 0 );
				glVertex3f( x3, y3, height );
				
				ntemp.set( n1 );
				ntemp *= -1;
				glNormal3f( ntemp.x, ntemp.y, ntemp.z );
				glVertex3f( x1, y1, height );
				
				// draw outer faces.
				ntemp.set( n1 );
				glNormal3f( ntemp.x, ntemp.y, ntemp.z );
				glVertex3f( x2, y2, 0 );
				
				ntemp.set( n2 );
				glNormal3f( ntemp.x, ntemp.y, ntemp.z );
				glVertex3f( x4, y4, 0 );
				glVertex3f( x4, y4, height );
				
				ntemp.set( n1 );
				glNormal3f( ntemp.x, ntemp.y, ntemp.z );
				glVertex3f( x2, y2, height );
				
				// draw bottom faces.
				glNormal3f( 0, 0, -1 );
				glVertex3f( x1, y1, 0 );
				glVertex3f( x3, y3, 0 );
				glVertex3f( x4, y4, 0 );
				glVertex3f( x2, y2, 0 );
				
				// draw top faces.
				glNormal3f( 0, 0, 1 );
				glVertex3f( x1, y1, height );
				glVertex3f( x3, y3, height );
				glVertex3f( x4, y4, height );
				glVertex3f( x2, y2, height );
			}
			
			glEnd();
		}
		
		if( drawNormals )
		{
			glBegin( GL_LINES );
			
			// draw end faces.
			ntemp.set( 0, -1, 0 );
			
			if( ps2 != ps1 )
			{
				if( ps2 > ps1 )
				{
					fx1	= ofxTrigLUT :: cosLUT[ j ] * ( radius + ( width * ps1 ) );
					fy1	= ofxTrigLUT :: sinLUT[ j ] * ( radius + ( width * ps1 ) );
					fx2	= ofxTrigLUT :: cosLUT[ j ] * ( radius + ( width * ps2 ) );
					fy2	= ofxTrigLUT :: sinLUT[ j ] * ( radius + ( width * ps2 ) );
					
					ntemp.rotate( j * DEG_TO_RAD, zaxis );
				}
				else
				{
					fx1	= ofxTrigLUT :: cosLUT[ i ] * ( radius + ( width * ps2 ) );
					fy1	= ofxTrigLUT :: sinLUT[ i ] * ( radius + ( width * ps2 ) );
					fx2	= ofxTrigLUT :: cosLUT[ i ] * ( radius + ( width * ps1 ) );
					fy2	= ofxTrigLUT :: sinLUT[ i ] * ( radius + ( width * ps1 ) );
					
					ntemp.rotate( i * DEG_TO_RAD, zaxis );
					ntemp *= -1;
				}
				
				vtemps[ 0 ].set( fx1, fy1, 0 );
				vtemps[ 1 ].set( fx2, fy2, 0 );
				vtemps[ 2 ].set( fx2, fy2, height );
				vtemps[ 3 ].set( fx1, fy1, height );
				
				for( k=0; k<4; k++ )
				{
					vtemp = vtemps[ k ];
					
					glVertex3f( vtemp.x, vtemp.y, vtemp.z );
					glVertex3f
					(
					 vtemp.x + ntemp.x * normLength,
					 vtemp.y + ntemp.y * normLength, 
					 vtemp.z + ntemp.z * normLength
					 );
				}
			}
			
			if( ( i != 0 ) && ( i % 20 == 0 ) )
			{
				// draw inner face normals.
				ntemp.set( n1 );
				ntemp *= -1;
				
				vtemps[ 0 ].set( x1, y1, 0 );
				vtemps[ 1 ].set( x3, y3, 0 );
				vtemps[ 2 ].set( x3, y3, height );
				vtemps[ 3 ].set( x1, y1, height );
				
				for( k=0; k<4; k++ )
				{
					vtemp = vtemps[ k ];
					
					glVertex3f( vtemp.x, vtemp.y, vtemp.z );
					glVertex3f
					(
					 vtemp.x + ntemp.x * normLength,
					 vtemp.y + ntemp.y * normLength, 
					 vtemp.z + ntemp.z * normLength
					 );
				}
				
				// draw outer face normals.
				ntemp.set( n1 );
				vtemps[ 0 ].set( x2, y2, 0 );
				vtemps[ 1 ].set( x4, y4, 0 );
				vtemps[ 2 ].set( x4, y4, height );
				vtemps[ 3 ].set( x2, y2, height );
				
				for( k=0; k<4; k++ )
				{
					vtemp = vtemps[ k ];
					
					glVertex3f( vtemp.x, vtemp.y, vtemp.z );
					glVertex3f
					(
					 vtemp.x + ntemp.x * normLength,
					 vtemp.y + ntemp.y * normLength, 
					 vtemp.z + ntemp.z * normLength
					 );
				}
				
				// draw bottom face normals.
				ntemp.set( 0, 0, -1 );
				vtemps[ 0 ].set( x1, y1, 0 );
				vtemps[ 1 ].set( x3, y3, 0 );
				vtemps[ 2 ].set( x4, y4, 0 );
				vtemps[ 3 ].set( x2, y2, 0 );
				
				for( k=0; k<4; k++ )
				{
					vtemp = vtemps[ k ];
					
					glVertex3f( vtemp.x, vtemp.y, vtemp.z );
					glVertex3f
					(
					 vtemp.x + ntemp.x * normLength,
					 vtemp.y + ntemp.y * normLength, 
					 vtemp.z + ntemp.z * normLength
					 );
				}
				
				// draw top face normals.
				ntemp.set( 0, 0, 1 );
				vtemps[ 0 ].set( x1, y1, height );
				vtemps[ 1 ].set( x3, y3, height );
				vtemps[ 2 ].set( x4, y4, height );
				vtemps[ 3 ].set( x2, y2, height );
				
				for( k=0; k<4; k++ )
				{
					vtemp = vtemps[ k ];
					
					glVertex3f( vtemp.x, vtemp.y, vtemp.z );
					glVertex3f
					(
					 vtemp.x + ntemp.x * normLength,
					 vtemp.y + ntemp.y * normLength, 
					 vtemp.z + ntemp.z * normLength
					 );
				}
			}
			
			glEnd();
		}
		
		if( drawAngles )
		{
			glBegin( GL_LINES );
			
			
			vtemp.set( x1, y1, 0 );
			vtemp.normalize();
			vtemp *= 700;
			
			glVertex3f( 0, 0, 0 );
			glVertex3f
			(
			 vtemp.x,
			 vtemp.y, 
			 vtemp.z
			 );
			
			glEnd();
		}
	}
	
	glPopMatrix();
}
