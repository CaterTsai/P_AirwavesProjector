#ifndef AIRWAVES_PROJECTOR_AUDIO_MGR
#define AIRWAVES_PROJECTOR_AUDIO_MGR

#include "constParameter.h"

class AudioMgr
{
public:
	void addAduio(string strName, string strFilepath, bool bIsLoop = false, bool bIsMultiPlay = true, float fVol = 1.0);
	void addBGM(string strName, string strFilepath);
	void playAudio(string strName);
	void stopAudio(string strName);

private:
	AudioMgr(){};
	AudioMgr(AudioMgr const&);
	void operator=(AudioMgr const&);

private:
	map<string, ofSoundPlayer>	_AudioMap;

//-------------------
//Singleton
//-------------------
public:
	static AudioMgr* GetInstance();
	static void Destroy();

private:
	static AudioMgr *pInstance;
};

#endif // !AIRWAVES_PROJECTOR_AUDIO_MGR