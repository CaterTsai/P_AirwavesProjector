#ifndef AIRWAVES_PROJECTORCONST_PARAMETER
#define AIRWAVES_PROJECTORCONST_PARAMETER

#include "ofMain.h"

const int cWINDOW_WIDTH = 1215;
const int cWINDOW_HEIGHT = 1024;

typedef enum eCHARACTER_TYPE
{
	eCHARACTER_ANGEL = 0,
	eCHARACTER_ROMA,
	eCHARACTER_ALIEN,
	eCHARACTER_MONEY,
	eCHARACTER_NUM
};

//Roma
static int cARROW_TRIGGER_DISTANCE_X = 30;
static int cARROW_DEFEND_DISTANCE_Y = 200;
static int cARROW_HIT_DISTANCE_X = 30;
static pair<float, float>	cAUTO_SHOOT_TIME = make_pair(0.3, 0.5);
static pair<float, float>	cARROW_DURATION = make_pair(2, 5);
static int cARROW_START_X = 300;
static int cARROW_START_RANDOM = 30;

//Alien
static float cALIEN_FADEIN_TIME = 1.0;
static float cALIEN_ATTRACTIVE_TIME = 5.0;
static int cALIEN_INTERVAL_NUM = 7;
static float cUFO_INTERVAL_DURATION = 0.5;
static pair<float, float>	cALIEN_SCALE = make_pair(0.5, 0.8);
static pair<float, float>	cALIEN_CREATE_DURATION = make_pair(0.5, 1.0);
static int cALIEN_TEACHING_STEPS = 2;

//Angel
static float cHEART_FADEOUT_TIME = 1.0;
static pair<float, float>	cHEART_SCALE = make_pair(0.3, 0.5);
static pair<float, float>	cFLOATING_HEART_DEGREE = make_pair(-15.0, 15.0);
static pair<float, float>	cFLOATING_HEART_TIME = make_pair(1.0, 2.0);
static pair<float, float>	cFLYING_HEART_DEGREE = make_pair(60.0, 90.0);
static pair<float, float>	cFLYING_HEART_DURATION = make_pair(3.0, 4.5);
static pair<int, int>		cFLYING_HEART_LIMIT = make_pair(4, 8);
static float cHEART_TOUCH_DISTANCE = 50.0;
static int cTEACHING_FLOATING_NUM = 10;
static int cGAMING_FLOATING_NUM = 3;
static int cMAX_FLOATING_HEART = 20;
#endif // !AIRWAVES_PROJECTOR_PARAMETER