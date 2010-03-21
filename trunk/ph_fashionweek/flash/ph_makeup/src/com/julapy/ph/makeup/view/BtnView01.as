package com.julapy.ph.makeup.view
{
	import caurina.transitions.Tweener;

	import com.holler.controls.BtnView;

	import fl.motion.easing.Quadratic;

	import flash.display.Sprite;
	import flash.events.MouseEvent;

	public class BtnView01 extends BtnView
	{
		private var alphaOver	: Number = 1.0;
		private var alphaOut	: Number = 0.5;

		public function BtnView01(sprite:Sprite=null)
		{
			super(sprite);

			alpha = alphaOut;
			doValidate();
		}

		protected override function mouseOver(event:MouseEvent):void
		{
			Tweener.addTween
			(
				this,
				{
					alpha		: alphaOver,
					time		: 0.3,
					delay		: 0.0,
					transition	: Quadratic.easeOut,
					onStart		: null,
					onUpdate	: tweenUpdateHandler,
					onComplete	: null
				}
			);
		}

		protected override function mouseOut(event:MouseEvent):void
		{
			Tweener.addTween
			(
				this,
				{
					alpha		: alphaOut,
					time		: 0.3,
					delay		: 0.0,
					transition	: Quadratic.easeOut,
					onStart		: null,
					onUpdate	: tweenUpdateHandler,
					onComplete	: null
				}
			);
		}

		private function tweenUpdateHandler ():void
		{
			doValidate();
		}
	}
}