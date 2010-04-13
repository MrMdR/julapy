package com.julapy.ph.hair.view
{
	import com.holler.core.View;

	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;

	public class MenuDropAreaRingView extends View
	{
		private var bShow	: Boolean = false;
		private var bInit	: Boolean = false;
		private var bOver	: Boolean = false;

		private var mc		: MovieClip;
		private var ring	: MovieClip;
		private var ring0	: MovieClip;
		private var ring1	: MovieClip;

		public function MenuDropAreaRingView(sprite:Sprite=null)
		{
			super(sprite);

			mc = _sprite as MovieClip;
			mc.gotoAndStop( 1 );

			visible = false;
			doValidate();
		}

		public function show ( b : Boolean ):void
		{
			if( bShow == b )
				return;

			bShow = b;

			if( bShow )
			{
				mc.play();

				visible = true;
				doValidate();

				mc.addEventListener( Event.ENTER_FRAME, enterFrameHandler );
			}
			else
			{
				ring.play();

				mc.addEventListener( Event.ENTER_FRAME, enterFrameHandler );
			}
		}

		public function over ( b : Boolean ):void
		{
			if( !bInit )
				return;

			if( bOver == b )
				return;

			bOver = b;

			if( bOver )
			{
				mc.addEventListener( Event.ENTER_FRAME, enterFrameHandler );
			}
			else
			{
				mc.removeEventListener( Event.ENTER_FRAME, enterFrameHandler );
			}
		}

		private function enterFrameHandler ( e : Event ):void
		{
			if( bInit )
			{
				ring0.rotation += 10;
				ring1.rotation -= 10;
			}

			if( mc.currentFrame == 9 )
			{
				if( !bInit )
				{
					mc.removeEventListener( Event.ENTER_FRAME, enterFrameHandler );

					bInit = true;

					mc.stop();

					ring	= mc.getChildByName( "ring" ) as MovieClip;
					ring0	= ring.getChildByName( "ring0" ) as MovieClip;
					ring1	= ring.getChildByName( "ring1" ) as MovieClip;
				}
			}

			if( mc.currentFrame == mc.totalFrames )
			{
				mc.removeEventListener( Event.ENTER_FRAME, enterFrameHandler );

				visible = false;
				doValidate();
			}
		}

	}
}