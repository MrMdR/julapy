////////////////////////////////////////////////////////////////////////////////
//
//  HOLLER SYDNEY
//  Copyright 2008 Holler Sydney
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

package com.holler.twoframe.control
{
	import com.adobe.cairngorm.control.FrontController;
	import com.holler.twoframe.commands.GetImagesCommand;
	import com.holler.twoframe.commands.GetInitialiseVOCommand;
	import com.holler.twoframe.events.GetImagesEvent;
	import com.holler.twoframe.events.GetInitialiseVOEvent;
	//HollerProjectGenerator marker: add import
	
	public class TwoFrameController extends FrontController
	{
		public function TwoFrameController()
		{
			super();
			
			initialiseCommands( );
		}
		
		private function initialiseCommands( ) : void
		{
			//HollerProjectGenerator marker: add command
			addCommand( GetInitialiseVOEvent.GET_INITIALISE_VO,	GetInitialiseVOCommand );
			addCommand( GetImagesEvent.GET_IMAGES,				GetImagesCommand );
		}
	}
}
