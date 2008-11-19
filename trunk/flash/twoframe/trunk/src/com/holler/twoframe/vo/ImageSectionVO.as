package com.holler.twoframe.vo
{
	public class ImageSectionVO
	{
		public var name		:String = "";
		public var images	:Array = new Array();
		
		public function ImageSectionVO( name:String="", image:Array=null )
		{
			this.name	= name;
			this.images	= images;
		}
		
//		public override function toString():String
//		{
//			trace("*******************************************");
//			trace("*   ImageSectionVO");
//			trace("*      name: " + name );
//			trace("*      images: " + images );
//			trace("*******************************************");
//		}

	}
}