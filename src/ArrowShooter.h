#ifndef AIRWAVE_PROJECTOR_ARROW_SHOOTER
#define AIRWAVE_PROJECTOR_ARROW_SHOOTER

#include "constParameter.h"
#include "ofxAnimatableFloat.h"
#include "NameManager.h"
#include "AudioMgr.h"
#include "ConfigLoader.h"
#include "SkeletonHandler.h"

enum eSHOOT_TARGET_TYPE
{
	eSHOOT_TOP,
	eSHOOT_MIDDLE,
	eSHOOT_BOTTOM,
	eSHOOT_NUM
};

//------------------------------
//@STRUCT stArrowPoint
typedef struct _stArrowPoint
{
	bool				bLive, bBroke;
	float				fDegree;
	ofVec2f				Position;
	ofVec2f				Velocity;
	ofxAnimatableFloat	AnimAlpha;

	static	ofEvent<bool>	ArrowEvent;

	_stArrowPoint(ofVec2f Start, ofVec2f End, float fDuration)
		:bLive(true)
		,bBroke(false)
	{
		Position = Start;
		Velocity = (End - Start)/fDuration;
		fDegree = Velocity.angle(ofVec2f(1, 0));

		AnimAlpha.reset(255);
		AnimAlpha.setCurve(AnimCurve::BLINK_AND_FADE_3);
		AnimAlpha.setRepeatType(AnimRepeat::PLAY_ONCE);
		AnimAlpha.setDuration(0.4);
	}

	void update(float fDelta, ofVec2f& SpearStart, ofVec2f& SpearEnd, ofVec2f& RightPos, ofRectangle& bodyRect);
}stArrowPoint;

typedef list<stArrowPoint>		ARROW_LIST;
typedef ARROW_LIST::iterator	ARROW_ITER;

//------------------------------
//@CLASS ArrowShooter
class ArrowShooter
{
public:
	ArrowShooter()
		:_bStartAutoShoot(false)
		,_fShooterTimer(0.0)
	{}
	void setup();
	void update(float fDelta, SkeletonHandler& SkeletonHandler, float fSpearSize);
	void draw();
	void clear();

	void shoot(ofVec2f Start, ofVec2f End, float fDuration);
	void shoot(ofVec2f End, eSHOOT_TARGET_TYPE eType);
	void shoot(ofVec2f End, eSHOOT_TARGET_TYPE eType, float fDuration, bool bFromLeft);

public:
	inline void setAutoShoot(bool bAuto)
	{
		_bStartAutoShoot = bAuto;

		if(bAuto)
		{
			_fShooterTimer = ofRandom(cAUTO_SHOOT_TIME.first, cAUTO_SHOOT_TIME.second);
		}
	}

private:
	ofImage			_ArrowImg;
	ofVec2f			_Anchor;
	ARROW_LIST		_ArrowList;

	map<eSHOOT_TARGET_TYPE, int>	_ShooterPos;

	bool			_bStartAutoShoot;
	float			_fShooterTimer;
};

#endif // !AIRWAVE_PROJECTOR_ARROW_SHOOTER