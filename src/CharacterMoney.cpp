#include "CharacterMoney.h"

void CharacterMoney::setupCharacter()
{
	//Hat
	CharacterObj	HatObj_;
	HatObj_.setup(NAME_MGR::C_Money_Hat, "Money/hat.mov", configLoader::GetInstance()->_exMoneyHatAnchor, configLoader::GetInstance()->_exMoneyHatScale);
	
	//Cup
	CharacterObj	LeftCupObj_, RightCupObj_;
	LeftCupObj_.setup(NAME_MGR::C_Money_Cup_L, "Money/cup1.mov", configLoader::GetInstance()->_exMoneyCupLAnchor, configLoader::GetInstance()->_exMoneyCupLScale);
	RightCupObj_.setup(NAME_MGR::C_Money_Cup_R, "Money/cup2.mov", configLoader::GetInstance()->_exMoneyCupRAnchor, configLoader::GetInstance()->_exMoneyCupRScale);
		
	_ObjectList.push_back(HatObj_);
	_ObjectList.push_back(LeftCupObj_);
	_ObjectList.push_back(RightCupObj_);
	
	_bLeftCup = false;
	_bRightCup = false;

	//Sycee Manager
	_SyceeManager.setup();
	ofAddListener(_SyceeManager._MoneyEvent, this, &CharacterMoney::onLastSycee);

	_bIsSetup = true;
}

//--------------------------------------------------------------
void CharacterMoney::reset()
{
	_SyceeManager.clear();
	_SyceeManager.setAutoCreate(false);
}

//--------------------------------------------------------------
void CharacterMoney::startGame()
{
	if(_eState == eCHARACTER_GAMING)
	{
		_SyceeManager.clear();
		_SyceeManager.setAutoCreate(true);
	}
}

#pragma region Sycee Manager
//--------------------------------------------------------------
void CharacterMoney::onLastSycee(string& e)
{
	switch(_eState)
	{
	case eCHARACTER_TEACHING:
		{
			if(_eTeachingState == eTEACHING_START)
			{
				_eTeachingState = eTEACHING_PASS1;
				pair<string, string> Event_ = make_pair(NAME_MGR::EVENT_TeachingCheck, ofToString(eCHARACTER_MONEY));
				ofNotifyEvent(IBaseCharacter::CharacterEvent, Event_);
				_fTeachingTimer = 3.0;
			}
			else if(_eTeachingState == eTEACHING_PASS1)
			{
				//Teaching clear!!
				_eState = eCHARACTER_GAMING;

				//Event
				pair<string, string> Event_ = make_pair(NAME_MGR::EVENT_TeachingFinish, ofToString(eCHARACTER_MONEY));
				ofNotifyEvent(IBaseCharacter::CharacterEvent, Event_);
			}
		}
		break;
	case eCHARACTER_GAMING:
		{
		}
		break;
	}
}

//--------------------------------------------------------------
void CharacterMoney::gestureCheck(SkeletonHandler& SkeletonHandler)
{
	ofVec2f LeftHand_ = SkeletonHandler.getJoints(_JointType::JointType_HandLeft);
	ofVec2f RightHand_ = SkeletonHandler.getJoints(_JointType::JointType_HandRight);
	ofVec2f Spin = SkeletonHandler.getJoints(_JointType::JointType_SpineBase);
	
	//Left hand check
	if(LeftHand_.y < Spin.y)
	{
		_bLeftCup = true;
	}
	else
	{
		_bLeftCup = false;
	}
	
	//Right hand check
	if(RightHand_.y < Spin.y)
	{
		_bRightCup = true;
	}
	else
	{
		_bRightCup = false;
	}
}
#pragma endregion

#pragma region Character object update
//--------------------------------------------------------------
void CharacterMoney::updateCharacterObj(CharacterObj& Obj, SkeletonHandler& SkeletonHandler)
{
	if(Obj.getName() == NAME_MGR::C_Money_Hat)
	{
		ofVec2f Head_ = SkeletonHandler.getJoints(_JointType::JointType_Head);
		ofVec2f Shoulder_ = SkeletonHandler.getJoints(_JointType::JointType_SpineShoulder);
		float HeadAngle_ = (Head_ - Shoulder_).angle(ofVec2f(0, -1));
		Obj.update(Head_, HeadAngle_);
	}
	else if(Obj.getName() == NAME_MGR::C_Money_Cup_L)
	{
		Obj.setDisplay(_bLeftCup);
		if(!_bLeftCup)
		{
			return;
		}
		ofVec2f LeftHand_ = SkeletonHandler.getJoints(_JointType::JointType_HandLeft);
		Obj.update(LeftHand_, 0.0);
	}
	else if(Obj.getName() == NAME_MGR::C_Money_Cup_R)
	{
		Obj.setDisplay(_bRightCup);

		if(!_bRightCup)
		{
			return;
		}
		ofVec2f RightHand_ = SkeletonHandler.getJoints(_JointType::JointType_HandRight);
		Obj.update(RightHand_, 0.0);
	}
}
#pragma endregion

#pragma region Teaching
//--------------------------------------------------------------
void CharacterMoney::setupTeaching()
{
	_eTeachingState = eTEACHING_START;
	_iPictureCounter = 0;
	_fPictureTimer = _fPirecureInterval = cALIEN_PICTURE_INTERVAL;
	_fTeachingTimer = 3.0;
}

//--------------------------------------------------------------
void CharacterMoney::updateTeaching(float fDelta, SkeletonHandler& SkeletonHandler)
{
	if(!SkeletonHandler.getHaveUser())
	{
		return;
	}

	this->gestureCheck(SkeletonHandler);
	_SyceeManager.setLeftCheck(_bLeftCup);
	_SyceeManager.setRightCheck(_bRightCup);
	_SyceeManager.update(fDelta, SkeletonHandler);

	_fTeachingTimer -= fDelta;
	if(_fTeachingTimer <= 0.0)
	{
		_fTeachingTimer = 3.0;
		if(_eTeachingState == eTEACHING_START)
		{
			_SyceeManager.addSycee(cWINDOW_WIDTH/2 - 200);
		}
		else if(_eTeachingState == eTEACHING_PASS1)
		{
			_SyceeManager.addSycee(cWINDOW_WIDTH/2 + 200);
		}		
	}

	this->takePicture(fDelta);
}

//--------------------------------------------------------------
void CharacterMoney::drawTeaching()
{
	_SyceeManager.draw();
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
	if(!SkeletonHandler.getHaveUser())
	{
		return;
	}

	this->gestureCheck(SkeletonHandler);
	_SyceeManager.setLeftCheck(_bLeftCup);
	_SyceeManager.setRightCheck(_bRightCup);
	_SyceeManager.update(fDelta, SkeletonHandler);
}

//--------------------------------------------------------------
void CharacterMoney::drawGaming()
{
	_SyceeManager.draw();
}
#pragma endregion