package com.julapy.ph.makeup.view
{
	import caurina.transitions.Tweener;

	import com.holler.controls.BtnView;

	import fl.motion.easing.Quadratic;

	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.MouseEvent;

	public class BtnView01 extends BtnView
	{
		private var tool		: MovieClip;

		public function BtnView01(sprite:Sprite=null)
		{
			super(sprite);

			tool	= _sprite.getChildByName( "tool" ) as MovieClip;
			tool.ix	= tool.x;
			tool.iy	= tool.y;

			doValidate();
		}

		protected override function mouseOver(event:MouseEvent):void
		{
//			over();
		}

		protected override function mouseOut(event:MouseEvent):void
		{
//			out();
		}

		public function reset ():void
		{
			Tweener.removeTweens( tool );

			tool.x = tool.ix;
			tool.y = tool.iy;
		}

		public function over ():void
		{
			Tweener.addTween
			(
				tool,
				{
					y			: tool.iy - 50,
					time		: 0.3,
					delay		: 0.0,
					transition	: Quadratic.easeOut,
					onStart		: null,
					onUpdate	: null,
					onComplete	: null
				}
			);
		}

		public function out ():void
		{
			Tweener.addTween
			(
				tool,
				{
					y			: tool.iy,
					time		: 0.3,
					delay		: 0.0,
					transition	: Quadratic.easeOut,
					onStart		: null,
					onUpdate	: null,
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