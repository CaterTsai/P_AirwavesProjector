#pragma once


#include "constParameter.h"
#include "SkeletonHandler.h"
#include "AirwavesConnector.h"
#include "ofxCTKinectV2.h"

class ProjectorView : public ofBaseApp
{
public:
	void setup();
	void update();
	void draw();
	void exit();
	void keyPressed(int key);


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
//Connector
//-------------------------------------------------
public:
	void onConnectorEvent(string& e);
private:
	AirwavesConnector	_Connector;
};