package com.julapy.ph.model
{
	import com.julapy.ph.events.ConnectedEvent;
	import com.julapy.ph.events.DisconnectEvent;
	import com.julapy.ph.vo.TrackerVO;

	import flash.events.EventDispatcher;

	public class OFDataModel extends EventDispatcher
	{
		protected var _ofStringData			: String;
		protected var _ofTrackerVOs			: Array		= new Array();
		protected var _ofPrimaryTrackerVO	: TrackerVO;
		protected var _ofPrimaryIndex		: int = -1;
		protected var _ofLastIndex			: int = -1;
		protected var _ofLastValidIndex		: int = 0;
		protected var _ofFocusIndex			: int = -1;
		protected var _connected			: Boolean	= false;

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
			if( _ofFocusIndex >= 0 )
			{
				return _ofTrackerVOs[ _ofFocusIndex ] as TrackerVO;
			}
			else
			{
				return _ofTrackerVOs[ _ofLastValidIndex ] as TrackerVO;
			}
		}

		//--

		protected function parseStringData ():void
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

		protected function findPrimaryTrackerVO ():void
		{
			//
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

		///////////////////////////////////////////////////
		//	FOCUS INDEX.
		///////////////////////////////////////////////////

		public function set focusIndex ( value : int ):void
		{
			_ofFocusIndex = value;
		}

		public function get focusIndex ():int
		{
			return _ofFocusIndex;
		}
	}
}