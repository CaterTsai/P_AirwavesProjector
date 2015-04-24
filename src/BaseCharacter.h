#ifndef AIRWAVE_PROJECT_BASE_CHARACTER
#define AIRWAVE_PROJECT_BASE_CHARACTER

#include "constParameter.h"
#include "configLoader.h"
#include "SkeletonHandler.h"
#include "ofxHapPlayer.h"

enum eCHARACTER_STATE
{
	eCHARACTER_WAIT	=	0
	,eCHARACTER_TEACHING
	,eCHARACTER_GAMING
};

enum eTEACHING_STATE
{
	eTEACHING_WAIT,
	eTEACHING_START,
	eTEACHING_PASS1,
	eTEACHING_FINISH
};

class CharacterObj
{
public:
	void setup(string strName, string Filename, ofVec2f anchor, float fBodyScale);
	void update(ofPoint DrawPos, float fRotate);
	void draw();
	void setSizebyBody(float fBody);
	void start();
	void stop();
public:
	inline string getName() const
	{
		return _Name;
	}

	inline ofPoint getDrawPos() const
	{
		return _DrawPos;
	}

	inline ofRectangle getScaleRect() const
	{
		return ofRectangle(0, 0, _fScaleWidth, _fScaleHeight);
	}

private:
	ofVideoPlayer	_obj;	//TODO: Image -> Video

	ofPoint		_DrawPos;
	float		_fRotate;
	ofPoint		_ScaleAnchor;
	float		_fScaleWidth, _fScaleHeight;

	//Fix
	string		_Name;
	ofVec2f		_anchor;
	float		_fBodyScale;
};

class IBaseCharacter
{
public:
	IBaseCharacter()
		:_bIsSetup(false)
		,_bIsDisplay(false)
		,_iPictureCounter(0)
		,_fPictureTimer(0.0)
		,_eState(eCHARACTER_WAIT){}

	virtual void setupCharacter(){};

	virtual void updateCharacter(float fDelta, SkeletonHandler& SkeletonHandler);
	virtual void drawCharacter();
	
	virtual void play();
	virtual void stop();
	virtual void setScale(float fBody);
	
	virtual void teachingTimeout();
	virtual void startGame(){};
public:
	static	ofEvent<pair<string, string>>	CharacterEvent;

protected:
	virtual void reset(){};
	virtual void updateCharacterObj(CharacterObj& Obj, SkeletonHandler& SkeletonHandler){};

	virtual void setupTeaching(){};
	virtual void updateTeaching(float fDelta, SkeletonHandler& SkeletonHandler){};
	virtual void drawTeaching(){};
	void takePicture(float fDelta);

	virtual void setupGaming(){};
	virtual void updateGaming(float fDelta, SkeletonHandler& SkeletonHandler){};
	virtual void drawGaming(){};

protected:
	bool					_bIsSetup, _bIsDisplay;
	eCHARACTER_STATE		_eState;
	eTEACHING_STATE			_eTeachingState;
	vector<CharacterObj>	_ObjectList;

	int						_iPictureCounter;
	float					_fPictureTimer, _fPirecureInterval;
};
#endif // !AIRWAVE_PROJECT_BASE_CHARACTER