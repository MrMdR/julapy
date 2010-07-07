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

package com.webqem.barcode.readers {

	import com.webqem.barcode.core.IEncodedBarcodeData;

	public class OneDimEncodedData implements IEncodedBarcodeData {

		private var _encodedData	: Array;
		private var _indexes		: Array;

		public function OneDimEncodedData( encodedData : Array, indexes : Array )
		{
			_encodedData	= encodedData;
			_indexes		= indexes;
		}

		public function get encodedData ():Array
		{
			return _encodedData;
		}

		private function set encodedData ( value : Array ):void
		{
			_encodedData = value;
		}

		public function get indexes ():Array
		{
			return _indexes;
		}

		private function set indexes( value : Array ):void
		{
			_indexes = value;
		}
	}
}