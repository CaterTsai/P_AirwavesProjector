#include "CharacterRoma.h"

void CharacterRoma::setupCharacter()
{
	//Hat
	CharacterObj	HatObj_;
	HatObj_.setup("oROMA_HAT", "Roma/hat.jpg", ofVec2f(170, 340), 0.3);

	//Shield

	//Spear

	_ObjectList.push_back(HatObj_);

	_bIsSetup = true;
}

//--------------------------------------------------------------
void CharacterRoma::updateCharacter(float fDelta, SkeletonHandler& SkeletonHandler)
{
	_bIsDisplay = SkeletonHandler.getHaveUser();
	if(!_bIsSetup || !_bIsDisplay)
	{
		return;
	}
	for(auto& Obj_ : _ObjectList)
	{
		if(Obj_.getName() == "oROMA_HAT")
		{
			ofVec2f Head_ = SkeletonHandler.getJoints(_JointType::JointType_Head);
			ofVec2f Shoulder_ = SkeletonHandler.getJoints(_JointType::JointType_SpineShoulder);

			float HeadAngle_ = (Head_ - Shoulder_).angle(ofVec2f(0, -1));

			Obj_.update(Head_, HeadAngle_);
		}
	}
}

//--------------------------------------------------------------
void CharacterRoma::drawCharacter()
{
	if(!_bIsSetup || !_bIsDisplay)
	{
		return;
	}

	for(auto& Obj_ : _ObjectList)
	{
		Obj_.draw();
	}
}

//--------------------------------------------------------------
void CharacterRoma::play()
{
	////Get Body Size
	//ofVec2f Head_ = pSkeletonHandler.getJoints(_JointType::JointType_Head);
	//ofVec2f Foot_ = pSkeletonHandler.getJoints(_JointType::JointType_FootLeft);
}