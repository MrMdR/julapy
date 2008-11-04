////////////////////////////////////////////////////////////////////////////////
//
//  HOLLER SYDNEY
//  Copyright 2008 Holler Sydney
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

package com.holler.twoframe.view
{
	import com.holler.constants.HollerStyleGuideConstants;
	import com.holler.containers.ScreenView;
	import flash.display.Shape;
	import flash.display.Sprite;
	import flash.text.TextField;
	import mx.core.SpriteAsset;

	public class PreloaderView extends ScreenView
	{
		private var labelTextField:TextField;
		private var _percentage:Number = 0;
		private var _label:String = "";
		
		private var bar:Shape;
		private var barBackground:Sprite;
		
		private static const BAR_HEIGHT:Number = 2;
		private static const BAR_BACKGROUND_HEIGHT:Number = 4;
		private static const LABEL_Y_OFFSET:Number = -21;
		
		public function PreloaderView(sprite:Sprite=null)
		{
			super(sprite);
			
			barBackground = new Sprite();
			_sprite.addChild(barBackground);
			
			bar = new Shape();
			bar.y = BAR_BACKGROUND_HEIGHT/2 - BAR_HEIGHT/2;
			barBackground.addChild(bar);
			
			var loader:SpriteAsset;
			loader = CommonAssets.loader;
			labelTextField = loader.getChildByName("labelTextField") as TextField;
			_sprite.addChild(loader);
			label = "";
			
			resize(stageWidth, stageHeight);
			doValidate();
		}
		
		/**
		 * @value
		 * A value between 0 and 1
		 */
		public function set percentage(value:Number):void
		{
			_percentage = value;
			drawPercentage();
		}
		
		public function get percentage():Number
		{
			return _percentage;
		}
		
		public function set label(value:String):void
		{		
			_label = value;
			labelTextField.text = value;
		}
		
		public function get label():String
		{
			return _label;
		}
		
		protected function drawPercentage():void
		{
			bar.graphics.clear();
			bar.graphics.beginFill(HollerStyleGuideConstants.HOLLER_BLACK);
			bar.graphics.drawRect(0, 0, percentage*stageWidth, BAR_HEIGHT);
			bar.graphics.endFill();
		}
		
		override protected function resize(stageWidth:Number, stageHeight:Number):void
		{
			barBackground.y = Math.round(stageHeight/2-BAR_BACKGROUND_HEIGHT/2);
			barBackground.graphics.clear();
			barBackground.graphics.beginFill(HollerStyleGuideConstants.HOLLER_RED);
			barBackground.graphics.drawRect(0, 0, stageWidth, BAR_BACKGROUND_HEIGHT);
			barBackground.graphics.endFill();
			labelTextField.y = barBackground.y + LABEL_Y_OFFSET;
			labelTextField.x = stageWidth/2 - labelTextField.width/2;
			
			drawPercentage();
		}
	}
}