package
{
	import com.absentdesign.core.webapis.events.ServiceEvent;
	import com.absentdesign.core.webapis.wordpress.*;
	import com.absentdesign.core.webapis.wordpress.events.WPServiceEvent;
	import com.holler.assets.AssetGroupVOEvent;
	import com.holler.assets.AssetLoader;
	import com.holler.assets.AssetVOEvent;
	import com.julapy.blog.model.ConfigModel;
	import com.julapy.blog.view.PostItem;
	import com.julapy.blog.vo.PostVO;

	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.events.Event;
	import flash.geom.Point;

	[ SWF( backgroundColor='0xCCCCCC', frameRate='25', width='1024', height='768' ) ]

	public class WordpressExample extends Sprite
	{
		public var config				: ConfigModel = ConfigModel.getInstance();
		public var assetLoader			: AssetLoader = AssetLoader.getInstance();
		public var container			: Sprite = new Sprite();

		public var service 				: WPService;
		public var wordpressData		: Array = new Array();

		public var loader				: MovieClip;

		public var postItems			: Array = new Array();
		public var postItemsLoadIndex	: int = 0;

		public var postItemsPad			: int = 200;
		public var postItemsGap			: int = 6;

		public function WordpressExample()
		{
			stage.scaleMode		= StageScaleMode.NO_SCALE;
			stage.align			= StageAlign.TOP_LEFT;

			stage.addChild( container );
			stage.addEventListener( Event.RESIZE, resizeHandler );
			stage.addEventListener( Event.ENTER_FRAME, enterFrameHandler );

			loadAssets();
		}

		//////////////////////////////////////////////////
		//	ASSETS.
		//////////////////////////////////////////////////

		private function loadAssets ():void
		{
			AssetLoader.USE_SIZE_SCRIPT = false;

			assetLoader.addEventListener( AssetGroupVOEvent.ASSET_GROUP_COMPLETE,	assetLoaderHandler );
			assetLoader.addEventListener( AssetGroupVOEvent.ASSET_GROUP_PROGRESS,	assetLoaderHandler );
			assetLoader.addEventListener( AssetGroupVOEvent.ASSET_GROUP_ERROR,		assetLoaderHandler );
			assetLoader.addEventListener( AssetVOEvent.ASSET_COMPLETE,				assetLoaderHandler );

			assetLoader.addAsset( "swfs/Assets.swf", "assets", "main" );
			assetLoader.loadAssets( "assets", false );
		}

		private function assetLoaderHandler ( e : Event ):void
		{
			if( e.type == AssetGroupVOEvent.ASSET_GROUP_COMPLETE )
			{
				showLoader();
				initWordpress();
			}
		}

		//////////////////////////////////////////////////
		//	LOADER.
		//////////////////////////////////////////////////

		private function showLoader ():void
		{
			loader = AssetLoader.getInstance().getClassInstance( "julapy.blog.loader" ) as MovieClip;
			stage.addChild( loader );

			positionLoader();
		}

		private function hideLoader ():void
		{
			stage.removeChild( loader );
		}

		private function positionLoader ():void
		{
			if( !loader )
				return;

			loader.x = (int)( stage.stageWidth  * 0.5 );
			loader.y = (int)( stage.stageHeight * 0.5 );
		}

		//////////////////////////////////////////////////
		//	WORDPRESS.
		//////////////////////////////////////////////////

		private function initWordpress ():void
		{
			service = new WPService( config.blogURL, config.blogUser, config.blogPass );
			service.addEventListener( WPServiceEvent.GET_RECENT_POSTS, getRecentPostsHandler );
			service.posts.getRecentPosts( 15 );
		}

		protected function getRecentPostsHandler(event:ServiceEvent):void
		{
			config.wordpressData = event.data as Array;
			config.parseWordpressData();

			hideLoader();

			initPosts();
			positionPostsForTheFirstTime();
			playInPosts();
			loadPostItem();
		}

		//////////////////////////////////////////////////
		//	POSTS.
		//////////////////////////////////////////////////

		private function initPosts ():void
		{
			for( var i:int=0; i<config.postVOs.length; i++ )
			{
				var postVO : PostVO;
				postVO = config.postVOs[ i ];

				var postItem : PostItem;
				postItem = new PostItem( container, postVO );

				postItems.push( postItem );
			}
		}

		private function positionPostsForTheFirstTime ():void
		{
			var pad : int;
			pad = postItemsPad;

			var gx : int;			// gap.
			var gy : int;

			gx = postItemsGap;
			gy = postItemsGap;

			var ix : int;
			ix = (int)( ( stage.stageWidth - pad * 2 ) / ( PostItem.WIDTH + gx ) );

			var iw : int;
			iw = ix * ( PostItem.WIDTH + gx );

			var dx : int;
			dx = (int)( ( stage.stageWidth - iw ) * 0.5 );

			var px : int;			// position.
			var py : int;

			px = dx;
			py = gy;

			for( var i:int=0; i<postItems.length; i++ )
			{
				var postItem : PostItem;
				postItem = postItems[ i ];
				postItem.setStartPosition( new Point( px, py ) );

				px += PostItem.WIDTH + gx;

				var px2 : int;
				px2 = px + PostItem.WIDTH + gx;

				if( px2 > dx + iw )
				{
					px	= dx;
					py += PostItem.HEIGHT + gy;
				}
			}
		}

		private function playInPosts ():void
		{
			for( var i:int=0; i<postItems.length; i++ )
			{
				var postItem : PostItem;
				postItem = postItems[ i ];
				postItem.playIn( i * 0.1 );
			}
		}

		private function loadPostItem ( e : Event = null ):void
		{
			var postItem : PostItem;

			if( !e )
			{
				postItem = postItems[ postItemsLoadIndex ];
				postItem.addEventListener( Event.CHANGE, loadPostItem );
				postItem.loadImage();
			}
			else
			{
				postItem = e.target as PostItem;
				postItem.removeEventListener( Event.CHANGE, loadPostItem );
				postItem = null;

				if( ++postItemsLoadIndex < postItems.length )
				{
					postItem = postItems[ postItemsLoadIndex ];
					postItem.addEventListener( Event.CHANGE, loadPostItem );
					postItem.loadImage();
				}
			}
		}

		private function positionPosts ():void
		{
			var pad : int;
			pad = postItemsPad;

			var gx : int;			// gap.
			var gy : int;

			gx = postItemsGap;
			gy = postItemsGap;

			var ix : int;
			ix = (int)( ( stage.stageWidth - pad * 2 ) / ( PostItem.WIDTH + gx ) );

			var iw : int;
			iw = ix * ( PostItem.WIDTH + gx );

			var dx : int;
			dx = (int)( ( stage.stageWidth - iw ) * 0.5 );

			var px : int;			// position.
			var py : int;

			px = dx;
			py = gy;

			for( var i:int=0; i<postItems.length; i++ )
			{
				var postItem : PostItem;
				postItem = postItems[ i ];
				postItem.setPosition( new Point( px, py ) );

				px += PostItem.WIDTH + gx;

				var px2 : int;
				px2 = px + PostItem.WIDTH + gx;

				if( px2 > dx + iw )
				{
					px	= dx;
					py += PostItem.HEIGHT + gy;
				}
			}
		}

		//////////////////////////////////////////////////
		//	RESIZE.
		//////////////////////////////////////////////////

		private function resizeHandler ( e : Event ):void
		{
			positionLoader();
			positionPosts();
		}

		private function enterFrameHandler ( e : Event ):void
		{
			for( var i:int=0; i<postItems.length; i++ )
			{
				var postItem : PostItem;
				postItem = postItems[ i ];
				postItem.update();
			}
		}
	}
}
