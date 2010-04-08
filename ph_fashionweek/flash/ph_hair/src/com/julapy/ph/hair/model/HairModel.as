package com.julapy.ph.hair.model
{
	import com.julapy.ph.hair.events.GirlEvent;
	import com.julapy.ph.hair.events.SectionEvent;
	import com.julapy.ph.hair.events.StyleEvent;
	import com.julapy.ph.hair.events.StylePartEvent;
	import com.julapy.ph.hair.vo.GirlVO;
	import com.julapy.ph.hair.vo.StyleVO;

	import flash.events.EventDispatcher;


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

		public function HairModel()
		{
			_girlOneVO = girlOneVO;
			_girlTwoVO = girlTwoVO;

			_girlVOs.push( _girlOneVO );
			_girlVOs.push( _girlTwoVO );
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

			var s1 : StyleVO;
			s1 = new StyleVO();
			s1.intro	= "makeup.video.01";
			s1.outro	= "makeup.video.02";
			s1.sections	=
			[
				"makeup.ivideo.g1.s1.01",
				"makeup.ivideo.g1.s1.02",
				"makeup.ivideo.g1.s1.03"
			];

			var s2 : StyleVO;
			s2 = new StyleVO();
			s2.intro	= "makeup.video.03";
			s2.outro	= "makeup.video.04";
			s2.sections	=
			[
				"makeup.ivideo.g1.s2.01",
				"makeup.ivideo.g1.s2.02",
				"makeup.ivideo.g1.s2.03"
			];

			vo.styles =
			[
				s1, s2
			];

			return vo;
		}

		private function get girlTwoVO ():GirlVO
		{
			var vo : GirlVO;
			vo = new GirlVO();

			var s1 : StyleVO;
			s1 = new StyleVO();
			s1.intro	= "makeup.video.05";
			s1.outro	= "makeup.video.06";
			s1.sections	=
			[
				"makeup.ivideo.g2.s1.01",
				"makeup.ivideo.g2.s1.02",
				"makeup.ivideo.g2.s1.03"
			];

			var s2 : StyleVO;
			s2 = new StyleVO();
			s2.intro	= "makeup.video.07";
			s2.outro	= "makeup.video.08";
			s2.sections	=
			[
				"makeup.ivideo.g2.s2.01",
				"makeup.ivideo.g2.s2.02",
				"makeup.ivideo.g2.s2.03"
			];

			vo.styles =
			[
				s1, s2
			];

			return vo;
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
	}
}