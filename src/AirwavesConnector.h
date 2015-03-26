#ifndef AIRWAVES_CONNECTOR
#define AIRWAVES_CONNECTOR

#include "ofMain.h"
#include "ofxNetwork.h"
#include "protocol.h"

#define cCMD_LENGTH 1000

const string cSTART_TEX = "\x01";
const string cVALUE_TEX = "\x02";
const string cEND_TEX = "\x03";

class AirwavesConnector
{
public:
	void initConnector(string strIP, int iPort, int MyPort);
	void updateConnector();
	void closeConnector();
	void sendCMD(eCONNECTOR_CMD eCMD, string strValue = "");

private:
	ofxUDPManager	_udpReciver, _udpSender;

//-------------------------------
//Event
//-------------------------------
public:
	ofEvent<pair<eCONNECTOR_CMD, string>>	AirwavesConnectorEvent;
};

#endif // !AIRWAVES_CONNECTOR
