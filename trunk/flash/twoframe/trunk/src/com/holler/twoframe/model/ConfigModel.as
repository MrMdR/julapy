////////////////////////////////////////////////////////////////////////////////
//
//  HOLLER SYDNEY
//  Copyright 2008 Holler Sydney
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

package com.holler.twoframe.model
{
	import com.holler.twoframe.events.ImagesChangeEvent;
	import com.holler.twoframe.events.InitialiseVOChangeEvent;
	import com.holler.twoframe.vo.ConfigVO;
	import com.holler.twoframe.vo.InitialiseVO;
	
	import flash.events.EventDispatcher;
	
	[Event(name="initialiseVOChange", type="com.holler.twoframe.events.InitialiseVOChangeEvent")]
	public class ConfigModel extends EventDispatcher
	{
		/**
		 *  This value denotes whether the amfphp services are enabled
		 */
		public var amfphpEnabled:Boolean = false;
		/**
		 *  This value denotes whether the xml services are enabled
		 */
		public var xmlEnabled:Boolean = true;
		/**
		 *  This value denotes whether the xml services are enabled
		 */
		public var httpEnabled:Boolean = true;
		
		/**
		 *  Contains the information loaded from the config.xml file
		 */
		public var configVO:ConfigVO;
		
		/**
		 *  The initialiseVO returned from the amfphp GetInitialiseVOCommand
		 */
		private var _initialiseVO:InitialiseVO;

		/**
		 *  The images returned from the amfphp GetImagesCommand
		 */
		private var _images:Array;
		
		public function ConfigModel()
		{
			
		}
		
		public function set initialiseVO(value:InitialiseVO):void
		{
			if(value != initialiseVO)
			{
				_initialiseVO = value;
				
				var initialiseEvent:InitialiseVOChangeEvent = new InitialiseVOChangeEvent();
				initialiseEvent.initialiseVO = initialiseVO;
				dispatchEvent(initialiseEvent);
			}
		}
		
		public function get initialiseVO():InitialiseVO
		{
			return _initialiseVO;
		}
		
		public function set images ( value:Array ):void
		{
			if( value != images )
			{
				_images = value;
				
				var imagesEvent:ImagesChangeEvent = new ImagesChangeEvent();
				dispatchEvent( imagesEvent );
			}
		}
		
		public function get images ():Array
		{
			return _images;
		}
	}
}