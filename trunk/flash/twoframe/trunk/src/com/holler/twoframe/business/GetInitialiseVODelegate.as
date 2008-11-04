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
	import com.adobe.cairngorm.business.RemotingService;
	import com.adobe.cairngorm.business.ServiceLocator;
	import com.holler.twoframe.vo.InitialiseVO;


	public class GetInitialiseVODelegate extends Delegate implements IResponder
	{
		private var service:RemotingService;

		public function GetInitialiseVODelegate(_responder:IResponder)
		{
			super(_responder);
		}

		public function initialise():void
		{
			if(modelLocator.configModel.amfphpEnabled)
			{
				ServiceLocator.getInstance().register(serviceLocator_registerCompleteHandler);
			}
			else
			{
				dummyData();
			}
		}
		
		public function serviceLocator_registerCompleteHandler():void
		{
			service = ServiceLocator.getInstance().getRemoteService("amfphpService");
			service.addResponder(this);
			service.getInitialiseVO();
		}
		
		protected function dummyData():void
		{			
			var initialiseVO:InitialiseVO = new InitialiseVO();
			initialiseVO.imagesDirectory = "";
			initialiseVO.videosDirectory = "";
			initialiseVO.xmlDirectory = "";
			initialiseVO.httpDirectory = "";
			
			onResult(initialiseVO);
		}

		override public function onResult(data:* = null):void
		{
			super.onResult(data);
		}

		override public function onFault(event:* = null):void
		{
			super.onFault(event);
		}
	}
}