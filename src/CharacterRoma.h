#ifndef AIRWAVE_PROJECTOR_ROMA
#define AIRWAVE_PROJECTOR_ROMA

#include "BaseCharacter.h"

class CharacterRoma : public IBaseCharacter
{
public:
	virtual void setupCharacter() override;
	virtual void updateCharacter(float fDelta, SkeletonHandler& SkeletonHandler) override;
	virtual void drawCharacter() override;

	virtual void play() override;
private:
	//virtual void clear() override;

private:

	
};

#endif // !AIRWAVE_PROJECTOR_ROMA
