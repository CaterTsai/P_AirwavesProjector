#include "SkeletonHandler.h"


//--------------------------------------------------------------
void SkeletonHandler::updateSkeleton(const bool bHaveUser_, ofVec2f (&Joints)[JointType_Count])
{
	_bHaveUser = bHaveUser_;

	if(_bHaveUser)
	{
		CopyMemory(_Skeleton.aJoints, Joints, sizeof(ofVec2f) * JointType_Count);
	}
}

//--------------------------------------------------------------
void SkeletonHandler::drawSkeleton()
{
	if(!_bHaveUser || !_bDisplaySkeleton)
	{
		return;
	}

	ofPushStyle();
	ofPushMatrix();
	{
		ofTranslate(_StartPos);
		ofSetColor(255, 0, 0);
		ofFill();
		for(auto Joint : _Skeleton.aJoints)
		{
			ofVec2f ScaleJoint_ = Joint * _fScale;
			ofCircle(ScaleJoint_, 10);
		}
	}
	ofPopMatrix();
	ofPopStyle();
}

//--------------------------------------------------------------
ofVec2f SkeletonHandler::getJoints(_JointType eJointType)
{
	if(!_bHaveUser)
	{
		return ofVec2f(0);
	}

	return _Skeleton.aJoints[eJointType] * (_fScale) + _StartPos;
}

