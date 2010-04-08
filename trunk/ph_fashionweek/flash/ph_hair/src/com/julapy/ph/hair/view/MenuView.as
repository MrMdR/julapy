package com.julapy.ph.hair.view
{
	import caurina.transitions.Tweener;

	import com.holler.controls.BtnView;
	import com.holler.core.View;
	import com.julapy.ph.hair.events.GirlEvent;
	import com.julapy.ph.hair.events.SectionEvent;
	import com.julapy.ph.hair.events.StyleEvent;
	import com.julapy.ph.hair.events.StylePartEvent;
	import com.julapy.ph.hair.model.HairModel;
	import com.julapy.ph.hair.model.ModelLocator;
	import com.julapy.ph.hair.vo.StyleVO;

	import fl.motion.easing.Quadratic;

	import flash.display.MovieClip;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.geom.Point;

	public class MenuView extends View
	{
		private var dropAreaDryer	: MovieClip;
		private var dropAreaCurlers	: MovieClip;
		private var dropAreaSpray	: MovieClip;
		private var dropArea		: MovieClip;
		private var dropAreas		: Array = new Array();

		private var toolDryer		: MovieClip;
		private var toolCurler		: MovieClip;
		private var toolSpray		: MovieClip;
		private var toolDryerBtn	: BtnView;
		private var toolCurlerBtn	: BtnView;
		private var toolSprayBtn	: BtnView;
		private var tool			: MovieClip;
		private var tools			: Array = new Array();
		private var toolBtns		: Array = new Array();
		private var toolIndex		: int = -1;

		public function MenuView(sprite:Sprite=null)
		{
			super(sprite);

			dropAreaDryer	= _sprite.getChildByName( "dropAreaDryer" ) as MovieClip;
			dropAreaCurlers	= _sprite.getChildByName( "dropAreaCurlers" ) as MovieClip;
			dropAreaSpray	= _sprite.getChildByName( "dropAreaSpray" ) as MovieClip;

			dropAreas		=
			[
				dropAreaDryer,
				dropAreaCurlers,
				dropAreaSpray
			];

			selectDropArea( -1 );

			//--

			toolDryer		= _sprite.getChildByName( "toolDryer" ) as MovieClip;
			toolCurler		= _sprite.getChildByName( "toolCurler" ) as MovieClip;
			toolSpray		= _sprite.getChildByName( "toolSpray" ) as MovieClip;

			tools 			=
			[
				toolDryer,
				toolCurler,
				toolSpray
			];

			for( var i:int=0; i<tools.length; i++ )
			{
				var t : MovieClip;
				t = tools[ i ] as MovieClip;

				t.ix = t.x;
				t.iy = t.y;

				t.sx = t.scaleX;
				t.sy = t.scaleY;
			}

			//--

			toolDryerBtn	= new BtnView( toolDryer );
			toolDryerBtn.addEventListener( MouseEvent.MOUSE_DOWN,	toolBtnHandler );
			toolDryerBtn.addEventListener( MouseEvent.MOUSE_UP,		toolBtnHandler );

			toolCurlerBtn	= new BtnView( toolCurler );
			toolCurlerBtn.addEventListener( MouseEvent.MOUSE_DOWN,	toolBtnHandler );
			toolCurlerBtn.addEventListener( MouseEvent.MOUSE_UP,	toolBtnHandler );

			toolSprayBtn	= new BtnView( toolSpray );
			toolSprayBtn.addEventListener( MouseEvent.MOUSE_DOWN, 	toolBtnHandler );
			toolSprayBtn.addEventListener( MouseEvent.MOUSE_UP, 	toolBtnHandler );

			toolBtns 		=
			[
				toolDryerBtn,
				toolCurlerBtn,
				toolSprayBtn
			];

			//--

			ModelLocator.getInstance().hairModel.addEventListener( GirlEvent.GIRL_CHANGE,				girlChangeHandler );
			ModelLocator.getInstance().hairModel.addEventListener( StyleEvent.STYLE_CHANGE,				styleChangeHandler );
			ModelLocator.getInstance().hairModel.addEventListener( SectionEvent.SECTION_CHANGE,			sectionChangeHandler );
			ModelLocator.getInstance().hairModel.addEventListener( StylePartEvent.STYLE_PART_CHANGE,	stylePartChangeHandler );
		}

		/////////////////////////////////////////////
		//	DROP AREAS.
		/////////////////////////////////////////////

		private function selectDropArea ( id : int ):void
		{
			for( var i:int=0; i<dropAreas.length; i++ )
			{
				if( i == id )
				{
					dropArea = dropAreas[ i ] as MovieClip;
				}

				( dropAreas[ i ] as MovieClip ).visible = ( i == id ) ? true : false;
			}
		}

		/////////////////////////////////////////////
		//	TOOLS.
		/////////////////////////////////////////////

		private function startToolDrag ( m : MovieClip ):void
		{
			tool = m;

			_sprite.addEventListener( Event.ENTER_FRAME, enterFrameHandler );
		}

		private function stopToolDrag ( m : MovieClip ):void
		{
			_sprite.removeEventListener( Event.ENTER_FRAME, enterFrameHandler );

			Tweener.addTween
			(
				m,
				{
					x			: m.ix,
					y			: m.iy,
					scaleX		: m.sx,
					scaleY		: m.sy,
					time		: 0.3,
					delay		: 0.0,
					transition	: Quadratic.easeOut,
					onStart		: null,
					onUpdate	: null,
					onComplete	: null
				}
			);

			if( checkToolMatch() )
			{
				ModelLocator.getInstance().hairModel.nextStylePart();
			}
		}

		private function enterFrameHandler ( e : Event ):void
		{
			tool.x = _sprite.mouseX;
			tool.y = _sprite.mouseY;

			if( checkToolMatch() )
			{
				tool.scaleX = 1;
				tool.scaleY = 1;
			}
			else
			{
				tool.scaleX = tool.sx;
				tool.scaleY = tool.sy;
			}
		}

		private function checkToolMatch ():Boolean
		{
			var t : MovieClip;
			t = tools[ toolIndex ] as MovieClip;

			var isRightTool : Boolean;
			isRightTool = ( toolIndex == ModelLocator.getInstance().hairModel.stylePart );

			if( !isRightTool )
			{
				return false;
			}

			var p1 : Point;
			p1 = new Point( dropArea.x, dropArea.y );

			var p2 : Point;
			p2 = new Point( t.x, t.y );

			var p3 : Point;
			p3 = p1.subtract( p2 );

			var isInProximity : Boolean;
			isInProximity = ( p3.length < 50 );

			return isInProximity;
		}

		/////////////////////////////////////////////
		//	ENABLE.
		/////////////////////////////////////////////

		private function enable ():void
		{
			visible = true;
			doValidate();
		}

		private function disable ():void
		{
			visible = false;
			doValidate();
		}

		/////////////////////////////////////////////
		//	HANDLERS.
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
			}
			else
			{
				disable();
			}
		}

		private function stylePartChangeHandler ( e : StylePartEvent ):void
		{
			var stylePart : int;
			stylePart = e.stylePart;

			selectDropArea( stylePart );
		}

		//--

		private function toolBtnHandler ( e : MouseEvent ):void
		{
			for( var i:int=0; i<toolBtns.length; i++ )
			{
				if( toolBtns[ i ] == e.target )
				{
					toolIndex = i;
					break;
				}
			}

			if( e.type == MouseEvent.MOUSE_DOWN )
			{
				startToolDrag( tools[ i ] as MovieClip );
			}

			if( e.type == MouseEvent.MOUSE_UP )
			{
				stopToolDrag( tools[ i ] as MovieClip );
			}
		}
	}
}