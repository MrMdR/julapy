package com.julapy.bats;

import java.awt.Dimension;
import java.awt.Toolkit;

import blobDetection.Blob;
import blobDetection.BlobDetection;
import blobDetection.EdgeVertex;
import processing.core.PApplet;
import processing.core.PImage;
import processing.video.Capture;

public class BlobDetectionTest extends PApplet
{
	
	int capWidth	= 320;
	int capHeight	= 240;

	Capture cap;
	PImage capImage;
	BlobDetection bd;
	
	static public void main( String args[] )
	{
		PApplet.main( 	new String[]
	                    {
							"--display=1",
							"--present",
							"--bgcolor=#000000",
							"--present-stop-color=#000000", 
							"com.julapy.bats.Main" 
						} 
		);
	}
	
	public void setup() 
	{
//		Dimension screen = Toolkit.getDefaultToolkit().getScreenSize();
//		size( screen.width, screen.height, OPENGL );
		size( 1280, 720, OPENGL );

		frameRate( 25 );
		
		background( 0 );
		
		// The name of the capture device is dependent those
		// plugged into the computer. To get a list of the 
		// choices, uncomment the following line 
		println( Capture.list() );
		
		cap			= new Capture( this, capWidth, capHeight, "USB Video Class Video", 25 );
		capImage	= new PImage( capWidth, capHeight );
		
		bd = new BlobDetection( capWidth, capHeight );
		bd.setPosDiscrimination( true );
		bd.setThreshold( 0.2f ); // will detect bright areas whose luminosity > 0.2f; 
	}

	public void draw() 
	{
		
		if( cap.available() )
		{
			background( 0 );

			cap.read();
			
			capImage = cap.get();

			image( cap, 0, 0, capWidth, capHeight );
			
			fastblur( capImage, 2 ); 
			bd.computeBlobs( capImage.pixels ); 
			
			noFill();
			strokeWeight( 1 ); 
			stroke( 255, 0, 0 ); 

			drawBlobAreas( capWidth, capHeight );
			
			noFill();
			strokeWeight( 3 ); 
			stroke( 0,255,0 );

			drawBlobEdges( capWidth, capHeight );

			noFill();
			strokeWeight( 3 ); 
			stroke( 255, 255, 255 ); 

			drawBlobAreas( width, height );
		}
	}
	
//	 ================================================== 
//	 drawBlobsAndEdges() 
//	 ================================================== 
	
	private void drawBlobAreas ( float w, float h )
	{
		Blob b; 
		int n;
		
		for ( n=0; n<bd.getBlobNb(); n++ ) 
		{ 
			b = bd.getBlob( n ); 
			
			if( b!=null ) 
			{ 
				rect( 
						b.xMin * w, b.yMin * h, 
						b.w * w, b.h * h 
					); 
			} 
		} 
	}
	
	private void drawBlobEdges ( float w, float h )
	{
		Blob b; 
		EdgeVertex eA,eB;
		int n, m;
		
		noFill();
		
		for ( n=0; n<bd.getBlobNb(); n++ ) 
		{ 
			b = bd.getBlob( n ); 
			
			if( b!=null ) 
			{ 
				for( m=0; m<b.getEdgeNb(); m++ ) 
				{ 
					eA = b.getEdgeVertexA( m ); 
					eB = b.getEdgeVertexB( m );
					
					if( eA !=null && eB !=null ) 
					{
						line( 
								eA.x * w, eA.y * h,
								eB.x * w, eB.y * h
							);
					}
				} 
			} 
		} 
	}
	 
//	 ================================================== 
//	 Super Fast Blur v1.1 
//	 by Mario Klingemann  
//	 <http://incubator.quasimondo.com> 
//	 ================================================== 
	private void fastblur( PImage img,int radius ) 
	{ 
		if (radius<1)
		{ 
			return; 
		} 

		int w=img.width; 
		int h=img.height; 
		int wm=w-1; 
		int hm=h-1; 
		int wh=w*h; 
		int div=radius+radius+1; 
		int r[]=new int[wh]; 
		int g[]=new int[wh]; 
		int b[]=new int[wh]; 
		int rsum,gsum,bsum,x,y,i,p,p1,p2,yp,yi,yw; 
		int vmin[] = new int[max(w,h)]; 
		int vmax[] = new int[max(w,h)]; 
		int[] pix=img.pixels; 
		
		int dv[]=new int[256*div]; 
		for (i=0;i<256*div;i++){ 
			dv[i]=(i/div); 
		} 
	 
		yw=yi=0; 
	 
		for (y=0;y<h;y++)
		{ 
			rsum=gsum=bsum=0;
			
			for(i=-radius;i<=radius;i++)
			{ 
				p=pix[yi+min(wm,max(i,0))]; 
				rsum+=(p & 0xff0000)>>16; 
				gsum+=(p & 0x00ff00)>>8; 
				bsum+= p & 0x0000ff; 
			} 
			for (x=0;x<w;x++)
			{ 
				r[yi]=dv[rsum]; 
				g[yi]=dv[gsum]; 
				b[yi]=dv[bsum]; 
		 
				if(y==0)
				{ 
					vmin[x]=min(x+radius+1,wm); 
					vmax[x]=max(x-radius,0); 
				} 
				p1=pix[yw+vmin[x]]; 
				p2=pix[yw+vmax[x]]; 
		 
				rsum+=((p1 & 0xff0000)-(p2 & 0xff0000))>>16; 
				gsum+=((p1 & 0x00ff00)-(p2 & 0x00ff00))>>8; 
				bsum+= (p1 & 0x0000ff)-(p2 & 0x0000ff); 
				yi++; 
			} 
			yw+=w; 
		} 
		 
		for (x=0;x<w;x++)
		{ 
			rsum=gsum=bsum=0; 
			yp=-radius*w; 
			for(i=-radius;i<=radius;i++)
			{ 
				yi=max(0,yp)+x; 
				rsum+=r[yi]; 
				gsum+=g[yi]; 
				bsum+=b[yi]; 
				yp+=w; 
			}
			
			yi=x;
			
			for (y=0;y<h;y++)
			{ 
				pix[yi]=0xff000000 | (dv[rsum]<<16) | (dv[gsum]<<8) | dv[bsum]; 
				if(x==0)
				{ 
					vmin[y]=min(y+radius+1,hm)*w; 
					vmax[y]=max(y-radius,0)*w; 
				} 
				p1=x+vmin[y]; 
				p2=x+vmax[y]; 
	 
				rsum+=r[p1]-r[p2]; 
				gsum+=g[p1]-g[p2]; 
				bsum+=b[p1]-b[p2]; 
	 
				yi+=w; 
			} 
		} 
	}
}
