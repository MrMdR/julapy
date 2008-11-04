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
	import com.holler.twoframe.business.GetInitialiseVODelegate;
	import com.holler.twoframe.events.GetInitialiseVOEvent;
	import com.holler.twoframe.vo.ConfigVO;
	import com.holler.twoframe.vo.InitialiseVO;
	
	public class GetInitialiseVOCommand extends Command
	{
		public function GetInitialiseVOCommand()
		{
			super();
		}
		
		override public function execute( event : CairngormEvent ):void
		{
			var initialiseEvent:GetInitialiseVOEvent = event as GetInitialiseVOEvent;
			
			var initialiseDelegate:GetInitialiseVODelegate = new GetInitialiseVODelegate(this);
			initialiseDelegate.initialise();
		}

		override public function onResult( result : * = null ) : void
		{
			modelLocator.configModel.initialiseVO = result as InitialiseVO;
			var configVO:ConfigVO = modelLocator.configModel.configVO;
			var initialiseVO:InitialiseVO = modelLocator.configModel.initialiseVO;
			
			trace("*******************************************");
			trace("*   Application Initialisation Complete");
			trace("*      amfphpEnabled: "+modelLocator.configModel.amfphpEnabled);
			trace("*      xmlEnabled: "+modelLocator.configModel.xmlEnabled);
			trace("*      httpEnabled: "+modelLocator.configModel.httpEnabled);
			trace("*   CONFIG XML PROPERTIES");
			trace("*      amfphpURL: "+configVO.amfphpURL);
			trace("*      amfphpService: "+configVO.amfphpService);
			trace("*      cacheDisabled: "+configVO.cacheDisabled);
			trace("*   INITIALISE VO PROPERTIES");
			trace("*      imagesDirectory: "+initialiseVO.imagesDirectory);
			trace("*      videosDirectory: "+initialiseVO.videosDirectory);
			trace("*      xmlDirectory: "+initialiseVO.xmlDirectory);
			trace("*******************************************");
		}

		override public function onFault( event : * = null ) : void
		{
			super.onFault(event);
		}
	}
}