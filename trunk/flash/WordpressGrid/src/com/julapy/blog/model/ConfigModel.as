package com.julapy.blog.model
{
	import com.absentdesign.core.webapis.wordpress.Blog;
	import com.absentdesign.core.webapis.wordpress.CustomField;
	import com.absentdesign.core.webapis.wordpress.Post;
	import com.absentdesign.core.webapis.wordpress.WPStruct;
	import com.julapy.blog.event.PostHolderSizeEvent;
	import com.julapy.blog.event.PostScrollPositionEvent;
	import com.julapy.blog.vo.BlogVO;
	import com.julapy.blog.vo.PostVO;

	import flash.events.EventDispatcher;
	import flash.geom.Rectangle;

	public class ConfigModel extends EventDispatcher
	{
		private static var instance : ConfigModel;

		private var _blogData			: Array;
		private var _blogVO				: BlogVO;
		private var _postData			: Array;
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
		//	BLOG DATA.
		///////////////////////////////////////////////////

		public function set blogData ( a :Array ):void
		{
			_blogData = a;
		}

		public function get blogData ():Array
		{
			return _blogData;
		}

		public function parseBlogData ():void
		{
			var blog : Blog;
			blog = _blogData[ 0 ] as Blog;

			_blogVO = new BlogVO();
			_blogVO.blogTitle	= blog.blog_name;
			_blogVO.blogSubTitle	= "";		// TODO.
			_blogVO.blogUrl		= blog.url;
		}

		public function get blogVO ():BlogVO
		{
			return _blogVO;
		}

		///////////////////////////////////////////////////
		//	WORDPRESS DATA.
		///////////////////////////////////////////////////

		public function set postData ( value : Array ):void
		{
			_postData = value;
		}

		public function get postData ():Array
		{
			return _postData;
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

		public function parsePostData ():void
		{
			for( var i:int=0; i<postData.length; i++ )
			{
				var post : Post;
				post = postData[ i ] as Post;

				var postVO : PostVO;
				postVO = new PostVO();
				postVO.title		= post.title;
				postVO.excerpt		= post.mt_excerpt;
				postVO.thumbPath	= parseCustomField( post.custom_fields, "thumbPath" );
				postVO.link			= parseCustomField( post.custom_fields, "link" );

				_postVOs.push( postVO );
			}
		}

		private function parseCustomField ( a : Array, key : String ):String
		{
			for( var i:int=0; i<a.length; i++ )
			{
				var customField : CustomField;
				customField = a[ i ];

				if( customField.key == key )
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