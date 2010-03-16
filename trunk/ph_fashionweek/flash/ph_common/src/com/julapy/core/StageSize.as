package com.julapy.core
{
	import flash.display.Stage;
	import flash.events.Event;
	import flash.events.EventDispatcher;
	import flash.geom.Rectangle;

	[Event( name="resize", type="com.julapy.core.StageSizeEvent" )]

	public class StageSize extends EventDispatcher
	{
		private static var instance : StageSize;

		private var stageRef	: Stage;
		private var stageSize	: Rectangle = new Rectangle();

		public function StageSize()
		{
			if( instance != null )
			{
				throw new Error( "StageSize is a singleton and must be accessed through getInstance()" );
			}

			instance = this;
		}

		public static function getInstance():StageSize
		{
			if( instance == null )
				instance = new StageSize();

			return instance;
		}

		public function set stage ( value : Stage ):void
		{
			if( !stageRef )
				stageRef = value;

			stageSize.width		= stageRef.stageWidth;
			stageSize.height	= stageRef.stageHeight;

			stageRef.addEventListener( Event.RESIZE, stageResizeHandler );
		}

		public function get stageWidth ():int
		{
			return stageSize.width;
		}

		public function get stageHeight ():int
		{
			return stageSize.height;
		}

		private function stageResizeHandler ( e : Event ):void
		{
			stageSize.width		= stageRef.stageWidth;
			stageSize.height	= stageRef.stageHeight;

			var event : StageSizeEvent;
			event = new StageSizeEvent();
			event.stageWidth	= stageSize.width;
			event.stageHeight	= stageSize.height;
			dispatchEvent( event );
		}
	}
}