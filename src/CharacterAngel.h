#ifndef AIRWAVE_PROJECTOR_ANGEL
#define AIRWAVE_PROJECTOR_ANGEL

#include "BaseCharacter.h"
#include "HeartManager.h"

enum eGESTURE_STATE
{
	eBOTH_HAND_WAIT,
	eBOTH_HAND_DOWN,
	eBOTH_HAND_UP,
};

class CharacterAngel : public IBaseCharacter
{
public:
	virtual void setupCharacter() override;
	virtual void reset() override;
private:
	float fHeartTimer_;
//----------------------------------
//Heart Manager
//----------------------------------
public:
	void onHeartHit(string& e);
private:
	void addFloatingHeart(int iNum);
	bool gestureCheck(SkeletonHandler& SkeletonHandler);
private:
	HeartManager	_HeartManager;

	eGESTURE_STATE	_eHandState;
	int				_GestureCounter;
//----------------------------------
//Character Object Update
//----------------------------------
private:
	void updateCharacterObj(CharacterObj& Obj, SkeletonHandler& SkeletonHandler);

//----------------------------------
//Teaching
//----------------------------------
private:
	virtual void setupTeaching();
	virtual void updateTeaching(float fDelta, SkeletonHandler& SkeletonHandler) override;
	virtual void drawTeaching() override;

private:
	bool bStartTeaching_;
	float fTeachingTimer_;

//----------------------------------
//Gaming
//----------------------------------
private:
	virtual void setupGaming();
	virtual void updateGaming(float fDelta, SkeletonHandler& SkeletonHandler) override;
	virtual void drawGaming() override;

private:
	float fGameFloatTimer_;
};

#endif // !AIRWAVE_PROJECTOR_ANGEL