package com.julapy.opengl;

import javax.media.opengl.GL;
import toxi.geom.Vec3D;
import toxi.math.MathUtils;

public class Primitive 
{
	GL gl;
	
	public Primitive( GL gl )
	{
		this.gl = gl;
	}
	
	public void drawCube()
	{
        // Front Face
        gl.glTexCoord2f(0.0f, 0.0f);
        gl.glVertex3f(-1.0f, -1.0f, 1.0f);
        gl.glTexCoord2f(1.0f, 0.0f);
        gl.glVertex3f(1.0f, -1.0f, 1.0f);
        gl.glTexCoord2f(1.0f, 1.0f);
        gl.glVertex3f(1.0f, 1.0f, 1.0f);
        gl.glTexCoord2f(0.0f, 1.0f);
        gl.glVertex3f(-1.0f, 1.0f, 1.0f);
        // Back Face
        gl.glTexCoord2f(1.0f, 0.0f);
        gl.glVertex3f(-1.0f, -1.0f, -1.0f);
        gl.glTexCoord2f(1.0f, 1.0f);
        gl.glVertex3f(-1.0f, 1.0f, -1.0f);
        gl.glTexCoord2f(0.0f, 1.0f);
        gl.glVertex3f(1.0f, 1.0f, -1.0f);
        gl.glTexCoord2f(0.0f, 0.0f);
        gl.glVertex3f(1.0f, -1.0f, -1.0f);
        // Top Face
        gl.glTexCoord2f(0.0f, 1.0f);
        gl.glVertex3f(-1.0f, 1.0f, -1.0f);
        gl.glTexCoord2f(0.0f, 0.0f);
        gl.glVertex3f(-1.0f, 1.0f, 1.0f);
        gl.glTexCoord2f(1.0f, 0.0f);
        gl.glVertex3f(1.0f, 1.0f, 1.0f);
        gl.glTexCoord2f(1.0f, 1.0f);
        gl.glVertex3f(1.0f, 1.0f, -1.0f);
        // Bottom Face
        gl.glTexCoord2f(1.0f, 1.0f);
        gl.glVertex3f(-1.0f, -1.0f, -1.0f);
        gl.glTexCoord2f(0.0f, 1.0f);
        gl.glVertex3f(1.0f, -1.0f, -1.0f);
        gl.glTexCoord2f(0.0f, 0.0f);
        gl.glVertex3f(1.0f, -1.0f, 1.0f);
        gl.glTexCoord2f(1.0f, 0.0f);
        gl.glVertex3f(-1.0f, -1.0f, 1.0f);
        // Right face
        gl.glTexCoord2f(1.0f, 0.0f);
        gl.glVertex3f(1.0f, -1.0f, -1.0f);
        gl.glTexCoord2f(1.0f, 1.0f);
        gl.glVertex3f(1.0f, 1.0f, -1.0f);
        gl.glTexCoord2f(0.0f, 1.0f);
        gl.glVertex3f(1.0f, 1.0f, 1.0f);
        gl.glTexCoord2f(0.0f, 0.0f);
        gl.glVertex3f(1.0f, -1.0f, 1.0f);
        // Left Face
        gl.glTexCoord2f(0.0f, 0.0f);
        gl.glVertex3f(-1.0f, -1.0f, -1.0f);
        gl.glTexCoord2f(1.0f, 0.0f);
        gl.glVertex3f(-1.0f, -1.0f, 1.0f);
        gl.glTexCoord2f(1.0f, 1.0f);
        gl.glVertex3f(-1.0f, 1.0f, 1.0f);
        gl.glTexCoord2f(0.0f, 1.0f);
        gl.glVertex3f(-1.0f, 1.0f, -1.0f);
	}
	
	public void drawSphere ( int n )
	{
	   int i,j;
	   float theta1, theta2, theta3;
	   Vec3D e = new Vec3D();
	   Vec3D p = new Vec3D();
	   
	   if (n < 0)
	      n = -n;
	   if (n < 4) 
	   {
	      gl.glBegin( GL.GL_POINTS );
	      gl.glColor3f( 1, 1, 1 );
	      gl.glVertex3f( 0.0f, 0.0f, 0.0f);
	      gl.glEnd();
	      
	      return;
	   }

	   for (j=0;j<n/2;j++) 
	   {
	      theta1 = j * 2*MathUtils.PI / n - MathUtils.PI/2;
	      theta2 = (j + 1) * 2*MathUtils.PI / n - MathUtils.PI/2;

	      gl.glBegin( GL.GL_TRIANGLE_STRIP );
	      for (i=0;i<=n;i++) 
	      {
	    	 theta3 = i * 2*MathUtils.PI / n;

	         e.x = (float)( Math.cos(theta2) * Math.cos(theta3));
	         e.y = (float)( Math.sin(theta2) );
	         e.z = (float)( Math.cos(theta2) * Math.sin(theta3) );
	         p.x = e.x;
	         p.y = e.y;
	         p.z = e.z;

	         gl.glNormal3f( e.x, e.y, e.z );
	         gl.glTexCoord2f( i/(float)n, 2*(j+1)/(float)n);
	         gl.glVertex3f(p.x,p.y,p.z);

	         e.x = (float)( Math.cos(theta1) * Math.cos(theta3) );
	         e.y = (float)( Math.sin(theta1) );
	         e.z = (float)( Math.cos(theta1) * Math.sin(theta3) );
	         p.x = e.x;
	         p.y = e.y;
	         p.z = e.z;

	         gl.glNormal3f(e.x,e.y,e.z);
	         gl.glTexCoord2f( i/(float)n, 2*j/(float)n );
	         gl.glVertex3f(p.x,p.y,p.z);
	      }
	      gl.glEnd();
	   }
	}
}
