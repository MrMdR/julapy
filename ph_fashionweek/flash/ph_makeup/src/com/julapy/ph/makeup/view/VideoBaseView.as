package com.julapy.ph.makeup.view
{
	import caurina.transitions.Tweener;

	import com.holler.assets.AssetLoader;
	import com.holler.controls.BtnView;
	import com.holler.controls.VideoView;
	import com.holler.core.View;
	import com.holler.events.VideoViewEvent;
	import com.julapy.ph.makeup.events.SectionEvent;
	import com.julapy.ph.makeup.model.MakeupModel;
	import com.julapy.ph.makeup.model.ModelLocator;

	import fl.motion.easing.Quadratic;

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

		private var videoStream			: VideoView;
		private var videoStreamHolder	: Sprite;
		private var videoStreamPaths	: Array = new Array();

		private var bStreamVideo		: Boolean = true;

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

			videoStreamPaths =
			[
				"flv/g1_s1_p1.f4v",
				"flv/g1_s1_p2.f4v",
				"flv/g1_s1_p3.f4v",
				"flv/g1_s2_p1.f4v"
			];

			continueBtn = new BtnView( _sprite.getChildByName( "continueBtn" ) as MovieClip );
			continueBtn.addEventListener( MouseEvent.MOUSE_DOWN, continueBtnHandler );

			ModelLocator.getInstance().makeupModel.addEventListener( SectionEvent.SECTION_CHANGE, sectionChangeHandler );
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
			ModelLocator.getInstance().makeupModel.nextSection();
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

		/////////////////////////////////////////////
		//	VIDEO STREAM.
		/////////////////////////////////////////////

		private function initStreamingVideo ():void
		{
			if( !videoStream )
			{
				videoHolder.addChild( videoStreamHolder = new Sprite() );

				videoStream = new VideoView( videoStreamHolder, ModelLocator.getInstance().makeupModel.videoRect );
				videoStream.addEventListener( VideoViewEvent.READY, videoStreamReadyHandler );
				videoStream.addEventListener( VideoViewEvent.STOP,	videoStreamStopHandler );
				videoStream.loop		= true;
				videoStream.videoURI	= videoStreamPaths[ videoIndex ];
			}
		}

		private function killStreamingVideo ():void
		{
			if( videoStream )
			{
				videoHolder.removeChild( videoStreamHolder );

				videoStream.removeEventListener( VideoViewEvent.READY,	videoStreamReadyHandler );
				videoStream.removeEventListener( VideoViewEvent.STOP,	videoStreamStopHandler );
				videoStream.paused		= false;
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
						killStreamingVideo();
						initStreamingVideo();
					}
					else
					{
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

		private function continueBtnHandler ( e : MouseEvent ):void
		{
			playIn( false );
		}
	}
}