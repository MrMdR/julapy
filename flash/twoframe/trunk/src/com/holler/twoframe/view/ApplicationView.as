////////////////////////////////////////////////////////////////////////////////
//
//  HOLLER SYDNEY
//  Copyright 2008 Holler Sydney
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

package com.holler.twoframe.view
{
	import com.holler.assets.AssetManager;
	import com.holler.containers.ScreenView;
	import com.holler.twoframe.constants.AssetConstants;
	import com.holler.twoframe.events.ImagesChangeEvent;
	import com.holler.twoframe.events.InitialiseVOChangeEvent;
	import com.holler.twoframe.model.ModelLocator;
	
	import flash.display.Sprite;
	import flash.text.TextField;

	public class ApplicationView extends ScreenView
	{
		private var loaderView : PreloaderView;
		
		public function ApplicationView(sprite:Sprite=null)
		{
			super(sprite);
			
			
			loaderView = PreloaderApplication.getInstance().loaderView;

			ModelLocator.getInstance().configModel.addEventListener( ImagesChangeEvent.IMAGES_CHANGE, configModel_imagesChangeHander );
		}
		
		/**
		 *  This function is called after the initialiseVO has been set in the ConfigModel
		 *  This is the perfect place to start coding your application
		 */
		private function configModel_imagesChangeHander( event:ImagesChangeEvent ):void
		{
			ModelLocator.getInstance().configModel.removeEventListener( ImagesChangeEvent.IMAGES_CHANGE, configModel_imagesChangeHander );
			
			var viewAsset:Sprite = AssetManager.getClassInstance(AssetConstants.APPLICATION_VIEW) as Sprite;
			(viewAsset.getChildByName("labelTextField") as TextField).height = 500;
			(viewAsset.getChildByName("labelTextField") as TextField).htmlText = "ApplicationView Instantiated\rContent.swf loaded\rInitialiseVO loaded\r\rThe ideal place to start coding is the configModel_initialiseVOChangeHander() function in ApplicationView.\r\rTo add additional Event\Command\Delegate sequences run the ant build.xml located in the .holler folder in the root of this project.";
			_sprite.addChild(viewAsset);
			
		}
	}
}