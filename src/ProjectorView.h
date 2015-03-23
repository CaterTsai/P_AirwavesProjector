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

private:
	bool				_bHaveUser;
	stSCREEN_SKELETON	_Skeleton;
	CTKinectV2			_Kinect;

	SkeletonHandler		_SkeletonHandler;

//-------------------------------------------------
//Character
//-------------------------------------------------
public:
	void onCharacterEvent(string& e);
private:
	CharacterMgr	_CharacterMgr;


//-------------------------------------------------
//Connector
//-------------------------------------------------
public:
	void onConnectorEvent(string& e);
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