package com.holler.twoframe.vo
{
	public class ImageVO
	{
		public var name			:String	= "";
		public var image01Path	:String	= "";
		public var image02Path	:String	= "";
		
		public function ImageVO( name:String="", image01Path:String="", image02Path:String="" )
		{
			this.name			= name;
			this.image01Path	= image01Path;
			this.image02Path	= image02Path;
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