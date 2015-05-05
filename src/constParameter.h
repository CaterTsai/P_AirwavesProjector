#ifndef AIRWAVES_PROJECTORCONST_PARAMETER
#define AIRWAVES_PROJECTORCONST_PARAMETER

#include "ofMain.h"
#include "NameManager.h"
#include "AudioMgr.h"
#include "ConfigLoader.h"
const int cWINDOW_WIDTH = 1024;
const int cWINDOW_HEIGHT = 768;

const float cDEFAULT_BODY_SIZE = 710;
const int cPICTURE_NUM = 3;
typedef enum eCHARACTER_TYPE
{
	eCHARACTER_ANGEL = 0,
	eCHARACTER_ROMA,
	eCHARACTER_ALIEN,
	eCHARACTER_MONEY,
	eCHARACTER_NUM
};

//Roma
const int cARROW_TRIGGER_SHIELD_X = 60;
const int cARROW_TRIGGER_SPEAR_X = 80;
const int cARROW_DEFEND_SHIELD_Y = 200;
const int cARROW_DEFEND_SPEAR_DIST = 30;
const int cARROW_HIT_DISTANCE_X = 30;
const float cSPEAR_RATIO = 560.0/650.0;
const float cSPEAR_HAND_RATIO = 0.48;
const pair<int, int> cARROW_TARGET_RANGE_Y = make_pair(-30, 30);
const pair<float, float>	cAUTO_SHOOT_TIME = make_pair(0.5, 0.7);
const pair<float, float>	cARROW_DURATION = make_pair(2, 5);
const int cARROW_START_X = 300;
const int cARROW_START_RANDOM = 30;
const float cROMA_SHORTEST_TIME = 9.0;
const float cROMA_PICTURE_INTERVAL = cROMA_SHORTEST_TIME/cPICTURE_NUM;

//Alien
const float cALIEN_FADEIN_TIME = 0.5;
const float cALIEN_ATTRACTIVE_TIME = 3.0;
const int cALIEN_INTERVAL_NUM = 7;
const float cUFO_INTERVAL_DURATION = 0.5;
const pair<float, float>	cALIEN_SCALE = make_pair(0.8, 1.2);
const pair<float, float>	cALIEN_CREATE_DURATION = make_pair(0.2, 0.5);
const float cALIEN_SHORTEST_TIME = 5.0;
const float cALIEN_PICTURE_INTERVAL = cALIEN_SHORTEST_TIME/cPICTURE_NUM;
const int cALIEN_MAX_NUM = 20;
const float cALINE_MIN_DIST = (cWINDOW_WIDTH/2.0 * 0.9);
const int cALIEN_UP = 80;


//Angel
const float cHEART_FADEOUT_TIME = 1.0;
const pair<float, float>	cHEART_SCALE = make_pair(0.2, 0.25);
const pair<float, float>	cFLOATING_HEART_DEGREE = make_pair(-15.0, 15.0);
const pair<float, float>	cFLOATING_HEART_TIME = make_pair(1.0, 2.0);
const pair<float, float>	cFLOATING_HEART_DIST = make_pair(0.8, 1.3);
const pair<float, float>	cFLYING_HEART_DEGREE = make_pair(60.0, 90.0);
const pair<float, float>	cFLYING_HEART_DURATION = make_pair(2.5, 3.5);
const pair<int, int>		cFLYING_HEART_LIMIT = make_pair(4, 8);
const float cHEART_TOUCH_DISTANCE = 60.0;
const int cTEACHING_FLOATING_NUM = 1;
const int cGAMING_FLOATING_NUM = 3;
const int cMAX_FLOATING_HEART = 20;
const float cANGEL_SHORTEST_TIME = 10.0;
const float cANGEL_PICTURE_INTERVAL = cANGEL_SHORTEST_TIME/cPICTURE_NUM;
#endif // !AIRWAVES_PROJECTOR_PARAMETER