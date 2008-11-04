////////////////////////////////////////////////////////////////////////////////
//
//  HOLLER SYDNEY
//  Copyright 2008 Holler Sydney
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

package com.holler.twoframe.business
{
	import com.adobe.cairngorm.business.IServicesList;
	import com.adobe.cairngorm.business.RemotingService;
	import com.adobe.cairngorm.business.ServiceLocator;
	import com.holler.twoframe.model.ModelLocator;
	//HollerProjectGenerator marker: add import

	public class Services implements IServicesList
	{
		private var serviceLocator 	: ServiceLocator;
		private var modelLocator:ModelLocator = ModelLocator.getInstance();

		public function Services()
		{
			ServiceLocator.services = this;
			serviceLocator = ServiceLocator.getInstance();
		}
		
		public function get amfphpService():RemotingService
		{
			return new RemotingService(modelLocator.configModel.configVO.amfphpURL, modelLocator.configModel.configVO.amfphpService);
		}
		
		//HollerProjectGenerator marker: add service
	}
}