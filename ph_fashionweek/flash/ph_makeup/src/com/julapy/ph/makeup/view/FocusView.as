package com.julapy.ph.makeup.view
{
	import caurina.transitions.Tweener;

	import com.holler.core.View;
	import com.julapy.ph.makeup.events.ModeEvent;
	import com.julapy.ph.makeup.model.MakeupModel;
	import com.julapy.ph.makeup.model.ModelLocator;

	import fl.motion.easing.Quadratic;

	import flash.display.Sprite;

	public class FocusView extends View
	{
		public  var tweenValue	: Number = 0;
		private var fillAlpha	: Number = 0.2;

		public function FocusView(sprite:Sprite=null)
		{
			super(sprite);

			ModelLocator.getInstance().makeupModel.addEventListener( ModeEvent.MODE, modeEvent );
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
					onComplete	: null
				}
			);
		}

		private function tweenUpdate ():void
		{
			var appW : int;
			var appH : int;

			appW = ModelLocator.getInstance().makeupModel.appWidth;
			appH = ModelLocator.getInstance().makeupModel.appHeight;

			_sprite.graphics.clear();

			_sprite.graphics.beginFill( 0x000000, fillAlpha );
			_sprite.graphics.drawRect( 0, 0, appW, tweenValue );
			_sprite.graphics.endFill();

			_sprite.graphics.beginFill( 0x000000, fillAlpha );
			_sprite.graphics.drawRect( 0, appH - tweenValue, appW, tweenValue );
			_sprite.graphics.endFill();
		}

		//////////////////////////////////////////////////////////
		//	HANDLERS.
		//////////////////////////////////////////////////////////

		private function modeEvent ( e : ModeEvent ):void
		{
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
//				focusOnFace();
				focusOut();
			}
		}
	}
}