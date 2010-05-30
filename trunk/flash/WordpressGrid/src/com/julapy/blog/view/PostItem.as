package com.julapy.blog.view
{
	import caurina.transitions.Tweener;

	import com.holler.assets.AssetLoader;
	import com.holler.controls.Image;
	import com.holler.events.ImageLoadedEvent;
	import com.julapy.blog.vo.PostVO;

	import fl.motion.easing.Quadratic;

	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.EventDispatcher;
	import flash.events.IOErrorEvent;
	import flash.events.MouseEvent;
	import flash.events.ProgressEvent;
	import flash.events.TimerEvent;
	import flash.filters.BitmapFilterQuality;
	import flash.filters.BlurFilter;
	import flash.geom.Point;
	import flash.net.URLRequest;
	import flash.net.navigateToURL;
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	import flash.utils.Timer;

	public class PostItem extends EventDispatcher
	{
		public static const WIDTH	: int = 200;
		public static const HEIGHT	: int = 324;

		private var container	: Sprite;
		public  var mc 			: MovieClip;

		private var vo			: PostVO;

		private var title		: TextField;
		private var excerpt		: TextField;
		private var overMc		: MovieClip;
		private var image		: MovieClip;
		private var imageHolder	: MovieClip;
		private var imageLoader	: MovieClip;
		private var imageView	: Image;

		public  var imageLoaderPer1	: Number = 0;
		public  var imageLoaderPer2	: Number = 0;

		private var point		: Point = new Point();
		private var pointTarget	: Point = new Point();
		private var pointEase	: Number = 0.3;

		private var bPlayedIn	: Boolean = false;

		private var overAlpha		: Number = 0.2;
		private var outAlpha		: Number = 0.0;

		private var flickerTimer	: Timer;
		private var flickerSwitch	: Boolean = false;

		public function PostItem( container : Sprite, vo : PostVO )
		{
			this.container	= container;
			this.vo			= vo;

			mc = AssetLoader.getInstance().getClassInstance( "julapy.blog.post.item" ) as MovieClip;
			container.addChild( mc );

			title		= mc.getChildByName( "title" ) as TextField;
			excerpt		= mc.getChildByName( "excerpt" ) as TextField;
			overMc		= mc.getChildByName( "over" ) as MovieClip;
			image		= mc.getChildByName( "image" ) as MovieClip;
			imageHolder	= image.getChildByName( "holder" ) as MovieClip;
			imageLoader	= image.getChildByName( "loader" ) as MovieClip;

			imageLoader.iw		= imageLoader.width;
			imageLoader.width	= 0;

			title.text			= vo.title;
			title.autoSize		= TextFieldAutoSize.LEFT;

			excerpt.htmlText	= vo.excerpt;
			excerpt.y			= title.y + title.height + 3;

			mc.addEventListener( MouseEvent.MOUSE_OVER, clickthruHandler );
			mc.addEventListener( MouseEvent.MOUSE_OUT,	clickthruHandler );
			mc.addEventListener( MouseEvent.MOUSE_DOWN,	clickthruHandler );
			mc.addEventListener( MouseEvent.MOUSE_UP,	clickthruHandler );
			mc.mouseChildren = false;
			mc.buttonMode = true;
		}

		public function playIn ( delay : Number = 0 ):void
		{
			mc.alpha	= 0.0;
			mc.iy		= mc.y;
			mc.y		+= 10;

			Tweener.addTween
			(
				mc,
				{
					alpha		: 1.0,
					y			: mc.iy,
					time		: 0.2,
					delay		: delay,
					transition	: Quadratic.easeOut,
					onComplete	: playInCompleteHandler
				}
			);
		}

		private function playInCompleteHandler ():void
		{
			bPlayedIn = true;
		}

		private function showImage ():void
		{
			imageHolder.alpha = 0.0;

			Tweener.addTween
			(
				imageHolder,
				{
					alpha		: 1.0,
					time		: 0.3,
					transition	: Quadratic.easeOut,
					onComplete	: null
				}
			);
		}

		public function loadImage ():void
		{
			imageView = new Image( imageHolder );
			imageView.addEventListener( ProgressEvent.PROGRESS,					imageViewHandler );
			imageView.addEventListener( ImageLoadedEvent.IMAGE_LOADED_EVENT,	imageViewHandler );
			imageView.addEventListener( IOErrorEvent.IO_ERROR, 					imageViewHandler );
			imageView.load( vo.thumbPath );
		}

		public function setStartPosition ( p : Point ):void
		{
			pointTarget = p;
			point		= p;

			mc.x = (int)( point.x );
			mc.y = (int)( point.y );
		}

		public function setPosition ( p : Point ):void
		{
			pointTarget = p;
		}

		public function update ():void
		{
			if( !bPlayedIn )
				return;

			point.x += ( pointTarget.x - point.x ) * pointEase;
			point.y += ( pointTarget.y - point.y ) * pointEase;

			mc.x = (int)( point.x );
			mc.y = (int)( point.y );

			//-- image loader.

			imageLoaderPer2 += ( imageLoaderPer1 - imageLoaderPer2 ) * 0.6;

			imageLoader.width = (int)( imageLoader.iw * imageLoaderPer2 );
		}

		//////////////////////////////////////////////////
		//	OVER / OUT.
		//////////////////////////////////////////////////

		private function over ():void
		{
			overFlicker();
//			overThumbBlur();
		}

		private function out ():void
		{
			outFlicker();
//			outThumbBlur();
		}

		private function down ():void
		{
			downFlicker();
		}

		//////////////////////////////////////////////////
		//	BLUR.
		//////////////////////////////////////////////////

		private var _thumbBlur : Number = 0;

		public function set thumbBlur ( value : Number ):void
		{
			_thumbBlur = value;

			updateThumbBlur();
		}

		public function get thumbBlur ():Number
		{
			return _thumbBlur;
		}

		private function overThumbBlur ():void
		{
			Tweener.addTween
			(
				this,
				{
					thumbBlur	: 1.0,
					time		: 0.25,
					transition	: Quadratic.easeOut
				}
			);
		}

		private function outThumbBlur ():void
		{
			Tweener.addTween
			(
				this,
				{
					thumbBlur	: 0.0,
					time		: 0.25,
					transition	: Quadratic.easeOut
				}
			);
		}

		private function updateThumbBlur ():void
		{
			var blurScale : Number;
			blurScale = 1;

			var blur : BlurFilter;
			blur = new BlurFilter( thumbBlur * blurScale, thumbBlur * blurScale, BitmapFilterQuality.HIGH );

			imageHolder.filters = [ blur ];
		}

		//////////////////////////////////////////////////
		//	FLICKER.
		//////////////////////////////////////////////////

		private function overFlicker ():void
		{
			Tweener.removeTweens( overMc );

			initFlickerTimer();
		}

		private function outFlicker ():void
		{
			killFlickerTimer();

			flickerSwitch	= false;
			overMc.alpha	= overAlpha;

			Tweener.addTween
			(
				overMc,
				{
					alpha		: outAlpha,
					time		: 0.2,
					transition	: Quadratic.easeOut
				}
			);

		}

		private function downFlicker ():void
		{
			killFlickerTimer();

			Tweener.addTween
			(
				overMc,
				{
					alpha		: overAlpha,
					time		: 0.2,
					transition	: Quadratic.easeOut
				}
			);
		}

		//--

		private function initFlickerTimer ():void
		{
			killFlickerTimer();

			flickerTimer = new Timer( 30 );
			flickerTimer.addEventListener( TimerEvent.TIMER, flickerTimerHandler );
			flickerTimer.start();
		}

		private function killFlickerTimer ():void
		{
			if( !flickerTimer )
				return;

			flickerTimer.removeEventListener( TimerEvent.TIMER, flickerTimerHandler );
			flickerTimer.stop();
			flickerTimer = null;
		}

		private function flickerTimerHandler ( e : TimerEvent ):void
		{
			flickerSwitch = !flickerSwitch;

			if( flickerSwitch )
				overMc.alpha = overAlpha;
			else
				overMc.alpha = outAlpha;

		}

		//////////////////////////////////////////////////
		//	HANDLERS.
		//////////////////////////////////////////////////

		private function imageViewHandler ( e : Event ):void
		{
			if( e.type == ProgressEvent.PROGRESS )
			{
				var pe : ProgressEvent;
				pe = e as ProgressEvent;

				imageLoaderPer1 = pe.bytesLoaded / pe.bytesTotal;
			}
			else
			{
				showImage();

				dispatchEvent( new Event( Event.CHANGE ) );
			}
		}

		private function clickthruHandler ( e : MouseEvent ):void
		{
			if( e.type == MouseEvent.MOUSE_OVER )
			{
				over();
			}

			if( e.type == MouseEvent.MOUSE_OUT )
			{
				out();
			}

			if( e.type == MouseEvent.MOUSE_DOWN )
			{
				down();
			}

			if( e.type == MouseEvent.MOUSE_UP )
			{
				navigateToURL( new URLRequest( vo.link ), "_blank" );
			}
		}
	}
}