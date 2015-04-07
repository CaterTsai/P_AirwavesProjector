#ifndef AIRWAVES_PROJECTOR_CHARACTER_MANAGER
#define AIRWAVES_PROJECTOR_CHARACTER_MANAGER

#include "BaseCharacter.h"
#include "CharacterRoma.h"
#include "CharacterAngel.h"
#include "CharacterAlien.h"
#include "CharacterMoney.h"

typedef map<eCHARACTER_TYPE, IBaseCharacter*>	CHARACTER_MAP;
typedef CHARACTER_MAP::iterator					CHARACTER_ITER;

class CharacterMgr
{
public:
	CharacterMgr()
		:_bIsPlay(false)
	{}
	~CharacterMgr();

	void setupCharacterMgr();
	void updateCharacterMgr(float fDelta, SkeletonHandler& Skeleton);
	void drawCharacterMgr();

	void setCharacter(eCHARACTER_TYPE eType, float fBodySize);
	
	void play();
	void stop();
	void timeoutTeachingMode();
	void startGaming();

private:
	CHARACTER_MAP	_CharacterMap;
	CHARACTER_ITER	_NowCharacter;
	bool			_bIsPlay;
};

#endif // !AIRWAVES_PROJECTOR_CHARACTER_MANAGER
