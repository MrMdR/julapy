package com.julapy.ph.makeup.vo
{
	public class GirlMakeupVO
	{
		public var name			: String = "";

		public var faceLinkage 	: Array = new Array();
		public var eyesLinkage	: Array = new Array();
		public var lipsLinkage	: Array = new Array();
		public var featureMasks	: Array = new Array();

		public var introVideos	: Array = new Array();
		public var outroVideos	: Array = new Array();
		public var videoIndex	: int	= 0;
		public var videoNo		: int	= 2;

		public function GirlMakeupVO()
		{
		}

		public function stepVideo ():void
		{
			if( ++videoIndex >= videoNo )
				videoIndex = 0;
		}

		public function get introVideo ():String
		{
			return introVideos[ videoIndex ];
		}

		public function get outroVideo ():String
		{
			return outroVideos[ videoIndex ];
		}
	}
}