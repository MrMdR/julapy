package
{
	import com.absentdesign.core.webapis.events.ServiceEvent;
	import com.absentdesign.core.webapis.wordpress.*;
	import com.absentdesign.core.webapis.wordpress.events.WPServiceEvent;
	import com.holler.assets.AssetGroupVOEvent;
	import com.holler.assets.AssetLoader;
	import com.holler.assets.AssetVOEvent;
	import com.julapy.blog.model.ConfigModel;
	import com.julapy.blog.view.PostHolder;
	import com.julapy.stage.StageSize;
	import com.julapy.stage.StageSizeEvent;

	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.events.Event;

	[ SWF( backgroundColor='0xCCCCCC', frameRate='25', width='1024', height='768' ) ]

	public class WordpressGrid extends Sprite
	{
		public var stageSize			: StageSize		= StageSize.getInstance();
		public var config				: ConfigModel	= ConfigModel.getInstance();
		public var assetLoader			: AssetLoader	= AssetLoader.getInstance();
		public var container			: Sprite		= new Sprite();

		public var service 				: WPService;
		public var wordpressData		: Array = new Array();

		public var loader				: MovieClip;
		public var postHolder			: PostHolder;

		public function WordpressGrid()
		{
			stage.scaleMode		= StageScaleMode.NO_SCALE;
			stage.align			= StageAlign.TOP_LEFT;

			stageSize.stage = stage;
			stageSize.addEventListener( StageSizeEvent.RESIZE, resizeHandler );

			stage.addChild( container );

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
			service.posts.getRecentPosts( 10 );
		}

		protected function getRecentPostsHandler(event:ServiceEvent):void
		{
			config.wordpressData = event.data as Array;
			config.parseWordpressData();

			hideLoader();
			initPosts();
		}

		//////////////////////////////////////////////////
		//	POSTS.
		//////////////////////////////////////////////////

		private function initPosts ():void
		{
			postHolder = new PostHolder( container );
			postHolder.init();
		}

		//////////////////////////////////////////////////
		//	RESIZE.
		//////////////////////////////////////////////////

		private function resizeHandler ( e : Event ):void
		{
			positionLoader();
		}
	}
}
