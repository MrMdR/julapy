package com.julapy.ph.hair.view
{
	import com.holler.core.View;

	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;

	public class MenuDropAreaCrossView extends View
	{
		private var mc			: MovieClip;
		private var bShow		: Boolean = false;
		private var bPlayingIn	: Boolean = false;
		private var bPlayingOut	: Boolean = false;
		private var overFrame	: int = 9;

		public function MenuDropAreaCrossView(sprite:Sprite=null)
		{
			super(sprite);

			mc = _sprite as MovieClip;
			mc.gotoAndStop( 1 );

			visible = false;
			doValidate();

			mc.addEventListener( Event.ENTER_FRAME, enterFrameHandler );
		}

		public function show ( b : Boolean ):void
		{
			if( bShow == b )
				return;

			bShow = b;

			if( bShow )
			{
				if( bPlayingIn || bPlayingOut )
					return;

				bPlayingIn	= true;
				bPlayingOut	= false;

				mc.play();

				visible = true;
				doValidate();
			}
			else
			{
				bPlayingIn	= false;
				bPlayingOut = true;

				mc.play();
			}
		}

		private function enterFrameHandler ( e : Event ):void
		{
			if( mc.currentFrame == overFrame && bPlayingIn )
			{
				bPlayingIn	= false;

				mc.stop();
			}

			if( mc.currentFrame == mc.totalFrames )
			{
				bPlayingIn	= false;
				bPlayingOut = false;

				mc.gotoAndStop( 1 );

				visible = false;
				doValidate();
			}
		}
	}
}