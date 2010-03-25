package com.julapy.ph.makeup.view
{
	import caurina.transitions.Tweener;

	import com.holler.core.View;
	import com.julapy.ph.makeup.events.ModeEvent;
	import com.julapy.ph.makeup.model.MakeupModel;
	import com.julapy.ph.makeup.model.ModelLocator;

	import fl.motion.easing.Quadratic;

	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.filters.BlurFilter;

	public class FocusBlurView extends View
	{
		private var face		: MovieClip;

		public  var tweenValue	: Number = 0;
		private var fillAlpha	: Number = 0.4;

		private var fadeTop		: MovieClip;
		private var fadeBtm		: MovieClip;

		private var faceBm		: Bitmap;
		private var faceBmd		: BitmapData;
		private var maskBmd		: BitmapData;
		private var blurFilter	: BlurFilter = new BlurFilter( 8.0, 8.0, 4 );

		public function FocusBlurView( sprite : Sprite, face : Sprite )
		{
			super( sprite );

			this.face = face as MovieClip;

			ModelLocator.getInstance().makeupModel.addEventListener( ModeEvent.MODE_ZOOM_IN,	modeZoomInHandler );
			ModelLocator.getInstance().makeupModel.addEventListener( ModeEvent.MODE_ZOOM_OUT,	modeZoomOutHandler );

			fadeTop	= _sprite.getChildByName( "fadeTop" ) as MovieClip;
			fadeBtm	= _sprite.getChildByName( "fadeBtm" ) as MovieClip;

			fadeTop.y		= -fadeTop.height;
			fadeTop.visible	= false;

			fadeBtm.y		= ModelLocator.getInstance().makeupModel.appHeight + fadeTop.height;
			fadeBtm.visible	= false;

			//-- blur.

			var appW : int;
			var appH : int;

			appW = ModelLocator.getInstance().makeupModel.appWidth;
			appH = ModelLocator.getInstance().makeupModel.appHeight;

			faceBmd	= new BitmapData( appW, appH, false, 0xFFFFFF );
			maskBmd	= new BitmapData( appW, appH, true,  0x00FFFFFF );
			faceBm	= new Bitmap( faceBmd );

			_sprite.addChild( faceBm );
		}

		private function focusOnEyes ():void
		{
			Tweener.addTween
			(
				this,
				{
					tweenValue	: 300,
					time		: 0.4,
					delay		: 0.0,
					transition	: Quadratic.easeOut,
					onStart		: null,
					onUpdate	: tweenUpdate,
					onComplete	: null
				}
			);
		}

		private function focusOnLips ():void
		{
			Tweener.addTween
			(
				this,
				{
					tweenValue	: 330,
					time		: 0.4,
					delay		: 0.0,
					transition	: Quadratic.easeOut,
					onStart		: null,
					onUpdate	: tweenUpdate,
					onComplete	: null
				}
			);
		}

		private function focusOnFace ():void
		{
			Tweener.addTween
			(
				this,
				{
					tweenValue	: 140,
					time		: 0.4,
					delay		: 0.0,
					transition	: Quadratic.easeOut,
					onStart		: null,
					onUpdate	: tweenUpdate,
					onComplete	: null
				}
			);
		}

		private function focusOut ():void
		{
			Tweener.addTween
			(
				this,
				{
					tweenValue	: 0,
					time		: 0.4,
					delay		: 0.0,
					transition	: Quadratic.easeOut,
					onStart		: null,
					onUpdate	: tweenUpdate,
					onComplete	: focusOutComplete
				}
			);
		}

		private function focusOutComplete ():void
		{
			fadeTop.visible	= false;
			fadeBtm.visible	= false;
		}

		private function tweenUpdate ():void
		{
			var appW : int;
			var appH : int;

			appW = ModelLocator.getInstance().makeupModel.appWidth;
			appH = ModelLocator.getInstance().makeupModel.appHeight;

			_sprite.graphics.clear();

			var h : Number;

			//-- top.

			h = tweenValue - fadeBtm.height;

			fadeTop.alpha	= fillAlpha;
			fadeTop.y		= h;

			_sprite.graphics.beginFill( 0x000000, fillAlpha );
			_sprite.graphics.drawRect( 0, 0, appW, fadeTop.y );
			_sprite.graphics.endFill();

			//-- btm

			h = tweenValue - fadeBtm.height;

			fadeBtm.alpha	= fillAlpha;
			fadeBtm.y		= appH - h;

			_sprite.graphics.beginFill( 0x000000, fillAlpha );
			_sprite.graphics.drawRect( 0, fadeBtm.y, appW, h );
			_sprite.graphics.endFill();

			//-- blur.

			faceBmd.draw( face );
		}

		//////////////////////////////////////////////////////////
		//	HANDLERS.
		//////////////////////////////////////////////////////////

		private function modeZoomInHandler ( e : ModeEvent ):void
		{
			fadeTop.visible	= true;
			fadeBtm.visible	= true;

			if( e.mode == MakeupModel.EYES_MODE )
			{
				focusOnEyes();
			}

			if( e.mode == MakeupModel.LIPS_MODE )
			{
				focusOnLips();
			}

			if( e.mode == MakeupModel.FACE_MODE )
			{
				focusOnFace();
			}
		}

		private function modeZoomOutHandler ( e : ModeEvent ):void
		{
			focusOut();
		}
	}
}