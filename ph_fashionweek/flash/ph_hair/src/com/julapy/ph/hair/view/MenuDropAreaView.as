package com.julapy.ph.hair.view
{
	import com.holler.core.View;
	import com.julapy.ph.hair.events.DropAreaEvent;
	import com.julapy.ph.hair.model.ModelLocator;

	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.utils.clearInterval;
	import flash.utils.setInterval;

	public class MenuDropAreaView extends View
	{
		private var base			: MovieClip;

		private var cross			: MenuDropAreaCrossView;
		private var arrows			: MenuDropAreaArrowView;
		private var ring			: MenuDropAreaRingView;
		private var ghost			: MenuDropAreaGhostView;

		private var bShow			: Boolean	= true;
		private var bIsOver			: Boolean	= false;
		private var bPlayingIn		: Boolean	= false;
		private var bPlayingOut		: Boolean	= false;

		private var circleRadiusFull	: Number = 170;
		private var circleWidthFull		: Number = 25;
		private var _circleRadius		: Number = 0;
		private var _circleWidth		: Number = 0;

		private var playInDelayIntv		: int;

		public function MenuDropAreaView(sprite:Sprite=null)
		{
			super(sprite);

			base			= _sprite.getChildByName( "base" ) as MovieClip;

			ghost			= new MenuDropAreaGhostView( _sprite.getChildByName( "ghost" ) as MovieClip );
			cross			= new MenuDropAreaCrossView( base.getChildByName( "cross" ) as MovieClip );
			arrows			= new MenuDropAreaArrowView( base.getChildByName( "arrows" ) as MovieClip );
			ring			= new MenuDropAreaRingView( base.getChildByName( "ring" ) as MovieClip );

			ModelLocator.getInstance().hairModel.addEventListener( DropAreaEvent.DROP_AREA_PLAYED_IN,	dropAreaPlayedInHandler );
			ModelLocator.getInstance().hairModel.addEventListener( DropAreaEvent.DROP_AREA_PLAYED_OUT,	dropAreaPlayedOutHandler );
			ModelLocator.getInstance().hairModel.addEventListener( DropAreaEvent.DROP_AREA_CHARGED, 	dropAreaChargedHandler );

			show( false );
		}

		public function show ( b : Boolean ):void
		{
			if( bShow == b )
				return;

			bShow = b;

			visible = bShow;
		}

		public function playIn ( b : Boolean ):void
		{
			if( b )
			{
				if( bPlayingIn )
					return;

				bPlayingIn = true;

				clearInterval( playInDelayIntv );
				playInDelayIntv = setInterval( playInDelay, 600 );
			}
			else
			{
				if( bPlayingOut )
					return;

				bPlayingOut = true;

				ghost.show( false );
				ring.show( false );
				arrows.show( false );
			}
		}

		private function playInDelay ():void
		{
			clearInterval( playInDelayIntv );

			ghost.show( true );
			ring.show( true );
			arrows.show( true );
		}

		public function reset ():void
		{
			show( false );
			clearInterval( playInDelayIntv );

			bPlayingIn	= false;
			bPlayingOut	= false;

			bIsOver		= false;

			ghost.reset();
			ring.reset();
			arrows.reset();
		}

		public function over ( b : Boolean ):void
		{
			if( bIsOver == b )
				return;

			bIsOver = b;

			ring.over( bIsOver );
		}

		public function showCross ( show : Boolean ):void
		{
			cross.show( show );
			ring.cross( show );
		}

		///////////////////////////////////////////////////
		//	MODEL HANDLERS.
		///////////////////////////////////////////////////

		private function dropAreaPlayedInHandler ( e : DropAreaEvent ):void
		{
			if( !bShow )
				return;

			bPlayingIn = false;
		}

		private function dropAreaPlayedOutHandler ( e : DropAreaEvent ):void
		{
			if( !bShow )
				return;

			bPlayingOut = false;
		}

		private function dropAreaChargedHandler ( e : DropAreaEvent ):void
		{
			if( !bShow )
				return;

			playIn( false );
		}
	}
}