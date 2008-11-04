@copy@

package @namespace@.@business.dir@
{
	import com.adobe.cairngorm.business.IResponder;
	import com.adobe.cairngorm.business.RemotingService;
	import com.adobe.cairngorm.business.ServiceLocator;
	import @service.package@.@service.name@


	public class @sequence.name@Delegate extends Delegate implements IResponder
	{
		private var service:@service.name@

		public function @sequence.name@Delegate(_responder:IResponder)
		{
			super(_responder);
			if(modelLocator.configModel.@service.enabled.name@) service = ServiceLocator.getInstance().get@service.name.short@Service(@service.name.id@);
		}

		public function @sequence.name.lowercase@():void
		{
			if(modelLocator.configModel.@service.enabled.name@)
			{
				service.addResponder(this);
				service.@service.name.execution@();
			}
			else
			{
				dummyData();
			}
		}
		
		protected function dummyData():void
		{
			onResult();
		}

		override public function onResult(data:* = null):void
		{
			super.onResult(data);
		}

		override public function onFault(event:* = null):void
		{
			super.onFault(event);
		}
	}
}