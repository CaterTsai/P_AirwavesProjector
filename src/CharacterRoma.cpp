#include "CharacterRoma.h"

void CharacterRoma::setupCharacter()
{
	//Hat
	CharacterObj	HatObj_;
	HatObj_.setup(NAME_MGR::C_Roma_Hat, "Roma/hat.mov", ofVec2f(150, 400), 1.0);

	//Shield
	CharacterObj	ShieldObj_;
	ShieldObj_.setup(NAME_MGR::C_Roma_Shield, "Roma/shield.mov", ofVec2f(60, 197), 0.8);

	//Spear
	CharacterObj	SpearObj_;
	SpearObj_.setup(NAME_MGR::C_Roma_Spear, "Roma/spear.mov", ofVec2f(94, 305), 1.5);

	_ObjectList.push_back(HatObj_);
	_ObjectList.push_back(ShieldObj_);
	_ObjectList.push_back(SpearObj_);

	//Arrow shooter
	_ArrowShooter.setup();
	ofAddListener(stArrowPoint::ArrowEvent, this, &CharacterRoma::onArrowHit);

	_bIsSetup = true;
}

//--------------------------------------------------------------
void CharacterRoma::reset()
{
	_ArrowShooter.clear();
}

//--------------------------------------------------------------
void CharacterRoma::startGame()
{
	if(_eState == eCHARACTER_GAMING)
	{
		_ArrowShooter.setAutoShoot(true);
	}
}

#pragma region Arrow Shooter
//--------------------------------------------------------------
void CharacterRoma::onArrowHit(bool& bDefence)
{
	switch(_eState)
	{
	case eCHARACTER_TEACHING:
		{
			if(bDefence)
			{
				_TeachingIndex++;
			}
		}
		break;
	case eCHARACTER_GAMING:
		{
		}
		break;
	}
}
#pragma endregion

#pragma region Character Object Update
//--------------------------------------------------------------
//Character Object Update
void CharacterRoma::updateCharacterObj(CharacterObj& Obj, SkeletonHandler& SkeletonHandler)
{
	if(Obj.getName() == NAME_MGR::C_Roma_Hat)
	{
		ofVec2f Head_ = SkeletonHandler.getJoints(_JointType::JointType_Head);
		ofVec2f Shoulder_ = SkeletonHandler.getJoints(_JointType::JointType_SpineShoulder);

		float HeadAngle_ = (Head_ - Shoulder_).angle(ofVec2f(0, -1));

		Obj.update(Head_, HeadAngle_);
	}
	else if(Obj.getName() == NAME_MGR::C_Roma_Shield)
	{
		ofVec2f RightHand_ = SkeletonHandler.getJoints(_JointType::JointType_HandRight);	
		Obj.update(RightHand_, 0.0);
	}
	else if(Obj.getName() == NAME_MGR::C_Roma_Spear)
	{
		ofVec2f LeftHand_ = SkeletonHandler.getJoints(_JointType::JointType_HandLeft);
		ofVec2f LeftElbow_ = SkeletonHandler.getJoints(_JointType::JointType_ElbowLeft);

		float SpearAngle_ = (LeftHand_ - LeftElbow_).angle(ofVec2f(0, -1)) - 5.0;
		Obj.update(LeftHand_, SpearAngle_);
	}
}
#pragma endregion

#pragma region Teaching
//--------------------------------------------------------------
//Teaching
void CharacterRoma::setupTeaching()
{
	_fShootTimer = 3.0;
	if(_TeachingList.empty())
	{
		_TeachingList.push_back(make_pair(false, eSHOOT_TARGET_TYPE::eSHOOT_TOP));
		_TeachingList.push_back(make_pair(true, eSHOOT_TARGET_TYPE::eSHOOT_MIDDLE));
		_TeachingList.push_back(make_pair(false, eSHOOT_TARGET_TYPE::eSHOOT_BOTTOM));
	}
	_TeachingIndex = 0;
	_iPictureCounter = 0;
	_fPictureTimer = _fPirecureInterval = cROMA_PICTURE_INTERVAL;

}

//--------------------------------------------------------------
void CharacterRoma::updateTeaching(float fDelta, SkeletonHandler& SkeletonHandler)
{
	if(!SkeletonHandler.getHaveUser())
	{
		return;
	}

	//update arrow shooter
	_ArrowShooter.update(fDelta, SkeletonHandler);

	if(_TeachingIndex >= _TeachingList.size())
	{
		//Teaching clear!!
		_ArrowShooter.clear();
		_eState = eCHARACTER_GAMING;

		//Event
		pair<string, string> Event_ = make_pair(NAME_MGR::EVENT_TeachingFinish, ofToString(eCHARACTER_ROMA));
		ofNotifyEvent(IBaseCharacter::CharacterEvent, Event_);
	}

	//Shooter
	_fShootTimer -= fDelta;
	if(_fShootTimer < 0.0)
	{
		try{
			auto Teaching_ = _TeachingList.at(_TeachingIndex);
			ofVec2f Target_;
			switch(Teaching_.second)
			{
			case eSHOOT_TOP:
				Target_ = SkeletonHandler.getJoints(_JointType::JointType_Neck);
				break;
			case eSHOOT_BOTTOM:
				Target_ = SkeletonHandler.getJoints(_JointType::JointType_KneeRight);
				break;
			case eSHOOT_MIDDLE:
			default:
				Target_ = SkeletonHandler.getJoints(_JointType::JointType_SpineMid);
				break;
			}
			_ArrowShooter.shoot(
				Target_,
				Teaching_.second,
				2.0,
				Teaching_.first
			);
		}
		catch(const exception& e)
		{
			ofLog(OF_LOG_ERROR, e.what());
		}
		_fShootTimer = 3.0;
	}

	this->takePicture(fDelta);

}

//--------------------------------------------------------------
void CharacterRoma::drawTeaching()
{
	ofPushStyle();
	{
		ofSetColor(255, 0, 0);
		ofCircle(100, 100, 50);

		_ArrowShooter.draw();
	}
	ofPopStyle();
}

#pragma endregion

#pragma region Gaming
//--------------------------------------------------------------
//Gaming
void CharacterRoma::setupGaming()
{
}

//--------------------------------------------------------------
void CharacterRoma::updateGaming(float fDelta, SkeletonHandler& SkeletonHandler)
{
	if(!SkeletonHandler.getHaveUser())
	{
		return;
	}

	//update arrow shooter
	_ArrowShooter.update(fDelta, SkeletonHandler);
}

//--------------------------------------------------------------
void CharacterRoma::drawGaming()
{
	ofPushStyle();
	{
		ofSetColor(0, 255, 0);
		ofCircle(100, 100, 50);

		_ArrowShooter.draw();
	}
	ofPopStyle();
}
#pragma endregion