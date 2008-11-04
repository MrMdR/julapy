@copy@

package @namespace@.@commands@
{
	import com.adobe.cairngorm.control.CairngormEvent;
	import @namespace@.@events@.@sequence.name@Event;
	//HollerProjectGenerator marker: add import
	
	public class @sequence.name@Command extends Command
	{
		public function @sequence.name@Command()
		{
			super();
		}
		
		override public function execute( event : CairngormEvent ):void
		{
			var @sequence.name.lowercase@Event:@sequence.name@Event = event as @sequence.name@Event;
			
			//HollerProjectGenerator marker: add delegate
		}

		override public function onResult( result : * = null ) : void
		{
			super.onResult(result);	
		}

		override public function onFault( event : * = null ) : void
		{
			super.onFault(event);
		}
	}
}