#ifndef PROJECTOR_SKELETON_HANDLER
#define PROJECTOR_SKELETON_HANDLER

#include "constParameter.h"
#include "ofxCTKinectV2.h"

class SkeletonHandler
{
public:
	SkeletonHandler(ofPoint StartPos = ofPoint(0, 0), float fScale = 1.0)
		:_StartPos(StartPos)
		,_fScale(fScale)
		,_bHaveUser(false)
		,_bIsMirror(false)
		,_bDisplaySkeleton(false){}

	void updateSkeleton(const bool bHaveUser_, ofVec2f (&Joints)[JointType_Count]);
	void drawSkeleton();

	ofVec2f getJoints(_JointType eJointType);

//Inline method
public:
	inline bool getHaveUser() const
	{
		return _bHaveUser;
	}

	inline float getBodySize() const
	{
		return (cDEPTH_HEIGHT - _Skeleton.aJoints[JointType_Head].y) * _fScale;
	}

	//Start Pos
	inline void setStartPos(const ofPoint StartPos)
	{
		_StartPos = StartPos;
	}
	inline ofPoint getStartPos() const
	{
		return _StartPos;
	}

	//Scale
	inline void setScale(const float fScale)
	{
		_fScale = fScale;
	}
	inline float getScale() const
	{
		return _fScale;
	}

	//Mirror
	inline void setMirror(const bool bValue)
	{
		_bIsMirror = bValue;
	}

	//for Debug
	inline void setDisplay(const bool bValue = true)
	{
		_bDisplaySkeleton = bValue;
	}

private:
	bool				_bDisplaySkeleton;
	bool				_bHaveUser;
	bool				_bIsMirror;

	ofPoint				_StartPos;
	float				_fScale;
	stSCREEN_SKELETON	_Skeleton;
};

#endif // !PROJECTOR_SKELETON_HANDLER
