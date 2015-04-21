#pragma once


#include "constParameter.h"
#include "SkeletonHandler.h"
#include "AudioMgr.h"
#include "AirwavesConnector.h"
#include "ofxCTKinectV2.h"
#include "ofxXmlSettings.h"
#include "ofxAnimatableFloat.h"
#include "CharacterMgr.h"
#include "configLoader.h"

class ProjectorView : public ofBaseApp
{
public:
	void setup();
	void update();
	void draw();
	void exit();
	void keyPressed(int key);

public:
	float		_fMainTimer;
	//DEBUG
	bool		_bCtrlMode;
	ofImage		_Human;

//-------------------------------------------------
//Background
//-------------------------------------------------
public:
	void updateBackground(float fDelta);
	void drawBackground();
private:
	ofxAnimatableFloat	_AnimBackgroundFade;
	bool				_bDisplayLight;
	ofImage				_BackgroundL, _BackgroundD;

//-------------------------------------------------
//Audio Manager
//-------------------------------------------------
public:
	void setupAudioMgr();

//-------------------------------------------------
//Kinect 
//-------------------------------------------------
public:
	bool initKinect();
	void updateKinect();
	void drawKinect();

	void startKinect();
	void stopKinect();
	void settingKinect(int key);
private:
	bool				_bHaveUser;
	stSCREEN_SKELETON	_Skeleton;
	CTKinectV2			_Kinect;

	SkeletonHandler		_SkeletonHandler;

//-------------------------------------------------
//Character
//-------------------------------------------------
public:
	void onCharacterEvent(pair<string, string>& e);
private:
	CharacterMgr	_CharacterMgr;
	eCHARACTER_TYPE	_eCharacterType;

//-------------------------------------------------
//Connector
//-------------------------------------------------
public:
	void onConnectorEvent(pair<eCONNECTOR_CMD, string>& e);
private:
	AirwavesConnector	_Connector;

////-------------------------------------------------
////Config
////-------------------------------------------------
//public:
//	void loadConfig();
//	void saveConfig();
//private:
//	ofPoint		_exKinectStartPos;
//	float		_exKinectScale;
//	bool		_exIsMirror;
};