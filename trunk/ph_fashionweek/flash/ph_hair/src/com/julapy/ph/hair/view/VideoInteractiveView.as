package com.julapy.ph.hair.view
{
	import caurina.transitions.Tweener;

	import com.holler.assets.AssetLoader;
	import com.holler.core.View;
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

	public class VideoInteractiveView extends View
	{
		private var videoHolder	: MovieClip;
		private var video		: MovieClip;
		private var videoID		: String = "";
		private var cover		: MovieClip;

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
		//	VIDEO LOGIC.
		/////////////////////////////////////////////

		private function initVideo ():void
		{
			video = AssetLoader.getInstance().getClassInstance( videoID ) as MovieClip
			video.gotoAndStop( 1 );

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

		private function playVideo ():void
		{
			if( video )
			{
				video.gotoAndPlay( 1 );
				video.addEventListener( Event.ENTER_FRAME, videoEnterFrameHandler );
			}
		}

		private function stopVideo ():void
		{
			if( video )
			{
				video.removeEventListener( Event.ENTER_FRAME, videoEnterFrameHandler );
				video.stop();
			}
		}

		private function videoEnterFrameHandler ( e : Event ):void
		{
			if( video.currentFrame == video.totalFrames )
//			if( video.currentFrame == 250 )
			{
				stopVideo();

				ModelLocator.getInstance().hairModel.nextStylePart();
			}
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
		//	COVER.
		/////////////////////////////////////////////

		private function showCover ( b : Boolean ):void
		{
			if( b )
			{
				cover.visible	= true;
				cover.alpha		= 0;

				Tweener.addTween
				(
					cover,
					{
						alpha			: 0.5,
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
				stopVideo();
				killVideo();

				return;
			}

			var girl : int;
			girl	= ModelLocator.getInstance().hairModel.girl;

			var style : int;
			style	= ModelLocator.getInstance().hairModel.style;

			var styleVO : StyleVO;
			styleVO = ModelLocator.getInstance().hairModel.getStyleVO( girl, style );

			var playPart : int;
			playPart = ModelLocator.getInstance().hairModel.stylePart;

			videoID = styleVO.playParts[ playPart ];

			enable();
			stopVideo();
			killVideo();
			initVideo();
		}

		private function stylePartChangeHandler ( e : StylePartEvent ):void
		{
			var girl : int;
			girl	= ModelLocator.getInstance().hairModel.girl;

			var style : int;
			style	= ModelLocator.getInstance().hairModel.style;

			var styleVO : StyleVO;
			styleVO = ModelLocator.getInstance().hairModel.getStyleVO( girl, style );

			var playPart : int;
			playPart = ModelLocator.getInstance().hairModel.stylePart;

			videoID = styleVO.playParts[ playPart ];

			stopVideo();
			killVideo();
			initVideo();

			showCover( true );
		}

		private function menuSelectHandler ( e : MenuEvent ):void
		{
			playVideo();
			showCover( false );
		}
	}
}