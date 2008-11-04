////////////////////////////////////////////////////////////////////////////////
//
//  HOLLER SYDNEY
//  Copyright 2008 Holler Sydney
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

package com.holler.twoframe.model
{
	import com.asual.swfaddress.SWFAddress;
	import com.asual.swfaddress.SWFAddressEvent;
	
	import flash.events.EventDispatcher;

	public class ApplicationState extends EventDispatcher
	{
		public function ApplicationState()
		{
			SWFAddress.addEventListener(SWFAddressEvent.CHANGE, swfAddress_changeHandler);
		}
		
		private function swfAddress_changeHandler(event:SWFAddressEvent):void
		{
			trace("###SWFAddress CHANGE: "+event.value+" ###");
		}
	}
}