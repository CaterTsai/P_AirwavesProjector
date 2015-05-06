#ifndef AIRWAVES_PROJECTOR_NAME_MANAGER
#define AIRWAVES_PROJECTOR_NAME_MANAGER

#include <string>
using namespace std;

#define CREATE_VALUE(NAME) static const string NAME = #NAME;
#define CREATE_VALUE_WITH_VALUE(NAME, VALUE) static const string NAME = VALUE;

namespace NAME_MGR
{
//---------------------------------------
// Character Object
//---------------------------------------
	//Roma
	CREATE_VALUE(C_Roma_Hat);
	CREATE_VALUE(C_Roma_Shield);
	CREATE_VALUE(C_Roma_Spear);
	
	//Alien
	CREATE_VALUE(C_Alien_Head);

	//Angel
	CREATE_VALUE(C_Angel_Ring);
	CREATE_VALUE(C_Angel_Wings);

	//Roma
	CREATE_VALUE(C_Money_Hat);
	CREATE_VALUE(C_Money_Cup_L);
	CREATE_VALUE(C_Money_Cup_R);

//---------------------------------------
// Character Event
//---------------------------------------
	CREATE_VALUE(EVENT_TeachingCheck);
	CREATE_VALUE(EVENT_TeachingFinish);
	CREATE_VALUE(EVENT_TakePicture);
};

namespace AUDIO_NAME_MGR
{
//---------------------------------------
// BGM
//---------------------------------------
	CREATE_VALUE(BGM_WAITING);
	CREATE_VALUE(BGM_GAME);
//---------------------------------------
// Audio
//---------------------------------------
	CREATE_VALUE(A_ROMA_SHOOT);
	CREATE_VALUE(A_ROMA_HIT);
	CREATE_VALUE(A_ROMA_DEFENCE);

	CREATE_VALUE(A_ALIEN_GOT);
	CREATE_VALUE(A_ALIEN_TRANSPORT);

	CREATE_VALUE(A_ANGEL_TOUCH);
	CREATE_VALUE(A_ANGEL_FLY);
};

#endif // !AIRWAVES_PROJECTOR_NAMEMANAGER
