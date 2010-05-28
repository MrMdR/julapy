/**
 * Copyright (c) 2009, Reuben Stanton
	All rights reserved.
	Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
	following conditions are met:
	
	    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
		  disclaimer.
	    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the 
		  following disclaimer in the documentation and/or other materials provided with the distribution.
	    * The name "Reuben Stanton" may not be used to endorse or promote products derived from this software without 
		  specific prior written permission.
	
	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
	INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
	DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
	SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
	WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


package com.absentdesign.core.webapis.wordpress.methodgroups{
	
	import com.absentdesign.core.webapis.Service;
	import com.absentdesign.core.webapis.ServiceRequest;
	import com.absentdesign.core.webapis.methodgroups.MethodGroup;
	import com.absentdesign.core.webapis.methodgroups.MethodGroupHelper;
	import com.absentdesign.core.webapis.events.ServiceEvent;
	import com.absentdesign.core.webapis.wordpress.WPServiceRequest;
	import com.absentdesign.core.webapis.wordpress.events.WPServiceEvent;
	import com.absentdesign.core.webapis.wordpress.WPService;
	import flash.events.Event;

	/**
	* abstract MethodGroup for making WordPress API calls
	*/
	public class WPMethodGroup extends MethodGroup{
		
		protected var requestFunction:Function;
		protected var requestArgs:Array;

		/**
		* @param service The WPService you are using to make the calls
		*/
		public function WPMethodGroup(service:WPService){
			super(service,new WPMethodGroupHelper());
		}
		
		public function get blogId():int{
			return (service as WPService).blogId;
		}
		
		public function get password():String{
			return (service as WPService).password;
		}
		
		public function get username():String{
			return (service as WPService).username;
		}
		
		/**
		* Load a WPServiceRequest
		* <p>Automatically calls service.connect() and delays the request until the service is connected 
		* if the WPService does not return a valid blogId</p>
		*
		* @param request The WPServiceRequest
		* @param requestFunction a direct reference to the function making the loadRequest call - required if you want
		* to be able to delay the call while the service is connecting
		* @param ...args any arguments for the delayed call 
		*/
		public function loadRequest(request:WPServiceRequest,requestFunction:Function,...args):void{
			var service:WPService = this.service as WPService;
			if(service.connected){
				request.addEventListener(ServiceEvent.COMPLETE,processAndDispatch,false,0,true);
				request.load();
			}
			else {
				this.requestFunction = requestFunction;
				this.requestArgs = args;
				service.addEventListener(WPServiceEvent.CONNECTED,serviceConnectedHandler,false,0,true);
				if(!service.connecting){
					service.connect();
				}
			}
		}
		
		/**
		* Re-submits a delayed call if the service needed to connect or was connecting while the call was made
		* @param event
		*/
		protected function serviceConnectedHandler(event:ServiceEvent):void{
			service.removeEventListener(WPServiceEvent.CONNECTED,serviceConnectedHandler);
			requestFunction.apply(null,requestArgs);
		}
		
		/**
		* Process the result using WPMethodGroupHelper and dispatch a ServiceEvent of type request.eventType with 
		* the parsed data
		* @param event
		*/
		protected override function processAndDispatch(event:Event):void{
			var request:WPServiceRequest = event.target as WPServiceRequest;
			request.removeEventListener(ServiceEvent.COMPLETE,processAndDispatch);
			service.dispatchEvent(new ServiceEvent(request.eventType,helper.parse(request.data,request.parseFunction)));
		}
		

	}


}

