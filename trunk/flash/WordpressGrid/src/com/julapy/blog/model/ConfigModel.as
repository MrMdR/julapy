package com.julapy.blog.model
{
	import com.absentdesign.core.webapis.wordpress.CustomField;
	import com.absentdesign.core.webapis.wordpress.Post;
	import com.absentdesign.core.webapis.wordpress.WPStruct;
	import com.julapy.blog.event.PostHolderSizeEvent;
	import com.julapy.blog.event.PostScrollPositionEvent;
	import com.julapy.blog.vo.PostVO;

	import flash.events.EventDispatcher;
	import flash.geom.Rectangle;

	public class ConfigModel extends EventDispatcher
	{
		private static var instance : ConfigModel;

		private var _wordpressData		: Array;
		private var _postVOs 			: Array = new Array();
		private var _postHolderSize		: Rectangle = new Rectangle();
		private var _postScrollPosition	: Number = 0;

		public function ConfigModel()
		{
			if ( instance != null )
			{
				throw new Error( "use ConfigModel.getInstance()" );
			}
			instance = this;
		}

		public static function getInstance ():ConfigModel
		{
			if ( instance == null )
				instance = new ConfigModel( );

			return instance;
		}

		///////////////////////////////////////////////////
		//	BLOG DEETS.
		///////////////////////////////////////////////////

		public function get blogURL ():String
		{
//			return "http://www.julapy.com/blog/";
			return "http://www.julapy.com/portfolio/";
		}

		public function get blogUser ():String
		{
			return "julapy";
		}

		public function get blogPass ():String
		{
			return "tkm217";
		}

		///////////////////////////////////////////////////
		//	WORDPRESS DATA.
		///////////////////////////////////////////////////

		public function set wordpressData ( value : Array ):void
		{
			_wordpressData = value;
		}

		public function get wordpressData ():Array
		{
			return _wordpressData;
		}

		public function get postVOs ():Array
		{
			return _postVOs;
		}

		public function traceWordpressData( data : * ):void
		{
			if( data is WPStruct )
			{
				trace( ( data as WPStruct ).getDump() );
			}
			else if( data is Array )
			{
				for each( var struct : WPStruct in data )
				{
					trace( ( struct as WPStruct ).getDump() );
				}
			}
			else
			{
				trace( struct );
			}
		}

		public function parseWordpressData ():void
		{
			for( var i:int=0; i<wordpressData.length; i++ )
			{
				var post : Post;
				post = wordpressData[ i ] as Post;

				var postVO : PostVO;
				postVO = new PostVO();
				postVO.title		= post.title;
				postVO.link			= post.link;
				postVO.excerpt		= post.mt_excerpt;
				postVO.thumbPath	= parseThumbPath( post.custom_fields );

				_postVOs.push( postVO );
			}
		}

		private function parseThumbPath ( a : Array ):String
		{
			for( var i:int=0; i<a.length; i++ )
			{
				var customField : CustomField;
				customField = a[ i ];

				if( customField.key == "thumbPath" )
					return customField.value;
			}

			return "";
		}

		///////////////////////////////////////////////////
		//	POST HOLDER SIZE.
		///////////////////////////////////////////////////

		public function set postHolderSize ( rect : Rectangle ):void
		{
			_postHolderSize = rect;

			dispatchEvent( new PostHolderSizeEvent( _postHolderSize ) );
		}

		public function get postHolderSize ():Rectangle
		{
			return _postHolderSize;
		}

		//__

		public function set postScrollPosition ( p : Number ):void
		{
			_postScrollPosition = p;

			dispatchEvent( new PostScrollPositionEvent( _postScrollPosition ) );
		}

		public function get postScrollPosition ():Number
		{
			return _postScrollPosition;
		}
	}
}