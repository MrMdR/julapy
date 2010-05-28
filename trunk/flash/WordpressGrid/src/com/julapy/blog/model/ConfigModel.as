package com.julapy.blog.model
{
	import com.absentdesign.core.webapis.wordpress.CustomField;
	import com.absentdesign.core.webapis.wordpress.Post;
	import com.absentdesign.core.webapis.wordpress.WPStruct;
	import com.julapy.blog.vo.PostVO;

	public class ConfigModel
	{
		private static var instance : ConfigModel;

		private var _wordpressData	: Array;
		private var _postVOs 		: Array = new Array();

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
			return "http://www.julapy.com/blog/";
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
	}
}