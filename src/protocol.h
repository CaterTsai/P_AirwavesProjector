#ifndef AIRWAVES_PROTOCOL
#define AIRWAVES_PROTOCOL

typedef enum eCONNECTOR_CMD
{
	//Display to Projector
	eD2P_CMD			=	100
	,eD2P_SET_CHARACTOR
	,eD2P_GAME_TIMEOUT
	,eD2P_MAX

	//Projector to Display
	,eP2D_CMD			=	200
	,eP2D_TEACHING_END
	,eP2D_MAX
};

#endif // !AIRWAVES_PROTOCOL
