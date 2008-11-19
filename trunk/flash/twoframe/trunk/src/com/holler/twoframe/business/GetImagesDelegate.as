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
	import com.holler.twoframe.vo.ImageSectionVO;
	import com.holler.twoframe.vo.ImageVO;
	
	import flash.events.ErrorEvent;
	import flash.events.Event;
	import flash.net.URLLoader;
	import flash.net.URLRequest;


	public class GetImagesDelegate extends Delegate implements IResponder
	{

		public function GetImagesDelegate(_responder:IResponder)
		{
			super(_responder);
		}

		public function initialise():void
		{
			var request:URLRequest;
            var loader:URLLoader;
            
            request = new URLRequest( "content.xml" );
            loader	= new URLLoader( );
            loader.addEventListener( Event.COMPLETE,	completeHandler );
            loader.addEventListener( ErrorEvent.ERROR,	errorHandler );

            try
            {
				loader.load( request );
            }
            catch ( error:ArgumentError )
            {
				trace ("An ArgumentError has occurred." );
            }
            catch ( error:SecurityError )
            {
				trace( "A SecurityError has occurred." );
            }
		}
		
		private function completeHandler ( e:Event ):void
		{
			var xml:XML;
			var sections:Array	= new Array();
			var images:Array	= new Array();
			var sectionVO:ImageSectionVO;
			var imageVO:ImageVO;
			
			var i:int;
			var root:XMLList;
			var sectionsXMLList:XMLList;
			var imagesXMLList:XMLList;
			
			xml				= new XML( e.target.data );
			sectionsXMLList	= xml.children();
			
			
			
			for( i=0; i<sectionsXMLList.length(); i++ )
			{
//				imagesXMLList = sectionsXMLList.children().;
//				trace( imagesXMLList );
			}
		}
		
		private function errorHandler ( e:ErrorEvent ):void
		{
			onFault( e );
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