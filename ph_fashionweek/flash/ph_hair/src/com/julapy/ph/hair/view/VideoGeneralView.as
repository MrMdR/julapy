package com.julapy.ph.hair.view
{
	import caurina.transitions.Tweener;

	import com.holler.assets.AssetLoader;
	import com.holler.controls.BtnView;
	import com.holler.controls.VideoView;
	import com.holler.core.View;
	import com.holler.events.VideoViewEvent;
	import com.julapy.ph.hair.events.GirlEvent;
	import com.julapy.ph.hair.events.SectionEvent;
	import com.julapy.ph.hair.events.StyleEvent;
	import com.julapy.ph.hair.model.HairModel;
	import com.julapy.ph.hair.model.ModelLocator;
	import com.julapy.ph.hair.vo.StyleVO;

	import fl.motion.easing.Quadratic;

	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.MouseEvent;

	public class VideoGeneralView extends View
	{
		private var videoHolder		: MovieClip;
		private var video			: MovieClip;
		private var videoID			: String = "";

		private var videoStream			: VideoView;
		private var videoStreamHolder	: Sprite;

		private var continueBtn		: BtnView;

		private var bStreamVideo	: Boolean = true;

		public function VideoGeneralView(sprite:Sprite=null)
		{
			super(sprite);

			videoHolder		= _sprite.getChildByName( "videoHolder" ) as MovieClip;
			videoHolder.y	= (int)( ( ModelLocator.getInstance().commonModel.appHeight - ModelLocator.getInstance().hairModel.videoGenRect.height ) * 0.5 );

			continueBtn = new BtnView( _sprite.getChildByName( "continueBtn" ) as MovieClip );
			continueBtn.addEventListener( MouseEvent.MOUSE_DOWN, continueBtnHandler );

			ModelLocator.getInstance().hairModel.addEventListener( GirlEvent.GIRL_CHANGE,		girlChangeHandler );
			ModelLocator.getInstance().hairModel.addEventListener( StyleEvent.STYLE_CHANGE,		styleChangeHandler );
			ModelLocator.getInstance().hairModel.addEventListener( SectionEvent.SECTION_CHANGE,	sectionChangeHandler );
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
			ModelLocator.getInstance().hairModel.nextSection();
		}

		/////////////////////////////////////////////
		//	TIMELINE VIDEO
		/////////////////////////////////////////////

		private function initTimelineVideo ():void
		{
			video = AssetLoader.getInstance().getClassInstance( videoID ) as MovieClip

			videoHolder.addChild( video );
		}

		private function killTimelineVideo ():void
		{
			if( video )
			{
				videoHolder.removeChild( video );

				video = null;
			}
		}

		/////////////////////////////////////////////
		//	STREAMING VIDEO
		/////////////////////////////////////////////

		private function initStreamingVideo ():void
		{
			if( !videoStream )
			{
				videoHolder.addChild( videoStreamHolder = new Sprite() );

				videoStream = new VideoView( videoStreamHolder, ModelLocator.getInstance().hairModel.videoGenRect.clone() );
				videoStream.addEventListener( VideoViewEvent.READY, videoStreamReadyHandler );
				videoStream.addEventListener( VideoViewEvent.STOP,	videoStreamStopHandler );
				videoStream.videoURI	= videoID;
			}
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

		private function girlChangeHandler ( e : GirlEvent ):void
		{
			//
		}

		private function styleChangeHandler ( e : StyleEvent ):void
		{

		}

		private function sectionChangeHandler ( e : SectionEvent ):void
		{
			var girl : int;
			girl	= ModelLocator.getInstance().hairModel.girl;

			var style : int;
			style	= ModelLocator.getInstance().hairModel.style;

			var sec : int;
			sec		= ModelLocator.getInstance().hairModel.section;

			var styleVO : StyleVO;
			styleVO = ModelLocator.getInstance().hairModel.getStyleVO( girl, style );

			var valid : Boolean;
			valid = ( ( sec == HairModel.SECTION_INTRO ) ||  ( sec == HairModel.SECTION_OUTRO ) );

			if( valid )
			{
				enable();

				if( sec == HairModel.SECTION_INTRO )
				{
					if( bStreamVideo )
					{
						videoID = styleVO.introStream;
					}
					else
					{
						videoID = styleVO.introTimeline;
					}
				}

				if( sec == HairModel.SECTION_OUTRO )
				{
					if( bStreamVideo )
					{
						videoID = styleVO.outroStream;
					}
					else
					{
						videoID = styleVO.outroTimeline;
					}
				}

				if( bStreamVideo )
				{
					killStreamingVideo();
					initStreamingVideo();
				}
				else
				{
					killTimelineVideo();
					initTimelineVideo();
					playIn( true );
				}
			}
			else
			{
				disable();

				ModelLocator.getInstance().soundModel.stopAllSounds();

				if( bStreamVideo )
				{
					killStreamingVideo();
				}
				else
				{
					killTimelineVideo();
				}
			}
		}

		private function continueBtnHandler ( e : MouseEvent ):void
		{
			playIn( false )
		}
	}
}