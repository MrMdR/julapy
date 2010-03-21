package com.julapy.ph.makeup.view
{
	import com.holler.assets.AssetLoader;
	import com.holler.core.View;
	import com.julapy.ph.makeup.model.ModelLocator;

	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.TimerEvent;
	import flash.utils.Timer;

	public class FaceView extends View
	{
		private var face		: MovieClip;
		private var blink		: MovieClip;

		private var blinkTimer	: Timer;

		public function FaceView(sprite:Sprite=null)
		{
			super( sprite );

			face	= AssetLoader.getInstance().getClassInstance( "makeup.face.base" ) as MovieClip;
			blink	= AssetLoader.getInstance().getClassInstance( "makeup.face.blink" ) as MovieClip;

			_sprite.addChild( face );
			_sprite.addChild( blink );

			hideBlink();
			initShowBlinkTimer();
		}

		private function initShowBlinkTimer ():void
		{
			var blinkDelay : int;
			blinkDelay = Math.random() * 5000 + 1000;

			blinkTimer = new Timer( blinkDelay, 1 );
			blinkTimer.addEventListener( TimerEvent.TIMER_COMPLETE, showBlinkTimerHandler );
			blinkTimer.start();
		}

		private function killShowBlinkTimer ():void
		{
			if( blinkTimer )
			{
				blinkTimer.removeEventListener( TimerEvent.TIMER_COMPLETE, showBlinkTimerHandler );
				blinkTimer.stop();
				blinkTimer = null;
			}
		}

		private function showBlinkTimerHandler ( e : TimerEvent ):void
		{
			killShowBlinkTimer();
			showBlink();
			initHideBlinkTimer();
		}

		private function initHideBlinkTimer ():void
		{
			var blinkDelay : int;
			blinkDelay = 300;

			blinkTimer = new Timer( blinkDelay, 1 );
			blinkTimer.addEventListener( TimerEvent.TIMER_COMPLETE, hideBlinkTimerHandler );
			blinkTimer.start();
		}

		private function killHideBlinkTimer ():void
		{
			if( blinkTimer )
			{
				blinkTimer.removeEventListener( TimerEvent.TIMER_COMPLETE, hideBlinkTimerHandler );
				blinkTimer.stop();
				blinkTimer = null;
			}
		}

		private function hideBlinkTimerHandler ( e : TimerEvent ):void
		{
			killHideBlinkTimer();
			hideBlink();
			initShowBlinkTimer();
		}

		private function showBlink ():void
		{
			blink.visible = true;

			ModelLocator.getInstance().makeupModel.blinking = true;
		}

		private function hideBlink ():void
		{
			blink.visible = false;

			ModelLocator.getInstance().makeupModel.blinking = false;
		}
	}
}