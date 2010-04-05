package com.julapy.ph.hair.model
{
	import com.julapy.ph.model.CommonModel;

	public class ModelLocator
	{
		private static var instance : ModelLocator;

		public var commonModel	: CommonModel;
		public var hairModel	: HairModel;
		public var ofDataModel	: OFDataHairModel;

		public function ModelLocator()
		{
			if ( instance != null )
			{
				throw new Error( "ModelLocator must be a singleton" );
			}

			instance	= this;

			commonModel	= new CommonModel();
			hairModel	= new HairModel();
			ofDataModel	= new OFDataHairModel();
		}

		public static function getInstance() : ModelLocator
		{
			if ( instance == null )
				instance = new ModelLocator();

			return instance;
		}
	}
}