package com.julapy.ph.hair.view
{
	import com.holler.assets.AssetLoader;
	import com.holler.controls.BtnView;
	import com.holler.core.View;
	import com.julapy.ph.hair.events.GirlEvent;
	import com.julapy.ph.hair.events.SectionEvent;
	import com.julapy.ph.hair.events.StyleEvent;
	import com.julapy.ph.hair.model.HairModel;
	import com.julapy.ph.hair.model.ModelLocator;
	import com.julapy.ph.hair.vo.StyleVO;

	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.MouseEvent;

	public class VideoGeneralView extends View
	{
		private var videoHolder	: MovieClip;
		private var video		: MovieClip;
		private var videoID		: String = "";

		private var continueBtn	: BtnView;

		public function VideoGeneralView(sprite:Sprite=null)
		{
			super(sprite);

			videoHolder = _sprite.getChildByName( "videoHolder" ) as MovieClip;

			continueBtn = new BtnView( _sprite.getChildByName( "continueBtn" ) as MovieClip );
			continueBtn.addEventListener( MouseEvent.MOUSE_DOWN, continueBtnHandler );

			ModelLocator.getInstance().hairModel.addEventListener( GirlEvent.GIRL_CHANGE,		girlChangeHandler );
			ModelLocator.getInstance().hairModel.addEventListener( StyleEvent.STYLE_CHANGE,		styleChangeHandler );
			ModelLocator.getInstance().hairModel.addEventListener( SectionEvent.SECTION_CHANGE,	sectionChangeHandler );
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
					videoID = styleVO.intro;
				}

				if( sec == HairModel.SECTION_OUTRO )
				{
					videoID = styleVO.outro;
				}

				killVideo();
				initVideo();
			}
			else
			{
				disable();

				killVideo();
			}
		}

		private function continueBtnHandler ( e : MouseEvent ):void
		{
			ModelLocator.getInstance().hairModel.nextSection();
		}
	}
}