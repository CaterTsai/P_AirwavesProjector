#ifndef AIRWAVE_PROJECTOR_MONEY
#define AIRWAVE_PROJECTOR_MONEY

#include "BaseCharacter.h"
#include "SyceeManager.h"
class CharacterMoney : public IBaseCharacter
{
public:
	virtual void setupCharacter() override;
	virtual void reset() override;
	virtual void startGame() override;

//----------------------------------
//Sycee Manager
//----------------------------------
public:
	void onLastSycee(string& e);
private:
	void gestureCheck(SkeletonHandler& SkeletonHandler);

private:
	SyceeManager	_SyceeManager;

	ofVideoPlayer	_CupVideo;
	ofVec2f			_CupAnchor;
	bool	_bLeftCup, _bRightCup;
//----------------------------------
//Character Object Update
//----------------------------------
private:
	void updateCharacterObj(CharacterObj& Obj, SkeletonHandler& SkeletonHandler);

//----------------------------------
//Teaching
//----------------------------------
private:
	virtual void setupTeaching() override;
	virtual void updateTeaching(float fDelta, SkeletonHandler& SkeletonHandler) override;
	virtual void drawTeaching() override;

private:
	float _fTeachingTimer;
//----------------------------------
//Gaming
//----------------------------------
private:
	virtual void setupGaming();
	virtual void updateGaming(float fDelta, SkeletonHandler& SkeletonHandler) override;
	virtual void drawGaming() override;
};

#endif // !AIRWAVE_PROJECTOR_MONEY
