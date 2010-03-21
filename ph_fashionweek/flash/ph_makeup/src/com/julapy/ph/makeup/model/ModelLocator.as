package com.julapy.ph.makeup.model
{
	import com.holler.vo.AbstractScreenVO;

	public class ModelLocator
	{
		private static var instance : ModelLocator;

		public var makeupModel : MakeupModel;
		public var ofDataModel : OFDataModel;

		public function ModelLocator()
		{
			if ( instance != null )
			{
				throw new Error( "ModelLocator must be a singleton" );
			}

			instance	= this;
			makeupModel = new MakeupModel();
		}

		public static function getInstance() : ModelLocator
		{
			if ( instance == null )
				instance = new ModelLocator();

			return instance;
		}
	}
}