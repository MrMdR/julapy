package com.julapy.ph.makeup.view
{
	import caurina.transitions.Tweener;

	import com.holler.core.View;
	import com.julapy.ph.makeup.events.ModeEvent;
	import com.julapy.ph.makeup.model.MakeupModel;
	import com.julapy.ph.makeup.model.ModelLocator;

	import fl.motion.easing.Quadratic;

	import flash.display.MovieClip;
	import flash.display.Sprite;

	public class FocusView extends View
	{
		public  var tweenValue	: Number = 0;
		private var fillAlpha	: Number = 0.4;

		private var fadeTop		: MovieClip;
		private var fadeBtm		: MovieClip;

		public function FocusView(sprite:Sprite=null)
		{
			super(sprite);

			ModelLocator.getInstance().makeupModel.addEventListener( ModeEvent.MODE_ZOOM_IN,	modeZoomInHandler );
			ModelLocator.getInstance().makeupModel.addEventListener( ModeEvent.MODE_ZOOM_OUT,	modeZoomOutHandler );

			fadeTop	= _sprite.getChildByName( "fadeTop" ) as MovieClip;
			fadeBtm	= _sprite.getChildByName( "fadeBtm" ) as MovieClip;

			fadeTop.y		= -fadeTop.height;
			fadeTop.visible	= false;

			fadeBtm.y		= ModelLocator.getInstance().commondModel.appHeight + fadeTop.height;
			fadeBtm.visible	= false;
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

			appW = ModelLocator.getInstance().commondModel.appWidth;
			appH = ModelLocator.getInstance().commondModel.appHeight;

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

			doValidate();
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