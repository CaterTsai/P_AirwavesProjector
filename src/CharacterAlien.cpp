#include "CharacterAlien.h"

void CharacterAlien::setupCharacter()
{
	//Alien Head
	CharacterObj	AlienHeadObj_;
	AlienHeadObj_.setup(NAME_MGR::C_Alien_Head, "Alien/head.jpg", ofVec2f(157, 295), 0.7);

	_ObjectList.push_back(AlienHeadObj_);

	//Alien Catcher
	_eHandState = eNO_LOCKON;
	_AlienCatcher.setup();
	ofAddListener(_AlienCatcher._AlienEvent, this, &CharacterAlien::onLastAlien);
	_bIsSetup = true;
}

//--------------------------------------------------------------
void CharacterAlien::reset()
{
	_eHandState = eNO_LOCKON;
	_AlienCatcher.clear();
}

#pragma region Alien Cathcer
//--------------------------------------------------------------
void CharacterAlien::onLastAlien(string& e)
{
	if(_eState == eCHARACTER_TEACHING)
	{
		this->addTeachingAlien();
	}
}

//--------------------------------------------------------------
void CharacterAlien::addTeachingAlien()
{
	switch(_iAlienCounter)
	{
	case 2:
		_AlienCatcher.addAlien(200);
		_iAlienCounter--;
		break;
	case 1:
		_AlienCatcher.addAlien(cWINDOW_WIDTH - 200);
		_iAlienCounter--;
		break;
	case 0:
		//Finish Teaching
		_eState = eCHARACTER_GAMING;
		_AlienCatcher.setAutoCreate(true);

		//Event
		pair<string, string> Event_ = make_pair(NAME_MGR::EVENT_TeachingFinish, ofToString(eCHARACTER_ALIEN));
		ofNotifyEvent(IBaseCharacter::CharacterEvent, Event_);

		break;
	}
}

//--------------------------------------------------------------
float CharacterAlien::getCtrlPos(SkeletonHandler& SkeletonHandler)
{
	float CtrlPos_ = -1;

	switch(_eHandState)
	{
	case eNO_LOCKON:
		{
			ofVec2f LeftHand_ = SkeletonHandler.getJoints(_JointType::JointType_HandLeft);
			ofVec2f RightHand_ = SkeletonHandler.getJoints(_JointType::JointType_HandRight);

			ofVec2f Shoulder_ = SkeletonHandler.getJoints(_JointType::JointType_SpineShoulder);

			if(RightHand_.y < Shoulder_.y)
			{
				_eHandState = eRIGHT_LOCKON;
			}
			else if(LeftHand_.y < Shoulder_.y)
			{
				_eHandState = eLEFT_LOCKON;
			}
		}
		break;
	case eLEFT_LOCKON:
		{
			ofVec2f LeftHand_ = SkeletonHandler.getJoints(_JointType::JointType_HandLeft);
			ofVec2f ShoulderLeft_ = SkeletonHandler.getJoints(_JointType::JointType_ShoulderLeft);
			ofVec2f ShoulderRight_ = SkeletonHandler.getJoints(_JointType::JointType_ShoulderRight);

			float fMin_ = ShoulderLeft_ .x - abs(ShoulderLeft_.x - ShoulderRight_.x);
			float fMax_ = ShoulderRight_.x;
			CtrlPos_ = ofMap(LeftHand_.x, fMin_, fMax_, 0, cWINDOW_WIDTH);

			if(LeftHand_.y > ShoulderLeft_.y || LeftHand_.y > ShoulderRight_.y)
			{
				_eHandState = eNO_LOCKON;
			}
		}
		break;
	case eRIGHT_LOCKON:
		{
			ofVec2f RightHand_ = SkeletonHandler.getJoints(_JointType::JointType_HandRight);
			ofVec2f ShoulderLeft_ = SkeletonHandler.getJoints(_JointType::JointType_ShoulderLeft);
			ofVec2f ShoulderRight_ = SkeletonHandler.getJoints(_JointType::JointType_ShoulderRight);

			float fMin_ = ShoulderLeft_ .x;
			float fMax_ = ShoulderRight_.x + abs(ShoulderLeft_.x - ShoulderRight_.x);
			CtrlPos_ = ofMap(RightHand_.x, fMin_, fMax_, 0, cWINDOW_WIDTH);
			
			if(RightHand_.y > ShoulderLeft_.y || RightHand_.y > ShoulderRight_.y)
			{
				_eHandState = eNO_LOCKON;
			}
		}
		break;
	}
	return CtrlPos_;
}
#pragma endregion

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
	_iAlienCounter = cALIEN_TEACHING_STEPS;
	this->addTeachingAlien();

	_iPictureCounter = 0;
	_fPictureTimer = _fPirecureInterval = cANGEL_PICTURE_INTERVAL;
}

//--------------------------------------------------------------
void CharacterAlien::updateTeaching(float fDelta, SkeletonHandler& SkeletonHandler)
{
	float fCtrlX_ = this->getCtrlPos(SkeletonHandler);
	_AlienCatcher.update(fDelta, fCtrlX_);

	this->takePicture(fDelta);
}

//--------------------------------------------------------------
void CharacterAlien::drawTeaching()
{
	ofPushStyle();
	{
		ofSetColor(255, 0, 0);
		ofCircle(100, 100, 50);

		_AlienCatcher.draw();
	}
	ofPopStyle();
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
	if(!SkeletonHandler.getHaveUser())
	{
		return;
	}
	float fCtrlX_ = this->getCtrlPos(SkeletonHandler);
	_AlienCatcher.update(fDelta, fCtrlX_);
}

//--------------------------------------------------------------
void CharacterAlien::drawGaming()
{
	ofPushStyle();
	{
		ofSetColor(0, 255, 0);
		ofCircle(100, 100, 50);

		_AlienCatcher.draw();
	}
	ofPopStyle();
}
#pragma endregion