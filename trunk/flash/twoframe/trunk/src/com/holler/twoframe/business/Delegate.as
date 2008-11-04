////////////////////////////////////////////////////////////////////////////////
//
//  HOLLER SYDNEY
//  Copyright 2008 Holler Sydney
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

package com.holler.twoframe.business
{
	import com.adobe.cairngorm.business.IResponder;
	import com.holler.twoframe.model.ModelLocator;

	public class Delegate implements IResponder
	{
		protected var responder : IResponder;
		protected var modelLocator:ModelLocator = ModelLocator.getInstance();		

		public function Delegate(_responder:IResponder)
		{
			responder = _responder;
		}

		public function onResult(data:*=null):void
		{
			responder.onResult(data);
		}

		public function onFault(event:* = null ):void
		{
			responder.onFault(event);
		}
	}
}