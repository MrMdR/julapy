package com.julapy.ph.hair.model
{
	import com.holler.assets.AssetLoader;
	import com.reintroducing.sound.SoundManager;

	public class SoundModel
	{

		public static const BLACKOUT		: String = "hair.sound.hair_blackout_1";

		public static const FINAL_LOOKS_0	: String = "hair.sound.hair_final_looks_0";
		public static const FINAL_LOOKS_1	: String = "hair.sound.hair_final_looks_1";
		public static const FINAL_LOOKS_5	: String = "hair.sound.hair_final_looks_5";
		public static const FINAL_LOOKS_10	: String = "hair.sound.hair_final_looks_10";

		public static const IDLE_1			: String = "hair.sound.hair_idle_1";
		public static const IDLE_2			: String = "hair.sound.hair_idle_2";
		public static const IDLE_3			: String = "hair.sound.hair_idle_3";
		public static const IDLE_4			: String = "hair.sound.hair_idle_4";

		public static const PLAY_1_DRYER	: String = "hair.sound.hair_play_1_dryer";
		public static const PLAY_2_CURLER	: String = "hair.sound.hair_play_2_curler";
		public static const PLAY_3_SPRAY	: String = "hair.sound.hair_play_3_spray";
		public static const PLAY_3_SPRAY_B	: String = "hair.sound.hair_play_3_spray_compliment";

		public static const PLAY_INTRO_1_A	: String = "hair.sound.hair_play_intro_1a";
		public static const PLAY_INTRO_1_B	: String = "hair.sound.hair_play_intro_1b";
		public static const PLAY_INTRO_2	: String = "hair.sound.hair_play_intro_2_dryer";
		public static const PLAY_INTRO_3	: String = "hair.sound.hair_play_intro_3_curler";

		public static const TOOL_SELECTED_1	: String = "hair.sound.hair_tool_selected_1_dryer";
		public static const TOOL_SELECTED_2	: String = "hair.sound.hair_tool_selected_2_curler";
		public static const TOOL_SELECTED_3	: String = "hair.sound.hair_tool_selected_3_spray";

		private var sm 		: SoundManager;
		private var sounds	: Array = new Array();

		public function SoundModel()
		{
			sm = SoundManager.getInstance();

			sounds =
			[
				BLACKOUT,

				FINAL_LOOKS_0,
				FINAL_LOOKS_1,
				FINAL_LOOKS_5,
				FINAL_LOOKS_10,

				IDLE_1,
				IDLE_2,
				IDLE_3,
				IDLE_4,

				PLAY_1_DRYER,
				PLAY_2_CURLER,
				PLAY_3_SPRAY,
				PLAY_3_SPRAY_B,

				PLAY_INTRO_1_A,
				PLAY_INTRO_1_B,
				PLAY_INTRO_2,
				PLAY_INTRO_3,

				TOOL_SELECTED_1,
				TOOL_SELECTED_2,
				TOOL_SELECTED_3
			]
		}

		public function init ():void
		{
			for( var i:int=0; i<sounds.length; i++ )
			{
				var soundID : String;
				soundID = sounds[ i ];

				var SoundClass : Class;
				SoundClass = AssetLoader.getInstance().getClassDefinitionByName( soundID );

				sm.addLibrarySound( SoundClass, soundID );
			}
		}

		public function stopAllSounds ():void
		{
			sm.stopAllSounds();
		}

		/////////////////////////////////////////////////
		//
		/////////////////////////////////////////////////

		public function playInteractiveInstruction ( i : int ):void
		{
			if( i == 0 )
			{
				if( Math.random() > 0.5 )
					sm.playSound( PLAY_INTRO_1_A );
				else
					sm.playSound( PLAY_INTRO_1_B );
			}
			else if( i == 1 )
			{
				sm.playSound( PLAY_INTRO_2 );
			}
			else if( i == 2 )
			{
				sm.playSound( PLAY_INTRO_3 );
			}
		}

		public function playInteractiveToolSelected ( i : int ):void
		{
			if( i == 0 )
			{
				sm.playSound( TOOL_SELECTED_1 );
			}
			else if( i == 1 )
			{
				sm.playSound( TOOL_SELECTED_2 );
			}
			else if( i == 2 )
			{
				sm.playSound( TOOL_SELECTED_3 );
			}
		}

		public function playInteractiveInfo ( i : int ):void
		{
			if( i == 0 )
			{
				sm.playSound( PLAY_1_DRYER );
			}
			else if( i == 1 )
			{
				sm.playSound( PLAY_2_CURLER );
			}
			else if( i == 2 )
			{
				if( Math.random() > 0.5 )
					sm.playSound( PLAY_3_SPRAY );
				else
					sm.playSound( PLAY_3_SPRAY_B );
			}
		}

		public function playFinalLook ( i : int ):void
		{
			if( i == 0 )
			{
				sm.playSound( FINAL_LOOKS_10 );
			}
			else if( i == 1 )
			{
				sm.playSound( FINAL_LOOKS_5 );
			}
			else if( i == 2 )
			{
				sm.playSound( FINAL_LOOKS_0 );
			}
			else if( i == 3 )
			{
				sm.playSound( FINAL_LOOKS_1 );
			}
		}

	}
}