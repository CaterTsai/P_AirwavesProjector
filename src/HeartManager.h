#ifndef AIRWAVE_PROJECTOR_HEART_MANAGER
#define AIRWAVE_PROJECTOR_HEART_MANAGER

#include "constParameter.h"
#include "ofxAnimatableFloat.h"
#include "SkeletonHandler.h"

#pragma region stFlyingHeartPoint
//------------------------------
//@STRUCT stFlyingHeartPoint
typedef struct _stFlyingHeartPoint
{
	bool				bLive;
	ofVec2f				Position;
	ofVec2f				Velocity;
	float				fScale;

	_stFlyingHeartPoint(ofVec2f Start, ofVec2f End, float fDuration)
		:bLive(true)
		,fScale(ofRandom(cHEART_SCALE.first, cHEART_SCALE.second))
	{
		Position = Start;
		Velocity = (End - Start)/fDuration;
	}

	void update(float fDelta)
	{
		if(	Position.x < 0 || Position.x > cWINDOW_WIDTH || 
			Position.y < 0 || Position.y > cWINDOW_HEIGHT)
		{
			bLive = false;
		}
		Position += Velocity * fDelta;
	}
}stFlyingHeartPoint;
#pragma endregion

#pragma region stFloatHeartPoint
//------------------------------
//@STRUCT stFloatHeartPoint
enum eFLOAT_HEART_STATE
{
	eHEART_FLOATING,
	eHEART_TOUCHING,
	eHEART_BIG_HEART,
	eHEART_FAREOUT,
	eHEART_CAN_REMOVE
};

typedef struct _stFloatHeartPoint
{
	ofVec2f				Position, Velocity;
	float				fDegree;
	float				fScale, fTouchScale;
	bool				bClockwise;
	eFLOAT_HEART_STATE	eState;

	//Animation
	float				FadeoutTimer;
	ofxAnimatableFloat	AnimFadeIn, AnimTouch, AnimBigHeart, AnimFadeout;

	_stFloatHeartPoint(ofVec2f Start)
		:Velocity(0.0)
		,Position(Start)
		,fDegree(ofRandom(cFLOATING_HEART_DEGREE.first, cFLOATING_HEART_DEGREE.second))
		,bClockwise(ofRandom(-1, 1) > 0)
		,eState(eHEART_FLOATING)
	{
		AnimFadeIn.setDuration(0.5);
		AnimFadeIn.setCurve(AnimCurve::EASE_IN_EASE_OUT);

		AnimTouch.setDuration(0.5);
		AnimTouch.setCurve(AnimCurve::EASE_OUT);

		AnimBigHeart.reset(0.0);
		AnimBigHeart.setDuration(0.5);
		AnimBigHeart.setCurve(AnimCurve::BOUNCY);

		AnimFadeout.setDuration(1.0);
		AnimFadeout.setCurve(AnimCurve::EASE_IN_EASE_OUT);

		fTouchScale = fScale = ofRandom(cHEART_SCALE.first, cHEART_SCALE.second);

		FadeoutTimer = cHEART_FADEOUT_TIME;
		AnimFadeIn.animateFromTo(0, 255);
	}
	void update(float fDelta);
}stFloatHeartPoint;
#pragma endregion

#pragma region HeartEventArgs
//------------------------------
//@CLASS HeartEventArgs
class HeartEventArgs : public ofEventArgs
{
public:
	stFlyingHeartPoint*		pFlyingHeart;
	stFloatHeartPoint*		pFloatingHeart;
	string					strMsg;

	HeartEventArgs()
		:pFlyingHeart(nullptr)
		,pFloatingHeart(nullptr)
		,strMsg("")
	{
	}

	//static ofEvent<HeartEventArgs>	events;
};
#pragma endregion

#pragma region HeartManager
//------------------------------
//@CLASS HeartManager
class HeartManager
{
public:
	HeartManager()
		:_bStartAutoFloat(false)
	{}
	void setup();
	void update(float fDelta);
	void draw();
	void clear();

	void addFlying(ofVec2f Start, ofVec2f End, float fDuration);
	void addFloting(ofVec2f Pos);

	int getFloatingNum();
	bool getFloatingPos(int id, ofVec2f& pos);
private:
	
	ofImage		_FlyingHeartImg, _FloatHeartImg, _BigHeartImg;
	
	bool		_bStartAutoFloat;
	float		_fFloatingTime;

	list<stFlyingHeartPoint>	_FlyingHeartList;
	list<stFloatHeartPoint>		_FloatingHeartList;

public:
	ofEvent<string>	HeartEvent;
};
#pragma endregion


#endif // !AIRWAVE_PROJECTOR_HEART_MANAGER
