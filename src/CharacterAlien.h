#ifndef AIRWAVE_PROJECTOR_ALIEN
#define AIRWAVE_PROJECTOR_ALIEN

#include "BaseCharacter.h"
#include "AlienCatcher.h"

enum eLOCKON_HAND
{
	eNO_LOCKON,
	eLEFT_LOCKON,
	eRIGHT_LOCKON,
};

class CharacterAlien : public IBaseCharacter
{
public:
	virtual void setupCharacter() override;
	virtual void reset() override;
	virtual void startGame() override;

//----------------------------------
//Alien Catcher
//----------------------------------
public:
	void onLastAlien(string& e);
private:
	float getCtrlPos(SkeletonHandler& SkeletonHandler);
private:
	eLOCKON_HAND	_eHandState;
	AlienCatcher	_AlienCatcher;

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
	float		_fStartTimer;

//----------------------------------
//Gaming
//----------------------------------
private:
	virtual void setupGaming();
	virtual void updateGaming(float fDelta, SkeletonHandler& SkeletonHandler) override;
	virtual void drawGaming() override;
};

#endif // !AIRWAVE_PROJECTOR_ALIEN
