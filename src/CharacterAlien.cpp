#include "CharacterAlien.h"

void CharacterAlien::setupCharacter()
{
	//Alien Head
	CharacterObj	AlienHeadObj_;
	AlienHeadObj_.setup(NAME_MGR::C_Alien_Head, "Alien/head.mov", configLoader::GetInstance()->_exAlienHeadAnchor, configLoader::GetInstance()->_exAlienHeadScale);

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

//--------------------------------------------------------------
void CharacterAlien::startGame()
{
	if(_eState == eCHARACTER_GAMING)
	{
		_AlienCatcher.setAutoCreate(true);
	}
}

#pragma region Alien Cathcer
//--------------------------------------------------------------
void CharacterAlien::onLastAlien(string& e)
{
	if(_eState == eCHARACTER_TEACHING && _eTeachingState == eTEACHING_PASS1)
	{
		_eTeachingState = eTEACHING_FINISH;
		_eState = eCHARACTER_GAMING;

		//Event
		pair<string, string> Event_ = make_pair(NAME_MGR::EVENT_TeachingFinish, ofToString(eCHARACTER_ALIEN));
		ofNotifyEvent(IBaseCharacter::CharacterEvent, Event_);
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
			CtrlPos_ = ofMap(LeftHand_.x, fMin_, fMax_, 0, cWINDOW_WIDTH, true);

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
			CtrlPos_ = ofMap(RightHand_.x, fMin_, fMax_, 0, cWINDOW_WIDTH, true);
			
			if(RightHand_.y > ShoulderLeft_.y || RightHand_.y > ShoulderRight_.y)
			{
				_eHandState = eNO_LOCKON;
			}
		}
		break;
	}

	if(_eHandState != eNO_LOCKON && _eTeachingState == eTEACHING_START && CtrlPos_ >= 0)
	{
		_eTeachingState = eTEACHING_PASS1;
		float fUFOPos_ = _AlienCatcher.getUFOPos();
		if(fUFOPos_ > (cWINDOW_WIDTH/2))
		{
			_AlienCatcher.addAlien(fUFOPos_ - cALINE_MIN_DIST);
		}
		else
		{
			_AlienCatcher.addAlien(fUFOPos_ + cALINE_MIN_DIST);
		}
		
		//Event
		pair<string, string> Event_ = make_pair(NAME_MGR::EVENT_TeachingCheck, ofToString(eCHARACTER_ALIEN));
		ofNotifyEvent(IBaseCharacter::CharacterEvent, Event_);
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
	//this->addTeachingAlien();
	_eHandState = eNO_LOCKON;
	_eTeachingState	=	eTEACHING_WAIT;
	_iPictureCounter = 0;
	_fPictureTimer = _fPirecureInterval = cALIEN_PICTURE_INTERVAL;
	_fStartTimer = 3.0;
}

//--------------------------------------------------------------
void CharacterAlien::updateTeaching(float fDelta, SkeletonHandler& SkeletonHandler)
{	
	float fCtrlX_ = this->getCtrlPos(SkeletonHandler);
	_AlienCatcher.update(fDelta, fCtrlX_);

	if(_eTeachingState != eTEACHING_WAIT)
	{
		this->takePicture(fDelta);
	}
	else
	{
		_fStartTimer -= fDelta;
		if(_fStartTimer <= 0.0)
		{
			_eTeachingState = eTEACHING_START;
		}
	}
}

//--------------------------------------------------------------
void CharacterAlien::drawTeaching()
{
	ofPushStyle();
	{
		//ofSetColor(255, 0, 0);
		//ofCircle(100, 100, 50);

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
		//ofSetColor(0, 255, 0);
		//ofCircle(100, 100, 50);

		_AlienCatcher.draw();
	}
	ofPopStyle();
}
#pragma endregion