package com.julapy.ph.makeup.model
{
	import com.julapy.ph.model.CommonModel;

	public class ModelLocator
	{
		private static var instance : ModelLocator;

		public var commondModel	: CommonModel;
		public var makeupModel	: MakeupModel;
		public var ofDataModel	: OFDataMakeupModel;
		public var soundModel	: SoundModel;

		public function ModelLocator()
		{
			if ( instance != null )
			{
				throw new Error( "ModelLocator must be a singleton" );
			}

			instance		= this;
			commondModel	= new CommonModel();
			makeupModel		= new MakeupModel();
			ofDataModel		= new OFDataMakeupModel();
			soundModel		= new SoundModel();
		}

		public static function getInstance() : ModelLocator
		{
			if ( instance == null )
				instance = new ModelLocator();

			return instance;
		}
	}
}