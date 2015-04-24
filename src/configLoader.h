#ifndef AIRWAVES_PROJECTOR_CONFIG_LOADER
#define AIRWAVES_PROJECTOR_CONFIG_LOADER

#include "ofMain.h"
#include "ofxXmlSettings.h"

class configLoader
{
public:
	void load();
	void save(ofVec2f StartPos, float fScale);

public:
	//Kinect
	ofPoint		_exKinectStartPos;
	float		_exKinectScale;
	bool		_exIsMirror;
	bool		_exDisplaySkeleton;

	bool		_exUseSerial;
	string		_exCOM;
	string		_exDisplayIP;

	//Roma
	ofVec2f		_exRomaHatAnchor, _exRomaShieldAnchor, _exRomaSpearAnchor;
	float		_exRomaHatScale, _exRomaShieldScale, _exRomaSpearScale;
	
	//Angel
	ofVec2f		_exAngelWingsAnchor, _exAngelRingAnchor;
	float		_exAngelWingsScale, _exAngelRingScale;

	//Alien
	ofVec2f		_exAlienHeadAnchor;
	float		_exAlienHeadScale;

//-------------------
//Singleton
//-------------------
private:
	configLoader(){};
	configLoader(configLoader const&);
	void operator=(configLoader const&);

public:
	static configLoader* GetInstance();
	static void Destroy();

private:
	static configLoader *pInstance;
};


#endif // !AIRWAVES_PROJECTOR_CONFIG_LOADER
