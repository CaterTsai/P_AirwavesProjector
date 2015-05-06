#include "CharacterAngel.h"

void CharacterAngel::setupCharacter()
{
	//Wings
	CharacterObj	WingsObj_;
	WingsObj_.setup(NAME_MGR::C_Angel_Wings, "Angel/wings.mov", configLoader::GetInstance()->_exAngelWingsAnchor, configLoader::GetInstance()->_exAngelWingsScale);

	//Ring
	CharacterObj	RingObj_;
	RingObj_.setup(NAME_MGR::C_Angel_Ring, "Angel/ring.mov", configLoader::GetInstance()->_exAngelRingAnchor, configLoader::GetInstance()->_exAngelRingScale);

	_ObjectList.push_back(WingsObj_);
	_ObjectList.push_back(RingObj_);

	//Heart Manager
	_HeartManager.setup();
	ofAddListener(_HeartManager.HeartEvent, this, &CharacterAngel::onHeartHit);

	_eHandState = eBOTH_HAND_WAIT;
	_GestureCounter = 0;
	fHeartTimer_ = 0.0;
	_bIsSetup = true;
}

//--------------------------------------------------------------
void CharacterAngel::reset()
{
	_eHandState = eBOTH_HAND_WAIT;
	_GestureCounter = 0;
	fHeartTimer_ = 0.0;

	_HeartManager.clear();
}

//--------------------------------------------------------------
void CharacterAngel::startGame()
{
	if(_eState == eCHARACTER_GAMING)
	{
		_bStartGaming = true;
	}
}

#pragma region Heart Manager
//--------------------------------------------------------------
void CharacterAngel::onHeartHit(string& e)
{
	if(_eState == eCHARACTER_TEACHING && _eTeachingState == eTEACHING_PASS1)
	{
		_eTeachingState = eTEACHING_FINISH;

		//Event
		pair<string, string> Event_ = make_pair(NAME_MGR::EVENT_TeachingFinish, ofToString(eCHARACTER_ANGEL));
		ofNotifyEvent(IBaseCharacter::CharacterEvent, Event_);
	}
}

//--------------------------------------------------------------
void CharacterAngel::addFloatingHeart(int iNum)
{
	ofVec2f Vec_(1, 0);
	float fBaseDegree_ = 360.0 / iNum;
	float fDegreeRange_ = fBaseDegree_/3.0;
	Vec_.rotate(ofRandom(0, 359)).normalize();
	for(int idx_ = 0; idx_ < iNum; idx_++)
	{
		//random in window
		//_HeartManager.addFloting( ofVec2f(ofRandom(0, cWINDOW_WIDTH), ofRandom(0, cWINDOW_HEIGHT)) );

		//random in ring
		Vec_.rotate(fBaseDegree_ * idx_ + ofRandom(-fDegreeRange_, fDegreeRange_)).normalize();
		float fDist_ = cWINDOW_HEIGHT/2 * ofRandom(cFLOATING_HEART_DIST.first, cFLOATING_HEART_DIST.second);
		_HeartManager.addFloting( ofVec2f(cWINDOW_WIDTH/2, cWINDOW_HEIGHT/2) + (Vec_ * fDist_));
	}
}

//--------------------------------------------------------------
bool CharacterAngel::gestureCheck(SkeletonHandler& SkeletonHandler)
{
	ofVec2f LeftHand_ = SkeletonHandler.getJoints(_JointType::JointType_HandLeft);
	ofVec2f RightHand_ = SkeletonHandler.getJoints(_JointType::JointType_HandRight);
	ofVec2f Shoulder_ = SkeletonHandler.getJoints(_JointType::JointType_SpineShoulder);
	bool bResult_ = false;
	if(_eHandState == eBOTH_HAND_WAIT)
	{
		if(LeftHand_.y > Shoulder_.y && RightHand_.y > Shoulder_.y)
		{
			_GestureCounter++;
		}
		else
		{
			_GestureCounter = 0;
		}
		
		if(_GestureCounter >= 3)
		{
			_eHandState = eBOTH_HAND_DOWN;
		}
	}
	else if(_eHandState == eBOTH_HAND_DOWN)
	{
		if(LeftHand_.y <= Shoulder_.y && RightHand_.y <= Shoulder_.y)
		{
			_GestureCounter++;
		}
		else
		{
			_GestureCounter = 0;
		}

		if(_GestureCounter >= 3)
		{
			_eHandState = eBOTH_HAND_UP;
		}
	}
	else if(_eHandState == eBOTH_HAND_UP)
	{
		AudioMgr::GetInstance()->playAudio(AUDIO_NAME_MGR::A_ANGEL_FLY);
		_eHandState = eBOTH_HAND_WAIT;
		bResult_ = true;
	}

	return bResult_;
}

#pragma endregion

#pragma region Character Object Update
//--------------------------------------------------------------
void CharacterAngel::updateCharacterObj(CharacterObj& Obj, SkeletonHandler& SkeletonHandler)
{
	if(Obj.getName() == NAME_MGR::C_Angel_Ring)
	{
		ofVec2f Head_ = SkeletonHandler.getJoints(_JointType::JointType_Head);
		ofVec2f Shoulder_ = SkeletonHandler.getJoints(_JointType::JointType_SpineShoulder);

		float HeadAngle_ = (Head_ - Shoulder_).angle(ofVec2f(0, -1));

		Obj.update(Head_ + ofVec2f(0, -50), HeadAngle_);
	}
	else if(Obj.getName() == NAME_MGR::C_Angel_Wings)
	{
		ofVec2f Shoulder_ = SkeletonHandler.getJoints(_JointType::JointType_SpineShoulder);
		ofVec2f ShoulderRight_ = SkeletonHandler.getJoints(_JointType::JointType_ShoulderRight);
		ofVec2f ShoulderLeft_ = SkeletonHandler.getJoints(_JointType::JointType_ShoulderLeft);

		float WingsAngle_ = (ShoulderRight_ - ShoulderLeft_).angle(ofVec2f(1, 0));

		Obj.update(Shoulder_, WingsAngle_);
	}
}
#pragma endregion

#pragma region Teaching
//--------------------------------------------------------------
void CharacterAngel::setupTeaching()
{
	_bStartTimer = true;
	_fTeachingTimer = 2.0;
	//_bStartTimer = true;
	_eTeachingState = eTEACHING_WAIT;
	_iPictureCounter = 0;
	_fPictureTimer = _fPirecureInterval = cANGEL_PICTURE_INTERVAL;
}

//--------------------------------------------------------------
void CharacterAngel::updateTeaching(float fDelta, SkeletonHandler& SkeletonHandler)
{
	if(!SkeletonHandler.getHaveUser())
	{
		_eHandState = eBOTH_HAND_WAIT;
		return;
	}

	_HeartManager.update(fDelta);

	if(_eTeachingState == eTEACHING_FINISH)
	{
		_HeartManager.clear();
		_eState = eCHARACTER_GAMING;
		return;
	}
	else if(_eTeachingState == eTEACHING_START || _eTeachingState == eTEACHING_PASS1)
	{
		if(gestureCheck(SkeletonHandler))
		{
			//add fly heart
			ofVec2f Body_ = SkeletonHandler.getJoints(_JointType::JointType_SpineMid);
		
			int iNum_ = (rand() % (cFLYING_HEART_LIMIT.second - cFLYING_HEART_LIMIT.first)) + cFLYING_HEART_LIMIT.first;
			ofVec2f Direction_ = ofVec2f(0, -1).rotate(ofRandom(0, 359));
			for(int idx_ = 0; idx_ < iNum_; ++idx_)
			{
				_HeartManager.addFlying(Body_, Direction_ * cWINDOW_WIDTH, ofRandom(cFLYING_HEART_DURATION.first, cFLYING_HEART_DURATION.second));
				Direction_.rotate(ofRandom(cFLYING_HEART_DEGREE.first, cFLYING_HEART_DEGREE.second));
				Direction_.normalize();
			}

			//add special heart
			ofVec2f Pos_;
			if(_HeartManager.getFloatingPos(0, Pos_))
			{
				_HeartManager.addFlying(Body_, Pos_, 1.5);
			}

			if(_eTeachingState == eTEACHING_START)
			{
				//Event
				pair<string, string> Event_ = make_pair(NAME_MGR::EVENT_TeachingCheck, ofToString(eCHARACTER_ANGEL));
				ofNotifyEvent(IBaseCharacter::CharacterEvent, Event_);

				_eTeachingState = eTEACHING_PASS1;
				_bStartTimer = true;
				_fTeachingTimer = 2.0;
			}
		}
	}
	this->takePicture(fDelta);

	//Timer
	if(_bStartTimer)
	{
		_fTeachingTimer -= fDelta;
		if(_fTeachingTimer > 0.0)
		{
			return;
		}

		_bStartTimer = false;
		if(_eTeachingState == eTEACHING_WAIT)
		{
			_eTeachingState = eTEACHING_START;
		}
		else if(_eTeachingState == eTEACHING_PASS1)
		{
			this->addFloatingHeart(cTEACHING_FLOATING_NUM);
		}
	}
}

//--------------------------------------------------------------
void CharacterAngel::drawTeaching()
{
	ofPushStyle();
	{
		//ofSetColor(255, 0, 0);
		//ofCircle(100, 100, 50);

		_HeartManager.draw();
	}
	ofPopStyle();
}

#pragma endregion

#pragma region Gaming
//--------------------------------------------------------------
void CharacterAngel::setupGaming()
{
	_fGameFloatTimer = 1.0;
	_bStartGaming = false;
}

//--------------------------------------------------------------
void CharacterAngel::updateGaming(float fDelta, SkeletonHandler& SkeletonHandler)
{
	if(!SkeletonHandler.getHaveUser())
	{
		_eHandState = eBOTH_HAND_WAIT;
		return;
	}
	_HeartManager.update(fDelta);

	if(gestureCheck(SkeletonHandler))
	{
		//add fly heart
		ofVec2f Body_ = SkeletonHandler.getJoints(_JointType::JointType_SpineMid);
		
		int iNum_ = (rand() % (cFLYING_HEART_LIMIT.second - cFLYING_HEART_LIMIT.first)) + cFLYING_HEART_LIMIT.first;
		ofVec2f Direction_ = ofVec2f(0, -1).rotate(ofRandom(0, 359));
		for(int idx_ = 0; idx_ < iNum_; ++idx_)
		{
			_HeartManager.addFlying(Body_, Direction_ * cWINDOW_WIDTH, ofRandom(cFLYING_HEART_DURATION.first, cFLYING_HEART_DURATION.second));
			Direction_.rotate(ofRandom(cFLYING_HEART_DEGREE.first, cFLYING_HEART_DEGREE.second));
		}
	}

	//Auto shoot floating heart
	if(!_bStartGaming)
	{
		return;
	}
	_fGameFloatTimer -= fDelta;
	if(_fGameFloatTimer <= 0.0)
	{	
		_fGameFloatTimer = ofRandom(cFLOATING_HEART_TIME.first, cFLOATING_HEART_TIME.second);
		if(_HeartManager.getFloatingNum() < cMAX_FLOATING_HEART)
		{
			this->addFloatingHeart(cGAMING_FLOATING_NUM);
		}
	}

}

//--------------------------------------------------------------
void CharacterAngel::drawGaming()
{
	ofPushStyle();
	{
		//ofSetColor(0, 255, 0);
		//ofCircle(100, 100, 50);

		_HeartManager.draw();
	}
	ofPopStyle();
}

#pragma endregion