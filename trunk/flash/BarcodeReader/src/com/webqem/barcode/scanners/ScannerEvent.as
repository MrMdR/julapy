/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is the Actionscript 3 barcode decoding library.
 *
 * The Initial Developer of the Original Code is
 * webqem pty ltd. http://www.webqem.com/.
 * Portions created by the Initial Developer are Copyright (C) 2008
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Mike Shaw <mikes@webqem.com>
 * ***** END LICENSE BLOCK ***** */

package com.webqem.barcode.scanners {

	import flash.events.Event;

	public class ScannerEvent extends Event {

		public static const SCANNED:String = "scanned";

		public function ScannerEvent(type:String, bubbles:Boolean=false, cancelable:Boolean=false) {
			super(type, bubbles, cancelable);
		}

		public override function clone():Event {
			return new ScannerEvent(type, bubbles, cancelable);
		}

	}
}