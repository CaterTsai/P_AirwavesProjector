#ifndef AIRWAVES_UDP_CONNECTOR
#define AIRWAVES_UDP_CONNECTOR

#include "AirwavesConnector.h"
#include "ofxNetwork.h"

class UDPConnector : public AirwavesConnector
{
public:
	UDPConnector(string strIP, int iPort, int iMyPort);
	void updateConnector() override;
	void closeConnector() override;
	void sendCMD(eCONNECTOR_CMD eCMD, string strValue = "") override;
private:
	ofxUDPManager	_udpReciver, _udpSender;
};

#endif //AIRWAVES_UDP_CONNECTOR