#ifndef AIRWAVE_PROJECT_BASE_CHARACTER
#define AIRWAVE_PROJECT_BASE_CHARACTER

#include "constParameter.h"
#include "SkeletonHandler.h"

enum eCHARACTER_STATE
{
	eCHARACTER_STOP	=	0
	,eCHARACTER_TEACHING
	,eCHARACTER_GAMEING
	,eCHARACTER_FINISH
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


private:
	ofImage		_obj;	//TODO: Image -> Video

	ofPoint		_DrawPos;
	float		_fRotate;
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
		,_eState(eCHARACTER_STOP){}


	virtual void setupCharacter(){};
	virtual void updateCharacter(float fDelta, SkeletonHandler& SkeletonHandler){};
	virtual void drawCharacter(){};

	virtual void play(){};
	
protected:
	virtual void clear(){};

protected:
	bool					_bIsSetup, _bIsDisplay;
	eCHARACTER_STATE		_eState;
	vector<CharacterObj>	_ObjectList;
};



#endif // !AIRWAVE_PROJECT_BASE_CHARACTER
