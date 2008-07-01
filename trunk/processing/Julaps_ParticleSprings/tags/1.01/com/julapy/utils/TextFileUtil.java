package com.julapy.utils;

import java.io.PrintWriter;

import processing.core.PApplet;

public class TextFileUtil 
{
	PApplet papp;
	PrintWriter output;
	String[] data;
	int dataIndex;

	public TextFileUtil( PApplet papp )
	{
		this.papp = papp;
	}
	
	public void createWriter ( String filename )
	{
		output = papp.createWriter( filename );
	}
	
	public void writeStringLine ( String[] values )
	{
		String line = "";
		for( int i=0; i<values.length; i++ )
		{
			line += values[i];
			if( i < values.length-1 )
				line += "\t";
		}
		output.println( line );
	}

	public void writeIntegerLine ( int[] values )
	{
		String line = "";
		for( int i=0; i<values.length; i++ )
		{
			line += values[i];
			if( i < values.length-1 )
				line += "\t";
		}
		output.println( line );
	}

	public void writeFloatLine ( float[] values )
	{
		String line = "";
		for( int i=0; i<values.length; i++ )
		{
			line += values[i];
			if( i < values.length-1 )
				line += "\t";
		}
		output.println( line );
	}
	
	public void closeWriter ()
	{
		output.flush();
		output.close();
	}
	
	public void readDataFromFile ( String filename )
	{
		data = papp.loadStrings( filename  );
		dataIndex = 0;
	}
	
	public float[] readFloatLine ()
	{
		if ( dataIndex < data.length) 
		{
			int i;
			String dataStr		= data[dataIndex];
			String[] dataSplit	= PApplet.split( dataStr, '\t');
			float[] dataRtn		= new float[dataSplit.length];
			
			for( i=0; i<dataSplit.length; i++)
			{
				dataRtn[i] = Float.valueOf( dataSplit[i] );
			}
			 
			++dataIndex;
			
			return dataRtn;
		}
		
		return new float[0];
	}

	public int[] readIntegerLine ()
	{
		if ( dataIndex < data.length) 
		{
			String dataStr	= data[dataIndex];
			String[] dataSplit = PApplet.split( dataStr, '\t');
			int[] dataRtn = new int[dataSplit.length];
			
			for( int j=0; j<dataSplit.length; j++)
			{
				dataRtn[j] = Integer.valueOf( dataSplit[j] );
			}
			 
			++dataIndex;
			
			return dataRtn;
		}
		
		return new int[0];
	}

	public String[] readStringLine ()
	{
		if ( dataIndex < data.length) 
		{
			String dataStr	= data[dataIndex];
			String[] dataSplit = PApplet.split( dataStr, '\t');
			 
			++dataIndex;
			
			return dataSplit;
		}
		
		return new String[0];
	}
}
