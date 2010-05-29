package com.julapy.blog.view
{
	import caurina.transitions.Tweener;

	import com.holler.assets.AssetLoader;
	import com.holler.controls.BtnView;
	import com.julapy.blog.event.PostHolderSizeEvent;
	import com.julapy.blog.model.ConfigModel;
	import com.julapy.stage.StageSize;
	import com.julapy.stage.StageSizeEvent;

	import fl.motion.easing.Quadratic;

	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.EventDispatcher;
	import flash.events.MouseEvent;
	import flash.geom.Point;
	import flash.geom.Rectangle;

	public class PostScroll extends EventDispatcher
	{
		private var config		: ConfigModel	= ConfigModel.getInstance();
		private var stageSize	: StageSize		= StageSize.getInstance();
		private var container	: Sprite;
		private var asset		: MovieClip;
		private var btn			: BtnView;

		public  var offsetX			: Number = 0;
		private var offsetXShow		: Number = 0;
		private var offsetXHide		: Number = 35;

		private var bShow			: Boolean = false;
		private var bDown			: Boolean = false;
		private var bScroll			: Boolean = false;

		private var assetPoint		: Point = new Point();
		private var movePoint		: Point = new Point();
		private var scrollPoint		: Point = new Point();
		private var dragPoint		: Point = new Point();
		private var dragVel			: Point = new Point();
		private var dragVelDecay	: Number = 0.8;
		private var dragEase		: Number = 0.3;

		public function PostScroll( container : Sprite )
		{
			this.container = container;

			asset = AssetLoader.getInstance().getClassInstance( "julapy.blog.scroll" ) as MovieClip;
			container.addChild( asset );

			asset.addEventListener( Event.ENTER_FRAME, updateDrag );

			btn = new BtnView( asset );
			btn.addEventListener( MouseEvent.MOUSE_DOWN,	mouseDown );
			btn.addEventListener( MouseEvent.MOUSE_UP,		mouseUp );
			btn.mouseUpOutside = true;

			stageSize.stage.addEventListener( MouseEvent.MOUSE_WHEEL, mouseScrollHandler );

			stageSize.addEventListener( StageSizeEvent.RESIZE, stageResizeHandler );

			config.addEventListener( PostHolderSizeEvent.POST_HOLDER_SIZE_EVENT_CHANGE, postHolderChangeSizeHandler );

			offsetX = offsetXHide;

			position();
			checkSize( config.postHolderSize );
		}

		//////////////////////////////////////////////////
		//	MOUSE HANDLERS.
		//////////////////////////////////////////////////

		private function mouseDown ( e : MouseEvent ):void
		{
			startDrag();
		}

		private function mouseUp ( e : MouseEvent ):void
		{
			stopDrag();
		}

		private function mouseScrollHandler ( e : MouseEvent ):void
		{
			scroll( -e.delta * 4 );
		}

		//////////////////////////////////////////////////
		//	DRAG.
		//////////////////////////////////////////////////

		private function startDrag ():void
		{
			assetPoint.x = asset.mouseX;
			assetPoint.y = asset.mouseY;

			dragPoint.x = stageSize.stage.mouseX;
			dragPoint.y = stageSize.stage.mouseY;

			bDown = true;
		}

		private function stopDrag ():void
		{
			bDown = false;
		}

		private function scroll ( value : int ):void
		{
			scrollPoint.x = 0;
			scrollPoint.y = value;

			bScroll = true;
		}

		private function updateDrag ( e : Event = null ):void
		{
			if( bDown || bScroll )
			{
				if( bScroll )
				{
					movePoint.x = dragPoint.x;
					movePoint.y = dragPoint.y;

					movePoint.x += scrollPoint.x;
					movePoint.y += scrollPoint.y;

					bScroll = false;
				}
				else
				{
					movePoint.x = stageSize.stage.mouseX;
					movePoint.y = stageSize.stage.mouseY;
				}

				var dragPointLast : Point;
				dragPointLast = new Point( dragPoint.x, dragPoint.y );

				dragPoint.x += ( movePoint.x - dragPoint.x ) * dragEase;
				dragPoint.y += ( movePoint.y - dragPoint.y ) * dragEase;

				//-- store velocity.

				dragVel.x = dragPoint.x - dragPointLast.x;
				dragVel.y = dragPoint.y - dragPointLast.y;
			}
			else
			{
				dragVel.x *= dragVelDecay;
				dragVel.y *= dragVelDecay;

				dragPoint.x += dragVel.x;
				dragPoint.y += dragVel.y;
			}

			dragPoint.x = Math.min( Math.max( dragPoint.x, 0 ), stageSize.stageWidth );
			dragPoint.y = Math.min( Math.max( dragPoint.y, 0 ), stageSize.stageHeight );

			var p : Number;
			p = ( dragPoint.y - assetPoint.y ) / stageSize.stageHeight;

			var dragH : int;
			dragH = stageSize.stageHeight - asset.height;

			var assetY : Number;
			assetY = p * stageSize.stageHeight;
			assetY = Math.min( Math.max( assetY, 0 ), dragH );

			asset.y = (int)( assetY );

			//-- update model.

			var scrollPosition : Number;
			scrollPosition = assetY / dragH;

			config.postScrollPosition = scrollPosition;
		}

		//////////////////////////////////////////////////
		//	SHOW / HIDE.
		//////////////////////////////////////////////////

		public function show ():void
		{
			if( bShow )
				return;

			bShow = true;

			Tweener.addTween
			(
				this,
				{
					offsetX		: offsetXShow,
					time		: 0.3,
					transition	: Quadratic.easeOut,
					onUpdate	: offsetTweenUpdateHandler,
					onComplete	: null
				}
			);
		}

		public function hide ():void
		{
			if( !bShow )
				return;

			bShow = false;

			Tweener.addTween
			(
				this,
				{
					offsetX		: offsetXHide,
					time		: 0.3,
					transition	: Quadratic.easeOut,
					onUpdate	: offsetTweenUpdateHandler,
					onComplete	: null
				}
			);
		}

		private function offsetTweenUpdateHandler ():void
		{
			position();
		}

		//////////////////////////////////////////////////
		//	POSITION.
		//////////////////////////////////////////////////

		private function position ():void
		{
			asset.x	= stageSize.stageWidth + offsetX;
		}

		//////////////////////////////////////////////////
		//	RESIZE.
		//////////////////////////////////////////////////

		private function stageResizeHandler ( e : Event ):void
		{
			position();
		}

		//////////////////////////////////////////////////
		//	HANDLERS.
		//////////////////////////////////////////////////

		private function postHolderChangeSizeHandler ( e : PostHolderSizeEvent ):void
		{
			checkSize( e.size );
		}

		private function checkSize ( rect : Rectangle ):void
		{
			if( rect.height > stageSize.stageHeight )
			{
				show();
			}
			else
			{
				hide();
			}
		}
	}
}