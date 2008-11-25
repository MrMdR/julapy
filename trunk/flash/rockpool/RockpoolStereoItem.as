package
{
	import flash.display.Bitmap;
	import flash.display.Sprite;
	import flash.events.EventDispatcher;
	import flash.events.TimerEvent;
	import flash.geom.Rectangle;
	import flash.utils.Timer;

	public class RockpoolStereoItem extends EventDispatcher
	{
		private var container	:Sprite;
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
			
			stereoBm01.x		= (int)( stereoRect.x - stereoBm01.width * 0.5 );
			stereoBm01.y		= (int)( stereoRect.y - stereoBm01.height * 0.5 );

			stereoBm02.x		= (int)( stereoRect.x - stereoBm02.width * 0.5 );
			stereoBm02.y		= (int)( stereoRect.y - stereoBm02.height * 0.5 );
			
			container.addChild( stereoBm01 );
			container.addChild( stereoBm02 );
			
			playStereo();
		}
		
		public function destroy ():void
		{
			stopStereo();
			
			container.removeChild( stereoBm01 );
			container.removeChild( stereoBm02 );
		}
		
		public function playIn ():void
		{
			
		}
		
		public function playOut ():void
		{
			
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
		
	}
}