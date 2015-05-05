#include "SerialConnector.h"

SerialConnector::SerialConnector(string strCOM)
{
	_bSetupSerial = _Serial.setup(strCOM, 9600);
	_bStartCmd = false;
	if(!_bSetupSerial)
	{
		ofLog(OF_LOG_WARNING, "[SerialConnector]Setup serial failed");
	}
}

//--------------------------------------------------------------
void SerialConnector::updateConnector()
{
	if(!_bSetupSerial)
	{
		return;
	}

	char acCmd_[cCMD_LENGTH] = {};
	int iReadNum_ = _Serial.readBytes((unsigned char*)acCmd_, cCMD_LENGTH);

	if(iReadNum_ > 0)
	{
		string strCmd_ = acCmd_;
		
		if(strCmd_ == "")
		{
			return;
		}

		int iStart_ = strCmd_.find(cSTART_TEX);
		int iValue_ = strCmd_.find(cVALUE_TEX);
		int iEnd_ = strCmd_.find(cEND_TEX);

		//padding
		bool bCompleteCmd_ = false;
		if(iStart_ != string::npos && iEnd_ == string::npos)
		{
			_strPadding = "";
			_strPadding = strCmd_;
			_bStartCmd = true;
		}
		else if(iStart_ == string::npos && iEnd_ == string::npos)
		{
			if(_bStartCmd)
			{
				_strPadding += strCmd_;
			}
		}
		else if(iStart_ == string::npos && iEnd_ != string::npos)
		{
			_strPadding += strCmd_;
			bCompleteCmd_ = true;
		}
		else
		{
			_strPadding = "";
			_strPadding = strCmd_;
			bCompleteCmd_ = true;
		}
			

		if(bCompleteCmd_)
		{
			string strValue_ = "";
			eCONNECTOR_CMD eCmd_ = eD2P_CMD;

			int iPaddingStart_ = _strPadding.find(cSTART_TEX);
			int iPaddingValue_ = _strPadding.find(cVALUE_TEX);
			int iPaddingEnd_ = _strPadding.find(cEND_TEX);
					
			try{
				eCmd_ = static_cast<eCONNECTOR_CMD>(ofToInt(_strPadding.substr(iPaddingStart_+1, iPaddingValue_ - iPaddingStart_ - 1)));
				strValue_ = _strPadding.substr(iPaddingValue_+1, iPaddingEnd_ - iPaddingValue_ - 1);

			}catch( const exception &e)
			{
				ofLog(OF_LOG_ERROR, e.what());
			}
			pair<eCONNECTOR_CMD, string> e_ = make_pair(eCmd_, strValue_);
			ofNotifyEvent(AirwavesConnectorEvent, e_, this);
			
			_strPadding = "";
			_bStartCmd = false;
		}
	}
}

//--------------------------------------------------------------
void SerialConnector::closeConnector()
{
	if(_bSetupSerial)
	{
		_Serial.close();
	}
}

//--------------------------------------------------------------
void SerialConnector::sendCMD(eCONNECTOR_CMD eCMD, string strValue)
{
	if(!_bSetupSerial)
	{
		return;
	}

	string CMD_ = cSTART_TEX + ofToString(eCMD) + cVALUE_TEX + strValue + cEND_TEX;
	
	_Serial.writeBytes((unsigned char*)CMD_.c_str(), CMD_.size());
}