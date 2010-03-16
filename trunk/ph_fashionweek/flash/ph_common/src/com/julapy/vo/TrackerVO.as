package com.julapy.vo
{
	import flash.geom.Rectangle;

	public class TrackerVO
	{
		public var active	: Boolean	= false;
		public var rect		: Rectangle = new Rectangle();
		public var points	: Array		= new Array();

		public function TrackerVO()
		{
		}

	}
}