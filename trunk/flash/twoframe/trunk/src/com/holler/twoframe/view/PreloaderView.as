////////////////////////////////////////////////////////////////////////////////
//
//  HOLLER SYDNEY
//  Copyright 2008 Holler Sydney
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

package com.holler.twoframe.view
{
	import com.holler.containers.ScreenView;

	import flash.display.Sprite;

	public class PreloaderView extends ScreenView
	{
		private static const BAR_HEIGHT:Number = 1;

		private var bar:Sprite;
		private var _percentage:Number = 0;
		
		public function PreloaderView(sprite:Sprite=null)
		{
			super(sprite);
			
			sprite.addChild( bar = new Sprite() );
			
			resize( stageWidth, stageHeight );
			doValidate();
		}
		
		public function set percentage( value:Number ):void
		{
			_percentage = value;
			drawPercentage();
		}
		
		public function get percentage():Number
		{
			return _percentage;
		}
		
		protected function drawPercentage():void
		{
			bar.graphics.clear();
			bar.graphics.beginFill( 0x000000 );
			bar.graphics.drawRect( 0, 0, percentage * stageWidth, BAR_HEIGHT );
			bar.graphics.endFill();
		}
		
		override protected function resize(stageWidth:Number, stageHeight:Number):void
		{
			bar.y	= (int)( ( stageHeight - BAR_HEIGHT ) * 0.5 );
			
			drawPercentage();
		}
	}
}