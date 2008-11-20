////////////////////////////////////////////////////////////////////////////////
//
//  HOLLER SYDNEY
//  Copyright 2008 Holler Sydney
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

package com.holler.twoframe.commands
{
	import com.adobe.cairngorm.control.CairngormEvent;
	import com.holler.twoframe.business.GetImagesDelegate;
	
	public class GetImagesCommand extends Command
	{
		public function GetImagesCommand()
		{
			super();
		}
		
		override public function execute( event : CairngormEvent ):void
		{
			var imagesDelegate:GetImagesDelegate = new GetImagesDelegate( this );
			imagesDelegate.initialise();
		}

		override public function onResult( result : * = null ) : void
		{
			modelLocator.configModel.content = result as Array;
		}

		override public function onFault( event : * = null ) : void
		{
			super.onFault(event);
		}
	}
}