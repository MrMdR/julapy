package com.julapy.model
{
	import com.julapy.vo.TrackerVO;

	public class OFDataModel
	{
		private var _ofStringData	: String;
		private var _ofTrackerVOs	: Array = new Array();

		public function OFDataModel()
		{
			//
		}

		///////////////////////////////////////////////////
		//
		///////////////////////////////////////////////////

		public function set ofStringData ( value : String ):void
		{
			_ofStringData = value;

			parseStringData();
		}

		public function get ofStringData ():String
		{
			return _ofStringData;
		}

		public function get trackerVOs ():Array
		{
			return _ofTrackerVOs;
		}

		private function parseStringData ():void
		{
			var str : String;
			str = ofStringData;

			str = str.split( "[/TCP]" )[ 0 ];

			var dataStr : Array;
			dataStr = str.split( "\n" );

			for( var i:int=0; i<dataStr.length; i++ )
			{
				var trackerVO : TrackerVO;

				if( _ofTrackerVOs.length > i )
				{
					trackerVO = _ofTrackerVOs[ i ] as TrackerVO;
				}
				else
				{
					trackerVO = new TrackerVO();
					_ofTrackerVOs.push( trackerVO );
				}

				var trackerStr : String;
				trackerStr = dataStr[ i ];
				trackerStr = trackerStr.split( "[" )[ 1 ];
				if( !trackerStr )
					continue;
				trackerStr = trackerStr.split( "]" )[ 0 ];
				if( !trackerStr )
					continue;

				var trackerValues : Array;
				trackerValues = trackerStr.split( "," );

				var j : int;
				j = 0;

				trackerVO.active		= trackerValues[ (int)( Math.min( j++, trackerValues.length - 1 ) ) ] == 1 ? true : false;
				trackerVO.rect.x		= trackerValues[ (int)( Math.min( j++, trackerValues.length - 1 ) ) ];
				trackerVO.rect.y		= trackerValues[ (int)( Math.min( j++, trackerValues.length - 1 ) ) ];
				trackerVO.rect.width	= trackerValues[ (int)( Math.min( j++, trackerValues.length - 1 ) ) ];
				trackerVO.rect.height	= trackerValues[ (int)( Math.min( j++, trackerValues.length - 1 ) ) ];
				trackerVO.angle			= trackerValues[ (int)( Math.min( j++, trackerValues.length - 1 ) ) ];
			}
		}
	}
}