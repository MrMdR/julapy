package com.julapy.ph.hair.view
{
	import caurina.transitions.Tweener;

	import com.holler.core.View;

	import fl.motion.easing.Quadratic;

	import flash.display.Sprite;

	public class MenuToolCoverView extends View
	{
		private var bShow : Boolean = false;

		public function MenuToolCoverView(sprite:Sprite=null)
		{
			super(sprite);

			visible = false;
			alpha	= 0.0;
			doValidate();
		}

		public function show ( b : Boolean ):void
		{
			if( bShow == b )
				return;

			bShow = b;

			if( bShow )
			{
				visible	= true;
				alpha	= 0.0;
				doValidate();

				Tweener.addTween
				(
					this,
					{
						alpha		: 0.54,
						time		: 0.3,
						delay		: 0.0,
						transition	: Quadratic.easeOut,
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
						time		: 0.3,
						delay		: 0.0,
						transition	: Quadratic.easeOut,
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
			visible = false;
			doValidate();
		}
	}
}