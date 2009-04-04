package com.julapy.pargoliers;

import processing.core.PApplet;
import processing.core.PImage;

public class PargoliersImage 
{
	public static int FIT_MODE_NONE = 0;
	public static int FIT_MODE_CROP = 1;
	public static int FIT_MODE_BEST = 2;
	
	private PApplet p;
	private PImage	pImage;
	private String	pImagePath;
	private int pImageFitMode;
	private float pImageScale;
	private int	pImageW;
	private int	pImageH;
	private int	pImageX;
	private int	pImageY;
	
	public Boolean isShowing = true;
	
	public PargoliersImage( PApplet p, String imagePath, int fitMode )
	{
		this.p 				= p;
		this.pImagePath		= imagePath;
		this.pImageFitMode	= fitMode;

		init();
	}
	
	private void init ()
	{
		if( pImagePath == "" )
		{
			pImage	= p.createImage( p.width, p.height, PApplet.RGB );
		}
		else
		{
			pImage 	= p.loadImage( pImagePath );
		}
		
		if( pImageFitMode == FIT_MODE_CROP )
		{
			pImageScale = getCropFitRatio( p.width, p.height, pImage.width, pImage.height );
		}
		
		if( pImageFitMode == FIT_MODE_BEST )
		{
			pImageScale = getBestFitRatio( p.width, p.height, pImage.width, pImage.height );
		}
		
		if( pImageFitMode == FIT_MODE_NONE )
		{
			pImageScale = 1;
		}
		
		pImageW = (int)( pImage.width * pImageScale );
		pImageH = (int)( pImage.height * pImageScale );
		pImageX = (int)( ( p.width - pImageW ) * 0.5f );
		pImageY = (int)( ( p.height - pImageH ) * 0.5f );
	}
	
	public void update ()
	{
		//
	}
	
	public void draw ()
	{
		if( isShowing )
		{
			p.image( pImage, pImageX, pImageY, pImageW, pImageH );
		}
	}
	
	public int[] getPixels ()
	{
		return pImage.pixels;
	}
	
	private float getCropFitRatio ( int destinationWidth, int destinationHeight, int sourceWidth, int sourceHeight )
	{
		float widthRatio	= (float)destinationWidth / (float)sourceWidth;
		float heightRatio	= (float)destinationHeight / (float)sourceHeight;

		return Math.max( widthRatio, heightRatio );
	}

	private float getBestFitRatio ( int destinationWidth, int destinationHeight, int sourceWidth, int sourceHeight )
	{
		float widthRatio	= (float)destinationWidth / (float)sourceWidth;
		float heightRatio	= (float)destinationHeight / (float)sourceHeight;

		return Math.min( widthRatio, heightRatio );
	}
}
