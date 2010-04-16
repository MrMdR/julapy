package com.julapy.ph.hair.model
{
	import com.holler.assets.AssetLoader;
	import com.reintroducing.sound.SoundManager;

	public class SoundModel
	{
		public static const GIRL_1_STYLE_1_INTRO	: String = "balspheming_barry.mp3";
		public static const GIRL_1_STYLE_1_OUTRO	: String = "being_blessed.mp3";

		public static const GIRL_1_STYLE_2_INTRO	: String = "fudu_fa_fa.mp3";
		public static const GIRL_1_STYLE_2_OUTRO	: String = "hortifkinculturist.mp3";

		public static const GIRL_2_STYLE_1_INTRO	: String = "i_was_guided.mp3";
		public static const GIRL_2_STYLE_1_OUTRO	: String = "its_been_emotional.mp3";

		public static const GIRL_2_STYLE_2_INTRO	: String = "ive_been_shot.mp3";
		public static const GIRL_2_STYLE_2_OUTRO	: String = "balspheming_barry.mp3";

		private var sm 		: SoundManager;
		private var sounds	: Array = new Array();

		public function SoundModel()
		{
			sm = SoundManager.getInstance();

			sounds =
			[
				GIRL_1_STYLE_1_INTRO,
				GIRL_1_STYLE_1_OUTRO,

				GIRL_1_STYLE_2_INTRO,
				GIRL_1_STYLE_2_OUTRO,

				GIRL_2_STYLE_1_INTRO,
				GIRL_2_STYLE_1_OUTRO,

				GIRL_2_STYLE_2_INTRO,
				GIRL_2_STYLE_2_OUTRO
			]
		}

		public function init ():void
		{
			for( var i:int=0; i<sounds.length; i++ )
			{
				var soundID : String;
				soundID = sounds[ i ];

				var SoundClass : Class;
				SoundClass = AssetLoader.getInstance().getClassDefinitionByName( soundID )

//				sm.addLibrarySound( SoundClass, soundID );
			}
		}

		public function playGeneralVideoSound ( girl : int, section : int, style : int ):void
		{
			return;

			if( girl == HairModel.GIRL_ONE )
			{
				if( section == HairModel.SECTION_INTRO )
				{
					if( style == HairModel.STYLE_ONE )
					{
						sm.playSound( GIRL_1_STYLE_1_INTRO );
					}

					if( style == HairModel.STYLE_TWO )
					{
						sm.playSound( GIRL_1_STYLE_2_INTRO );
					}
				}

				if( section == HairModel.SECTION_OUTRO )
				{
					if( style == HairModel.STYLE_ONE )
					{
						sm.playSound( GIRL_1_STYLE_1_OUTRO );
					}

					if( style == HairModel.STYLE_TWO )
					{
						sm.playSound( GIRL_1_STYLE_2_OUTRO );
					}
				}
			}

			if( girl == HairModel.GIRL_TWO )
			{
				if( section == HairModel.SECTION_INTRO )
				{
					if( style == HairModel.STYLE_ONE )
					{
						sm.playSound( GIRL_2_STYLE_1_INTRO );
					}

					if( style == HairModel.STYLE_TWO )
					{
						sm.playSound( GIRL_2_STYLE_2_INTRO );
					}
				}

				if( section == HairModel.SECTION_OUTRO )
				{
					if( style == HairModel.STYLE_ONE )
					{
						sm.playSound( GIRL_2_STYLE_1_OUTRO );
					}

					if( style == HairModel.STYLE_TWO )
					{
						sm.playSound( GIRL_2_STYLE_2_OUTRO );
					}
				}
			}
		}

		public function stopAllSounds ():void
		{
			sm.stopAllSounds();
		}
	}
}