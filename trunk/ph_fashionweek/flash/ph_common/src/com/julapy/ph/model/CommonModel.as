package com.julapy.ph.model
{
	import flash.geom.Rectangle;

	public class CommonModel
	{
		private var _appRect		: Rectangle	= new Rectangle( 0, 0, 576, 1024 );

		public function CommonModel()
		{
			//
		}

		/////////////////////////////////////
		//	APP SIZE.
		/////////////////////////////////////

		public function get appWidth ():int
		{
			return _appRect.width;
		}

		public function get appHeight ():int
		{
			return _appRect.height;
		}
	}
}