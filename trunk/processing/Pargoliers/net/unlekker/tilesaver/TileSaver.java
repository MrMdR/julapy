package net.unlekker.tilesaver;

import processing.core.PApplet;
import processing.core.PImage;

// aTileSaver.pde - v0.12 2007.0326
// Marius Watz - http://workshop.evolutionzone.com
//
// Class for rendering high-resolution images by splitting them into
// tiles using the viewport.
//
// Builds heavily on solution by "surelyyoujest":
// http://processing.org/discourse/yabb_beta/YaBB.cgi?
// board=OpenGL;action=display;num=1159148942

public class TileSaver 
{
	public boolean isTiling=false,done=true;
	public boolean doSavePreview=true;

	PApplet p;
	float FOV=60; // initial field of view
	float cameraZ, width, height;
	int tileNum=10,tileNumSq; // number of tiles
	int tileImgCnt, tileX, tileY, tilePad;
	boolean firstFrame=false, secondFrame=false;
	String tileFilename,tileFileExt=".tga";
	PImage tileImg;
	float perc,percMilestone;

	// The constructor takes a PApplet reference to your sketch.
	public TileSaver(PApplet _p) 
	{
		p=_p;
	}

	// If init() is called without specifying number of tiles, getMaxTiles()
	// will be called to estimate number of tiles according to free memory.
	public void init(String _filename) 
	{
		init(_filename,getMaxTiles(p.width));
	}

	// Initialize using a filename to output to and number of tiles to use.
	public void init(String _filename,int _num) 
	{
		tileFilename	= _filename;
		tileNum			= _num;
		tileNumSq		= ( tileNum * tileNum );

		width	= p.width;
		height	= p.height;
		cameraZ	= (height/2.0f)/PApplet.tan(PApplet.PI*FOV/360.0f);
		PApplet.println("aTileSaver: "+tileNum+" tilesnResolution: "+ (p.width*tileNum)+"x"+(p.height*tileNum));

		// remove extension from filename
		if(!new java.io.File(tileFilename).isAbsolute())
			tileFilename=p.dataPath(tileFilename);
		tileFilename=noExt(tileFilename);
		PApplet.createPath(tileFilename);

		// save preview
		if(doSavePreview) p.g.save(tileFilename+"_preview.png");

		// set up off-screen buffer for saving tiled images
		tileImg=new PImage(p.width*tileNum, p.height*tileNum);

		// start tiling
		done=false;
		isTiling=false;
		perc=0;
		percMilestone=0;
		tileInc();
	}

	// set filetype, default is TGA. pass a valid image extension as parameter.
	public void setSaveType(String extension) 
	{
		tileFileExt=extension;
		if(tileFileExt.indexOf(".")==-1) tileFileExt="."+tileFileExt;
	}

	// pre() handles initialization of each frame.
	// It should be called in draw() before any drawing occurs.
	public void pre() 
	{
		if(!isTiling) return;
		if(firstFrame) firstFrame=false;
		else if(secondFrame) 
		{
			secondFrame=false;
			tileInc();
		}
		
		setupCamera();
	}

	// post() handles tile update and image saving.
	// It should be called at the very end of draw(), after any drawing.
	public void post() 
	{
		// If first or second frame, don't update or save.
		if(firstFrame||secondFrame|| (!isTiling)) return;

		// Find image ID from reverse row order
		int imgid=tileImgCnt%tileNum+(tileNum-tileImgCnt/tileNum-1)*tileNum;
		int idx=(imgid%tileNum);
		int idy=(imgid/tileNum);

		// Get current image from sketch and draw it into buffer
		p.loadPixels();
		tileImg.set(idx*p.width, idy*p.height, p.g);

		// Increment tile index
		tileImgCnt++;
		perc=100*((float)tileImgCnt/(float)tileNumSq);
		if(perc-percMilestone> 5 || perc>99) 
		{
			PApplet.println(PApplet.nf(perc,3,2)+"% completed. "+tileImgCnt+"/"+tileNumSq+" images saved.");
			percMilestone=perc;
		}

		if(tileImgCnt==tileNumSq) tileFinish();
		else tileInc();
	}

	public boolean checkStatus() 
	{
		return isTiling;
	}

	// tileFinish() handles saving of the tiled image
	public void tileFinish() 
	{
		isTiling=false;

		restoreCamera();

		// save large image to TGA
		tileFilename+="_"+(p.width*tileNum)+"x"+(p.height*tileNum)+tileFileExt;
		PApplet.println("Save: "+ tileFilename.substring( tileFilename.lastIndexOf(java.io.File.separator)+1));
		tileImg.save(tileFilename);
		PApplet.println("Done tiling.n");

		// clear buffer for garbage collection
		tileImg=null;
		done=true;
	}

	// Increment tile coordinates
	public void tileInc() 
	{
		if(!isTiling) 
		{
			isTiling=true;
			firstFrame=true;
			secondFrame=true;
			tileImgCnt=0;
		} 
		else 
		{
			if(tileX==tileNum-1) 
			{
				tileX=0;
				tileY=(tileY+1)%tileNum;
			} 
			else
			{
				tileX++;
			}
		}
	}

	// set up camera correctly for the current tile
	public void setupCamera() 
	{
		p.camera
		(
				width/2.0f, height/2.0f, cameraZ,
				width/2.0f, height/2.0f, 0, 
				0,			1,			 0
		);

		if(isTiling) 
		{
			float mod=1f/10f;
			p.frustum
			(
				width*((float)tileX/(float)tileNum-.5f)*mod,
				width*((tileX+1)/(float)tileNum-.5f)*mod,
				height*((float)tileY/(float)tileNum-.5f)*mod,
				height*((tileY+1)/(float)tileNum-.5f)*mod,
				cameraZ*mod,
				10000
			);
		}
	}

	// restore camera once tiling is done
	public void restoreCamera()
	{
		float mod=1f/10f;
		
		p.camera
		(
			width/2.0f, height/2.0f, cameraZ,
			width/2.0f, height/2.0f, 0, 
			0,			1,			 0
		);
		
		p.frustum
		(
			-(width/2)*mod,
			(width/2)*mod,
			-(height/2)*mod,
			(height/2)*mod,
			cameraZ*mod,
			10000
		);
	}

	// checks free memory and gives a suggestion for maximum tile
	// resolution. It should work well in most cases, I've been able
	// to generate 20k x 20k pixel images with 1.5 GB RAM allocated.
	public int getMaxTiles(int width) 
	{
		// get an instance of java.lang.Runtime, force garbage collection
		java.lang.Runtime runtime=java.lang.Runtime.getRuntime();
		runtime.gc();

		// calculate free memory for ARGB (4 byte) data, giving some slack
		// to out of memory crashes.
		int num=(int)(Math.sqrt((float)(runtime.freeMemory()/4)*0.925f))/width;
		PApplet.println(((float)runtime.freeMemory()/(1024*1024))+"/"+((float)runtime.totalMemory()/(1024*1024)));

		// warn if low memory
		if(num==1) 
		{
			PApplet.println("Memory is low. Consider increasing memory.");
			num=2;
		}

		return num;
	}

	// strip extension from filename
	String noExt(String name) 
	{
		int last=name.lastIndexOf(".");
		if(last>0)
		{
			return name.substring(0, last);
		}

		return name;
	}
}