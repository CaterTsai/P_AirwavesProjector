#include "CharacterAlien.h"

void CharacterAlien::setupCharacter()
{
	//Alien Head
	CharacterObj	AlienHeadObj_;
	AlienHeadObj_.setup(NAME_MGR::C_Alien_Head, "Alien/head.jpg", ofVec2f(157, 295), 0.7);


	_ObjectList.push_back(AlienHeadObj_);

	_bIsSetup = true;
}

#pragma region Character object update
//--------------------------------------------------------------
void CharacterAlien::updateCharacterObj(CharacterObj& Obj, SkeletonHandler& SkeletonHandler)
{
	ofVec2f Head_ = SkeletonHandler.getJoints(_JointType::JointType_Head);
	ofVec2f Shoulder_ = SkeletonHandler.getJoints(_JointType::JointType_SpineShoulder);

	float HeadAngle_ = (Head_ - Shoulder_).angle(ofVec2f(0, -1));

	Obj.update(Head_, HeadAngle_);
}
#pragma endregion

#pragma region Teaching
//--------------------------------------------------------------
void CharacterAlien::setupTeaching()
{
}

//--------------------------------------------------------------
void CharacterAlien::updateTeaching(float fDelta, SkeletonHandler& SkeletonHandler)
{
}

//--------------------------------------------------------------
void CharacterAlien::drawTeaching()
{
}
#pragma endregion

#pragma region Gaming
//--------------------------------------------------------------
void CharacterAlien::setupGaming()
{
}

//--------------------------------------------------------------
void CharacterAlien::updateGaming(float fDelta, SkeletonHandler& SkeletonHandler)
{
}

//--------------------------------------------------------------
void CharacterAlien::drawGaming()
{
}
#pragma endregion