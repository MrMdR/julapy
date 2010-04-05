package com.julapy.ph.hair.view
{
	import com.holler.assets.AssetLoader;
	import com.holler.core.View;

	import flash.display.MovieClip;
	import flash.display.Sprite;

	public class VideoGeneralView extends View
	{
		private var videoHolder	: MovieClip;
		private var videos		: Array;
		private var video		: MovieClip;
		private var videoIndex	: int = 0;

		public function VideoGeneralView(sprite:Sprite=null)
		{
			super(sprite);

			videoHolder = _sprite.getChildByName( "videoHolder" ) as MovieClip;

			videos =
			[
				"makeup.video.01",
				"makeup.video.02",
				"makeup.video.03",
				"makeup.video.04"
			];

//			initVideo();
		}

		/////////////////////////////////////////////
		//	VIDEO LOGIC.
		/////////////////////////////////////////////

		private function initVideo ():void
		{
			video = AssetLoader.getInstance().getClassInstance( videos[ videoIndex ] ) as MovieClip

			videoHolder.addChild( video );
		}

		private function killVideo ():void
		{
			if( video )
			{
				videoHolder.removeChild( video );

				video = null;
			}
		}

		private function stepVideo ():void
		{
			if( ++videoIndex >= videos.length )
				videoIndex = 0;
		}

		private function nextVideo ():void
		{
			stepVideo();
			killVideo();
			initVideo();
		}

		private function playVideo ( index : uint ):void
		{
			videoIndex = Math.min( videos.length - 1, Math.max( 0, index ) );
			killVideo();
			initVideo();
		}

		/////////////////////////////////////////////
		//	ENABLE.
		/////////////////////////////////////////////

		private function enable ():void
		{
			visible = true;
			doValidate();
		}

		private function disable ():void
		{
			visible = false;
			doValidate();
		}
	}
}