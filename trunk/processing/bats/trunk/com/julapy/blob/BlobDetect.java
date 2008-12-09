package com.julapy.blob;

import blobDetection.Blob;
import blobDetection.BlobDetection;
import blobDetection.EdgeVertex;
import processing.core.PApplet;
import processing.core.PImage;
import processing.video.Capture;

public class BlobDetect
{
	private PApplet papp;
	private Capture cap;
	private PImage capImage;
	private BlobDetection bd;
	
	private int capWidth;
	private int capHeight;
	
	public boolean showEdges = true;
	public boolean showAreas = true;
	public boolean showVideo = true;
	
	public BlobDetect ( PApplet papp, Capture cap )
	{
		this.papp	= papp;
		this.cap	= cap;
		
		capWidth	= cap.width;
		capHeight	= cap.height;

		capImage	= new PImage( cap.width, cap.width );
		
		bd = new BlobDetection( capWidth, capHeight );
		bd.setPosDiscrimination( true );
		bd.setThreshold( 0.8f ); // will detect bright areas whose luminosity > 0.2f; 
	}
	
	public void update ()
	{
		if( cap.available() )
		{
			cap.read();
			
			capImage = cap.get();

			fastblur( capImage, 2 ); 
			bd.computeBlobs( capImage.pixels ); 
		}
	}
	
	public void draw ()
	{
		if( showVideo )
		{
			papp.image( cap, 0, 0, capWidth, capHeight );
		}
		
		if( showAreas )
		{
			papp.noFill();
			papp.strokeWeight( 1 );
			papp.stroke( 255, 0, 0 );
			
			drawBlobAreas( capWidth, capHeight );
		}
		
		if( showEdges )
		{
			papp.noFill();
			papp.strokeWeight( 3 ); 
			papp.stroke( 0,255,0 );
			
			drawBlobEdges( capWidth, capHeight );
		}
		
		if( showAreas )
		{
			papp.noFill();
			papp.strokeWeight( 3 ); 
			papp.stroke( 255, 255, 255 ); 
			
			drawBlobAreas( papp.width, papp.height );
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
				papp.rect
					( 
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
						papp.line
							( 
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
		int vmin[] = new int[Math.max(w,h)]; 
		int vmax[] = new int[Math.max(w,h)]; 
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
				p=pix[yi+Math.min(wm,Math.max(i,0))]; 
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
					vmin[x]=Math.min(x+radius+1,wm); 
					vmax[x]=Math.max(x-radius,0); 
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
				yi=Math.max(0,yp)+x; 
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
					vmin[y]=Math.min(y+radius+1,hm)*w; 
					vmax[y]=Math.max(y-radius,0)*w; 
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
