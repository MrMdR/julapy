////////////////////////////////////////////////////////////////////////////////
//
//  HOLLER SYDNEY
//  Copyright 2008 Holler Sydney
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

package {
	import com.holler.twoframe.business.Services;
	import com.holler.twoframe.control.TwoFrameController;
	import com.holler.twoframe.model.ModelLocator;
	import com.holler.twoframe.view.ApplicationView;
	import com.holler.twoframe.events.GetInitialiseVOEvent;
	import com.holler.twoframe.vo.ConfigVO;
	
	import flash.display.MovieClip;
	import flash.display.Sprite;

	/**
	 *  Frame metadata. Defines a class that loads before the Main application
	 *  this class is used to display a preloader
	 */
	[Frame(factoryClass="PreloaderApplication")]
	
	/**
	 *  SWF metadata. Defines swf properties.
	 *  These properties will be overidden by swfobject embed properties if present
	 */
	[SWF(backgroundColor='0x000000', frameRate='60', width='970', height='570' ) ]
	public class Main extends MovieClip
	{
		private static var modelLocator:ModelLocator = ModelLocator.getInstance();
		private static var controller:TwoFrameController = new TwoFrameController();
		private static var services:Services;
		private static var applicationView:ApplicationView;
		private static var preloaderApplication:PreloaderApplication = PreloaderApplication.getInstance();

		public function Main(configVO:ConfigVO) 
		{
			modelLocator.configModel.configVO = configVO;
			services = new Services();
			
			var applicationAsset:Sprite = new Sprite(); 
		
			preloaderApplication.mainStage.addChild(applicationAsset);
			applicationView = new ApplicationView(applicationAsset);
			
			/**
			 *  Now that main application is instantiated load the Content.swf
			 *  and add a handler for when the load is complete
			 */
			preloaderApplication.contentLoadCompleteHandler = contentLoadCompleteHandler;
			preloaderApplication.loadContent();
		}
		
		private function contentLoadCompleteHandler():void
		{
			var initialiseEvent:GetInitialiseVOEvent = new GetInitialiseVOEvent();
			initialiseEvent.dispatch();
		}
	}
}
