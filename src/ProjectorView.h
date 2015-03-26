#pragma once


#include "constParameter.h"
#include "SkeletonHandler.h"
#include "AirwavesConnector.h"
#include "ofxCTKinectV2.h"
#include "ofxXmlSettings.h"

#include "CharacterMgr.h"

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
	ofImage		_Background;
	ofImage		_Human;

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

//-------------------------------------------------
//Config
//-------------------------------------------------
public:
	void loadConfig();
	void saveConfig();
private:
	ofPoint		_exKinectStartPos;
	float		_exKinectScale;
};