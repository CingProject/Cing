#ifndef QUICKGUIEFFECT_H
#define QUICKGUIEFFECT_H

#include "QuickGUIForwardDeclarations.h"
#include "QuickGUIManager.h"

namespace QuickGUI
{
	//------------------------------------------------------------------------------------------------
	class _QuickGUIExport Effect
	{
	public:
		enum EffectInterpolatorType
		{
			EFFECT_LINEAR = 0,
			EFFECT_LINEARX2,
			EFFECT_LINEARX4,
			EFFECT_SIN
		};
	public:
		Effect(Widget * const widget, const float duration, 
			const float startTime, const float currTime = 0, const bool repeat = false,
			EffectInterpolatorType type = EFFECT_LINEARX4) : 
		mWidget(widget), 
			mStartTime(startTime), 
			mDuration(duration), 
			mRepeat(repeat),
			mCurrTime(currTime),
			mType(type)
		{
			assert (duration != 0);
		}
		virtual ~Effect(){};

		virtual void updateValue(const float factor) = 0;

		bool update(const float timeElapsed);
		float getTimeFactor() ;
		bool getTimeOrIsFinished();
		float linearInterpolate(const float start, 
			const float end, 
			const float timeFactor) const;
		float interpolate(const float start, 
			const float end, 
			const float timeFactor) const;

	protected:
		Widget * const mWidget;
		const float mStartTime; 
		const float mDuration;
		const bool mRepeat;

		float mCurrTime;
		const EffectInterpolatorType mType;
	};
	//------------------------------------------------------------------------------------------------
	class _QuickGUIExport AlphaEffect : public Effect
	{
	public:

		AlphaEffect(Widget * const widget, 
			const float duration, 
			const float startAlpha,
			const float endAlpha,
			const float startTime, 
			const float currTime = 0, 
			const bool repeat = false,
			EffectInterpolatorType type = EFFECT_LINEARX4) : 
		Effect(widget, duration, startTime,  currTime, repeat), 
			mStartAlpha(startAlpha), 
			mEndAlpha(endAlpha)
		{
			assert(mStartAlpha >= 0.0 && mStartAlpha <= 1.0);
			assert(mEndAlpha >= 0.0 && mEndAlpha <= 1.0);
		}
		virtual ~AlphaEffect(){};

		void updateValue(const float factor);
	private:
		const float mStartAlpha;
		const float mEndAlpha;
	};
	//------------------------------------------------------------------------------------------------
	class _QuickGUIExport MoveEffect : public Effect
	{
	public:
		MoveEffect(Widget * const widget, 
			const float duration, 
			const Point &startPosition,
			const Point &endPosition,
			const float startTime, 
			const float currTime = 0, 
			const bool repeat = false,
			EffectInterpolatorType type = EFFECT_LINEARX4) : 
		Effect(widget, duration, startTime,  currTime, repeat), 
			mStartPosition(startPosition), 
			mEndPosition(endPosition)
		{}
		virtual ~MoveEffect(){};
		void updateValue(const float factor);

	private:
		const Point mStartPosition;
		const Point mEndPosition;
	};
	//------------------------------------------------------------------------------------------------
	class _QuickGUIExport SizeEffect : public Effect
	{
	public:
		SizeEffect(Widget * const widget, 
			const float duration, 
			const Size &startSize,
			const Size &endSize,
			const float startTime, 
			const float currTime = 0, 
			const bool repeat = false,
			EffectInterpolatorType type = EFFECT_LINEARX4) : 
		Effect(widget, duration, startTime, currTime, repeat), 
			mStartSize(startSize), 
			mEndSize(endSize)
		{}
		virtual ~SizeEffect(){};
		void updateValue(const float factor);

	private:
		const Size mStartSize;
		const Size mEndSize;
	};
	//------------------------------------------------------------------------------------------------
	class _QuickGUIExport ActivateEffect : public Effect
	{
	public:
		ActivateEffect(Widget * const widget, 
			const float duration, 
			const float startTime, 
			const float currTime = 0, 
			const bool repeat = false,
			EffectInterpolatorType type = EFFECT_LINEARX4): 
		Effect(widget, duration, startTime, currTime, repeat)
		{};
		virtual ~ActivateEffect(){};
		void updateValue(const float factor);

	};
	//------------------------------------------------------------------------------------------------
	class _QuickGUIExport OverEffect : public Effect
	{
	public:
		OverEffect(Widget * const widget, 
			const float duration, 
			const float startTime, 
			const float currTime = 0, 
			const bool repeat = false,
			EffectInterpolatorType type = EFFECT_LINEARX4): 
		Effect(widget, duration, startTime, currTime, repeat)
		{};
		virtual ~OverEffect(){};
		void updateValue(const float factor);

	};
	//------------------------------------------------------------------------------------------------
}

#endif
