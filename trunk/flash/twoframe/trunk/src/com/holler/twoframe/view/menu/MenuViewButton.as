package com.holler.twoframe.view.menu
{
	import com.holler.controls.BtnView;
	import com.holler.twoframe.vo.ImageVO;
	
	import flash.display.Sprite;

	public class MenuViewButton extends BtnView
	{
		public static const WIDTH	:int	= 20;
		public static const HEIGHT	:int	= 20;
		
		private var _imageVO:ImageVO;
		
		public function MenuViewButton( sprite:Sprite=null, imageVO:ImageVO=null )
		{
			super(sprite);
			
			_imageVO = imageVO;
		}
		
		public function get imageVO ():ImageVO
		{
			return _imageVO;
		}
		
	}
}