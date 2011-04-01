//
//  NawlzFishBowlGLView.h
//  Nawlz
//
//  Created by lukasz karluk on 20/02/11.
//  Copyright 2011 com.nawlz. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

#include "ofMain.h"
#include "NawlzFishBowl.h"
#include "NawlzImage.h"

@interface NawlzFishBowlGLView : UIView {
	/* The pixel dimensions of the backbuffer */
	GLint backingWidth;
	GLint backingHeight;
	
	NawlzFishBowl*	nawlzFishBowl;
	
	EAGLContext *context;
	
	/* OpenGL names for the renderbuffer and framebuffers used to render to this view */
	GLuint viewRenderbuffer, viewFramebuffer;
	
	/* OpenGL name for the depth buffer that is attached to viewFramebuffer, if it exists (0 if it does not exist) */
	GLuint depthRenderbuffer;	
	
	NSInteger animationFrameInterval;
	
	NSTimer *animationTimer;
	
	CFTimeInterval lastTime;
	
	CGPoint originalPos;
	CGPoint originalSurroundingPos[8];
	unsigned int surrVertLoc[8];
	unsigned int vertLoc;
	CGPoint touchLocation;
	CGPoint oldTouchLocation;
	BOOL firstTouch;
	
}

@property(readonly, nonatomic) EAGLContext *context;
@property(readonly, nonatomic) GLint backingWidth;
@property(readonly, nonatomic) GLint backingHeight;
@property(retain, nonatomic) NSTimer *animationTimer;

// OpenGL drawing
- (BOOL)createFramebuffer;
- (void)destroyFramebuffer;
- (void)clearScreen;
- (void)beginRender;
- (void)endRender;
- (void)initOpenGL;

- (void)render;
- (void)startAnimation;

- (id)initWithFrame:(CGRect)aRect;

- (void) initNawlzFishBowl;
- (void) loadImage :(NSString*) fileName : (NawlzImage*) imageOut;

@end