/*
 *  DataNormaliser.h
 *  openFrameworks
 *
 *  Created by lukasz karluk on 13/06/09.
 *
 */

#ifndef OFX_DATA_NORMALISER_H_
#define OFX_DATA_NORMALISER_H_

#define		DEFAULT_BUFFER_SIZE		100

class ofxDataNormaliser
{

public :
	
	ofxDataNormaliser();
	~ofxDataNormaliser();
	
	void  init();
	void  addValue( float value );
	float getNormalisedValue();

	void  setUseBuffer( bool b );
	void  setBufferSize( int size );
	
private :

	void checkValueBounds();
	void checkValueBoundsFromBuffer();
	
	float   valueCurr;
	int		valuesSize;
	int		valuesIndex;
	float	*values;
	float	valueMin;
	float	valueMax;
	bool	isFull;
	bool	useBuffer;
};

#endif