////////////////////////////////////////////////////////////////////////////////
//
//  HOLLER SYDNEY
//  Copyright 2008 Holler Sydney
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

package com.holler.twoframe.model
{
	import com.adobe.cairngorm.CairngormError;
	import com.adobe.cairngorm.CairngormMessageCodes;
	import com.adobe.cairngorm.model.IModelLocator;
	
   /**
    * <p>Throughout the rest of the application, the developer can then access
    * the products from the model, as follows:</p>
    * <pre>
    *   var products : ICollectionView = ShopModelLocator.getInstance().products;
    * </pre>
    */
    public class ModelLocator implements IModelLocator
 	{
		public var configModel:ConfigModel;
   		public var applicationState:ApplicationState;

   		private static var instance:ModelLocator;
  
   		public function ModelLocator() 
		{   
			if(instance != null)
			{
				throw new CairngormError(CairngormMessageCodes.SINGLETON_EXCEPTION, "AbstractModelLocator");
			}
        
			instance = this;
			configModel = new ConfigModel();
			applicationState = new ApplicationState();
		}
    
		public static function getInstance():ModelLocator 
		{
		   if(instance == null) instance = new ModelLocator();
		       
		   return instance;
		}
 	}
}