#include "configLoader.h"

//--------------------------------------------------------------
void configLoader::load()
{
	ofxXmlSettings	xml_;
	_exKinectStartPos.set(0, 0);
	_exKinectScale = 1.0;
	if(!xml_.loadFile("_config.xml"))
	{
		ofLog(OF_LOG_WARNING, "Can't found _config.xml used default setting");
		return;
	}

	_exKinectStartPos.x = xml_.getValue("KINECT_START_X", 0);
	_exKinectStartPos.y = xml_.getValue("KINECT_START_Y", 1.0);
	_exKinectScale = xml_.getValue("KINECT_SCALE", 1.0);
	_exIsMirror = (xml_.getValue("MIRROR", 0) == 1);
	_exDisplaySkeleton = (xml_.getValue("SKELETON", 0) == 1);
	_exUseSerial = (xml_.getValue("USE_SERIAL", 0, 0) == 1);
	_exCOM = xml_.getValue("COM", "COM4", 0);
	_exDisplayIP = xml_.getValue("DISPLAY_IP", "127.0.0.1");

	//Roma
	_exRomaHatAnchor.set(xml_.getValue("ROMA:HAT:X", 150), xml_.getValue("ROMA:HAT:Y", 500));
	_exRomaHatScale = xml_.getValue("ROMA:HAT:SCALE", 0.8);
	_exRomaShieldAnchor.set(xml_.getValue("ROMA:SHIELD:X", 30), xml_.getValue("ROMA:SHIELD:Y", 197));
	_exRomaShieldScale = xml_.getValue("ROMA:SHIELD:SCALE", 0.6);
	_exRomaSpearAnchor.set(xml_.getValue("ROMA:SPEAR:X", 94), xml_.getValue("ROMA:SPEAR:Y", 360));
	_exRomaSpearScale = xml_.getValue("ROMA:SPEAR:SCALE", 1.3);

	//Angel
	_exAngelWingsAnchor.set(xml_.getValue("ANGEL:WINGS:X", 546), xml_.getValue("ANGEL:WINGS:Y", 343));
	_exAngelWingsScale = xml_.getValue("ANGEL:WINGS:SCALE", 0.7);
	_exAngelRingAnchor.set(xml_.getValue("ANGEL:RING:X", 156), xml_.getValue("ANGEL:RING:Y", 240));
	_exAngelRingScale = xml_.getValue("ANGEL:RING:SCALE", 0.2);

	//Alien
	_exAlienHeadAnchor.set(xml_.getValue("ALIEN:HEAD:X", 193), xml_.getValue("ALIEN:HEAD:Y", 337));
	_exAlienHeadScale = xml_.getValue("ALIEN:HEAD:SCALE", 0.5);

	//Money
	_exMoneyHatAnchor.set(xml_.getValue("MONEY:HAT:X", 546), xml_.getValue("MONEY:HAT:Y", 343));
	_exMoneyHatScale = xml_.getValue("MONEY:HAT:SCALE", 0.7);
	_exMoneyCupAnchor.set(xml_.getValue("MONEY:CUP:X", 156), xml_.getValue("MONEY:CUP:Y", 240));
	_exMoneyCupScale = xml_.getValue("MONEY:CUP:SCALE", 0.2);
}

//--------------------------------------------------------------
void configLoader::save(ofVec2f StartPos, float fScale)
{
	ofxXmlSettings	xml_;
	xml_.setValue("KINECT_START_X", StartPos.x);
	xml_.setValue("KINECT_START_Y",StartPos.y);
	xml_.setValue("KINECT_SCALE", fScale);
	xml_.setValue("MIRROR", _exIsMirror);
	xml_.setValue("SKELETON", _exDisplaySkeleton);
	xml_.setValue("DISPLAY_IP", _exDisplayIP);
	xml_.setValue("USE_SERIAL", _exUseSerial);
	xml_.setValue("COM", _exCOM);

	//Roma
	xml_.setValue("ROMA:HAT:X", _exRomaHatAnchor.x);
	xml_.setValue("ROMA:HAT:Y", _exRomaHatAnchor.y);
	xml_.setValue("ROMA:HAT:SCALE", _exRomaHatScale);
	xml_.setValue("ROMA:SHIELD:X", _exRomaShieldAnchor.x);
	xml_.setValue("ROMA:SHIELD:Y", _exRomaShieldAnchor.y);
	xml_.setValue("ROMA:SHIELD:SCALE", _exRomaShieldScale);
	xml_.setValue("ROMA:SPEAR:X", _exRomaSpearAnchor.x);
	xml_.setValue("ROMA:SPEAR:Y", _exRomaSpearAnchor.y);
	xml_.setValue("ROMA:SPEAR:SCALE", _exRomaSpearScale);

	//Angel
	xml_.setValue("ANGEL:WINGS:X", _exAngelWingsAnchor.x);
	xml_.setValue("ANGEL:WINGS:Y", _exAngelWingsAnchor.y);
	xml_.setValue("ANGEL:WINGS:SCALE", _exAngelWingsScale);
	xml_.setValue("ANGEL:RING:X", _exAngelRingAnchor.x);
	xml_.setValue("ANGEL:RING:Y", _exAngelRingAnchor.y);
	xml_.setValue("ANGEL:RING:SCALE", _exAngelRingScale);

	//Alien
	xml_.setValue("ALIEN:HEAD:X", _exAlienHeadAnchor.x);
	xml_.setValue("ALIEN:HEAD:Y", _exAlienHeadAnchor.y);
	xml_.setValue("ALIEN:HEAD:SCALE", _exAlienHeadScale);

	//Money
	xml_.setValue("MONEY:HAT:X", _exMoneyHatAnchor.x);
	xml_.setValue("MONEY:HAT:Y", _exMoneyHatAnchor.y);
	xml_.setValue("MONEY:HAT:SCALE", _exMoneyHatScale);
	xml_.setValue("MONEY:CUP:X", _exMoneyHatAnchor.x);
	xml_.setValue("MONEY:CUP:Y", _exMoneyHatAnchor.y);
	xml_.setValue("MONEY:CUP:SCALE", _exMoneyHatScale);

	if(xml_.saveFile("_config.xml"))
	{
		ofLog(OF_LOG_NOTICE, "Save config file success");
	}
}

//--------------------------------------------------------------
configLoader* configLoader::pInstance = 0;
configLoader* configLoader::GetInstance()
{
	if(pInstance == 0)
	{
		pInstance = new configLoader();
	}
	return pInstance;
}

//--------------------------------------------------------------
void configLoader::Destroy()
{
	if(pInstance != 0)
	{
		delete pInstance;
	}
}