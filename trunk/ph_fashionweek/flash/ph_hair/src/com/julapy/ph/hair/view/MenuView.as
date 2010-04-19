package com.julapy.ph.hair.view
{
	import caurina.transitions.Tweener;

	import com.holler.core.View;
	import com.julapy.ph.hair.events.DropAreaEvent;
	import com.julapy.ph.hair.events.GirlEvent;
	import com.julapy.ph.hair.events.InteractiveVideoEvent;
	import com.julapy.ph.hair.events.SectionEvent;
	import com.julapy.ph.hair.events.StyleEvent;
	import com.julapy.ph.hair.events.StylePartEvent;
	import com.julapy.ph.hair.events.ToolEvent;
	import com.julapy.ph.hair.model.HairModel;
	import com.julapy.ph.hair.model.ModelLocator;
	import com.julapy.ph.hair.vo.StyleVO;
	import com.julapy.ph.vo.TrackerVO;

	import fl.motion.easing.Quadratic;

	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.events.TimerEvent;
	import flash.geom.Point;
	import flash.geom.Rectangle;
	import flash.utils.Timer;

	public class MenuView extends View
	{
		private var dropArea		: MenuDropAreaView;
		private var dropAreas		: Array = new Array();

		private var tool			: MenuToolView;
		private var tools			: Array = new Array();
		private var toolCovers		: Array = new Array();
		private var toolIndex		: int = -1;
		private var toolSelected	: int = -1;
		private var toolProximity	: int = 100;

		private var toolPath		: MenuToolPathView;
		private var toolPathShowing	: Boolean = false;

		private var toolAnim		: MenuToolAnimView;

		private var idleSoundTimer	: Timer;

		private var timeoutTimer	: Timer;
		private var timeoutTimerSec	: int = 60;

		private var isRightTool		: Boolean = false;
		private var isInProximity	: Boolean = false;

		private var bEnabled		: Boolean = false;

		public function MenuView(sprite:Sprite=null)
		{
			super(sprite);

			toolPath	= new MenuToolPathView( _sprite.getChildByName( "toolPath" ) as MovieClip );
			toolAnim	= new MenuToolAnimView( _sprite.getChildByName( "toolAnim" ) as MovieClip );

			initDropAreas();
			initTools();

			ModelLocator.getInstance().hairModel.addEventListener( GirlEvent.GIRL_CHANGE,				girlChangeHandler );
			ModelLocator.getInstance().hairModel.addEventListener( StyleEvent.STYLE_CHANGE,				styleChangeHandler );
			ModelLocator.getInstance().hairModel.addEventListener( SectionEvent.SECTION_CHANGE,			sectionChangeHandler );
			ModelLocator.getInstance().hairModel.addEventListener( StylePartEvent.STYLE_PART_CHANGE,	stylePartChangeHandler );
			ModelLocator.getInstance().hairModel.addEventListener( ToolEvent.TOOL_CHANGE,				toolChangeHandler );

			ModelLocator.getInstance().hairModel.addEventListener( DropAreaEvent.DROP_AREA_PLAYED_IN,	dropAreaPlayedInHandler );
			ModelLocator.getInstance().hairModel.addEventListener( DropAreaEvent.DROP_AREA_PLAYED_OUT,	dropAreaPlayedOutHandler );
			ModelLocator.getInstance().hairModel.addEventListener( DropAreaEvent.DROP_AREA_CHARGED, 	dropAreaChargedHandler );

			ModelLocator.getInstance().hairModel.addEventListener( InteractiveVideoEvent.PLAYING_IN,	interactiveVideoHandler );
			ModelLocator.getInstance().hairModel.addEventListener( InteractiveVideoEvent.PLAYED_IN,		interactiveVideoHandler );
			ModelLocator.getInstance().hairModel.addEventListener( InteractiveVideoEvent.PLAYING_OUT,	interactiveVideoHandler );
			ModelLocator.getInstance().hairModel.addEventListener( InteractiveVideoEvent.PLAYED_OUT,	interactiveVideoHandler );
		}

		private function initDropAreas ():void
		{
			dropAreas =
			[
				new MenuDropAreaView( _sprite.getChildByName( "dropAreaDryer" ) as MovieClip ),
				new MenuDropAreaView( _sprite.getChildByName( "dropAreaCurlers" ) as MovieClip ),
				new MenuDropAreaView( _sprite.getChildByName( "dropAreaSpray" ) as MovieClip )
			];
		}

		private function initTools ():void
		{
			tools =
			[
				new MenuToolDryerView(  _sprite.getChildByName( "toolDryer" )  as MovieClip ),
				new MenuToolCurlerView( _sprite.getChildByName( "toolCurler" ) as MovieClip ),
				new MenuToolSprayTool(  _sprite.getChildByName( "toolSpray" )  as MovieClip )
			];

			for( var i:int=0; i<tools.length; i++ )
			{
				var t : MenuToolView;
				t = tools[ i ] as MenuToolView;

				t.addEventListener( MouseEvent.MOUSE_DOWN,	toolBtnHandler );
				t.addEventListener( MouseEvent.MOUSE_UP,	toolBtnHandler );
			}

			toolCovers =
			[
				new MenuToolCoverView( _sprite.getChildByName( "toolCover0" ) as MovieClip ),
				new MenuToolCoverView( _sprite.getChildByName( "toolCover1" ) as MovieClip ),
				new MenuToolCoverView( _sprite.getChildByName( "toolCover2" ) as MovieClip ),
			];
		}

		/////////////////////////////////////////////
		//	PLAY IN.
		/////////////////////////////////////////////

		public function playIn ( b : Boolean ):void
		{
			if( b )
			{
				alpha = 1;
				doValidate();

				return;

				//-- tween, remove return.

				alpha = 0;
				doValidate();

				Tweener.addTween
				(
					this,
					{
						alpha			: 1.0,
						time			: 0.3,
						delay			: 0.0,
						transition		: Quadratic.easeOut,
						onStart			: null,
						onUpdate		: playInTweenUpdateHandler,
						onComplete		: null
					}
				);
			}
			else
			{
				Tweener.addTween
				(
					this,
					{
						alpha			: 0.0,
						time			: 0.3,
						delay			: 0.0,
						transition		: Quadratic.easeOut,
						onStart			: null,
						onUpdate		: playInTweenUpdateHandler,
						onComplete		: null
					}
				);
			}
		}

		private function playInTweenUpdateHandler ():void
		{
			doValidate();
		}

		/////////////////////////////////////////////
		//	DROP AREAS.
		/////////////////////////////////////////////

		private function selectDropArea ( id : int ):void
		{
			if( !dropArea && id == -1 )		// all already off, nothing to turn off.
				return;

			if( dropArea )
			{
				dropArea = null;
			}

			for( var i:int=0; i<dropAreas.length; i++ )
			{
				var da : MenuDropAreaView;
				da = dropAreas[ i ] as MenuDropAreaView;

				var match : Boolean;
				match = ( i == id );

				if( match )
				{
					dropArea = da;
					dropArea.reset();
					dropArea.playIn( true );
				}

				da.show( match );
			}
		}

		/////////////////////////////////////////////
		//	TOOL COVERS
		/////////////////////////////////////////////

		private function selectToolCover ( j : int ):void
		{
			for( var i:int=0; i<toolCovers.length; i++ )
			{
				( toolCovers[ i ] as MenuToolCoverView ).show( ( i != j ) && ( j != -1 ) );
			}
		}

		/////////////////////////////////////////////
		//	LOGIC.
		/////////////////////////////////////////////

		private function startToolDrag ( t : MenuToolView ):void
		{
			if( toolSelected > -1 && toolSelected != toolIndex )
				return;

			tool = t;

			if( ModelLocator.getInstance().hairModel.bInteractiveVideoPlaying )
			{
				playInToolPath( true );
//				playToolAnim( true );
			}
			else
			{
				tool.rotateToDropArea();
			}

			_sprite.addEventListener( Event.ENTER_FRAME, enterFrameHandler );
		}

		private function stopToolDrag ( t : MenuToolView ):void
		{
			if( !t )
				return;

			_sprite.removeEventListener( Event.ENTER_FRAME, enterFrameHandler );

			if( ModelLocator.getInstance().hairModel.bInteractiveVideoPlaying )
			{
				playInToolPath( false );
				playToolAnim( false );
			}

			t.scaleUp( false );
			t.returnToMenu();

			if( dropArea )		// if tool is deselects, reset dropArea.
			{
				dropArea.over( false );
				dropArea.showCross( false );
			}

			initIdleSoundTimer();
			initTimeoutTimer();
		}

		private function enterFrameHandler ( e : Event ):void
		{
			if( ModelLocator.getInstance().hairModel.bInteractiveVideoPlaying )
			{
				positionToolAlongPath();
			}
			else
			{
				isRightTool		= checkIsRightTool();
				isInProximity	= checkIsInProximity();

				updateToolPosition();
				updateToolScale();
				updateDropArea();
			}
		}

		/////////////////////////////////////////////
		//	PUT TOOL IN DROP AREA.
		/////////////////////////////////////////////

		private function checkIsRightTool ():Boolean
		{
			if( !dropArea )
				return false;

			isRightTool = ( toolIndex == ModelLocator.getInstance().hairModel.stylePart );

			return isRightTool;
		}

		private function checkIsInProximity ():Boolean
		{
			if( !dropArea )
				return false;

			var t : MenuToolView;
			t = tools[ toolIndex ] as MenuToolView;

			var p1 : Point;
			p1 = new Point( dropArea.x, dropArea.y );

			var p2 : Point;
			p2 = new Point( t.x, t.y );

			var p3 : Point;
			p3 = p1.subtract( p2 );

			isInProximity = ( p3.length < toolProximity );

			return isInProximity;
		}

		//--

		private function updateToolPosition ():void
		{
			if( ModelLocator.getInstance().ofDataModel.connected )
			{
				var trackerVO : TrackerVO;
				trackerVO = ModelLocator.getInstance().ofDataModel.primaryTrackerVO;

				var area : Rectangle;
				area = ModelLocator.getInstance().hairModel.videoIntRect;

				var tx : int;
				var ty : int;

				tx = trackerVO.rect.x * area.width;
				ty = trackerVO.rect.y * area.height;

				var ease : Number;
				ease = 0.5;

				tool.x += ( tx - tool.x ) * ease;
				tool.y += ( ty - tool.y ) * ease;
			}
			else
			{
				tool.x = _sprite.mouseX;
				tool.y = _sprite.mouseY;
			}

			tool.doValidate();
		}

		private function updateToolScale ():void
		{
			tool.scaleUp( isRightTool && isInProximity );
		}

		private function updateDropArea ():void
		{
			if( !dropArea )
				return;

			if( isInProximity )
			{
				if( isRightTool )
				{
					dropArea.over( true );
					dropArea.showCross( false );
				}
				else
				{
					dropArea.showCross( true );
				}
			}
			else
			{
				dropArea.over( false );
				dropArea.showCross( false );
			}
		}

		/////////////////////////////////////////////
		//	USE TOOL WHILE VIDEO IS PLAYING.
		/////////////////////////////////////////////

		private function playInToolPath ( b : Boolean ):void
		{
			if( toolPathShowing == b )
				return;

			if( toolSelected == -1 )
				return;

			toolPathShowing = b;

			var t : MenuToolView;
			t = tools[ toolSelected ];

			var toolPathWidth	: Number;
			var toolPathHeight	: Number;

			toolPathWidth	= t.toolPathRadiusX * 2;
			toolPathHeight	= t.toolPathRadiusY * 2;

			if( toolPathShowing )
			{
				toolPath.setPath
				(
					t.toolPathCenter.x,
					t.toolPathCenter.y,
					toolPathWidth,
					toolPathHeight
				);

				toolPath.show( true );
			}
			else
			{
				toolPath.show( false );
			}
		}

		private function positionToolAlongPath ():void
		{
			if( toolSelected == -1 )
				return;

			var area : Rectangle;
			area = ModelLocator.getInstance().hairModel.videoIntRect;

			var p1 : Point;
			p1 = new Point( area.width * 0.5, area.height * 0.5 );

			var p2 : Point;
			p2 = new Point();

			var tx : int;
			var ty : int;

			if( ModelLocator.getInstance().ofDataModel.connected )
			{
				var trackerVO : TrackerVO;
				trackerVO = ModelLocator.getInstance().ofDataModel.primaryTrackerVO;

				trace( ModelLocator.getInstance().ofDataModel.focusIndex );

				tx = trackerVO.rect.x * area.width;
				ty = trackerVO.rect.y * area.height;
			}
			else
			{
				tx = _sprite.mouseX;
				ty = _sprite.mouseY;
			}

			p2.x = tx;
			p2.y = ty;

			var p3 : Point;
			p3 = p1.subtract( p2 );

			var a : Number;
			a = Math.atan2( p3.x, p3.y );

			( tools[ toolSelected ] as MenuToolView ).angle = a;
		}

		private function restoreSelectedTool ():void
		{
			if( toolSelected == -1 )
				return;

			var t : MenuToolView;
			t = tools[ toolSelected ];

//			t.rotateBackToNormal();
			t.returnToMenu();
		}

		private function scaleToolForInteractiveVideo ():void
		{
			if( toolSelected == -1 )
				return;

			var t : MenuToolView;
			t = tools[ toolSelected ];

			t.scaleUp( false ); // scaling down here, but could be any scale.
		}

		private function playToolAnim ( b : Boolean ):void
		{
			if( toolSelected == -1 )
				return;

			var t : MenuToolView;
			t = tools[ toolSelected ];

			if( b )
			{
				t.playToolAnim();
			}
			else
			{
				t.stopToolAnim();
			}
		}

		/////////////////////////////////////////////
		//	IDLE SOUND TIMER.
		/////////////////////////////////////////////

		private function initIdleSoundTimer ():void
		{
			if( idleSoundTimer )
				killIdleSoundTimer();

			var time : int;
			time = (int)( Math.random() * 10000 ) + 8000;

			idleSoundTimer = new Timer( time, 1 );
			idleSoundTimer.addEventListener( TimerEvent.TIMER_COMPLETE, idleSoundTimerHandler );
			idleSoundTimer.start();
		}

		private function killIdleSoundTimer ():void
		{
			if( idleSoundTimer )
			{
				idleSoundTimer.removeEventListener( TimerEvent.TIMER_COMPLETE, idleSoundTimerHandler );
				idleSoundTimer.stop();
				idleSoundTimer = null;
			}
		}

		private function idleSoundTimerHandler ( e : TimerEvent ):void
		{
			initIdleSoundTimer();

			if( !bEnabled )
				return;

			if( ModelLocator.getInstance().hairModel.bInteractiveVideoPlaying )
				return;

//			ModelLocator.getInstance().soundModel.playRandomIdle();
			ModelLocator.getInstance().soundModel.playNextIdle();
		}

		/////////////////////////////////////////////
		//	TIMEOUT TIMER.
		/////////////////////////////////////////////

		private function initTimeoutTimer ():void
		{
			if( timeoutTimer )
				killTimeoutTimer();

			if( !bEnabled )
				return;

			if( ModelLocator.getInstance().hairModel.bInteractiveVideoPlaying )
				return;

			var time : int;
			time = ModelLocator.getInstance().hairModel.attractorTimeoutSeconds * 1000;

			timeoutTimer = new Timer( time, 1 );
			timeoutTimer.addEventListener( TimerEvent.TIMER_COMPLETE, timeoutTimerHandler );
			timeoutTimer.start();
		}

		private function killTimeoutTimer ():void
		{
			if( timeoutTimer )
			{
				timeoutTimer.removeEventListener( TimerEvent.TIMER_COMPLETE, timeoutTimerHandler );
				timeoutTimer.stop();
				timeoutTimer = null;
			}
		}

		private function timeoutTimerHandler ( e : TimerEvent ):void
		{
			killTimeoutTimer();

			if( !bEnabled )
				return;

			if( ModelLocator.getInstance().hairModel.bInteractiveVideoPlaying )
				return;

			ModelLocator.getInstance().hairModel.reset();
			ModelLocator.getInstance().hairModel.bAttractor = true;
		}

		/////////////////////////////////////////////
		//	ENABLE.
		/////////////////////////////////////////////

		private function enable ():void
		{
			bEnabled	= true;
			visible		= true;
			doValidate();
		}

		private function disable ():void
		{
			bEnabled	= false;
			visible		= false;
			doValidate();
		}

		/////////////////////////////////////////////
		//	MODEL HANDLERS.
		/////////////////////////////////////////////

		private function girlChangeHandler ( e : GirlEvent ):void
		{
			//
		}

		private function styleChangeHandler ( e : StyleEvent ):void
		{

		}

		private function sectionChangeHandler ( e : SectionEvent ):void
		{
			var girl : int;
			girl	= ModelLocator.getInstance().hairModel.girl;

			var style : int;
			style	= ModelLocator.getInstance().hairModel.style;

			var sec : int;
			sec		= ModelLocator.getInstance().hairModel.section;

			var styleVO : StyleVO;
			styleVO = ModelLocator.getInstance().hairModel.getStyleVO( girl, style );

			if( sec == HairModel.SECTION_PLAY )
			{
				enable();

				playIn( true );
				stylePartChangeHandler();

				initIdleSoundTimer();
				initTimeoutTimer();

				ModelLocator.getInstance().soundModel.playBackground();
			}
			else
			{
				disable();

				ModelLocator.getInstance().hairModel.menuSelection	= -1;
				ModelLocator.getInstance().ofDataModel.focusIndex	= -1;

				stopToolDrag( tools[ toolIndex ] );
				restoreSelectedTool();
				toolSelected = -1;

				selectDropArea( -1 );

				killIdleSoundTimer();
				killTimeoutTimer();

				ModelLocator.getInstance().soundModel.stopAllSounds();
			}
		}

		private function stylePartChangeHandler ( e : StylePartEvent = null ):void
		{
			if( !bEnabled )
				return;

			ModelLocator.getInstance().hairModel.bInteractiveVideoPlaying = false;

//			var stylePart : int;													// MOVED to interactive video, playing in handler.
//			stylePart = ModelLocator.getInstance().hairModel.stylePart;

			stopToolDrag( tools[ toolIndex ] );			// return tool back to menu after interactive video has finished.
			restoreSelectedTool();
			playInToolPath( false );
			playToolAnim( false );
			toolSelected = -1;

			ModelLocator.getInstance().ofDataModel.focusIndex = -1;

//			selectDropArea( stylePart );
			selectToolCover( -1 );
		}

		private function toolChangeHandler ( e : ToolEvent ):void		// this happens when OF tool has changed.
		{
			if( !bEnabled )
				return;

			if( toolIndex >= 0 )
			{
				stopToolDrag( tools[ toolIndex ] );
			}

			toolIndex = e.tool;

			if( toolIndex >= 0 )
			{
				startToolDrag( tools[ toolIndex ] );
			}
		}

		///////////////////////////////////////////////////
		//	MODEL HANDLERS.
		///////////////////////////////////////////////////

		private function dropAreaPlayedInHandler ( e : DropAreaEvent ):void
		{
			//
		}

		private function dropAreaPlayedOutHandler ( e : DropAreaEvent ):void
		{
			ModelLocator.getInstance().hairModel.bInteractiveVideoPlaying = true;

			ModelLocator.getInstance().hairModel.menuSelection = toolSelected;

			selectDropArea( -1 );
			selectToolCover( toolIndex );
			scaleToolForInteractiveVideo();
			playInToolPath( true );
//			playToolAnim( true );
		}

		private function dropAreaChargedHandler ( e : DropAreaEvent ):void
		{
			toolSelected = toolIndex;

			ModelLocator.getInstance().ofDataModel.focusIndex = toolSelected;
		}

		/////////////////////////////////////////////
		//	INTERACTIVE VIDEO HANDLERS.
		/////////////////////////////////////////////

		private function interactiveVideoHandler ( e : InteractiveVideoEvent ):void
		{
			if( e.type == InteractiveVideoEvent.PLAYING_IN )
			{
				var stylePart : int;
				stylePart = ModelLocator.getInstance().hairModel.stylePart;

				selectDropArea( stylePart );
			}

			if( e.type == InteractiveVideoEvent.PLAYING_OUT )
			{
				if( ModelLocator.getInstance().hairModel.stylePart == HairModel.STYLE_PART_THREE )
				{
					playIn( false );
				}
			}
		}

		/////////////////////////////////////////////
		//	TOOL MOUSE CLICK HANDLERS.
		/////////////////////////////////////////////

		private function toolBtnHandler ( e : MouseEvent ):void
		{
			if( ModelLocator.getInstance().ofDataModel.connected )
				return;

			for( var i:int=0; i<tools.length; i++ )
			{
				if( tools[ i ] == e.target )
				{
					toolIndex = i;
					break;
				}
			}

			if( e.type == MouseEvent.MOUSE_DOWN )
			{
				startToolDrag( tools[ toolIndex ] as MenuToolView );
			}

			if( e.type == MouseEvent.MOUSE_UP )
			{
				stopToolDrag( tools[ toolIndex ] as MenuToolView );
			}
		}
	}
}