package com.julapy.ph.hair.events
{
	import flash.events.Event;

	public class MenuEvent extends Event
	{
		public static const MENU_SELECT		: String = "menuSelect";

		public var menuSelection	: int;

		public function MenuEvent( menuSelection : int )
		{
			super( MENU_SELECT );

			this.menuSelection = menuSelection;
		}

	}
}