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
	CREATE_VALUE(C_Alien_UFO);

	//Angel
	CREATE_VALUE(C_Angel_Ring);
	CREATE_VALUE(C_Angel_Wings);

//---------------------------------------
// Character Event
//---------------------------------------
	CREATE_VALUE(EVENT_TeachingFinsih);
};

#endif // !AIRWAVES_PROJECTOR_NAMEMANAGER
