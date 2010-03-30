package com.julapy.ph.vo
{
	import flash.geom.Rectangle;

	public class TrackerVO
	{
		public var active	: Boolean	= false;
		public var rect		: Rectangle = new Rectangle();
		public var angle	: Number	= 0;
		public var points	: Array		= new Array();

		public function TrackerVO()
		{
		}

	}
}