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
				trackerStr = trackerStr.split( "]" )[ 0 ];

				var trackerValues : Array;
				trackerValues = trackerStr.split( "," );

				trackerVO.active		= trackerValues[ 0 ];
				trackerVO.rect.x		= trackerValues[ 1 ];
				trackerVO.rect.y		= trackerValues[ 2 ];
				trackerVO.rect.width	= trackerValues[ 3 ];
				trackerVO.rect.height	= trackerValues[ 4 ];
			}
		}
	}
}