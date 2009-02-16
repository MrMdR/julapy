package com.julapy.experiments;

////////////////////////////////////////////////////////////////////////////////
//
// Project: 		Julaps_E1
// File: 			Main.java
// Created by: 		lukaszkarluk, Feb 26, 2008
//
////////////////////////////////////////////////////////////////////////////////

import java.util.ArrayList;

import javax.media.opengl.GL;

import com.julapy.utils.TileSaver;
import com.sun.corba.se.spi.ior.MakeImmutable;

//import krister.Ess.AudioChannel;
//import krister.Ess.Ess;
//import krister.Ess.FFT;

import damkjer.ocd.Camera;
import ddf.minim.AudioPlayer;
import ddf.minim.Minim;
import ddf.minim.analysis.BeatDetect;
import ddf.minim.analysis.FFT;

import processing.core.*;
import processing.opengl.PGraphicsOpenGL;
import toxi.geom.Vec3D;

public class ArcRays extends PApplet
{
	int framesPerSecond = 25;

	float sinLUT[];
	float cosLUT[];	
	float SINCOS_PRECISION=1.0f;
	int SINCOS_LENGTH= (int)(360.0/SINCOS_PRECISION);
	
	ArrayList<ArcBar> arcBars;
	int angleStep = 5;
	float rx = 0;
	float ry = 0;
	float rz = 0;
	
	PGraphicsOpenGL pgl;
	GL gl;
	
	PImage glow;
	PImage corona;
	
	AudioPlayer song;
	FFT fftLin;
	BeatDetect beat;
	float energyScale = 1;
	
//	AudioChannel channel;
//	FFT fft;
//	int frameNumber = 0;
//	float energyScale = 1;
	
	String[] audiodata;
	int audiodataIndex = 0;	
	
	boolean isRecording = false;
	int imageCount;
	
	int squareList;
	
//	Camera cam;
//	Vec3D camTarget;
//	Vec3D camPosition;
//	Vec3D camNormal;
//	int camDirection = 1;
	
	TileSaver tiler;
	Boolean isTiling = false;
	 
	public void setup() 
	{
		size(600, 600, OPENGL);
		
		frameRate( framesPerSecond );
		
		smooth();
		
		colorMode( RGB, 1.0f );
		  
		// Turn on 4X antialiasing
		hint( ENABLE_OPENGL_4X_SMOOTH );
		  
		pgl         = (PGraphicsOpenGL) g;
		gl          = pgl.gl;
		gl.setSwapInterval(1);
		
		glow	= loadImage( "texture/glow.png" );
		corona	= loadImage( "texture/corona.png" );
		
		//__________________________________________________________ trig
		
		sinLUT = new float[SINCOS_LENGTH];
		cosLUT = new float[SINCOS_LENGTH];
		for (int i = 0; i < SINCOS_LENGTH; i++)
		{
			sinLUT[i]= (float)Math.sin( i * DEG_TO_RAD * SINCOS_PRECISION );
			cosLUT[i]= (float)Math.cos( i * DEG_TO_RAD * SINCOS_PRECISION );
		}
		
		//__________________________________________________________ arcs
		
		arcBars = new ArrayList<ArcBar>();
		
		float anglePerStep = PI / (float)angleStep;
		
		for(int i=0; i<angleStep; i++)
		{
			for(int j=0; j<angleStep; j++)
			{
//				for(int k=0; k<angleStep; k++)
//				{
					arcBars.add( new ArcBar( i*anglePerStep, j*anglePerStep, 0 ) );			
//				}
			}
		}
		
//		initEss();
		initMinim();
		
//		audiodata = loadStrings( "data/audiodata.txt"  );
		
//		cam 		= new Camera( this, width/2, height/2, 1000, width/2, width/2, width/2);
//		camPosition = new Vec3D();
//		camTarget 	= new Vec3D();
//		camNormal 	= new Vec3D();
		
		tiler = new TileSaver( this );
		
		initGL();
	}

	public void initGL()
	{
		pgl.beginGL();
		squareList = gl.glGenLists(1);
		gl.glNewList(squareList, GL.GL_COMPILE);
		gl.glBegin(GL.GL_POLYGON);
		gl.glTexCoord2f(0, 0);    gl.glVertex2f(-.5f, -.5f);
		gl.glTexCoord2f(1, 0);    gl.glVertex2f( .5f, -.5f);
		gl.glTexCoord2f(1, 1);    gl.glVertex2f( .5f,  .5f);
		gl.glTexCoord2f(0, 1);    gl.glVertex2f(-.5f,  .5f);
		gl.glEnd();
		gl.glEndList();
		pgl.endGL();
	}
	
/*	private void initEss ()
	{
		Ess.start( this );
		channel 		= new AudioChannel( "secondpeng.aif" );
		fft = new FFT( 512 );
		fft.limits();					// normalize the spectrum
		fft.averages( arcBars.size() );
		fft.damp( 0.9f );
//		fft.envelope( 2 );
//		fft.equalizer( true );
		channel.play( Ess.FOREVER );
	}
*/	
	private void initMinim ()
	{
		Minim.start( this );
		song = Minim.loadFile("audio/secondpeng.mp3");
		song.play();
		song.skip(50000);
		fftLin = new FFT(song.bufferSize(), song.sampleRate());
		fftLin.linAverages(arcBars.size());
	}
	
	public void keyPressed()
	{
		if(key == 'r')
		{
			isRecording = !isRecording;
			
			if(isRecording) println("started recording.");
			if(!isRecording) println("stopped recording.");
		}
		
		if(key=='t')
		{
			isTiling = !isTiling;
			
			tiler.init("Simple"+nf(frameCount,10),10);
		}
	}
	
	public void draw() 
	{
		tiler.pre();
		
		background(0);
		
//		if( !isTiling )
//		{
//			cam.circle( radians( ( 3 * camDirection ) ) );
//		}
//		cam.feed();
//
//		camPosition.set( cam.position()[0], cam.position()[1], cam.position()[2] );
//		camTarget.set( cam.target()[0], cam.target()[1], cam.target()[2] );
//		camNormal = camPosition.sub(camTarget).normalize();
		
//		fft.getSpectrum( channel );
//		fft.getSpectrum(channel.samples, (int)(frameNumber * channel.sampleRate / framesPerSecond));
		fftLin.forward(song.mix);

		gl.glDepthMask( false );
		gl.glEnable( GL.GL_BLEND );
		gl.glBlendFunc( GL.GL_SRC_ALPHA, GL.GL_ONE );
		
		pushMatrix();
		
		float cameraZ = ( height / 2.0f ) / PApplet.tan( PApplet.PI * 60 /360.0f );
		
		translate( width/2, width/2, -cameraZ );

		if( !isTiling )
		{
			rx = ry = rz += 2*PI / 3600;
		}
		rotateX(rx);
		rotateX(ry);
		rotateX(rz);

		float bandEnergy = 0;
		float totalEnergy = 0;
		
//		float[] audioDataValues = new float[25];
//		if ( audiodataIndex < audiodata.length) 
//		{
//			String audioDataString	= audiodata[audiodataIndex];
//			String[] audioDataSplit = split( audioDataString, '\t'); 
//			
//			for( int j=0; j<audioDataValues.length; j++)
//			{
//				audioDataValues[j] = Float.valueOf( audioDataSplit[j] );
//			}
//			 
//			++audiodataIndex;
//		}

		for(int i=arcBars.size(); --i>=0;)
		{
			ArcBar arcBar = arcBars.get( i );
			
			bandEnergy = fftLin.getBand( i );
//			bandEnergy = fft.averages[i];
//			bandEnergy = audioDataValues[i];
			totalEnergy += bandEnergy;
			
			if( !isTiling )
			{
				arcBar.scale( bandEnergy );
				arcBar.update();
			}
			arcBar.render();
		}
		
		// this should be calculated before drawing.
//		float avg = totalEnergy / audioDataValues.length;
		float avg = totalEnergy / fftLin.avgSize();
//		float avg = totalEnergy / fft.averages.length;
		if(avg > energyScale) energyScale = avg;
		else				  energyScale *= 0.96f;
		
		popMatrix();
		
//		drawCenterGlow();
		
		tiler.post();
		if( !tiler.checkStatus() && isTiling )
		{
			isTiling = false;
		}
		
		if(isRecording) save("export/image" + imageCount++ + ".png");
	}
	
	private void drawCenterGlow ()
	{
		pgl.texture( glow );
		
		textureMode( NORMALIZED );

		int size = (int)(energyScale * 30);
		
		gl.glPushMatrix();
		gl.glTranslatef( width/2, width/2, width/2 );
		
		// face texture to camera.
//		float deltaX   = camTarget.x - camPosition.x; 
//		float deltaY   = camTarget.y - camPosition.y; 
//		float deltaZ   = camTarget.z - camPosition.z;
//
//		float angleZ   = atan2( deltaY, deltaX ); 
//		float hyp      = sqrt( sq( deltaX ) + sq( deltaY ) ); 
//		float angleY   = atan2( hyp, deltaZ ); 
//	 
//		gl.glRotatef( degrees( angleZ ), 0, 0,    1.0f );
//		gl.glRotatef( degrees( angleY ), 0, 1.0f, 0    );
		
		gl.glScalef( size, size, size );
		gl.glColor4f( red(0xFFFFFF), green(0xFFFFFF), blue(0xFFFFFF), 1 );
        gl.glCallList( squareList );
		
        gl.glPopMatrix();
	}
	
	public void stop()
	{
		song.close();
//		Ess.stop();
		super.stop();
	}
	
//	public void arcLine(float x,float y,float deg,float rad, float w) 
//	{
//		int a=(int)(min (deg/SINCOS_PRECISION,SINCOS_LENGTH-1));
//		int numlines=(int)(w/2);
//	 
//		for (int j=0; j<numlines; j++) 
//		{
//			beginShape();
//			for (int i=0; i<a; i++) 
//			{ 
//				vertex(cosLUT[i]*rad+x,sinLUT[i]*rad+y);
//			}
//			endShape();
//			rad += 2;
//		}
//	}
//	
////	 Draw arc line with bars
//	public void arcLineBars(float x,float y,float deg,float rad,float w) {
//	  int a = (int)(min (deg/SINCOS_PRECISION,SINCOS_LENGTH-1));
//	  a /= 4;
//	 
//	  beginShape(QUADS);
//	  for (int i=0; i<a; i+=4) {
//	    vertex(cosLUT[i]*(rad)+x,sinLUT[i]*(rad)+y);
//	    vertex(cosLUT[i]*(rad+w)+x,sinLUT[i]*(rad+w)+y);
//	    vertex(cosLUT[i+2]*(rad+w)+x,sinLUT[i+2]*(rad+w)+y);
//	    vertex(cosLUT[i+2]*(rad)+x,sinLUT[i+2]*(rad)+y);
//	  }
//	  endShape();
//	}

	public class ArcBar
	{
		float rx;
		float ry;
		float rz;
		float degOffset;
		float degInc;
		float deg;
		float radius;
		float width;
		float scale;
		
		public ArcBar ()
		{
			rx = random(2*PI);
			ry = random(2*PI);
			rz = random(2*PI);
			
			init();
		}
		
		public ArcBar (float rx, float ry, float rz)
		{
			this.rx = rx;
			this.ry = ry;
			this.rz = rz;
			
			init();
		}
		
		public void init ()
		{
			scale		= 1;
			degOffset	= random(radians(2*PI));
//			deg 		= degOffset;
			degInc		= random(-radians(5), -radians(2));
//			radius		= random(200, 300);
			deg 		= 0;
//			degInc		= -radians(2);
			radius		= 200;
//			width		= random(10, 30);
			width		= 10;
		}
		
		public void scale ( float s )
		{
			s *= 0.2f;
			
			if(s > scale)
			{
				this.scale = max(s, 1);
			}
			else
			{
				this.scale *= 0.92;
			}
			
		}
		
		public void update ()
		{
			deg += degInc;
		}
		
		public void render ()
		{
			pushMatrix();
			
			rotateX(rx);
			rotateY(ry);
			rotateZ(rz);
			
			rotate(deg);

			drawArcBar(0, 0, 200, radius, width*scale);
			
			popMatrix();
		}
		
		public void drawArcBar (float x,float y,float deg,float rad,float w) 
		{
			pgl.beginGL();
			
			gl.glBegin( GL.GL_QUAD_STRIP );
			
			int ang = (int)min (deg/SINCOS_PRECISION,SINCOS_LENGTH-1);
			
			for (int i = 0; i < ang; i++) 
			{
				float per = 1.0f - (float)i/(float)(ang-1);

				gl.glVertex3f( cosLUT[i]*(rad)+x, sinLUT[i]*(rad)+y, 0 );
				gl.glVertex3f( cosLUT[i]*(rad+w)+x, sinLUT[i]*(rad+w)+y, 0 );
				
				float as	= min(scale/3, 1);		// audio scale
				float r		= 1;					// red
				float g		= 1 - per;				// green
				float b		= per * 0.2f;			// blue
				float a		= per * 0.5f * as;		// alpha
				
				gl.glColor4f( r, g, b, a );
			}
			
			gl.glEnd();
			
			pgl.endGL();
		}
	}	
}