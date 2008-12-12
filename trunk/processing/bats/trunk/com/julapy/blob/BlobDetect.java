package com.julapy.blob;

import java.awt.Rectangle;

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
	private PImage snapshot;
	private PImage difference;
	private BlobDetection bd;

	private Blob kingBlob;
	private Blob[] blobs;
	private Rectangle[] rects;
	private float minBlobArea = 0.01f;

	private boolean hasSnapshot = false;
	
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
		
		blobs		= new Blob[ 0 ];
		kingBlob	= null;

		capImage	= new PImage( cap.width, cap.height );
		snapshot	= new PImage( cap.width, cap.height );
		difference	= new PImage( cap.width, cap.height );
		
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

			if( !hasSnapshot )
			{
				takeSnapshot();
			}
			
			calcDifference();
			
//			fastblur( difference, 2 );
			bd.computeBlobs( difference.pixels );
			
//			blobs = bd.blob;
			
			validateBlobs();
		}
	}
	
	public void draw ()
	{
		if( showVideo )
		{
			papp.image( difference, 0, 0, capWidth, capHeight );
		}
		
		if( showAreas )
		{
			papp.noFill();
			papp.strokeWeight( 1 );
			papp.stroke( 1, 0, 0 );
			
			drawBlobAreas( capWidth, capHeight );
		}
		
		if( showEdges )
		{
			papp.noFill();
			papp.strokeWeight( 3 ); 
			papp.stroke( 0, 1, 0 );
			
			drawBlobEdges( capWidth, capHeight );
		}
		
		if( showAreas )
		{
			papp.noFill();
			papp.strokeWeight( 3 ); 
			papp.stroke( 1, 1, 1 ); 
			
			drawBlobAreas( papp.width, papp.height );
		}
	}
	
	public void drawKingBlob ()
	{
		if( showVideo )
		{
			papp.image( difference, 0, 0, capWidth, capHeight );
		}
		
	}
	
	public void takeSnapshot ()
	{
		hasSnapshot = true;
		
		snapshot = cap.get();
	}
	
	public Rectangle[] getRects ()
	{
		return rects;
	}
	
	public Rectangle getKingBlob ()
	{
		if( kingBlob != null )
		{
			return new Rectangle
			(
				(int)( kingBlob.x * papp.width ),
				(int)( kingBlob.y * papp.height ),
				(int)( kingBlob.w * papp.width ),
				(int)( kingBlob.h * papp.height ) 
			);
		}
		else
		{
			return null;
		}
	}
	
	private void calcDifference ()
	{
		int i;
		float cb, sb, db;
		
		for( i=0; i<capImage.pixels.length; i++ )
		{
			sb = papp.brightness( snapshot.pixels[ i ] );
			cb = papp.brightness( capImage.pixels[ i ] );
			
			db = sb - cb;
			if( db < 0 ) db *= -1;

			difference.loadPixels();
			if( db > 0.2f )
			{
				difference.pixels[ i ] = 0xFFFFFF;
			}
			else
			{
				difference.pixels[ i ] = 0x000000;
			}
			difference.updatePixels();
		}
	}
	
	private void validateBlobs ()
	{
		int n = 0;
		int i;
		float a= 0;
		float aMax = 0;
		Blob b;
		
		kingBlob = null;
		
		for( i=0; i<bd.getBlobNb(); i++ )
		{
			b = bd.getBlob( i );
			a = b.w * b.h;
			
			if( a > minBlobArea )
			{
				if( kingBlob == null || a > aMax )
				{
					aMax		= a;
					kingBlob	= b;
				}
				
				++n;
			}
		}

		blobs = new Blob[ n ];
		rects = new Rectangle[ n ];
		
		n = 0;
		for( i=0; i<bd.getBlobNb(); i++ )
		{
			b = bd.getBlob( i );
			a = b.w * b.h;
			
			if( a > minBlobArea )
			{
				blobs[ n ] = b;
				rects[ n ] =new Rectangle
								(
									(int)( b.x * papp.width ), 
									(int)( b.y * papp.height ),
									(int)( b.w * papp.width ),
									(int)( b.h * papp.height ) 
								);
				
				++n;
			}
		}
	}
	
//	 ================================================== 
//	 drawBlobsAndEdges() 
//	 ================================================== 
	
	private void drawBlobAreas ( float w, float h )
	{
		Blob b; 
		int n;
		
		for ( n=0; n<blobs.length; n++ ) 
		{ 
			b = blobs[ n ]; 
			
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
		
		for ( n=0; n<blobs.length; n++ ) 
		{ 
			b = blobs[ n ]; 
			
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
