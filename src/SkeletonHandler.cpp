#include "SkeletonHandler.h"


//--------------------------------------------------------------
void SkeletonHandler::updateSkeleton(const bool bHaveUser_, ofVec2f (&Joints)[JointType_Count])
{
	_bHaveUser = bHaveUser_;

	if(_bHaveUser)
	{
		CopyMemory(_Skeleton.aJoints, Joints, sizeof(ofVec2f) * JointType_Count);

		if(_bIsMirror)
		{
			for(auto& Joint : _Skeleton.aJoints )
			{
				Joint.x = cDEPTH_WIDTH - Joint.x;
			}

			swap(_Skeleton.aJoints[JointType_ShoulderLeft], _Skeleton.aJoints[JointType_ShoulderRight]);
			swap(_Skeleton.aJoints[JointType_ElbowLeft], _Skeleton.aJoints[JointType_ElbowRight]);
			swap(_Skeleton.aJoints[JointType_WristLeft], _Skeleton.aJoints[JointType_WristRight]);
			swap(_Skeleton.aJoints[JointType_HandLeft], _Skeleton.aJoints[JointType_HandRight]);
			swap(_Skeleton.aJoints[JointType_HipLeft], _Skeleton.aJoints[JointType_HipRight]);
			swap(_Skeleton.aJoints[JointType_KneeLeft], _Skeleton.aJoints[JointType_KneeRight]);
			swap(_Skeleton.aJoints[JointType_AnkleLeft], _Skeleton.aJoints[JointType_AnkleRight]);
			swap(_Skeleton.aJoints[JointType_FootLeft], _Skeleton.aJoints[JointType_FootRight]);
			swap(_Skeleton.aJoints[JointType_HandTipLeft], _Skeleton.aJoints[JointType_HandTipRight]);
			swap(_Skeleton.aJoints[JointType_ThumbLeft], _Skeleton.aJoints[JointType_ThumbRight]);
		}
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

