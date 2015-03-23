#include "CharacterAngel.h"

void CharacterAngel::setupCharacter()
{
	//Wings
	CharacterObj	WingsObj_;
	WingsObj_.setup(NAME_MGR::C_Angel_Wings, "Angel/wings.jpg", ofVec2f(350, 127), 0.7);

	//Ring
	CharacterObj	RingObj_;
	RingObj_.setup(NAME_MGR::C_Angel_Ring, "Angel/ring.jpg", ofVec2f(172, 171), 0.2);

	_ObjectList.push_back(WingsObj_);
	_ObjectList.push_back(RingObj_);

	//Heart Manager
	_HeartManager.setup();
	ofAddListener(_HeartManager.HeartEvent, this, &CharacterAngel::onHeartHit);

	//Teaching
	this->setupTeaching();

	//Gaming
	this->setupGaming();

	_eHandState = eBOTH_HAND_WAIT;
	_GestureCounter = 0;
	fHeartTimer_ = 0.0;
	TeachingCounter_ = 0;
	_bIsSetup = true;
}

#pragma region Heart Manager
//--------------------------------------------------------------
void CharacterAngel::onHeartHit(string& e)
{
	if(_eState == eCHARACTER_TEACHING)
	{
		TeachingCounter_--;
	}
}

//--------------------------------------------------------------
void CharacterAngel::addFloatingHeart(int iNum)
{
	for(int idx_ = 0; idx_ < iNum; idx_++)
	{
		_HeartManager.addFloting( ofVec2f(ofRandom(0, cWINDOW_WIDTH), ofRandom(0, cWINDOW_HEIGHT)) );
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
	fTeachingTimer_ = 5.0;
	bStartTeaching_ = false;
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
	
	if(!bStartTeaching_)
	{
		fTeachingTimer_ -= fDelta;
		if(fTeachingTimer_ < 0.0)
		{
			this->addFloatingHeart(cTEACHING_FLOATING_NUM);
			TeachingCounter_ = cTEACHING_FLOATING_NUM;
			bStartTeaching_ = true;
		}
	}
	else
	{
		if(TeachingCounter_ == 0)
		{
			bStartTeaching_ = false;
			_eState = eCHARACTER_GAMING;
		}
	}
}

//--------------------------------------------------------------
void CharacterAngel::drawTeaching()
{
	ofPushStyle();
	{
		ofSetColor(255, 0, 0);
		ofCircle(100, 100, 50);

		_HeartManager.draw();
	}
	ofPopStyle();
}

#pragma endregion

#pragma region Gaming
//--------------------------------------------------------------
void CharacterAngel::setupGaming()
{
	fGameFloatTimer_ = 2.0;
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
	fGameFloatTimer_ -= fDelta;
	if(fGameFloatTimer_ <= 0.0)
	{	
		fGameFloatTimer_ = ofRandom(cFLOATING_HEART_TIME.first, cFLOATING_HEART_TIME.second);
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
		ofSetColor(0, 255, 0);
		ofCircle(100, 100, 50);

		_HeartManager.draw();
	}
	ofPopStyle();
}

#pragma endregion