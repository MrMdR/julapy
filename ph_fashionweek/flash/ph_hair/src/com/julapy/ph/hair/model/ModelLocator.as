package com.julapy.ph.hair.model
{

	public class ModelLocator
	{
		private static var instance : ModelLocator;

		public var hairModel	: HairModel;

		public function ModelLocator()
		{
			if ( instance != null )
			{
				throw new Error( "ModelLocator must be a singleton" );
			}

			instance	= this;
			hairModel	= new HairModel();
		}

		public static function getInstance() : ModelLocator
		{
			if ( instance == null )
				instance = new ModelLocator();

			return instance;
		}
	}
}