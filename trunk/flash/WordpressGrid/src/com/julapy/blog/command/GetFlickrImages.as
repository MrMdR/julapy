package com.julapy.blog.command
{
	import com.adobe.webapis.flickr.FlickrService;
	import com.adobe.webapis.flickr.Photo;
	import com.adobe.webapis.flickr.PhotoSet;
	import com.adobe.webapis.flickr.events.FlickrResultEvent;
	import com.adobe.webapis.flickr.methodgroups.PhotoSets;
	import com.adobe.webapis.flickr.methodgroups.Photos;

	import flash.events.Event;

	public class GetFlickrImages
	{
		private var service 	: FlickrService = new FlickrService( "7ad48c160654958af2d031f8bcaab6b4" );

		private var photoSet	: PhotoSet;
		private var photos		: Array = new Array();
		private var photoIndex	: int = 0;

		public function GetFlickrImages()
		{
			service.secret = "38432409aa29467e";
			service.addEventListener( FlickrResultEvent.PHOTOSETS_GET_PHOTOS,		handlePhotoSet );
			service.addEventListener( FlickrResultEvent.PHOTOS_GET_ALL_CONTEXTS,	handlePhoto );

			var photos : PhotoSets;
			photos = new PhotoSets( service );
			photos.getPhotos( "72157624164777738" );
		}

		private function handlePhotoSet( e : FlickrResultEvent ):void
		{
			photoSet = ( e.data as Object ).photoSet;

			loadPhoto();
		}

		private function loadPhoto ():void
		{
			var photo : Photo;
			photo = photoSet.photos[ photoIndex ];

			var photoCall : Photos;
			photoCall = new Photos( service );
			photoCall.getAllContexts( photo.id );
		}

		private function handlePhoto ( e : FlickrResultEvent ):void
		{
			//
		}
	}
}