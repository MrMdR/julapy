package com.julapy.ph.ui
{
	import flash.display.DisplayObject;
	import flash.display.Sprite;
	import flash.events.MouseEvent;

	[Event( name="mouseDown", type="flash.events.MouseEvent" )]

	public class Btn01 extends Sprite
	{
		public function Btn01()
		{
			super();

			useHandCursor = true;

			drawBtn();
		}

		protected function drawBtn ():void
		{
			graphics.beginFill( 0xFF0000 );
			graphics.drawRect( 0, 0, 40, 40 );
			graphics.endFill();
		}
	}
}