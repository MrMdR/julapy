package com.julapy.ph.hair.view
{
	import caurina.transitions.Tweener;

	import com.holler.assets.AssetLoader;
	import com.holler.controls.VideoView;
	import com.holler.core.View;
	import com.holler.events.VideoViewEvent;
	import com.julapy.ph.hair.events.GirlEvent;
	import com.julapy.ph.hair.events.MenuEvent;
	import com.julapy.ph.hair.events.SectionEvent;
	import com.julapy.ph.hair.events.StyleEvent;
	import com.julapy.ph.hair.events.StylePartEvent;
	import com.julapy.ph.hair.model.HairModel;
	import com.julapy.ph.hair.model.ModelLocator;
	import com.julapy.ph.hair.vo.StyleVO;

	import fl.motion.easing.Quadratic;

	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.TimerEvent;
	import flash.utils.Timer;

	public class VideoInteractiveView extends View
	{
		private var bEnabled	: Boolean = false;

		private var playPart	: int = -1;

		private var videoHolder	: MovieClip;
		private var video		: MovieClip;
		private var videoID		: String = "";
		private var cover		: MovieClip;

		private var videoStream			: VideoView;
		private var videoStreamHolder	: Sprite;

		private var infoSoundTimer		: Timer;
		private var finalLookSoundTimer	: Timer;

		private var bUseTimelineVideo	: Boolean = false;

		public function VideoInteractiveView(sprite:Sprite=null)
		{
			super(sprite);

			videoHolder = _sprite.getChildByName( "videoHolder" ) as MovieClip;
			cover		= _sprite.getChildByName( "cover" ) as MovieClip;
			cover.visible	= false;
			cover.alpha		= 0.0;

			ModelLocator.getInstance().hairModel.addEventListener( GirlEvent.GIRL_CHANGE,				girlChangeHandler );
			ModelLocator.getInstance().hairModel.addEventListener( StyleEvent.STYLE_CHANGE,				styleChangeHandler );
			ModelLocator.getInstance().hairModel.addEventListener( SectionEvent.SECTION_CHANGE,			sectionChangeHandler );
			ModelLocator.getInstance().hairModel.addEventListener( StylePartEvent.STYLE_PART_CHANGE,	stylePartChangeHandler );
			ModelLocator.getInstance().hairModel.addEventListener( MenuEvent.MENU_SELECT,				menuSelectHandler );
		}

		/////////////////////////////////////////////
		//	TIMELINE VIDEO.
		/////////////////////////////////////////////

		private function initTimelineVideo ():void
		{
			video = AssetLoader.getInstance().getClassInstance( videoID ) as MovieClip
			video.gotoAndStop( 1 );

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

		private function playTimelineVideo ():void
		{
			if( video )
			{
				video.gotoAndPlay( 1 );
				video.addEventListener( Event.ENTER_FRAME, timelineVideoEnterFrameHandler );
			}
		}

		private function stopTimelineVideo ():void
		{
			if( video )
			{
				video.removeEventListener( Event.ENTER_FRAME, timelineVideoEnterFrameHandler );
				video.stop();
			}
		}

		private function timelineVideoEnterFrameHandler ( e : Event ):void
		{
//			if( video.currentFrame == video.totalFrames )
			if( video.currentFrame == 180 )
			{
				stopTimelineVideo();

				ModelLocator.getInstance().hairModel.nextStylePart();
			}
		}

		/////////////////////////////////////////////
		//	STREAM VIDEO.
		/////////////////////////////////////////////

		private function initStreamingVideo ():void
		{
			if( videoStream )
				killStreamingVideo();

			videoHolder.addChild( videoStreamHolder = new Sprite() );

			videoStream = new VideoView( videoStreamHolder, ModelLocator.getInstance().hairModel.videoIntRect.clone() );
			videoStream.addEventListener( VideoViewEvent.READY, videoStreamReadyHandler );
			videoStream.addEventListener( VideoViewEvent.STOP,	videoStreamStopHandler );
			videoStream.videoURI = videoID;
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

		private function playStreamingVideo ():void
		{
			if( videoStream )
			{
				videoStream.paused = false;
			}
		}

		private function stopStreamingVideo ():void
		{
			if( videoStream )
			{
				videoStream.paused = true;
			}
		}

		private function playInStreamingVideo ( b : Boolean ):void
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
						onComplete	: playOutStreamingVideoCompleteHandler
					}
				);
			}
		}

		private function videoStreamReadyHandler ( e : VideoViewEvent ):void
		{
			playInStreamingVideo( true );

			ModelLocator.getInstance().hairModel.interactiveVideoPlayingIn();
		}

		private function videoStreamStopHandler ( e : VideoViewEvent ):void
		{
			playInStreamingVideo( false );

			ModelLocator.getInstance().hairModel.interactiveVideoPlayingOut();
		}

		private function playOutStreamingVideoCompleteHandler ():void
		{
			ModelLocator.getInstance().hairModel.nextStylePart();
		}

		/////////////////////////////////////////////
		//	ENABLE.
		/////////////////////////////////////////////

		private function enable ():void
		{
			bEnabled	= true;

			visible 	= true;
			doValidate();
		}

		private function disable ():void
		{
			bEnabled	= false;

			visible		= false;
			doValidate();
		}

		/////////////////////////////////////////////
		//	COVER.
		/////////////////////////////////////////////

		private function showCover ( b : Boolean ):void
		{
			if( b )
			{
				cover.visible	= true;
				cover.alpha		= 0.5;

				return;

				//-- tween.

				cover.visible	= true;
				cover.alpha		= 0;

				Tweener.addTween
				(
					cover,
					{
						alpha			: 0.3,
						time			: 0.3,
						delay			: 0.0,
						transition		: Quadratic.easeOut,
						onStart			: null,
						onUpdate		: null,
						onComplete		: null
					}
				);
			}
			else
			{
				Tweener.addTween
				(
					cover,
					{
						alpha			: 0.0,
						time			: 0.3,
						delay			: 0.0,
						transition		: Quadratic.easeOut,
						onStart			: null,
						onUpdate		: null,
						onComplete		: hideCoverCompleteHandler
					}
				);
			}
		}

		private function hideCoverCompleteHandler ():void
		{
			cover.visible = false;
		}

		/////////////////////////////////////////////
		//	INFO SOUND TIMER.
		/////////////////////////////////////////////

		private function initInfoSoundTimer ():void
		{
			if( infoSoundTimer )
				killInfoSoundTimer();

			infoSoundTimer = new Timer( 4000, 1 );
			infoSoundTimer.addEventListener( TimerEvent.TIMER_COMPLETE, infoSoundTimerHandler );
			infoSoundTimer.start();
		}

		private function killInfoSoundTimer ():void
		{
			if( infoSoundTimer )
			{
				infoSoundTimer.removeEventListener( TimerEvent.TIMER_COMPLETE, infoSoundTimerHandler );
				infoSoundTimer.stop();
				infoSoundTimer = null;
			}
		}

		private function infoSoundTimerHandler ( e : TimerEvent ):void
		{
			killInfoSoundTimer();

			ModelLocator.getInstance().soundModel.playInteractiveInfo( playPart );
		}
		/////////////////////////////////////////////
		//	FINAL LOOK SOUND TIMER.
		/////////////////////////////////////////////

		private function initFinalLookSoundTimer ():void
		{
			if( finalLookSoundTimer )
				killFinalLookSoundTimer();

			var time : int;
			time = 23000;

			if( playPart == 0 )
			{
				time= (int)( ( videoStream.duration - 3.1 ) * 1000 );
			}
			else if( playPart == 1 )
			{
				time= (int)( ( videoStream.duration - 4.6 ) * 1000 );
			}
			else if( playPart == 2 )
			{
				time= (int)( ( videoStream.duration - 7.6 ) * 1000 );
			}

			finalLookSoundTimer = new Timer( time, 1 );
			finalLookSoundTimer.addEventListener( TimerEvent.TIMER_COMPLETE, finalLookSoundTimerHandler );
			finalLookSoundTimer.start();
		}

		private function killFinalLookSoundTimer ():void
		{
			if( finalLookSoundTimer )
			{
				finalLookSoundTimer.removeEventListener( TimerEvent.TIMER_COMPLETE, finalLookSoundTimerHandler );
				finalLookSoundTimer.stop();
				finalLookSoundTimer = null;
			}
		}

		private function finalLookSoundTimerHandler ( e : TimerEvent ):void
		{
			killFinalLookSoundTimer();

			ModelLocator.getInstance().soundModel.playFinalLook( playPart );
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
			var sec : int;
			sec		= ModelLocator.getInstance().hairModel.section;

			if( sec != HairModel.SECTION_PLAY )
			{
				disable();

				if( bUseTimelineVideo )
				{
					stopTimelineVideo();
					killTimelineVideo();
				}
				else
				{
					stopStreamingVideo();
					killStreamingVideo();
				}

				killFinalLookSoundTimer();
				killInfoSoundTimer();

				return;
			}

			var girl : int;
			girl	= ModelLocator.getInstance().hairModel.girl;

			var style : int;
			style	= ModelLocator.getInstance().hairModel.style;

			var styleVO : StyleVO;
			styleVO = ModelLocator.getInstance().hairModel.getStyleVO( girl, style );

			playPart = ModelLocator.getInstance().hairModel.stylePart;

			if( bUseTimelineVideo )
			{
				videoID = styleVO.playParts[ playPart ];
			}
			else
			{
				videoID = styleVO.playStreams[ playPart ];
			}

			enable();

			if( bUseTimelineVideo )
			{
				stopTimelineVideo();
				killTimelineVideo();
				initTimelineVideo();
			}
			else
			{
				initStreamingVideo();
			}

			showCover( true );

			//-- sound.

			ModelLocator.getInstance().soundModel.playInteractiveInstruction( playPart );
		}

		private function stylePartChangeHandler ( e : StylePartEvent ):void
		{
			if( !bEnabled )
				return;

			var girl : int;
			girl	= ModelLocator.getInstance().hairModel.girl;

			var style : int;
			style	= ModelLocator.getInstance().hairModel.style;

			var styleVO : StyleVO;
			styleVO = ModelLocator.getInstance().hairModel.getStyleVO( girl, style );

			playPart = ModelLocator.getInstance().hairModel.stylePart;

			if( bUseTimelineVideo )
			{
				videoID = styleVO.playParts[ playPart ];
			}
			else
			{
				videoID = styleVO.playStreams[ playPart ];
			}

			if( bUseTimelineVideo )
			{
				stopTimelineVideo();
				killTimelineVideo();
				initTimelineVideo();
			}
			else
			{
				stopStreamingVideo();
				killStreamingVideo();
				initStreamingVideo();
			}

			showCover( true );

			//-- sound.

			ModelLocator.getInstance().soundModel.playInteractiveInstruction( playPart );
		}

		private function menuSelectHandler ( e : MenuEvent ):void
		{
			if( !bEnabled )
				return;

			if( bUseTimelineVideo )
			{
				playTimelineVideo();
			}
			else
			{
				playStreamingVideo();
			}

			showCover( false );

			//-- sound.

			ModelLocator.getInstance().soundModel.playInteractiveToolSelected( playPart );

			initInfoSoundTimer();
			initFinalLookSoundTimer();
		}
	}
}