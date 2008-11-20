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
	import flash.xml.XMLDocument;
	import flash.xml.XMLNode;


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
			
			var i:int, j:int;
			var root:XMLNode;
			var sectionsXMLNode:XMLNode;
			var imagesXMLNode:XMLNode;
			var xmlNode0:XMLNode, xmlNode1:XMLNode, xmlNode2:XMLNode;

			var xmlDoc:XMLDocument	= new XMLDocument();
            xmlDoc.ignoreWhite		= true;
            xmlDoc.parseXML( e.target.data );
            
            root			= xmlDoc.firstChild;
           
			for( i=0; i<root.childNodes.length; i++ )
			{
				sectionsXMLNode = root.childNodes[ i ] as XMLNode;
				
				images				= new Array();
				sectionVO			= new ImageSectionVO();
				sectionVO.images	= images;
				sections.push( sectionVO );
				
				for( j=0; j<sectionsXMLNode.childNodes.length; j++ )
				{
					imagesXMLNode	= sectionsXMLNode.childNodes[ j ] as XMLNode;
					
					xmlNode0		= ( imagesXMLNode.childNodes[ 0 ] as XMLNode ).firstChild;
					xmlNode1		= ( imagesXMLNode.childNodes[ 1 ] as XMLNode ).firstChild;
					xmlNode2		= ( imagesXMLNode.childNodes[ 2 ] as XMLNode ).firstChild;
					
					imageVO				= new ImageVO();
					imageVO.name		= ( xmlNode0 is XMLNode ) ? xmlNode0.toString() : "";
					imageVO.image01Path	= ( xmlNode1 is XMLNode ) ? xmlNode1.toString() : "";
					imageVO.image02Path	= ( xmlNode2 is XMLNode ) ? xmlNode2.toString() : "";
					
					images.push( imageVO );
				}
			}
			
			onResult( sections );
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