#ifndef AIRWAVES_PROJECTOR_SYCEE_MANAGER
#define AIRWAVES_PROJECTOR_SYCEE_MANAGER

#include "constParameter.h"
#include "ofxAnimatableFloat.h"
#include "ofxHapPlayer.h"
#include "SkeletonHandler.h"
#pragma region stSyceePoint
//------------------------------
//@STRUCT stSyceePoint
enum eSYCEE_STATE
{
	eSYCEE_FALLING,
	eSYCEE_CATCH,
	eSYCEE_CAN_REMOVE
};

typedef struct _stSyceePoint
{
	ofVec2f			Position;
	ofxAnimatableFloat	AnimFalling, AnimScale;
	float			fDegree, fScale;
	eSYCEE_STATE	eState;

	_stSyceePoint(ofVec2f	Start)
		:eState(eSYCEE_FALLING)
		,Position(Start)
		,fScale(1.0)
		,fDegree(ofRandom(0, 359))
	{
		AnimFalling.setCurve(AnimCurve::EASE_IN);
		AnimFalling.setDuration(ofRandom(cSYCEE_FALLING_DURACTION.first, cSYCEE_FALLING_DURACTION.second));
		AnimFalling.animateFromTo(0,cWINDOW_HEIGHT);

		AnimScale.setCurve(AnimCurve::EASE_IN_EASE_OUT);
		AnimScale.setDuration(0.3);
	}

	void update(float fDelta);

}stSyceePoint;
#pragma endregion

#pragma region SyceeManager
//------------------------------
//@CLASS SyceeManager
class SyceeManager
{
public:
	void setup();
	void update(float fDelta, SkeletonHandler& SkeletonHandler);
	void draw();
	void clear();
	void addSycee(float fPosX = ofRandom(0, cWINDOW_WIDTH));

public:
	inline void setAutoCreate(const bool bValue)
	{
		_bStartAuto = bValue;
	}

	inline void setLeftCheck(const bool bValue)
	{
		_bLeftCheck = bValue;
	}

	inline void setRightCheck(const bool bValue)
	{
		_bRightCheck = bValue;
	}

private:
	bool	_bStartAuto;
	float	_fCreateTimer;
	//
	bool	_bLeftCheck, _bRightCheck;

	//Sycee
	ofImage				_SyceeImg;
	list<stSyceePoint>	_SyceeList;

public:
	//Event
	ofEvent<string>		_MoneyEvent;
};
#pragma endregion



#endif // !AIRWAVES_PROJECTOR_SYCEE_MANAGER
