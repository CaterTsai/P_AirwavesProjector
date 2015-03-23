#include "CharacterMoney.h"

void CharacterMoney::setupCharacter()
{
	//Hat
	CharacterObj	HatObj_;
	HatObj_.setup(NAME_MGR::C_Alien_Head, "Money/hat.jpg", ofVec2f(272, 200), 0.25);


	_ObjectList.push_back(HatObj_);

	_bIsSetup = true;
}

#pragma region Character object update
//--------------------------------------------------------------
void CharacterMoney::updateCharacterObj(CharacterObj& Obj, SkeletonHandler& SkeletonHandler)
{
	ofVec2f Head_ = SkeletonHandler.getJoints(_JointType::JointType_Head);
	ofVec2f Shoulder_ = SkeletonHandler.getJoints(_JointType::JointType_SpineShoulder);
	float HeadAngle_ = (Head_ - Shoulder_).angle(ofVec2f(0, -1));
	Obj.update(Head_ + ofVec2f(0, -30), HeadAngle_);
}
#pragma endregion

#pragma region Teaching
//--------------------------------------------------------------
void CharacterMoney::setupTeaching()
{
}

//--------------------------------------------------------------
void CharacterMoney::updateTeaching(float fDelta, SkeletonHandler& SkeletonHandler)
{
}

//--------------------------------------------------------------
void CharacterMoney::drawTeaching()
{
}
#pragma endregion

#pragma region Gaming
//--------------------------------------------------------------
void CharacterMoney::setupGaming()
{
}

//--------------------------------------------------------------
void CharacterMoney::updateGaming(float fDelta, SkeletonHandler& SkeletonHandler)
{
}

//--------------------------------------------------------------
void CharacterMoney::drawGaming()
{
}
#pragma endregion