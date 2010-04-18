package com.julapy.ph.makeup.view
{
	import caurina.transitions.Tweener;

	import com.holler.assets.AssetLoader;
	import com.holler.controls.BtnView;
	import com.holler.controls.VideoView;
	import com.holler.core.View;
	import com.holler.events.VideoViewEvent;
	import com.julapy.ph.makeup.events.AttractorChangeEvent;
	import com.julapy.ph.makeup.events.SectionEvent;
	import com.julapy.ph.makeup.model.MakeupModel;
	import com.julapy.ph.makeup.model.ModelLocator;

	import fl.motion.easing.Quadratic;

	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.MouseEvent;

	public class VideoBaseView extends View
	{
		private var bEnabled	: Boolean = false;

		private var videoHolder	: MovieClip;
		private var videos		: Array;
		private var video		: MovieClip;
		private var videoID		: String = "";
		private var videoIndex	: int = 0;

		private var continueBtn	: BtnView;

		private var videoStream			: VideoView;
		private var videoStreamHolder	: Sprite;

		private var bStreamVideo		: Boolean = true;

		public function VideoBaseView(sprite:Sprite=null)
		{
			super(sprite);

			videoHolder		= _sprite.getChildByName( "videoHolder" ) as MovieClip;
			videoHolder.y	= (int)( ( ModelLocator.getInstance().commondModel.appHeight - ModelLocator.getInstance().makeupModel.videoRect.height ) * 0.5 );

			videos =
			[
				"makeup.video.01",
				"makeup.video.02",
				"makeup.video.03",
				"makeup.video.04"
			];

			continueBtn = new BtnView( _sprite.getChildByName( "continueBtn" ) as MovieClip );
			continueBtn.addEventListener( MouseEvent.MOUSE_DOWN, continueBtnHandler );

			ModelLocator.getInstance().makeupModel.addEventListener( SectionEvent.SECTION_CHANGE,			sectionChangeHandler );
			ModelLocator.getInstance().makeupModel.addEventListener( AttractorChangeEvent.ATTRACTOR_CHANGE,	attractorChangeHandler );
		}

		/////////////////////////////////////////////
		//	PLAY IN / OUT.
		/////////////////////////////////////////////

		private function playIn ( b : Boolean ):void
		{
			if( b )
			{
				videoHolder.alpha = 0;

				Tweener.addTween
				(
					videoHolder,
					{
						alpha		: 1.0,
						time		: 0.3,
						delay		: 0.0,
						transition	: Quadratic.easeOut,
						onStart		: null,
						onUpdate	: null,
						onComplete	: null
					}
				);
			}
			else
			{
				Tweener.addTween
				(
					videoHolder,
					{
						alpha		: 0.0,
						time		: 0.3,
						delay		: 0.0,
						transition	: Quadratic.easeOut,
						onStart		: null,
						onUpdate	: null,
						onComplete	: playOutCompleteHandler
					}
				);
			}
		}

		private function playOutCompleteHandler ():void
		{
			if( ModelLocator.getInstance().makeupModel.bAttractor )
			{
				initAttractor();
			}
			else
			{
				ModelLocator.getInstance().makeupModel.nextSection();
			}
		}

		/////////////////////////////////////////////
		//	VIDEO LOGIC.
		/////////////////////////////////////////////

		private function initVideo ():void
		{
			video = AssetLoader.getInstance().getClassInstance( videoID ) as MovieClip

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

		/////////////////////////////////////////////
		//	VIDEO STREAM.
		/////////////////////////////////////////////

		private function initStreamingVideo ():void
		{
			if( videoStream )
				killStreamingVideo();

			videoHolder.addChild( videoStreamHolder = new Sprite() );

			videoStream = new VideoView( videoStreamHolder, ModelLocator.getInstance().makeupModel.videoRect );
			videoStream.addEventListener( VideoViewEvent.READY, videoStreamReadyHandler );
			videoStream.addEventListener( VideoViewEvent.STOP,	videoStreamStopHandler );
			videoStream.videoURI	= videoID;
		}

		private function killStreamingVideo ():void
		{
			if( videoStream )
			{
				videoHolder.removeChild( videoStreamHolder );

				videoStream.removeEventListener( VideoViewEvent.READY,	videoStreamReadyHandler );
				videoStream.removeEventListener( VideoViewEvent.STOP,	videoStreamStopHandler );
//				videoStream.paused		= false;
				videoStream.container	= null;
				videoStream				= null;
				videoStreamHolder		= null;
			}
		}

		private function videoStreamReadyHandler ( e : VideoViewEvent ):void
		{
			videoStream.paused = false;

			playIn( true );
		}

		private function videoStreamStopHandler ( e : VideoViewEvent ):void
		{
			playIn( false );
		}

		/////////////////////////////////////////////
		//	ATTRACTOR.
		/////////////////////////////////////////////

		private function initAttractor ():void
		{
			ModelLocator.getInstance().makeupModel.nextAttractor();

			videoID = ModelLocator.getInstance().makeupModel.attractorFlvPath;

			initStreamingVideo();
		}

		/////////////////////////////////////////////
		//	ENABLE.
		/////////////////////////////////////////////

		private function enable ():void
		{
			bEnabled	= true;
			visible		= true;
			doValidate();
		}

		private function disable ():void
		{
			bEnabled	= false;
			visible 	= false;
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
				var valid : Boolean;
				valid = false;

				if( girl == MakeupModel.GIRL_ONE )
				{
					if( section == MakeupModel.SECTION_INTRO )
					{
						valid		= true;
						videoIndex	= 0;
					}

					if( section == MakeupModel.SECTION_OUTRO )
					{
						valid		= true;
						videoIndex	= 1;
					}
				}

				if( girl == MakeupModel.GIRL_TWO )
				{
					if( section == MakeupModel.SECTION_INTRO )
					{
						valid		= true;
						videoIndex	= 2;
					}

					if( section == MakeupModel.SECTION_OUTRO )
					{
						valid		= true;
						videoIndex	= 3;
					}
				}

				if( valid )
				{
					if( bStreamVideo )
					{
						if( section == MakeupModel.SECTION_INTRO )
						{
							videoID = ModelLocator.getInstance().makeupModel.girlVO.introVideo;
						}
						else
						{
							videoID = ModelLocator.getInstance().makeupModel.girlVO.outroVideo;
						}

						initStreamingVideo();
					}
					else
					{
						videoID = videos[ videoIndex ];

						killVideo();
						initVideo();
						playIn( true );
					}
				}

				enable();
			}

			if( section == MakeupModel.SECTION_PLAY )
			{
				if( bStreamVideo )
				{
					killStreamingVideo();
				}
				else
				{
					killVideo();
				}

				disable();
			}
		}

		private function attractorChangeHandler ( e : AttractorChangeEvent ):void
		{
			if( !bEnabled )
				return;

			playIn( false );
		}

		private function continueBtnHandler ( e : MouseEvent ):void
		{
			playIn( false );
		}
	}
}