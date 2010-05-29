package com.julapy.blog.view
{
	import com.julapy.blog.event.PostScrollPositionEvent;
	import com.julapy.blog.model.ConfigModel;
	import com.julapy.blog.vo.PostVO;
	import com.julapy.stage.StageSize;
	import com.julapy.stage.StageSizeEvent;

	import flash.display.Sprite;
	import flash.events.Event;
	import flash.geom.Point;
	import flash.geom.Rectangle;

	public class PostHolder
	{
		private var config				: ConfigModel	= ConfigModel.getInstance();
		private var stageSize			: StageSize		= StageSize.getInstance();
		private var container 			: Sprite;
		private var postContainer		: Sprite = new Sprite();

		private var postItems			: Array = new Array();
		private var postItemsLoadIndex	: int = 0;

		private var postItemsPad		: int = 200;
		private var postItemsGap		: int = 6;

		private var postScroll			: PostScroll;

		public function PostHolder( container : Sprite )
		{
			this.container = container;

			container.addChild( postContainer );

			postContainer.addEventListener( Event.ENTER_FRAME, enterFrameHandler );

			StageSize.getInstance().addEventListener( StageSizeEvent.RESIZE, resizeHandler );

			config.addEventListener( PostScrollPositionEvent.POST_SCROLL_POSITION_CHANGE, postScrollPositionHandler );
		}

		public function init ():void
		{
			initPosts();
			positionPostsForTheFirstTime();
			playInPosts();
			loadPostItems();
			initPostScroll();
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
				postItem = new PostItem( postContainer, postVO );

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
			ix = (int)( ( StageSize.getInstance().stageWidth - pad * 2 ) / ( PostItem.WIDTH + gx ) );

			var iw : int;
			iw = ix * ( PostItem.WIDTH + gx );

			var dx : int;
			dx = (int)( ( StageSize.getInstance().stageWidth - iw ) * 0.5 );

			var px : int;			// position.
			var py : int;

			px = dx;
			py = gy;

			for( var i:int=0; i<postItems.length; i++ )
			{
				var postItem : PostItem;
				postItem = postItems[ i ];
				postItem.setStartPosition( new Point( px, py ) );

				if( i == postItems.length - 1 )
					break;

				px += PostItem.WIDTH + gx;

				var px2 : int;
				px2 = px + PostItem.WIDTH + gx;

				if( px2 > dx + iw )
				{
					px	= dx;
					py += PostItem.HEIGHT + gy;
				}
			}

			var rect : Rectangle;
			rect = new Rectangle( 0, 0, StageSize.getInstance().stageWidth, py + PostItem.HEIGHT );
			config.postHolderSize = rect;
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

		private function loadPostItems ():void
		{
			loadPostItem();
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
			ix = (int)( ( StageSize.getInstance().stageWidth - pad * 2 ) / ( PostItem.WIDTH + gx ) );

			var iw : int;
			iw = ix * ( PostItem.WIDTH + gx );

			var dx : int;
			dx = (int)( ( StageSize.getInstance().stageWidth - iw ) * 0.5 );

			var px : int;			// position.
			var py : int;

			px = dx;
			py = gy;

			for( var i:int=0; i<postItems.length; i++ )
			{
				var postItem : PostItem;
				postItem = postItems[ i ];
				postItem.setPosition( new Point( px, py ) );

				if( i == postItems.length - 1 )
					break;

				px += PostItem.WIDTH + gx;

				var px2 : int;
				px2 = px + PostItem.WIDTH + gx;

				if( px2 > dx + iw )
				{
					px	= dx;
					py += PostItem.HEIGHT + gy;
				}
			}

			var rect : Rectangle;
			rect = new Rectangle( 0, 0, StageSize.getInstance().stageWidth, py + PostItem.HEIGHT );
			config.postHolderSize = rect;
		}

		//////////////////////////////////////////////////
		//	SCROLL.
		//////////////////////////////////////////////////

		private function initPostScroll ():void
		{
			postScroll = new PostScroll( container );
		}

		private function postScrollPositionHandler ( e : PostScrollPositionEvent ):void
		{
			var rect : Rectangle;
			rect = config.postHolderSize.clone();

			var postContainerY : Number;
			postContainerY = -( rect.height - stageSize.stageHeight ) * e.position;

			postContainer.y = (int)( postContainerY );
		}

		//////////////////////////////////////////////////
		//	RESIZE.
		//////////////////////////////////////////////////

		private function resizeHandler ( e : Event ):void
		{
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