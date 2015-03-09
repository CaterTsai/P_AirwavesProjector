#include "AirwavesConnector.h"

void AirwavesConnector::initConnector(string strIP, int iPort, int iMyPort)
{
	//Reciver
	_udpReciver.Create();
	_udpReciver.Bind(iMyPort);
	_udpReciver.SetNonBlocking(true);

	//Sender
	_udpSender.Create();
	_udpSender.Connect(strIP.c_str(), iPort);
	_udpSender.SetNonBlocking(true);
}

//--------------------------------------------------------------
void AirwavesConnector::updateConnector()
{
	char udpCmd_[cCMD_LENGTH];
	
	_udpReciver.Receive(udpCmd_, cCMD_LENGTH);
	string strCmd_ = udpCmd_;

	if(strCmd_ != "")
	{
		int iStart_ = strCmd_.find(cSTART_TEX);
		int iEnd_ = strCmd_.find(cEND_TEX);
		
		if(iStart_ == string::npos || iEnd_ == string::npos)
		{
			return;
		}

		try{
			strCmd_ = strCmd_.substr(iStart_+1, iEnd_ - iStart_ - 1);
		}catch( const exception &e)
		{
			ofLog(OF_LOG_ERROR, e.what());
		}

		ofNotifyEvent(AirwavesConnectorEvent, strCmd_, this);
	}
}

//--------------------------------------------------------------
void AirwavesConnector::closeConnector()
{
	_udpReciver.Close();
	_udpSender.Close();
}

//--------------------------------------------------------------
void AirwavesConnector::sendCMD(eCONNECTOR_CMD eCMD, string strValue)
{
	string CMD_ = cSTART_TEX + ofToString(eCMD) + cVALUE_TEX + strValue + cEND_TEX;
	
	_udpSender.Send(CMD_.c_str(), CMD_.length());
}