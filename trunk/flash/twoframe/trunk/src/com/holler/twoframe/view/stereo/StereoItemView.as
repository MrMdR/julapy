package com.holler.twoframe.view.stereo
{
	import com.holler.containers.ScreenView;
	import com.holler.controls.Image;
	import com.holler.events.ImageLoadedEvent;
	import com.holler.twoframe.vo.ImageVO;
	
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.IOErrorEvent;
	import flash.events.TimerEvent;
	import flash.utils.Timer;

	public class StereoItemView extends ScreenView
	{
		public static const ITEM_WIDTH	: int = 400;
		public static const ITEM_HEIGHT	: int = 600;
		
		private var imageVO:ImageVO;
		private var image01Container:Sprite;
		private var image02Container:Sprite;
		private var image01:Image;
		private var image02:Image;
		
		private var stereoTimer:Timer;
		private var stereoFPS:int = 10;
		private var stereoIndex:int = 0;
		
		private var isPlayingOut:Boolean = false;
		
		
		public function StereoItemView( sprite:Sprite=null, imageVO:ImageVO=null )
		{
			super(sprite);
			
			this.imageVO = imageVO;
			
			image01Container	= new Sprite();
			image02Container	= new Sprite();
			
			image01 = new Image( image01Container );
			image01.addEventListener( ImageLoadedEvent.IMAGE_LOADED_EVENT,	onImageLoadComplete );
			image01.addEventListener( IOErrorEvent.IO_ERROR,				onImageLoadError );
			image01.load( imageVO.image01Path );
			
			image02 = new Image( image02Container );
			image02.addEventListener( ImageLoadedEvent.IMAGE_LOADED_EVENT,	onImageLoadComplete );
			image02.addEventListener( IOErrorEvent.IO_ERROR,				onImageLoadError );
			image02.visible = false;
			
			addChildView( image01 );
			addChildView( image02 );
			
			doValidate();
		}
		
		public function destroy ():void
		{
			stopStereo();
			
			image01.close();
			image01.removeEventListener( ImageLoadedEvent.IMAGE_LOADED_EVENT,	onImageLoadComplete );
			image01.removeEventListener( IOErrorEvent.IO_ERROR,					onImageLoadError );
			image01.remove();
			image01 = null;

			image02.close();
			image02.removeEventListener( ImageLoadedEvent.IMAGE_LOADED_EVENT,	onImageLoadComplete );
			image02.removeEventListener( IOErrorEvent.IO_ERROR,					onImageLoadError );
			image02.remove();
			image02 = null;
		}
		
		public function playStereo ():void
		{
			stereoTimer = new Timer( 1000 / stereoFPS );
			stereoTimer.addEventListener( TimerEvent.TIMER, stereoTick );
			stereoTimer.start();
			stereoTick();
		}
		
		public function stopStereo ():void
		{
			if( stereoTimer is Timer )
			{
				stereoTimer.removeEventListener( TimerEvent.TIMER, stereoTick );
				stereoTimer.stop();
				stereoTimer = null;
			}
		}
		
		private function stereoTick ( e:*=null ):void
		{
			if( ++stereoIndex > 1 )
				stereoIndex = 0;
				
			if( stereoIndex == 0 )
			{
				image01.visible = true;
				image02.visible = false;
			}
			
			if( stereoIndex == 1 )
			{
				image01.visible = false;
				image02.visible = true;
			}
			
			doValidate();
		}
		
		public function playIn ():void
		{
			onPlayedIn();
		}
		
		public function playOut ():void
		{
			if( !isPlayingOut )
			{
				isPlayingOut = true;
				
				onPlayedOut();
			}
		}
		
		private function onImageLoadComplete ( e:ImageLoadedEvent ):void
		{
			if( e.target == image01 )
				image02.load( imageVO.image02Path );
				
			if( e.target == image02 )
			{
				playStereo();
				
				dispatchEvent( new Event( Event.INIT ) );
			}
		}
		
		private function onImageLoadError ( e:IOErrorEvent ):void
		{
			
		}
		
		private function onPlayedIn ():void
		{
			
		}
		
		private function onPlayedOut ():void
		{
			dispatchEvent( new Event( Event.COMPLETE ) );
		}
		
	}
}