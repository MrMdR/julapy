package com.julapy.ph.hair.view
{
	import caurina.transitions.Tweener;

	import com.holler.assets.AssetLoader;
	import com.holler.controls.BtnView;
	import com.holler.controls.VideoView;
	import com.holler.core.View;
	import com.holler.events.VideoViewEvent;
	import com.julapy.ph.hair.events.AttractorChangeEvent;
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
		private var bEnabled		: Boolean = false;

		private var videoHolder		: MovieClip;
		private var video			: MovieClip;
		private var videoID			: String = "";

		private var videoStream			: VideoView;
		private var videoStreamHolder	: Sprite;

		private var continueBtn		: BtnView;

		private var top				: MovieClip;
		private var btm				: MovieClip;

		private var bStreamVideo	: Boolean = true;

		public function VideoGeneralView(sprite:Sprite=null)
		{
			super(sprite);

			videoHolder		= _sprite.getChildByName( "videoHolder" ) as MovieClip;
			videoHolder.y	= (int)( ( ModelLocator.getInstance().commonModel.appHeight - ModelLocator.getInstance().hairModel.videoGenRect.height ) * 0.5 );

			var useContinueBtn : Boolean;
			useContinueBtn = true;

			var continueBtnMc : MovieClip;
			continueBtnMc = _sprite.getChildByName( "continueBtn" ) as MovieClip;

			if( useContinueBtn )
			{
				continueBtn = new BtnView( continueBtnMc );
				continueBtn.addEventListener( MouseEvent.MOUSE_DOWN, continueBtnHandler );
				continueBtn.alpha = 0;
				continueBtn.doValidate();
			}

			top	= _sprite.getChildByName( "top" ) as MovieClip;
			btm = _sprite.getChildByName( "btm" ) as MovieClip;

			ModelLocator.getInstance().hairModel.addEventListener( GirlEvent.GIRL_CHANGE,					girlChangeHandler );
			ModelLocator.getInstance().hairModel.addEventListener( StyleEvent.STYLE_CHANGE,					styleChangeHandler );
			ModelLocator.getInstance().hairModel.addEventListener( SectionEvent.SECTION_CHANGE,				sectionChangeHandler );
			ModelLocator.getInstance().hairModel.addEventListener( AttractorChangeEvent.ATTRACTOR_CHANGE,	attractorChangeHandler );
		}

		/////////////////////////////////////////////
		//	PLAY IN / OUT.
		/////////////////////////////////////////////

		public function playIn ( b : Boolean ):void
		{
			if( b )
			{
//				top.y = -top.height;

				Tweener.addTween
				(
					top,
					{
						y			: 0,
						time		: 0.3,
						delay		: 0.0,
						transition	: Quadratic.easeOut,
						onStart		: null,
						onUpdate	: null,
						onComplete	: null
					}
				);

//				btm.y = ModelLocator.getInstance().commonModel.appHeight + btm.height;

				Tweener.addTween
				(
					btm,
					{
						y			: ModelLocator.getInstance().commonModel.appHeight,
						time		: 0.3,
						delay		: 0.0,
						transition	: Quadratic.easeOut,
						onStart		: null,
						onUpdate	: null,
						onComplete	: null
					}
				);

				videoHolder.alpha = 0;

				Tweener.addTween
				(
					videoHolder,
					{
						alpha		: 1.0,
						time		: 0.3,
						delay		: 0.3,
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
					top,
					{
						y			: -top.height,
						time		: 0.3,
						delay		: 0.0,
						transition	: Quadratic.easeOut,
						onStart		: null,
						onUpdate	: null,
						onComplete	: null
					}
				);

				Tweener.addTween
				(
					btm,
					{
						y			: ModelLocator.getInstance().commonModel.appHeight + btm.height,
						time		: 0.3,
						delay		: 0.0,
						transition	: Quadratic.easeOut,
						onStart		: null,
						onUpdate	: null,
						onComplete	: null
					}
				);

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
			if( ModelLocator.getInstance().hairModel.bAttractor )
			{
				initAttractor();
			}
			else
			{
				ModelLocator.getInstance().hairModel.nextSection();
			}
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
			if( videoStream )
				killStreamingVideo();

			videoHolder.addChild( videoStreamHolder = new Sprite() );

			videoStream = new VideoView( videoStreamHolder, ModelLocator.getInstance().hairModel.videoGenRect.clone() );
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
			ModelLocator.getInstance().hairModel.nextAttractor();

			videoID = ModelLocator.getInstance().hairModel.attractorFlvPath;

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
			visible		= false;
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

		private function sectionChangeHandler ( e : SectionEvent = null ):void
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

		private function attractorChangeHandler ( e : AttractorChangeEvent ):void
		{
			if( !bEnabled )
				return;

			playIn( false );
		}

		/////////////////////////////////////////////////////
		//	BTN HANDLER.
		/////////////////////////////////////////////////////

		private function continueBtnHandler ( e : MouseEvent ):void
		{
			playIn( false );
		}
	}
}