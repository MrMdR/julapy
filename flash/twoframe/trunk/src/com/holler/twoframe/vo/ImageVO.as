package com.holler.twoframe.vo
{
	public class ImageVO
	{
		public var id			:int = -1;
		public var name			:String	= "";
		public var image01Path	:String	= "";
		public var image02Path	:String	= "";
		
		public function ImageVO( id:int=-1, name:String="", image01Path:String="", image02Path:String="" )
		{
			this.id				= id;
			this.name			= name;
			this.image01Path	= image01Path;
			this.image02Path	= image02Path;
		}
		
		public function copy ():ImageVO
		{
			var imageVO:ImageVO;
			
			imageVO				= new ImageVO();
			imageVO.id			= id;
			imageVO.name		= name;
			imageVO.image01Path	= image01Path;
			imageVO.image02Path	= image02Path;
			
			return imageVO;
		}
		
//		public function toString():String
//		{
//			trace("*******************************************");
//			trace("*   ImageVO");
//			trace("*      name: " + name );
//			trace("*      image01Path: " + image01Path );
//			trace("*      image02Path: " + image02Path );
//			trace("*******************************************");
//		}

	}
}