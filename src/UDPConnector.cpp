#include "UDPConnector.h"

UDPConnector::UDPConnector(string strIP, int iPort, int iMyPort)
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
void UDPConnector::updateConnector()
{
	char udpCmd_[cCMD_LENGTH];
	
	_udpReciver.Receive(udpCmd_, cCMD_LENGTH);
	string strCmd_ = udpCmd_;
	eCONNECTOR_CMD eCmd_ = eD2P_CMD;
	string strValue_ = "";
	if(strCmd_ != "")
	{
		int iStart_ = strCmd_.find(cSTART_TEX);
		int iValue_ = strCmd_.find(cVALUE_TEX);
		int iEnd_ = strCmd_.find(cEND_TEX);

		if(iStart_ == string::npos || iEnd_ == string::npos)
		{
			return;
		}

		try{
			eCmd_ = static_cast<eCONNECTOR_CMD>(ofToInt(strCmd_.substr(iStart_+1, iValue_ - iStart_ - 1)));
			strValue_ = strCmd_.substr(iValue_+1, iEnd_ - iValue_ - 1);

		}catch( const exception &e)
		{
			ofLog(OF_LOG_ERROR, e.what());
		}
		pair<eCONNECTOR_CMD, string> e_ = make_pair(eCmd_, strValue_);
		ofNotifyEvent(AirwavesConnectorEvent, e_, this);
	}
}

//--------------------------------------------------------------
void UDPConnector::closeConnector()
{
	_udpReciver.Close();
	_udpSender.Close();
}

//--------------------------------------------------------------
void UDPConnector::sendCMD(eCONNECTOR_CMD eCMD, string strValue)
{
	string CMD_ = cSTART_TEX + ofToString(eCMD) + cVALUE_TEX + strValue + cEND_TEX;
	
	_udpSender.Send(CMD_.c_str(), CMD_.length());
}