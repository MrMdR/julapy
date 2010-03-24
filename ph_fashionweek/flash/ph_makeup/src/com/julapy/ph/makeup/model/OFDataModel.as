package com.julapy.ph.makeup.model
{
	import com.julapy.ph.makeup.events.ConnectedEvent;
	import com.julapy.ph.makeup.events.DisconnectEvent;
	import com.julapy.ph.makeup.vo.TrackerVO;

	import flash.events.EventDispatcher;

	public class OFDataModel extends EventDispatcher
	{
		private var _ofStringData		: String;
		private var _ofTrackerVOs		: Array		= new Array();
		private var _ofPrimaryTrackerVO	: TrackerVO;
		private var _ofPrimaryIndex		: int = -1;
		private var _connected			: Boolean	= false;

		public function OFDataModel()
		{
			//
		}

		///////////////////////////////////////////////////
		//	TRACKER VOs.
		///////////////////////////////////////////////////

		public function set ofStringData ( value : String ):void
		{
			_ofStringData = value;

			parseStringData();
			findPrimaryTrackerVO();
		}

		public function get ofStringData ():String
		{
			return _ofStringData;
		}

		public function get trackerVOs ():Array
		{
			return _ofTrackerVOs;
		}

		public function get primaryTrackerVO ():TrackerVO
		{
			return _ofPrimaryTrackerVO;
		}

		//--

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

		private function findPrimaryTrackerVO ():void
		{
			var trackerVO			: TrackerVO;
			var biggestTrackerVO	: TrackerVO;
			var highestTrackerVO	: TrackerVO;

			var biggestIndex		: int = -1;
			var highestIndex		: int = -1;

			var i : int;

			//-- biggest.

			var a : Number;
			a = 0;

			for( i=0; i<_ofTrackerVOs.length; i++ )
			{
				trackerVO = _ofTrackerVOs[ i ] as TrackerVO;

				if( !trackerVO.active )
					continue;

				var ta : Number;
				ta = trackerVO.rect.width * trackerVO.rect.height;

				if( ta > a )
				{
					a = ta;
					biggestIndex		= i;
					biggestTrackerVO	= trackerVO;
				}
			}

			//-- highest

			var y : Number;
			y = 1;

			for( i=0; i<_ofTrackerVOs.length; i++ )
			{
				trackerVO = _ofTrackerVOs[ i ] as TrackerVO;

				if( !trackerVO.active )
					continue;

				var ty : Number;
				ty = trackerVO.rect.y;

				if( ty < y )
				{
					y = ty;
					highestIndex		= i;
					highestTrackerVO	= trackerVO;
				}
			}

			_ofPrimaryTrackerVO = highestTrackerVO;
			_ofPrimaryIndex		= highestIndex;

			ModelLocator.getInstance().makeupModel.mode = _ofPrimaryIndex;
		}

		///////////////////////////////////////////////////
		//	CONNECTED
		///////////////////////////////////////////////////

		public function set connected ( value :Boolean ):void
		{
			if( _connected != value )
			{
				_connected = value;

				dispatchEvent( new ConnectedEvent( _connected ) );
			}
		}

		public function get connected ():Boolean
		{
			return _connected;
		}

		//--

		public function disconnect ():void
		{
			if( _connected )
			{
				dispatchEvent( new DisconnectEvent() );
			}
		}
	}
}