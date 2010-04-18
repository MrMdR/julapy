package com.julapy.ph.makeup.view
{
	import caurina.transitions.Tweener;

	import com.holler.core.View;
	import com.julapy.ph.makeup.events.SectionEvent;
	import com.julapy.ph.makeup.model.MakeupModel;
	import com.julapy.ph.makeup.model.ModelLocator;

	import fl.motion.easing.Quadratic;

	import flash.display.Sprite;

	public class CoverView extends View
	{
		private var bShow : Boolean = true;

		public function CoverView(sprite:Sprite=null)
		{
			super(sprite);

			ModelLocator.getInstance().makeupModel.addEventListener( SectionEvent.SECTION_CHANGE, sectionChangeHandler );
		}

		public function show ( b : Boolean ):void
		{
			if( bShow == b )
				return;

			bShow = b;

			if( bShow )
			{
				visible	= true;
				alpha	= 0;
				doValidate();

				Tweener.addTween
				(
					this,
					{
						alpha		: 1.0,
						time		: 0.4,
						delay		: 0.0,
						transition	: Quadratic.easeOut,
						onStart		: null,
						onUpdate	: tweenUpdateHandler,
						onComplete	: null
					}
				);
			}
			else
			{
				Tweener.addTween
				(
					this,
					{
						alpha		: 0.0,
						time		: 0.4,
						delay		: 0.0,
						transition	: Quadratic.easeOut,
						onStart		: null,
						onUpdate	: tweenUpdateHandler,
						onComplete	: tweenCompleteHandler
					}
				);
			}
		}

		private function tweenUpdateHandler ():void
		{
			doValidate();
		}

		private function tweenCompleteHandler ():void
		{
			visible = false;
			alpha	= 0;
			doValidate();
		}

		private function sectionChangeHandler ( e : SectionEvent ):void
		{
			var section : int;
			section = ModelLocator.getInstance().makeupModel.section;

			if( section == MakeupModel.SECTION_PLAY )
			{
				show( false );
			}
			else
			{
				show( true );
			}
		}
	}
}