#ifndef AIRWAVES_SERIAL_CONNECTOR
#define AIRWAVES_SERIAL_CONNECTOR

#include "AirwavesConnector.h"

class SerialConnector : public AirwavesConnector
{
public:
	SerialConnector(string strCOM);
	void updateConnector() override;
	void closeConnector() override;
	void sendCMD(eCONNECTOR_CMD eCMD, string strValue = "") override;

private:
	bool		_bSetupSerial;
	ofSerial	_Serial;

	bool		_bStartCmd;
	string		_strPadding;
};

#endif // !AIRWAVES_SERIAL_CONNECTOR
