package com.julapy.ph.hair.view
{
	import caurina.transitions.Tweener;

	import com.holler.core.View;

	import fl.motion.easing.Quadratic;

	import flash.display.Sprite;

	public class MenuDropAreaArrowView extends View
	{
		public function MenuDropAreaArrowView(sprite:Sprite=null)
		{
			super(sprite);

			visible	= false;
			alpha	= 0;
			doValidate();
		}

		public function reset ():void
		{
			visible	= false;
			alpha	= 0;
			doValidate();
		}

		public function show ( b : Boolean ):void
		{
			if( b )
			{
				visible	= true;
				alpha	= 0;
				doValidate();

				Tweener.addTween
				(
					this,
					{
						alpha		: 1.0,
						time		: 0.2,
						delay		: 0.0,
						transition	: Quadratic.easeIn,
						onStart		: null,
						onUpdate	: tweenUpdateHandler,
						onComplete	: null
					}
				);
			}
			else
			{
				Tweener.addTween
				(
					this,
					{
						alpha		: 0.0,
						time		: 0.2,
						delay		: 0.0,
						transition	: Quadratic.easeIn,
						onStart		: null,
						onUpdate	: tweenUpdateHandler,
						onComplete	: tweenCompleteHandler
					}
				);
			}
		}

		private function tweenUpdateHandler ():void
		{
			doValidate();
		}

		private function tweenCompleteHandler ():void
		{
			visible	= false;
			alpha	= 0;
			doValidate();
		}
	}
}