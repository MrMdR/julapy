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

package com.absentdesign.core.webapis.wordpress{
	
	import com.absentdesign.core.webapis.Service;
	import com.absentdesign.core.webapis.wordpress.methodgroups.*;
	import com.absentdesign.core.webapis.wordpress.events.*;
	import com.absentdesign.core.webapis.events.ServiceEvent;
	
	/**
	* concrete Service class for making WordPress XML-RPC calls
	*/
	public class WPService extends Service{
		
		internal static const XML_RPC:String = "xmlrpc.php";
		
		private var _blogId:int = 0;
		private var _username:String;
		private var _password:String;
		private var _posts:Posts;
		private var _pages:Pages;
		private var _blogs:Blogs;
		private var _tags:Tags;
		private var _categories:Categories;
		private var _comments:Comments;
		private var _authors:Authors;
		private var _connected:Boolean;
		private var _connecting:Boolean;

		/**
		* @param endpoint The main endpoint for the WordPress install eg: http://myblogname.wordpress.com/
		* @param username The username for this WordPress install
		* @param password The password for this WordPress install
		*/
		public function WPService(endpoint:String,username:String,password:String){
			this.endpoint = endpoint;
		    _username = username;
		    _password = password;
			_blogs = new Blogs(this);
			_posts = new Posts(this);
			_pages = new Pages(this);
			_tags = new Tags(this);
			_categories = new Categories(this);
			_comments = new Comments(this);
			_authors = new Authors(this);
		}
		
		public function get username():String{
			return _username;
		}
		
		public function get password():String{
			return _password;
		}
		
		public function get blogId():int{
			return _blogId;
		}
		
		public function set blogId(id:int):void{
			_blogId = id;
		}
			
		//methodgroups	
		public function get blogs():Blogs{
			return _blogs;
		}
		
		public function get posts():Posts{
			return _posts;
		}
		
		public function get pages():Pages{
			return _pages;
		}
		
		public function get tags():Tags{
			return _tags;
		}
		
		public function get categories():Categories{
			return _categories;
		}
		
		public function get comments():Comments{
			return _comments;
		}
		
		public function get authors():Authors{
			return _authors;
		}
		
		/**
		* Find the blogId to allow connections to xmlrpc.php
		*
		* Makes a call to wp.getUsersBlogs to find out the correct blogId based on the endpoint/username/password
		* combination. You do not need to call this function directly, as it will be called automatically the first time you
		* make any method call using any WPMethodGroup.
		* <p>This is necessary because there seems to be no way to find out what your blogId is without calling 
		* wp.getUsersBlogs and iterating for a match, and blogId is a required parameter in most wp service calls</p>
		* <p>If you have already set the blogId manually this function will be ignored</p>
		*/
		public function connect():void{
			if(connected){
				return;
			}
			_connecting = true;
			addEventListener(WPServiceEvent.GET_USERS_BLOGS,connectedHandler, false, 0, true);
			blogs.getUsersBlogs();
		}
		
		private function connectedHandler(event:ServiceEvent):void{
			removeEventListener(WPServiceEvent.GET_USERS_BLOGS,connectedHandler);
			for each(var blog:Blog in event.data){
				if(blog.url == endpoint){
					_blogId = blog.blog_id;
					break;
				}
			}
			_connecting = false;
			dispatchEvent(new WPServiceEvent(WPServiceEvent.CONNECTED,event.data));
		}
		
		public function get connected():Boolean{
			return blogId != 0;
		}
		
		public function get connecting():Boolean{
			return _connecting;
		}

	}


}

