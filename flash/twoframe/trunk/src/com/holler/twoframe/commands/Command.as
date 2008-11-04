////////////////////////////////////////////////////////////////////////////////
//
//  HOLLER SYDNEY
//  Copyright 2008 Holler Sydney
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

package com.holler.twoframe.commands
{
	import com.adobe.cairngorm.business.IResponder;
	import com.adobe.cairngorm.commands.ICommand;
	import com.adobe.cairngorm.control.CairngormEvent;
	import com.holler.twoframe.model.ModelLocator;

	public class Command implements ICommand, IResponder
	{
		protected var modelLocator:ModelLocator = ModelLocator.getInstance();
		
		public function Command()
		{
			
		}

		public function execute(event:CairngormEvent):void
		{
		}
		
		public function onResult(result:*=null):void
		{
		}
		
		public function onFault(event:*=null):void
		{
			
		}
		
	}
}