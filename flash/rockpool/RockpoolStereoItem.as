package
{
	import caurina.transitions.Tweener;
	
	import flash.display.Bitmap;
	import flash.display.BlendMode;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.EventDispatcher;
	import flash.events.TimerEvent;
	import flash.geom.Rectangle;
	import flash.utils.Timer;

	public class RockpoolStereoItem extends EventDispatcher
	{
		private var container	:Sprite;
		private var stereoCont	:Sprite = new Sprite();
		private var stereoBm01	:Bitmap;
		private var stereoBm02	:Bitmap;
		private var stereoRect	:Rectangle;
		
		private var stereoTimer	:Timer;
		private var stereoFPS	:int = 10;
		private var stereoIndex	:int = 0;
		
		public function RockpoolStereoItem( container:Sprite, stereoBm01:Bitmap, stereoBm02:Bitmap, stereoRect:Rectangle )
		{
			this.container		= container;
			this.stereoBm01		= stereoBm01;
			this.stereoBm02		= stereoBm02;
			this.stereoRect		= stereoRect;
			
			container.addChild( stereoCont );
			
			stereoBm01.x		= stereoRect.x;
			stereoBm01.y		= stereoRect.y;

			stereoBm02.x		= stereoRect.x;
			stereoBm02.y		= stereoRect.y;
			
			stereoCont.addChild( stereoBm01 );
			stereoCont.addChild( stereoBm02 );
//			stereoCont.blendMode = BlendMode.ALPHA;
			
			playIn();
			playStereo();
		}
		
		public function destroy ():void
		{
			stopStereo();
			
			stereoCont.removeChild( stereoBm01 );
			stereoCont.removeChild( stereoBm02 );
			
			container.removeChild( stereoCont );
		}
		
		public function playIn ():void
		{
			stereoCont.alpha = 0;
			
			Tweener.addTween
			(
				stereoCont,
				{
					alpha		: 1,
					time		: 0.3,
					transition	: "easeOutQuad",
					onComplete	: playOut
				}
			);
		}
		
		public function playOut ():void
		{
			Tweener.addTween
			(
				stereoCont,
				{
					alpha		: 0,
					time		: 5,
					delay		: 10,
					transition	: "easeOutQuad",
					onComplete	: onComplete
				}
			);
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
				stereoBm01.visible = true;
				stereoBm02.visible = false;
			}
			
			if( stereoIndex == 1 )
			{
				stereoBm01.visible = false;
				stereoBm02.visible = true;
			}
		}
		
		private function onComplete ():void
		{
			dispatchEvent( new Event( Event.COMPLETE ) );
		}
	}
}