package com.julapy.ph.hair.model
{
	import com.julapy.ph.hair.events.DropAreaEvent;
	import com.julapy.ph.hair.events.GirlEvent;
	import com.julapy.ph.hair.events.InteractiveVideoEvent;
	import com.julapy.ph.hair.events.MenuEvent;
	import com.julapy.ph.hair.events.SectionEvent;
	import com.julapy.ph.hair.events.StyleEvent;
	import com.julapy.ph.hair.events.StylePartEvent;
	import com.julapy.ph.hair.events.ToolEvent;
	import com.julapy.ph.hair.events.ToolPathEvent;
	import com.julapy.ph.hair.vo.GirlVO;
	import com.julapy.ph.hair.vo.StyleVO;

	import flash.events.EventDispatcher;
	import flash.geom.Point;
	import flash.geom.Rectangle;


	public class HairModel extends EventDispatcher
	{
		public static const GIRL_ONE			: int = 0;
		public static const GIRL_TWO			: int = 1;

		public static const STYLE_ONE			: int = 0;
		public static const STYLE_TWO			: int = 1;

		public static const SECTION_INTRO		: int = 0;
		public static const SECTION_PLAY		: int = 1;
		public static const SECTION_OUTRO		: int = 2;

		public static const STYLE_PART_ONE		: int = 0;
		public static const STYLE_PART_TWO		: int = 1;
		public static const STYLE_PART_THREE	: int = 2;

		public static const TOOL_DRYER			: int = 0;
		public static const TOOL_CURLER			: int = 1;
		public static const TOOL_SPRAY			: int = 2;

		private var _videoGenRect	: Rectangle = new Rectangle( 0, 0, 576, 324 );
		private var _videoIntRect	: Rectangle = new Rectangle( 0, 0, 576, 768 );

		private var _girls			: Array = [ GIRL_ONE, GIRL_TWO ];
		private var _styles			: Array = [ STYLE_ONE, STYLE_TWO ];
		private var _sections		: Array = [ SECTION_INTRO, SECTION_PLAY, SECTION_OUTRO ];
		private var _styleParts		: Array = [ STYLE_PART_ONE, STYLE_PART_TWO, STYLE_PART_THREE ];

		private var _girl			: int = -1;
		private var _section		: int = -1;
		private var _style			: int = -1;
		private var _stylePart		: int = -1;

		private var _girlVOs		: Array = new Array();
		private var _girlOneVO		: GirlVO;
		private var _girlTwoVO		: GirlVO;

		private var _menuSelection	: int = -1;

		private var _tool			: int = -1;

		public function HairModel()
		{
			_girlOneVO = girlOneVO;
			_girlTwoVO = girlTwoVO;

			_girlVOs.push( _girlOneVO );
			_girlVOs.push( _girlTwoVO );
		}

		/////////////////////////////////////
		//	VIDEO RECT.
		/////////////////////////////////////

		public function get videoGenRect ():Rectangle
		{
			return _videoGenRect.clone();
		}

		public function get videoIntRect ():Rectangle
		{
			return _videoIntRect.clone();
		}

		/////////////////////////////////////
		//	VOs.
		/////////////////////////////////////

		public function getGirlVO ( girlID : int ):GirlVO
		{
			return _girlVOs[ girlID ];
		}

		public function getStyleVO ( girlID : int, styleID : int ):StyleVO
		{
			return ( _girlVOs[ girlID ] as GirlVO ).styles[ styleID ] as StyleVO;
		}

		//--

		private function get girlOneVO ():GirlVO
		{
			var vo : GirlVO;
			vo = new GirlVO();
			vo.styles =	[ g1s1_vo, g2s1_vo ];
			return vo;
		}

		private function get girlTwoVO ():GirlVO
		{
			var vo : GirlVO;
			vo = new GirlVO();
			vo.styles = [ g1s2_vo, g2s2_vo ];
			return vo;
		}

		private function get g1s1_vo ():StyleVO
		{
			var styleVO : StyleVO;
			styleVO = new StyleVO();
			styleVO.introTimeline	= "makeup.video.01";
			styleVO.outroTimeline	= "makeup.video.02";
			styleVO.introStream		= "";
			styleVO.outroStream		= "flv/hairmodel01look01outro.f4v";
			styleVO.playParts		=
			[
				"makeup.ivideo.g1.s1.01",
				"makeup.ivideo.g1.s1.02",
				"makeup.ivideo.g1.s1.03"
			];
			styleVO.playStreams	=
			[
				"flv/g1_s1_p1.f4v",
				"flv/g1_s1_p2.f4v",
				"flv/g1_s1_p3.f4v"
			];

			return styleVO;
		}

		private function get g1s2_vo ():StyleVO
		{
			var styleVO : StyleVO;
			styleVO = new StyleVO();
			styleVO.introTimeline	= "makeup.video.03";
			styleVO.outroTimeline	= "makeup.video.04";
			styleVO.introStream		= "flv/hairmodel01look02intro.f4v";
			styleVO.outroStream		= "flv/hairmodel01look02outro.f4v";
			styleVO.playParts		=
			[
				"makeup.ivideo.g1.s2.01",
				"makeup.ivideo.g1.s2.02",
				"makeup.ivideo.g1.s2.03"
			];
			styleVO.playStreams	=
			[
				"flv/g1_s2_p1.f4v",
				"flv/g1_s2_p2.f4v",
				"flv/g1_s2_p3.f4v"
			];

			return styleVO;
		}

		private function get g2s1_vo ():StyleVO
		{
			var styleVO : StyleVO;
			styleVO = new StyleVO();
			styleVO.introTimeline	= "makeup.video.05";
			styleVO.outroTimeline	= "makeup.video.06";
			styleVO.introStream		= "flv/hairmodel02look01intro.f4v";
			styleVO.outroStream		= "flv/hairmodel02look01outro.f4v";
			styleVO.playParts		=
			[
				"makeup.ivideo.g2.s1.01",
				"makeup.ivideo.g2.s1.02",
				"makeup.ivideo.g2.s1.03"
			];
			styleVO.playStreams	=
			[
				"flv/g2_s1_p1.f4v",
				"flv/g2_s1_p2.f4v",
				"flv/g2_s1_p3.f4v"
			];

			return styleVO;
		}

		private function get g2s2_vo ():StyleVO
		{
			var styleVO : StyleVO;
			styleVO = new StyleVO();
			styleVO.introTimeline	= "makeup.video.07";
			styleVO.outroTimeline	= "makeup.video.08";
			styleVO.introStream		= "flv/hairmodel02look02intro.f4v";
			styleVO.outroStream		= "flv/hairmodel02look02outro.f4v";
			styleVO.playParts		=
			[
				"makeup.ivideo.g2.s2.01",
				"makeup.ivideo.g2.s2.02",
				"makeup.ivideo.g2.s2.03"
			];
			styleVO.playStreams	=
			[
				"flv/g2_s2_p1.f4v",
				"flv/g2_s2_p2.f4v",
				"flv/g2_s2_p3.f4v"
			];

			return styleVO;
		}

		/////////////////////////////////////
		//	GIRL.
		/////////////////////////////////////

		public function set girl ( value : int ):void
		{
			if( _girl != value )
			{
				_girl = value;

				dispatchEvent( new GirlEvent( _girl ) );
			}
		}

		public function get girl ():int
		{
			return _girl;
		}

		public function nextGirl ():void
		{
			var girlTemp : int;
			girlTemp = _girl;

			if( ++girlTemp >= _girls.length )
			{
				girl = 0;

				return;
			}

			girl = girlTemp;
		}

		/////////////////////////////////////
		//	STYLE.
		/////////////////////////////////////

		public function set style ( value : int ):void
		{
			if( _style != value )
			{
				_style = value;

				dispatchEvent( new StyleEvent( _style ) );
			}
		}

		public function get style ():int
		{
			return _style;
		}

		public function nextStyle ():void
		{
			var styleTemp : int;
			styleTemp = _style;

			if( ++styleTemp >= _styles.length )
			{
				nextGirl();
				style = 0;

				return;
			}

			style = styleTemp;
		}

		/////////////////////////////////////
		//	SECTION.
		/////////////////////////////////////

		public function set section ( value : int ):void
		{
			if( _section != value )
			{
				_section = value;

				dispatchEvent( new SectionEvent( _section ) );
			}
		}

		public function get section ():int
		{
			return _section;
		}

		public function nextSection ():void
		{
			var sectionTemp : int;
			sectionTemp = _section;

			if( ++sectionTemp >= _sections.length )
			{
				nextStyle();
				section = 0;

				return;
			}

			section = sectionTemp;
		}

		/////////////////////////////////////
		//	STYLE PART.
		/////////////////////////////////////

		public function set stylePart ( value : int ):void
		{
			if( _stylePart != value )
			{
				_stylePart = value;

				dispatchEvent( new StylePartEvent( _stylePart ) );
			}
		}

		public function get stylePart ():int
		{
			return _stylePart;
		}

		public function nextStylePart ():void
		{
			var stylePartTemp : int;
			stylePartTemp = _stylePart;

			if( ++stylePartTemp >= _styleParts.length )
			{
				nextSection();
				stylePart = 0;

				return;
			}

			stylePart = stylePartTemp;
		}

		/////////////////////////////////////
		//	MENU SELECTION.
		/////////////////////////////////////

		public function set menuSelection ( value : int ):void
		{
			if( _menuSelection != value )
			{
				_menuSelection = value;

				dispatchEvent( new MenuEvent( _menuSelection ) );
			}
		}

		public function get menuSelection ():int
		{
			return _menuSelection;
		}

		/////////////////////////////////////
		//	TOOL.
		/////////////////////////////////////

		public function set tool ( value : int ):void
		{
			if( _tool != value )
			{
				_tool = value;

				dispatchEvent( new ToolEvent( _tool ) );
			}
		}

		public function get tool ():int
		{
			return _tool;
		}

		/////////////////////////////////////
		//	DROP AREA.
		/////////////////////////////////////

		public function dropAreaCharged ():void
		{
			dispatchEvent( new DropAreaEvent( DropAreaEvent.DROP_AREA_CHARGED ) );
		}

		public function dropAreaPlayedIn ():void
		{
			dispatchEvent( new DropAreaEvent( DropAreaEvent.DROP_AREA_PLAYED_IN ) );
		}

		public function dropAreaPlayedOut ():void
		{
			dispatchEvent( new DropAreaEvent( DropAreaEvent.DROP_AREA_PLAYED_OUT ) );
		}

		/////////////////////////////////////
		//	TOOL PATH
		/////////////////////////////////////

		public function updateToolPathPosition ( p : Point ):void
		{
			dispatchEvent( new ToolPathEvent( p.clone() ) );
		}

		/////////////////////////////////////
		//	INTERACTIVE VIDEO
		/////////////////////////////////////

		public function interactiveVideoPlayingIn ():void
		{
			dispatchEvent( new InteractiveVideoEvent( InteractiveVideoEvent.PLAYING_IN ) );
		}

		public function interactiveVideoPlayedIn ():void
		{
			dispatchEvent( new InteractiveVideoEvent( InteractiveVideoEvent.PLAYED_IN ) );
		}

		public function interactiveVideoPlayingOut ():void
		{
			dispatchEvent( new InteractiveVideoEvent( InteractiveVideoEvent.PLAYING_OUT ) );
		}

		public function interactiveVideoPlayedOut ():void
		{
			dispatchEvent( new InteractiveVideoEvent( InteractiveVideoEvent.PLAYED_OUT ) );
		}
	}
}