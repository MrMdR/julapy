package com.julapy.ph.makeup.view
{
	import com.holler.assets.AssetLoader;
	import com.holler.controls.BtnView;
	import com.holler.core.View;
	import com.julapy.ph.makeup.events.SectionEvent;
	import com.julapy.ph.makeup.model.MakeupModel;
	import com.julapy.ph.makeup.model.ModelLocator;

	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.MouseEvent;

	public class VideoBaseView extends View
	{
		private var videoHolder	: MovieClip;
		private var videos		: Array;
		private var video		: MovieClip;
		private var videoIndex	: int = 0;

		private var continueBtn	: BtnView;

		public function VideoBaseView(sprite:Sprite=null)
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

			continueBtn = new BtnView( _sprite.getChildByName( "continueBtn" ) as MovieClip );
			continueBtn.addEventListener( MouseEvent.MOUSE_DOWN, continueBtnHandler );

			ModelLocator.getInstance().makeupModel.addEventListener( SectionEvent.SECTION_CHANGE, sectionChangeHandler );
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

		/////////////////////////////////////////////
		//	HANDLERS.
		/////////////////////////////////////////////

		private function sectionChangeHandler ( e : SectionEvent ):void
		{
			var girl	: int;
			var section : int;

			girl	= ModelLocator.getInstance().makeupModel.girl;
			section = ModelLocator.getInstance().makeupModel.section;

			if( section == MakeupModel.SECTION_INTRO || section == MakeupModel.SECTION_OUTRO )
			{
				if( girl == MakeupModel.GIRL_ONE )
				{
					if( section == MakeupModel.SECTION_INTRO )
						playVideo( 0 );

					if( section == MakeupModel.SECTION_OUTRO )
						playVideo( 1 );
				}

				if( girl == MakeupModel.GIRL_TWO )
				{
					if( section == MakeupModel.SECTION_INTRO )
						playVideo( 2 );

					if( section == MakeupModel.SECTION_OUTRO )
						playVideo( 3 );
				}

				enable();
			}

			if( section == MakeupModel.SECTION_PLAY )
			{
				killVideo();
				disable();
			}
		}

		private function continueBtnHandler ( e : MouseEvent ):void
		{
			ModelLocator.getInstance().makeupModel.nextSection();
		}
	}
}