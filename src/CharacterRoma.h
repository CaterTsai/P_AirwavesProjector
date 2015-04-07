#ifndef AIRWAVE_PROJECTOR_ROMA
#define AIRWAVE_PROJECTOR_ROMA

#include "BaseCharacter.h"
#include "ArrowShooter.h"

class CharacterRoma : public IBaseCharacter
{
public:
	virtual void setupCharacter() override;
	virtual void reset() override;
	virtual void startGame() override;
//----------------------------------
//Arrow Shooter
//----------------------------------
public:
	void onArrowHit(bool& bDefence);

private:
	ArrowShooter	_ArrowShooter;

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
	float										_fShootTimer;
	vector<pair<bool, eSHOOT_TARGET_TYPE>>		_TeachingList;
	int											_TeachingIndex;
	
//----------------------------------
//Gaming
//----------------------------------
private:
	virtual void setupGaming();
	virtual void updateGaming(float fDelta, SkeletonHandler& SkeletonHandler) override;
	virtual void drawGaming() override;
};

#endif // !AIRWAVE_PROJECTOR_ROMA
