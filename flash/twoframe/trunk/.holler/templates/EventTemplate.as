@copy@

package @namespace@.@events@
{
	import com.adobe.cairngorm.control.CairngormEvent;
	import flash.events.Event;

	public class @sequence.name@Event extends CairngormEvent
	{
		public static const @sequence.name.uppercase@:String = "@sequence.name@Event.@sequence.name.lowercase@";
		
		public function @sequence.name@Event(type:String=@sequence.name.uppercase@, bubbles:Boolean=false, cancelable:Boolean=false)
		{
			super(type, bubbles, cancelable);
		}
		
		override public function clone():Event
		{
			return new @sequence.name@Event(type);
		}
		
	}
}