#ifndef AIRWAVE_PROJECTOR_ALIEN
#define AIRWAVE_PROJECTOR_ALIEN

#include "BaseCharacter.h"

class CharacterAlien : public IBaseCharacter
{
public:
	virtual void setupCharacter() override;

//----------------------------------
//Character Object Update
//----------------------------------
private:
	void updateCharacterObj(CharacterObj& Obj, SkeletonHandler& SkeletonHandler);

//----------------------------------
//Teaching
//----------------------------------
private:
	void setupTeaching();
	virtual void updateTeaching(float fDelta, SkeletonHandler& SkeletonHandler) override;
	virtual void drawTeaching() override;

//----------------------------------
//Gaming
//----------------------------------
private:
	void setupGaming();
	virtual void updateGaming(float fDelta, SkeletonHandler& SkeletonHandler) override;
	virtual void drawGaming() override;
};

#endif // !AIRWAVE_PROJECTOR_ALIEN
