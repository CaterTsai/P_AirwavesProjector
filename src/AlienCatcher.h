#ifndef AIRWAVES_PROJECTOR_ALIEN_CATCHER
#define AIRWAVES_PROJECTOR_ALIEN_CATCHER

#include "constParameter.h"
#include "ofxAnimatableFloat.h"

#pragma region stAlienPoint
//------------------------------
//@STRUCT stAlienPoint
enum eALIEN_STATE
{
	eALIEN_STAND,
	eALIEN_FLY,
	eALIEN_SCALE,
	eALIEN_CAN_REMOVE,
};

typedef struct _stAlienPoint
{
	ofVec2f				Position;
	ofxAnimatableFloat	AnimUp, AnimAlpah, AnimScale,AnimScaleFadeout;
	float				fScale;
	eALIEN_STATE		eState;

	_stAlienPoint(ofVec2f Start)
		:eState(eALIEN_STAND)
		,fScale(ofRandom(cALIEN_SCALE.first, cALIEN_SCALE.second))
		,Position(Start)
	{
		AnimUp.setCurve(AnimCurve::EASE_IN);
		AnimUp.setDuration(cALIEN_ATTRACTIVE_TIME);

		AnimScale.setCurve(AnimCurve::EASE_IN);
		AnimScale.setDuration(cALIEN_ATTRACTIVE_TIME);

		AnimScaleFadeout.setCurve(AnimCurve::EASE_OUT);
		AnimScaleFadeout.setDuration(0.3);

		AnimAlpah.setCurve(AnimCurve::LINEAR);
		AnimAlpah.setDuration(cALIEN_FADEIN_TIME);
		AnimAlpah.animateFromTo(0, 255);
	}

	void update(float fDelta, float UFOPosX, float fScaleLimit, float fminDist);
}stAlienPoint;
#pragma endregion

#pragma region AlienCathcer
//------------------------------
//@CLASS AlienCathcer
class AlienCatcher
{
public:
	AlienCatcher()
		:_bStartAuto(false)
	{}

	void setup();
	void update(float fDelta, float PosX = -1);
	void draw();

	void addAlien(float fPoxX);
	void addAlien();

public:
	inline void setAutoCreate(bool bValue)
	{
		_bStartAuto = bValue;
		if(bValue)
		{
			_fCreateTimer = 3.0;
		}
	}
private:
	inline int getIntervalID(float PosX)
	{
		int iReturn_ = -1;
		for (int idx_ = 0; idx_  < _PositionList.size(); idx_ ++)
		{
			if((_PositionList[idx_] + _IntervalSize) > PosX)
			{
				iReturn_ = idx_;
				break;
			}
		}

		return iReturn_;
	}

private:

	bool				_bStartAuto;
	ofImage				_UFO, _Alien;

	//Position
	float				_IntervalSize;
	float				_IntervalSizeHalf;
	vector<float>		_PositionList;
	
	//UFO
	int					_iNowIntervalID;
	ofxAnimatableFloat	_AnimUFOPosX;

	//Alien
	list<stAlienPoint>	_AlienList;
	vector<int>			_IntervalCreateOrder;
	int					_iAlienCounter;
	float				_fCreateTimer;

public:
	//Event
	ofEvent<string>		_AlienEvent;

};

#pragma endregion


#endif // !AIRWAVES_PROJECTOR_ALIEN_CATCHER
