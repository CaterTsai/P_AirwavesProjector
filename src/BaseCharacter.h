#ifndef AIRWAVE_PROJECT_BASE_CHARACTER
#define AIRWAVE_PROJECT_BASE_CHARACTER

#include "constParameter.h"
#include "SkeletonHandler.h"
#include "NameManager.h"

enum eCHARACTER_STATE
{
	eCHARACTER_WAIT	=	0
	,eCHARACTER_TEACHING
	,eCHARACTER_GAMING
};

class CharacterObj
{
public:
	void setup(string strName, string Filename, ofVec2f anchor, float fBodyScale);
	void update(ofPoint DrawPos, float fRotate);
	void draw();
	void setSizebyBody(float fBody);
public:
	inline string getName() const
	{
		return _Name;
	}

	inline ofPoint getDrawPos() const
	{
		return _DrawPos;
	}

private:
	ofImage		_obj;	//TODO: Image -> Video

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
		,_eState(eCHARACTER_WAIT){}

	virtual void setupCharacter(){};

	virtual void updateCharacter(float fDelta, SkeletonHandler& SkeletonHandler);
	virtual void drawCharacter();
	
	virtual void play();
	virtual void stop();
	virtual void setScale(float fBody);
	
	virtual void teachingTimeout(){};

public:
	static	ofEvent<pair<string, string>>	CharacterEvent;

protected:
	virtual void reset(){};
	virtual void updateCharacterObj(CharacterObj& Obj, SkeletonHandler& SkeletonHandler){};

	virtual void setupTeaching(){};
	virtual void updateTeaching(float fDelta, SkeletonHandler& SkeletonHandler){};
	virtual void drawTeaching(){};

	virtual void setupGaming(){};
	virtual void updateGaming(float fDelta, SkeletonHandler& SkeletonHandler){};
	virtual void drawGaming(){};

protected:
	bool					_bIsSetup, _bIsDisplay;
	eCHARACTER_STATE		_eState;
	vector<CharacterObj>	_ObjectList;
};
#endif // !AIRWAVE_PROJECT_BASE_CHARACTER
