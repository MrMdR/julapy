package com.julapy.cilindricoCollapse;

import javax.media.opengl.GL;

import processing.core.PApplet;

import toxi.geom.Vec3D;
import toxi.math.MathUtils;

import com.julapy.noise.PerlinStep;
import com.julapy.utils.TrigLookUpTable;

public class ArcBar
{
	GL gl;
	
	Vec3D loc	= new Vec3D();
	Vec3D vel	= new Vec3D();
	float rx	= 0;
	float ry	= 0;
	float rz	= 0;
	float rInc	= 2;
	float dx	= 0;
	float dy	= 0;
	float dz	= 0;
	
	float[] colour		= { 1, 1, 1, 1 };
	float[] wfColour	= { 0.4f, 0.4f, 0.4f, 0.8f };
	
	int   id				= 0;
	float angle				= 200;
	int	  angleStep			= 200;
	float radius			= 300;
	float width				= 50;
	float height			= 50;
	float scale				= 1;
	float wireframePad		= 10f;
	
	PerlinStep ps;
	
	float normLength = 20;
	
	Boolean drawQuads	= true;
	Boolean drawNormals = false;
	Boolean drawAngles	= false;
	
	Boolean isTiling	= false;
	
	public ArcBar ()
	{
		rx = MathUtils.random( 2 * MathUtils.PI );
		ry = MathUtils.random( 2 * MathUtils.PI );
		rz = MathUtils.random( 2 * MathUtils.PI );
	}
	
	public ArcBar (float rx, float ry, float rz)
	{
		this.rx = rx;
		this.ry = ry;
		this.rz = rz;
	}

	public ArcBar (float rx, float ry, float rz, float cr, float cg, float cb )
	{
		this.rx = rx;
		this.ry = ry;
		this.rz = rz;
		
		colour[ 0 ] = cr;
		colour[ 1 ] = cg;
		colour[ 2 ] = cb;
	}
	
	public void init ()
	{
		angleStep	= (int)( angle / TrigLookUpTable.SINCOS_PRECISION );
		ps			= new PerlinStep( angleStep, 1.5f, 5, 30 );
	}
	
	public void createSolidModel ()
	{
		float x1, y1, x2, y2, x3, y3, x4, y4;
		float fx1, fy1, fx2, fy2;
		float ps1, ps2;
		Vec3D ntemp, n1, n2;
		int i, j;
		
		x1 = y1 = x2 = y2 = x3 = y3 = x4 = y4 = 0;
		fx1 = fy1 = fx2 = fy2 = 0;
		n1 = n2 = new Vec3D();
		ps1 = 0;
		ps2 = 0;
		
		gl.glNewList( id, GL.GL_COMPILE );
		
		for( i = 0; i<angleStep; i++ ) 
		{
			j = ( i < angleStep - 1 ) ? ( i + 1 ) : 0;
			
			ps1 = ps.perlinSteps[ i ];
			ps2 = ps.perlinSteps[ j ];
			
			x1	= TrigLookUpTable.cosLUT[ j ] * ( radius );
			y1	= TrigLookUpTable.sinLUT[ j ] * ( radius );
			x2	= TrigLookUpTable.cosLUT[ j ] * ( radius + ( width * ps1 ) );
			y2	= TrigLookUpTable.sinLUT[ j ] * ( radius + ( width * ps1 ) );
			
			x3	= TrigLookUpTable.cosLUT[ i ] * ( radius );
			y3	= TrigLookUpTable.sinLUT[ i ] * ( radius );
			x4	= TrigLookUpTable.cosLUT[ i ] * ( radius + ( width * ps1 ) );
			y4	= TrigLookUpTable.sinLUT[ i ] * ( radius + ( width * ps1 ) );
			
			// normal pointing inwards.
			n1	= new Vec3D( 1, 0, 0 );
			n1	= n1.rotateAroundAxis( Vec3D.Z_AXIS, j * PApplet.DEG_TO_RAD );
			
			n2  = new Vec3D( 1, 0, 0 );
			n2	= n2.rotateAroundAxis( Vec3D.Z_AXIS, i * PApplet.DEG_TO_RAD );
			
			gl.glBegin( GL.GL_QUADS );
			
			if( ps1 != ps2 )
			{
				// draw edge faces.
				ntemp = new Vec3D( 0, -1, 0 );
				
				if( ps1 < ps2  )	// facing left.
				{
					fx1	= TrigLookUpTable.cosLUT[ j ] * ( radius + ( width * ps1 ) );
					fy1	= TrigLookUpTable.sinLUT[ j ] * ( radius + ( width * ps1 ) );
					fx2	= TrigLookUpTable.cosLUT[ j ] * ( radius + ( width * ps2 ) );
					fy2	= TrigLookUpTable.sinLUT[ j ] * ( radius + ( width * ps2 ) );
					
					ntemp = ntemp.rotateAroundAxis( Vec3D.Z_AXIS.copy(), j * PApplet.DEG_TO_RAD );
				}
				else				// facing right.
				{
					fx1	= TrigLookUpTable.cosLUT[ j ] * ( radius + ( width * ps2 ) );
					fy1	= TrigLookUpTable.sinLUT[ j ] * ( radius + ( width * ps2 ) );
					fx2	= TrigLookUpTable.cosLUT[ j ] * ( radius + ( width * ps1 ) );
					fy2	= TrigLookUpTable.sinLUT[ j ] * ( radius + ( width * ps1 ) );
					
					ntemp = ntemp.rotateAroundAxis( Vec3D.Z_AXIS.copy(), i * PApplet.DEG_TO_RAD );
					ntemp.invert();
				}
				
				gl.glNormal3f( ntemp.x, ntemp.y, ntemp.z );
				gl.glVertex3f( fx1, fy1, 0 );
				gl.glVertex3f( fx1, fy1, height );
				gl.glVertex3f( fx2, fy2, height );
				gl.glVertex3f( fx2, fy2, 0 );
			}
			
			if( ( ps1 > 0 ) || ( ps2 > 0 ) && ( ps1 != 0 ) )
			{
				// draw inner faces.
				ntemp = n1.copy().invert();
				gl.glNormal3f( ntemp.x, ntemp.y, ntemp.z );
				gl.glVertex3f( x1, y1, 0 );
				
				ntemp = n2.copy().invert();
				gl.glNormal3f( ntemp.x, ntemp.y, ntemp.z );
				gl.glVertex3f( x3, y3, 0 );
				gl.glVertex3f( x3, y3, height );

				ntemp = n1.copy().invert();
				gl.glNormal3f( ntemp.x, ntemp.y, ntemp.z );
				gl.glVertex3f( x1, y1, height );

				// draw outer faces.
				ntemp = n1.copy();
				gl.glNormal3f( ntemp.x, ntemp.y, ntemp.z );
				gl.glVertex3f( x2, y2, 0 );

				ntemp = n2.copy();
				gl.glNormal3f( ntemp.x, ntemp.y, ntemp.z );
				gl.glVertex3f( x4, y4, 0 );
				gl.glVertex3f( x4, y4, height );
				
				ntemp = n1.copy();
				gl.glNormal3f( ntemp.x, ntemp.y, ntemp.z );
				gl.glVertex3f( x2, y2, height );

				// draw bottom faces.
				gl.glNormal3f( 0, 0, -1 );
				gl.glVertex3f( x1, y1, 0 );
				gl.glVertex3f( x3, y3, 0 );
				gl.glVertex3f( x4, y4, 0 );
				gl.glVertex3f( x2, y2, 0 );

				// draw top faces.
				gl.glNormal3f( 0, 0, 1 );
				gl.glVertex3f( x1, y1, height );
				gl.glVertex3f( x3, y3, height );
				gl.glVertex3f( x4, y4, height );
				gl.glVertex3f( x2, y2, height );
			}

			gl.glEnd();
		}
		
		gl.glEndList();
	}
	
	public void createWireframe ()
	{
		float x1, y1, x2, y2, x3, y3, x4, y4;
		float fx1, fy1, fx2, fy2;
		float ps1, ps2;
		int i, j;
		
		x1 = y1 = x2 = y2 = x3 = y3 = x4 = y4 = 0;
		fx1 = fy1 = fx2 = fy2 = 0;
		ps1 = 0;
		ps2 = 0;
		
		gl.glNewList( id + 1, GL.GL_COMPILE );
		gl.glNormal3f( 0, 0, 0 );
		
		for( i = 0; i<angleStep; i++ ) 
		{
			j = ( i < angleStep - 1 ) ? ( i + 1 ) : 0;
			
			ps1 = ps.perlinSteps[ i ];
			ps2 = ps.perlinSteps[ j ];
			
			x1	= TrigLookUpTable.cosLUT[ j ] * ( radius - wireframePad );
			y1	= TrigLookUpTable.sinLUT[ j ] * ( radius - wireframePad );
			x2	= TrigLookUpTable.cosLUT[ j ] * ( radius + wireframePad + ( width * ps1 ) );
			y2	= TrigLookUpTable.sinLUT[ j ] * ( radius + wireframePad + ( width * ps1 ) );
			
			x3	= TrigLookUpTable.cosLUT[ i ] * ( radius - wireframePad );
			y3	= TrigLookUpTable.sinLUT[ i ] * ( radius - wireframePad );
			x4	= TrigLookUpTable.cosLUT[ i ] * ( radius + wireframePad + ( width * ps1 ) );
			y4	= TrigLookUpTable.sinLUT[ i ] * ( radius + wireframePad + ( width * ps1 ) );
			
//			if( ps1 != ps2 )
//			{
//				if( ps1 == 0 )
//				{
//					fx1	= cosLUT[ j ] * ( radius - wireframePad );
//					fy1	= sinLUT[ j ] * ( radius - wireframePad );
//					fx2	= cosLUT[ j ] * ( radius + wireframePad + ( width * ps2 ) );
//					fy2	= sinLUT[ j ] * ( radius + wireframePad + ( width * ps2 ) );
//				}
//				else if( ps2 == 0 )
//				{
//					fx1	= cosLUT[ j ] * ( radius + wireframePad + ( width * ps1 ) );
//					fy1	= sinLUT[ j ] * ( radius + wireframePad + ( width * ps1 ) );
//					fx2	= cosLUT[ j ] * ( radius - wireframePad );
//					fy2	= sinLUT[ j ] * ( radius - wireframePad );
//				}
//				
//				gl.glBegin( GL.GL_LINE_STRIP );		// draw side faces.
//					gl.glVertex3f( fx1, fy1, 0 - wireframePad );
//					gl.glVertex3f( fx1, fy1, height + wireframePad );
//					gl.glVertex3f( fx2, fy2, height + wireframePad );
//					gl.glVertex3f( fx2, fy2, 0 - wireframePad );
//					gl.glVertex3f( fx1, fy1, 0 - wireframePad );
//				gl.glEnd();
//			}
			
			if( ( ps1 > 0 ) || ( ( ps2 > 0 ) && ( ps1 > 0 ) ) )
			{
				gl.glBegin( GL.GL_LINE_STRIP );		// draw inner faces.
					gl.glVertex3f( x1, y1, 0 - wireframePad );
					gl.glVertex3f( x3, y3, 0 - wireframePad );
					gl.glVertex3f( x3, y3, height + wireframePad );
					gl.glVertex3f( x1, y1, height + wireframePad );
					gl.glVertex3f( x1, y1, 0 - wireframePad );
				gl.glEnd();

				gl.glBegin( GL.GL_LINE_STRIP );		// draw outer faces.
					gl.glVertex3f( x2, y2, 0 - wireframePad );
					gl.glVertex3f( x4, y4, 0 - wireframePad );
					gl.glVertex3f( x4, y4, height + wireframePad );
					gl.glVertex3f( x2, y2, height + wireframePad );
					gl.glVertex3f( x2, y2, 0 - wireframePad );
				gl.glEnd();

				gl.glBegin( GL.GL_LINE_STRIP );		// draw bottom faces.
					gl.glVertex3f( x1, y1, 0 - wireframePad );
					gl.glVertex3f( x3, y3, 0 - wireframePad );
					gl.glVertex3f( x4, y4, 0 - wireframePad );
					gl.glVertex3f( x2, y2, 0 - wireframePad );
					gl.glVertex3f( x1, y1, 0 - wireframePad );
				gl.glEnd();	

				gl.glBegin( GL.GL_LINE_STRIP );		// draw top faces.
					gl.glVertex3f( x1, y1, height + wireframePad );
					gl.glVertex3f( x3, y3, height + wireframePad );
					gl.glVertex3f( x4, y4, height + wireframePad );
					gl.glVertex3f( x2, y2, height + wireframePad );
					gl.glVertex3f( x1, y1, height + wireframePad );
				gl.glEnd();
			}
		}
		
		gl.glEndList();
	}
	
	public void update ()
	{
		rx += rInc;
		ry += rInc;
		rz += rInc;
	}
	
	public void renderSolidModel ()
	{
		gl.glPushMatrix();
			gl.glRotatef( 90, 1, 0, 0 );
			gl.glTranslatef( loc.x, loc.y, loc.z );
			gl.glTranslatef( dx, dy, dz );
			gl.glRotatef( rz, 0, 0, 1 );
			gl.glMaterialfv( GL.GL_FRONT, GL.GL_AMBIENT_AND_DIFFUSE, colour, 0 );
			gl.glCallList( id );
		gl.glPopMatrix();
	}
	
	public void renderWireframe ()
	{
		gl.glPushMatrix();
			gl.glRotatef( 90, 1, 0, 0 );
			gl.glTranslatef( loc.x, loc.y, loc.z );
			gl.glRotatef( rz, 0, 0, 1 );
			gl.glMaterialfv( GL.GL_FRONT, GL.GL_AMBIENT_AND_DIFFUSE, wfColour, 0 );
			if( isTiling )
			{
				gl.glLineWidth( 4 );
			}
			else
			{
				gl.glLineWidth( 0.5f );
			}
			gl.glCallList( id + 1 );
		gl.glPopMatrix();
	}
	
	public void render ()
	{
		float x1, y1, x2, y2, x3, y3, x4, y4;
		float fx1, fy1, fx2, fy2;
		float ps1, ps2;
		Vec3D vtemp, ntemp, n1, n2;
		Vec3D[] vtemps;
		int i, j, k;
		
		x1 = y1 = x2 = y2 = x3 = y3 = x4 = y4 = 0;
		fx1 = fy1 = fx2 = fy2 = 0;
		n1 = n2 = new Vec3D();
		ps1 = 0;
		ps2 = 0;
		
		gl.glPushMatrix();

		gl.glRotatef( 90, 1, 0, 0 );
		gl.glTranslatef( loc.x, loc.y, loc.z );
		gl.glRotatef( rz, 0, 0, 1 );
//		gl.glRotatef( 90, 0, 1, 0 );
//		gl.glRotatef( 90, 1, 0, 0 );
		
		for( i = 0; i<angleStep; i++ ) 
		{
			j = ( i < angleStep - 1 ) ? ( i + 1 ) : 0;
			
			ps1 = ps.perlinSteps[ i ];
			ps2 = ps.perlinSteps[ j ];
			
			x1	= TrigLookUpTable.cosLUT[ j ] * ( radius );
			y1	= TrigLookUpTable.sinLUT[ j ] * ( radius );
			x2	= TrigLookUpTable.cosLUT[ j ] * ( radius + ( width * ps1 ) );
			y2	= TrigLookUpTable.sinLUT[ j ] * ( radius + ( width * ps1 ) );
			
			x3	= TrigLookUpTable.cosLUT[ i ] * ( radius );
			y3	= TrigLookUpTable.sinLUT[ i ] * ( radius );
			x4	= TrigLookUpTable.cosLUT[ i ] * ( radius + ( width * ps1 ) );
			y4	= TrigLookUpTable.sinLUT[ i ] * ( radius + ( width * ps1 ) );
			
			// normal pointing inwards.
			n1	= new Vec3D( 1, 0, 0 );
			n1	= n1.rotateAroundAxis( Vec3D.Z_AXIS, j * PApplet.DEG_TO_RAD );
			
			n2  = new Vec3D( 1, 0, 0 );
			n2	= n2.rotateAroundAxis( Vec3D.Z_AXIS, i * PApplet.DEG_TO_RAD );
			
			if( drawQuads )
			{
				gl.glBegin( GL.GL_QUADS );
				
				gl.glMaterialfv( GL.GL_FRONT, GL.GL_AMBIENT_AND_DIFFUSE, colour, 0 );
				
				if( ps1 != ps2 )
				{
					// draw edge faces.
					ntemp = new Vec3D( 0, -1, 0 );
					
					if( ps1 < ps2  )	// facing left.
					{
						fx1	= TrigLookUpTable.cosLUT[ j ] * ( radius + ( width * ps1 ) );
						fy1	= TrigLookUpTable.sinLUT[ j ] * ( radius + ( width * ps1 ) );
						fx2	= TrigLookUpTable.cosLUT[ j ] * ( radius + ( width * ps2 ) );
						fy2	= TrigLookUpTable.sinLUT[ j ] * ( radius + ( width * ps2 ) );
						
						ntemp = ntemp.rotateAroundAxis( Vec3D.Z_AXIS.copy(), j * PApplet.DEG_TO_RAD );
					}
					else				// facing right.
					{
						fx1	= TrigLookUpTable.cosLUT[ j ] * ( radius + ( width * ps2 ) );
						fy1	= TrigLookUpTable.sinLUT[ j ] * ( radius + ( width * ps2 ) );
						fx2	= TrigLookUpTable.cosLUT[ j ] * ( radius + ( width * ps1 ) );
						fy2	= TrigLookUpTable.sinLUT[ j ] * ( radius + ( width * ps1 ) );
						
						ntemp = ntemp.rotateAroundAxis( Vec3D.Z_AXIS.copy(), i * PApplet.DEG_TO_RAD );
						ntemp.invert();
					}
					
					gl.glNormal3f( ntemp.x, ntemp.y, ntemp.z );
					gl.glVertex3f( fx1, fy1, 0 );
					gl.glVertex3f( fx1, fy1, height );
					gl.glVertex3f( fx2, fy2, height );
					gl.glVertex3f( fx2, fy2, 0 );
				}
				
				if( ( ps1 > 0 ) || ( ps2 > 0 ) && ( ps1 != 0 ) )
				{
					// draw inner faces.
					ntemp = n1.copy().invert();
					gl.glNormal3f( ntemp.x, ntemp.y, ntemp.z );
					gl.glVertex3f( x1, y1, 0 );
					
					ntemp = n2.copy().invert();
					gl.glNormal3f( ntemp.x, ntemp.y, ntemp.z );
					gl.glVertex3f( x3, y3, 0 );
					gl.glVertex3f( x3, y3, height );

					ntemp = n1.copy().invert();
					gl.glNormal3f( ntemp.x, ntemp.y, ntemp.z );
					gl.glVertex3f( x1, y1, height );

					// draw outer faces.
					ntemp = n1.copy();
					gl.glNormal3f( ntemp.x, ntemp.y, ntemp.z );
					gl.glVertex3f( x2, y2, 0 );

					ntemp = n2.copy();
					gl.glNormal3f( ntemp.x, ntemp.y, ntemp.z );
					gl.glVertex3f( x4, y4, 0 );
					gl.glVertex3f( x4, y4, height );
					
					ntemp = n1.copy();
					gl.glNormal3f( ntemp.x, ntemp.y, ntemp.z );
					gl.glVertex3f( x2, y2, height );

					// draw bottom faces.
					gl.glNormal3f( 0, 0, -1 );
					gl.glVertex3f( x1, y1, 0 );
					gl.glVertex3f( x3, y3, 0 );
					gl.glVertex3f( x4, y4, 0 );
					gl.glVertex3f( x2, y2, 0 );

					// draw top faces.
					gl.glNormal3f( 0, 0, 1 );
					gl.glVertex3f( x1, y1, height );
					gl.glVertex3f( x3, y3, height );
					gl.glVertex3f( x4, y4, height );
					gl.glVertex3f( x2, y2, height );
				}
				
				gl.glEnd();
			}
			
			if( drawNormals )
			{
				gl.glBegin( GL.GL_LINES );
				
				// draw end faces.
				ntemp = new Vec3D( 0, -1, 0 );
				
				if( ps2 != ps1 )
				{
					if( ps2 > ps1 )
					{
						fx1	= TrigLookUpTable.cosLUT[ j ] * ( radius + ( width * ps1 ) );
						fy1	= TrigLookUpTable.sinLUT[ j ] * ( radius + ( width * ps1 ) );
						fx2	= TrigLookUpTable.cosLUT[ j ] * ( radius + ( width * ps2 ) );
						fy2	= TrigLookUpTable.sinLUT[ j ] * ( radius + ( width * ps2 ) );

						ntemp = ntemp.rotateAroundAxis( Vec3D.Z_AXIS.copy(), j * PApplet.DEG_TO_RAD );
					}
					else
					{
						fx1	= TrigLookUpTable.cosLUT[ i ] * ( radius + ( width * ps2 ) );
						fy1	= TrigLookUpTable.sinLUT[ i ] * ( radius + ( width * ps2 ) );
						fx2	= TrigLookUpTable.cosLUT[ i ] * ( radius + ( width * ps1 ) );
						fy2	= TrigLookUpTable.sinLUT[ i ] * ( radius + ( width * ps1 ) );
						
						ntemp = ntemp.rotateAroundAxis( Vec3D.Z_AXIS.copy(), i * PApplet.DEG_TO_RAD );
						ntemp.invert();
					}
					
					vtemps		= new Vec3D[ 4 ];
					vtemps[ 0 ]	= new Vec3D( fx1, fy1, 0 );
					vtemps[ 1 ]	= new Vec3D( fx2, fy2, 0 );
					vtemps[ 2 ]	= new Vec3D( fx2, fy2, height );
					vtemps[ 3 ]	= new Vec3D( fx1, fy1, height );
					
					for( k=0; k<vtemps.length; k++ )
					{
						vtemp = vtemps[ k ];
						
						gl.glVertex3f( vtemp.x, vtemp.y, vtemp.z );
						gl.glVertex3f
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
					ntemp		= n1.copy().invert();
					vtemps		= new Vec3D[ 4 ];
					vtemps[ 0 ]	= new Vec3D( x1, y1, 0 );
					vtemps[ 1 ]	= new Vec3D( x3, y3, 0 );
					vtemps[ 2 ]	= new Vec3D( x3, y3, height );
					vtemps[ 3 ]	= new Vec3D( x1, y1, height );
					
					for( k=0; k<vtemps.length; k++ )
					{
						vtemp = vtemps[ k ];
						
						gl.glVertex3f( vtemp.x, vtemp.y, vtemp.z );
						gl.glVertex3f
						(
							vtemp.x + ntemp.x * normLength,
							vtemp.y + ntemp.y * normLength, 
							vtemp.z + ntemp.z * normLength
						);
					}
					
					// draw outer face normals.
					ntemp		= n1.copy();
					vtemps		= new Vec3D[ 4 ];
					vtemps[ 0 ]	= new Vec3D( x2, y2, 0 );
					vtemps[ 1 ]	= new Vec3D( x4, y4, 0 );
					vtemps[ 2 ]	= new Vec3D( x4, y4, height );
					vtemps[ 3 ]	= new Vec3D( x2, y2, height );
					
					for( k=0; k<vtemps.length; k++ )
					{
						vtemp = vtemps[ k ];
						
						gl.glVertex3f( vtemp.x, vtemp.y, vtemp.z );
						gl.glVertex3f
						(
							vtemp.x + ntemp.x * normLength,
							vtemp.y + ntemp.y * normLength, 
							vtemp.z + ntemp.z * normLength
						);
					}
					
					// draw bottom face normals.
					ntemp		= new Vec3D( 0, 0, -1 );
					vtemps		= new Vec3D[ 4 ];
					vtemps[ 0 ]	= new Vec3D( x1, y1, 0 );
					vtemps[ 1 ]	= new Vec3D( x3, y3, 0 );
					vtemps[ 2 ]	= new Vec3D( x4, y4, 0 );
					vtemps[ 3 ]	= new Vec3D( x2, y2, 0 );
					
					for( k=0; k<vtemps.length; k++ )
					{
						vtemp = vtemps[ k ];
						
						gl.glVertex3f( vtemp.x, vtemp.y, vtemp.z );
						gl.glVertex3f
						(
							vtemp.x + ntemp.x * normLength,
							vtemp.y + ntemp.y * normLength, 
							vtemp.z + ntemp.z * normLength
						);
					}
					
					// draw top face normals.
					ntemp		= new Vec3D( 0, 0, 1 );
					vtemps		= new Vec3D[ 4 ];
					vtemps[ 0 ]	= new Vec3D( x1, y1, height );
					vtemps[ 1 ]	= new Vec3D( x3, y3, height );
					vtemps[ 2 ]	= new Vec3D( x4, y4, height );
					vtemps[ 3 ]	= new Vec3D( x2, y2, height );
					
					for( k=0; k<vtemps.length; k++ )
					{
						vtemp = vtemps[ k ];
						
						gl.glVertex3f( vtemp.x, vtemp.y, vtemp.z );
						gl.glVertex3f
						(
							vtemp.x + ntemp.x * normLength,
							vtemp.y + ntemp.y * normLength, 
							vtemp.z + ntemp.z * normLength
						);
					}
				}
				
				gl.glEnd();
			}
			
			if( drawAngles )
			{
				gl.glBegin( GL.GL_LINES );
				
				
				vtemp = new Vec3D( x1, y1, 0 );
				vtemp.normalize();
				vtemp.scaleSelf( 700 );
				
				gl.glVertex3f( 0, 0, 0 );
				gl.glVertex3f
				(
					vtemp.x,
					vtemp.y, 
					vtemp.z
				);
				
				gl.glEnd();
			}
		}
		
		gl.glPopMatrix();
	}
}	
