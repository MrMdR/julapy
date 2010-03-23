package com.julapy.ph.makeup.view
{
	import com.holler.core.View;

	import flash.display.DisplayObjectContainer;
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;

	public class MakeupMaskView extends View
	{
		private var tweens : Array = new Array();

		public function MakeupMaskView(sprite:Sprite=null)
		{
			super( sprite );

			for( var i:int=0; i<_sprite.numChildren; i++ )
			{
				var tween : MovieClip;
				tween = _sprite.getChildAt( i ) as MovieClip;
				tween.gotoAndStop( 1 );

				tweens.push( tween );
			}
		}

		public override function set container(value:DisplayObjectContainer):void
		{
			if( !value )
			{
				stop();
			}

			super.container = value;
		}

		public function get mc ():MovieClip
		{
			return _sprite as MovieClip;
		}

		public function play ():void
		{
			for( var i:int=0; i<tweens.length; i++ )
			{
				var tween : MovieClip;
				tween = tweens[ i ] as MovieClip;
				tween.gotoAndPlay( 1 );
				tween.addEventListener( Event.ENTER_FRAME, enterFrameHandler );
			}
		}

		public function stop ():void
		{
			for( var i:int=0; i<tweens.length; i++ )
			{
				var tween : MovieClip;
				tween = tweens[ i ] as MovieClip;
				tween.stop();
				tween.removeEventListener( Event.ENTER_FRAME, enterFrameHandler );
			}
		}

		private function enterFrameHandler ( e : Event ):void
		{
			for( var i:int=0; i<tweens.length; i++ )
			{
				var tween : MovieClip;
				tween = tweens[ i ] as MovieClip;

				if( tween.currentFrame == tween.totalFrames )
				{
					stop();

					dispatchEvent( new Event( Event.COMPLETE ) );
				}
			}
		}
	}
}