package com.julapy.ph.makeup.model
{
	import com.holler.assets.AssetLoader;
	import com.julapy.ph.makeup.events.GirlEvent;
	import com.julapy.ph.makeup.events.ModeEvent;
	import com.julapy.ph.makeup.events.SectionEvent;
	import com.julapy.ph.makeup.events.ZoomEvent;
	import com.reintroducing.sound.SoundManager;

	import flash.utils.getQualifiedClassName;

	public class SoundModel
	{
		public static const BLACKOUT				: String = "makeup_18.mp3";

		public static const FINAL_LOOKS_01			: String = "makeup_28.mp3";
		public static const FINAL_LOOKS_02			: String = "makeup_29.mp3";
		public static const FINAL_LOOKS_03			: String = "makeup_30.mp3";
		public static const FINAL_LOOKS_04			: String = "makeup_31.mp3";

		public static const IDLE_DO_SOMETHING_01	: String = "makeup_16.mp3";
		public static const IDLE_DO_SOMETHING_02	: String = "makeup_17.mp3";
		public static const IDLE_DO_SOMETHING_03	: String = "makeup_19.mp3";
		public static const IDLE_DO_SOMETHING_04	: String = "makeup_20.mp3";
		public static const IDLE_DO_SOMETHING_05	: String = "makeup_21.mp3";

		public static const INFO_01					: String = "makeup_12.mp3";
		public static const INFO_02					: String = "makeup_13.mp3";
		public static const INFO_03					: String = "makeup_14.mp3";
		public static const INFO_04					: String = "makeup_15.mp3";

		public static const INTRO_01				: String = "makeup_01.mp3";
		public static const INTRO_02				: String = "makeup_02.mp3";
		public static const INTRO_03				: String = "makeup_03.mp3";
		public static const INTRO_04				: String = "makeup_04.mp3";

		public static const PLAY_INTRO_INSTRUCT_01	: String = "makeup_05.mp3";
		public static const PLAY_INTRO_INSTRUCT_02	: String = "makeup_07.mp3";
		public static const PLAY_INTRO_INSTRUCT_03	: String = "makeup_08.mp3";
		public static const PLAY_INTRO_INSTRUCT_04	: String = "makeup_09.mp3";
		public static const PLAY_INTRO_INSTRUCT_05	: String = "makeup_10.mp3";
		public static const PLAY_INTRO_INSTRUCT_06	: String = "makeup_11.mp3";

		public static const QUICK_COMPLIMENT_01_A	: String = "makeup_32_a.wav";
		public static const QUICK_COMPLIMENT_01_B	: String = "makeup_32_b.wav";
		public static const QUICK_COMPLIMENT_02_A	: String = "makeup_33_a.wav";
		public static const QUICK_COMPLIMENT_02_B	: String = "makeup_33_b.wav";
		public static const QUICK_COMPLIMENT_03_A	: String = "makeup_34_a.wav";
		public static const QUICK_COMPLIMENT_03_B	: String = "makeup_34_b.wav";
		public static const QUICK_COMPLIMENT_04_A	: String = "makeup_35_a.wav";
		public static const QUICK_COMPLIMENT_04_B	: String = "makeup_35_b.wav";
		public static const QUICK_COMPLIMENT_05_A	: String = "makeup_36_a.wav";
		public static const QUICK_COMPLIMENT_05_B	: String = "makeup_36_b.wav";

		public static const REACTIVE_LIPS			: String = "makeup_06.mp3";

		public static const SELECT_ANOTHER_TOOL_01	: String = "makeup_22.mp3";
		public static const SELECT_ANOTHER_TOOL_02	: String = "makeup_23.mp3";
		public static const SELECT_ANOTHER_TOOL_03	: String = "makeup_24.mp3";

		private var finalLooks			: Array = new Array();
		private var idleDoSomethign		: Array = new Array();
		private var info				: Array = new Array();
		private var intro				: Array = new Array();
		private var instructions		: Array = new Array();
		private var compliment			: Array = new Array();
		private var selectAnotherTool	: Array = new Array();

		private var sm 					: SoundManager;
		private var sounds				: Array = new Array();

		public function SoundModel()
		{
			sm = SoundManager.getInstance();

			sounds =
			[
				BLACKOUT,

				FINAL_LOOKS_01,
				FINAL_LOOKS_02,
				FINAL_LOOKS_03,
				FINAL_LOOKS_04,

				IDLE_DO_SOMETHING_01,
				IDLE_DO_SOMETHING_02,
				IDLE_DO_SOMETHING_03,
				IDLE_DO_SOMETHING_04,
				IDLE_DO_SOMETHING_05,

				INFO_01,
				INFO_02,
				INFO_03,
				INFO_04,

				INTRO_01,
				INTRO_02,
				INTRO_03,
				INTRO_04,

				PLAY_INTRO_INSTRUCT_01,
				PLAY_INTRO_INSTRUCT_02,
				PLAY_INTRO_INSTRUCT_03,
				PLAY_INTRO_INSTRUCT_04,
				PLAY_INTRO_INSTRUCT_05,
				PLAY_INTRO_INSTRUCT_06,

				QUICK_COMPLIMENT_01_A,
				QUICK_COMPLIMENT_01_B,
				QUICK_COMPLIMENT_02_A,
				QUICK_COMPLIMENT_02_B,
				QUICK_COMPLIMENT_03_A,
				QUICK_COMPLIMENT_03_B,
				QUICK_COMPLIMENT_04_A,
				QUICK_COMPLIMENT_04_B,
				QUICK_COMPLIMENT_05_A,
				QUICK_COMPLIMENT_05_B,

				REACTIVE_LIPS,

				SELECT_ANOTHER_TOOL_01,
				SELECT_ANOTHER_TOOL_02,
				SELECT_ANOTHER_TOOL_03
			];

			finalLooks =
			[
				FINAL_LOOKS_01,
				FINAL_LOOKS_02,
				FINAL_LOOKS_03,
				FINAL_LOOKS_04
			];

			idleDoSomethign =
			[
				IDLE_DO_SOMETHING_01,
				IDLE_DO_SOMETHING_02,
				IDLE_DO_SOMETHING_03,
				IDLE_DO_SOMETHING_04,
				IDLE_DO_SOMETHING_05
			];

			info =
			[
				INFO_01,
				INFO_02,
				INFO_03,
				INFO_04
			];

			intro =
			[
				INTRO_01,
				INTRO_02,
				INTRO_03,
				INTRO_04
			];

			instructions =
			[
				PLAY_INTRO_INSTRUCT_01,
				PLAY_INTRO_INSTRUCT_02,
				PLAY_INTRO_INSTRUCT_03,
				PLAY_INTRO_INSTRUCT_04,
				PLAY_INTRO_INSTRUCT_05,
				PLAY_INTRO_INSTRUCT_06
			];

			compliment =
			[
				QUICK_COMPLIMENT_01_A,
				QUICK_COMPLIMENT_01_B,
				QUICK_COMPLIMENT_02_A,
				QUICK_COMPLIMENT_02_B,
				QUICK_COMPLIMENT_03_A,
				QUICK_COMPLIMENT_03_B,
				QUICK_COMPLIMENT_04_A,
				QUICK_COMPLIMENT_04_B,
				QUICK_COMPLIMENT_05_A,
				QUICK_COMPLIMENT_05_B
			];

			selectAnotherTool =
			[
				SELECT_ANOTHER_TOOL_01,
				SELECT_ANOTHER_TOOL_02,
				SELECT_ANOTHER_TOOL_03
			];
		}

		//////////////////////////////////////////////
		//	INIT.
		//////////////////////////////////////////////

		public function init ():void
		{
			for( var i:int=0; i<sounds.length; i++ )
			{
				var soundID : String;
				soundID = sounds[ i ];

				var SoundClass : Class;
				SoundClass = AssetLoader.getInstance().getClassDefinitionByName( soundID );

				trace( getQualifiedClassName( this ) + " :: loading sound :: " + soundID  );

				sm.addLibrarySound( SoundClass, soundID );
			}

			ModelLocator.getInstance().makeupModel.addEventListener( GirlEvent.GIRL_CHANGE,			girlChangeHandler );
			ModelLocator.getInstance().makeupModel.addEventListener( SectionEvent.SECTION_CHANGE,	sectionChangeHandler );
			ModelLocator.getInstance().makeupModel.addEventListener( ModeEvent.MODE_ZOOM_IN,		modeHandler );
			ModelLocator.getInstance().makeupModel.addEventListener( ModeEvent.MODE_ZOOM_OUT,		modeHandler );
			ModelLocator.getInstance().makeupModel.addEventListener( ModeEvent.MODE_ANIM_IN,		modeHandler );
			ModelLocator.getInstance().makeupModel.addEventListener( ZoomEvent.ZOOM,				zoomHandler );
		}

		//////////////////////////////////////////////
		//	HANDLERS.
		//////////////////////////////////////////////

		private function girlChangeHandler ( e : GirlEvent ):void
		{
			//
		}

		private function sectionChangeHandler ( e : SectionEvent ):void
		{
			if( e.section == MakeupModel.SECTION_INTRO )
			{
				//
			}

			if( e.section == MakeupModel.SECTION_PLAY )
			{
				playRandomInstructions();
			}

			if( e.section == MakeupModel.SECTION_OUTRO )
			{
				//
			}
		}

		private function modeHandler ( e : ModeEvent ):void
		{
			if( e.type == ModeEvent.MODE_ZOOM_IN )
			{
				//
			}

			if( e.type == ModeEvent.MODE_ANIM_IN )
			{
				//
			}

			if( e.type == ModeEvent.MODE_ZOOM_OUT )
			{
				playRandomCompliment();

				if( e.mode == MakeupModel.EYES_MODE )
				{
					//
				}

				if( e.mode == MakeupModel.FACE_MODE )
				{
					//
				}

				if( e.mode == MakeupModel.LIPS_MODE )
				{
					//
				}
			}
		}

		private function zoomHandler ( e : ZoomEvent ):void
		{
			//
		}

		//////////////////////////////////////////////
		//	GENERIC.
		//////////////////////////////////////////////

		public function play ( soundID : String ):void
		{
			sm.playSound( soundID );
		}

		public function stopAllSounds ():void
		{
			sm.stopAllSounds();
		}

		//////////////////////////////////////////////
		//	PLAY RANDOM.
		//////////////////////////////////////////////

		public function playRandomFinalLooks		():void		{	playRandom( finalLooks );			}
		public function playRandomIdle				():void		{	playRandom( idleDoSomethign );		}
		public function playRandomInfo				():void		{	playRandom( info );					}
		public function playRandomIntro				():void		{	playRandom( intro );				}
		public function playRandomInstructions		():void		{	playRandom( instructions );			}
		public function playRandomCompliment		():void		{	playRandom( compliment );			}
		public function playRandomSelectAnotherTool	():void		{	playRandom( selectAnotherTool );	}

		private function playRandom ( a : Array ):void
		{
			var i : int;
			i = (int)( Math.random() * a.length );

			sm.playSound( a[ i ] );
		}
	}
}