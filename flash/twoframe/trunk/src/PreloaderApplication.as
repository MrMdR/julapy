////////////////////////////////////////////////////////////////////////////////
//
//  HOLLER SYDNEY
//  Copyright 2008 Holler Sydney
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

package 
{
	import com.adobe.cairngorm.CairngormError;
	import com.adobe.cairngorm.CairngormMessageCodes;
	import com.holler.containers.ScreenView;
	import com.holler.assets.AssetEvent;
	import com.holler.assets.AssetManager;
	import com.holler.twoframe.view.PreloaderView;
	import com.holler.twoframe.vo.ConfigVO;
	
	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.display.Stage;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.events.ErrorEvent;
	import flash.events.Event;
	import flash.events.IOErrorEvent;
	import flash.net.URLLoader;
	import flash.net.URLRequest;
	import flash.utils.getDefinitionByName;

	/**
	 *  Preloader Application. As a preloader it needs to be kept as small as possible
	 *  Avoid making any reference to any of the cairngorm framework (model, commands,
	 *  cairngorm events) any interaction with the rest of the application should be
	 *  performed via callback functions
	 */
	public class PreloaderApplication extends MovieClip 
	{
		public static const CONFIG_URL:String = "config.xml";
		
		/**
		 *  APPLICATION_PERCENTAGE, CONTENT_PERCENTAGE. The amount of the
		 *  total preload percentage taken up by the application and the
		 *  content respectively. Modify these values to change the
		 *  percentage distribution
		 */
		public static const APPLICATION_PERCENTAGE:Number = .5;
		public static const CONTENT_PERCENTAGE:Number = .5;
		
		public var contentLoadCompleteHandler:Function;
		
		public var mainStage:Stage;
		public var loaderView:PreloaderView;
		
		private var assetManager:AssetManager = AssetManager.getInstance();
		private var configVO:ConfigVO;
		
		private var configLoaded:Boolean = false;
		private var applicationLoaded:Boolean = false;
		
		private static var instance:PreloaderApplication;
  
   		public function PreloaderApplication() 
		{   
			if(instance != null)
			{
				throw new CairngormError(CairngormMessageCodes.SINGLETON_EXCEPTION, "PreloaderApplication");
			}
        
			instance = this;
			
			initialise();
		}
    
		public static function getInstance():PreloaderApplication 
		{
		   if(instance == null) instance = new PreloaderApplication();
		       
		   return instance;
		}
		
		private function initialise():void
		{
			mainStage		= stage;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			stage.align		= StageAlign.TOP_LEFT;

			var loaderSprite : Sprite = new Sprite();
		
			addChild(loaderSprite);
			loaderView = new PreloaderView(loaderSprite);
			
			loadConfig();
			addEventListener(Event.ENTER_FRAME, onEnterFrame);
			onEnterFrame();
		}
		
		/**
		 *  For google indexing to work the config xml file must reside in
		 *  the same directory as the swf file. Google executes swfs independantly
		 *  of their embedding page so the config file must exist relative to the
		 *  swf and not the html page.
		 */
		private function loadConfig():void
		{
			var rootURL:String = stage.loaderInfo.loaderURL;
			rootURL	= rootURL.substr( 0, rootURL.lastIndexOf("/") ).concat( "/" );
			
			var request:URLRequest = new URLRequest(rootURL + CONFIG_URL+"?time="+new Date().time);
			var loader:URLLoader = new URLLoader(request);
			loader.addEventListener(ErrorEvent.ERROR, loadConfig_errorHandler);
			loader.addEventListener(Event.COMPLETE, loadConfig_completeHandler);
			loader.load(request);
		}
		
		private function loadConfig_completeHandler(event:Event):void
		{
			var xml:XML = new XML(URLLoader(event.currentTarget).data);
			
			configVO = new ConfigVO();
			configVO.amfphpService = xml.parameters.amfphpService;
			configVO.amfphpURL = xml.parameters.amfphpURL;
			configVO.cacheDisabled = (xml.parameters.cacheDisabled == "true");
			
			configLoaded = true;
			
			if(applicationLoaded) instantiateMain();
		}
		
		private function loadConfig_errorHandler(event:ErrorEvent):void
		{
			trace("***PreloaderApplication config.xml load ERROR***: "+ event.text);
		}
		
		public function loadContent(cacheDisabled:Boolean=true):void
		{
			assetManager.addEventListener(AssetEvent.ASSET_COMPLETE, assetManager_completehandler);
			assetManager.addEventListener(AssetEvent.ASSET_PROGRESS, assetManager_progressHandler);
			assetManager.addEventListener(IOErrorEvent.IO_ERROR, assetManager_errorHandler);
			
			assetManager.addAsset("Content", "application", "swfs/Content.swf" + (cacheDisabled ? "?time="+new Date().time : ""));
			assetManager.initialise();
		}
		
		private function assetManager_errorHandler(e:IOErrorEvent):void
		{
			trace("***PreloaderApplication assetManager ERROR***: "+ e);
		}
		
		private function assetManager_progressHandler(e:AssetEvent) : void
		{
			loaderView.label = "Loading Visual Assets: " + int((APPLICATION_PERCENTAGE*100)+(e.bytesLoaded/e.bytesTotal)*(CONTENT_PERCENTAGE*100)).toString() + "%";
			loaderView.percentage = APPLICATION_PERCENTAGE + (e.bytesLoaded/e.bytesTotal)*CONTENT_PERCENTAGE;
		}
		
		private function assetManager_completehandler(event:AssetEvent):void
		{
			if(event.groupBytesTotal == event.groupBytesLoaded )
			{
				assetManager.removeEventListener(AssetEvent.ASSET_COMPLETE, assetManager_completehandler);
				assetManager.removeEventListener(AssetEvent.ASSET_PROGRESS, assetManager_progressHandler);
				assetManager.removeEventListener(IOErrorEvent.IO_ERROR, assetManager_errorHandler);
				
				loaderView.label = "Initialising...";
				loaderView.percentage = 1;
				
				contentLoadCompleteHandler();
			}
		}
		
		/**
		 *  Updates preload progress while the application swf is loading
		 */
		public function onEnterFrame( event : Event = null ):void
		{
			
			if(framesLoaded == totalFrames)
			{
				applicationLoaded = true;
				removeEventListener(Event.ENTER_FRAME, onEnterFrame);
				
				if(configLoaded) instantiateMain();
			}
			else
			{
				loaderView.label = "Loading Application: "+int((APPLICATION_PERCENTAGE*100)*root.loaderInfo.bytesLoaded/root.loaderInfo.bytesTotal).toString()+"%";
				loaderView.percentage = (root.loaderInfo.bytesLoaded/root.loaderInfo.bytesTotal)*APPLICATION_PERCENTAGE;
			}
		
		}
		
		/**
		 *  Instantiates the Main application file
		 */
		private function instantiateMain():void
		{
			var mainClass:Class = Class(getDefinitionByName("Main"));
			
			if(mainClass)
			{
				var app:Object = new mainClass(configVO);
			}
		}
	}
}