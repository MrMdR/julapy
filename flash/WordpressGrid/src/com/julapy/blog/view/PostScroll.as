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
	import flash.events.KeyboardEvent;
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

		private var assetPoint		: Point = new Point();
		private var clickPoint		: Point = new Point();
		private var movePoint		: Point = new Point();
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
			trace( e.delta );
		}

		//////////////////////////////////////////////////
		//	DRAG.
		//////////////////////////////////////////////////

		private function startDrag ():void
		{
			bDown = true;

			assetPoint.x = asset.mouseX;
			assetPoint.y = asset.mouseY;

			clickPoint.x = stageSize.stage.mouseX;
			clickPoint.y = stageSize.stage.mouseY;
		}

		private function stopDrag ():void
		{
			bDown = false;
		}

		private function updateDrag ( e : Event = null ):void
		{
			if( bDown )
			{
				movePoint.x = stageSize.stage.mouseX;
				movePoint.y = stageSize.stage.mouseY;

				var dragPointLast : Point;
				dragPointLast = new Point( dragPoint.x, dragPoint.y );

				dragPoint.x += ( movePoint.x - dragPoint.x ) * dragEase;
				dragPoint.y += ( movePoint.y - dragPoint.y ) * dragEase;

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

			var p : Number;
			p = ( dragPoint.y - assetPoint.y ) / stageSize.stageHeight;

			var assetY : Number;
			assetY = p * stageSize.stageHeight;
			assetY = Math.min( Math.max( assetY, 0 ), stageSize.stageHeight - asset.height );

			asset.y = (int)( assetY );

			//-- update model.

			var scrollPosition : Number;
			scrollPosition = assetY / ( stageSize.stageHeight - asset.height );

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