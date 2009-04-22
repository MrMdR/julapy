#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate( 60 );
	
	ofBackground( 255, 255, 255 );
	
	noiseFieldWidth		= 640;
	noiseFieldHeight	= 480;
	noiseFieldRes		= 10;
	noiseField			= new NoiseField();

	camWidth		= 320;
	camHeight		= 240;
	opticalField	= new OpticalField( camWidth, camHeight );
	
	psWidth		= 640;
	psHeight	= 480;
	ps			= new ParticleSystem( psWidth, psHeight );
}

//--------------------------------------------------------------
void testApp::update()
{
	int i;
	float nx;	// noise velocity in the x direction.
	float ny;	// noise velocity in the y direction.
	float ox;	// optical field velocity in the x direction.
	float oy;	// optical field velocity in the y direction.
	
	float ns;	// noise scale.
	float rs;	// random scale.
	float os;	// optical scale.
	
	ns = 2;
	rs = 2;
	os = 3;
	
	noiseField->update();
	opticalField->update();

	while( ps->hasNextParticle() )
	{
		noiseField->get2DVectorAtPixel
		(
			ps->getParticle()->x(),
			ps->getParticle()->y(),
			&nx,
			&ny
		);
		
		opticalField->getVelAtNorm
		(
			(float)ps->getParticle()->x() / (float)noiseFieldWidth,
			(float)ps->getParticle()->y() / (float)noiseFieldHeight, 
			&ox,
			&oy
		);
		
		ps->getParticle()->addForce( nx * ns, ny * ns );
		ps->getParticle()->addForce( ofRandom( -1.0f, 1.0f) * rs, ofRandom( -1.0f, 1.0f) * rs );
		ps->getParticle()->addForce( ox * os, oy * os );
	}
	
	ps->update();
}

//--------------------------------------------------------------
void testApp::draw()
{
	int x;
	int y;
	float nx;
	float ny;

	glPushMatrix( );
	glTranslated( 100, 100, 0 );
	
	ofSetColor( 0, 0, 0 );
	ofFill();
	ofRect( 0, 0, noiseFieldWidth, noiseFieldHeight );

	ofSetColor( 255, 255, 255, 100 );
	ofNoFill();
	
	for( y=0; y<noiseFieldHeight; y += 10 )
	{
		for( x=0; x<noiseFieldWidth; x+=10 )
		{
			noiseField->get2DVectorAtPixel( x, y, &nx, &ny );
			
			glPushMatrix();
			
			ofLine( x, y, x + ( nx * noiseFieldRes ), y + ( ny * noiseFieldRes ) );
			
			glPopMatrix();
		}
	}
	
	ps->draw();
	
	glPopMatrix();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

