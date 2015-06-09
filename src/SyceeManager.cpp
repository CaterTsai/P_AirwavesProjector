#include "SyceeManager.h"

#pragma region stSyceePoint
//--------------------------------------------------------------
void stSyceePoint::update(float fDelta)
{
	AnimFalling.update(fDelta);
	AnimScale.update(fDelta);
	switch(eState)
	{
	case eSYCEE_FALLING:
		{
			Position.y = AnimFalling.getCurrentValue();
			if(AnimFalling.hasFinishedAnimating() && AnimFalling.getPercentDone() == 1.0)
			{
				eState = eSYCEE_CAN_REMOVE;
			}
		}
		break;
	case eSYCEE_CATCH:
		{
			fScale = AnimScale.getCurrentValue();
			if(AnimScale.hasFinishedAnimating() && AnimScale.getPercentDone() == 1.0)
			{
				eState = eSYCEE_CAN_REMOVE;
			}
		}
		break;
	case eSYCEE_CAN_REMOVE:
		{
		}
		break;
	default:
		{
			//unknow state
			eState = eSYCEE_CAN_REMOVE;
		}
	}
}
#pragma endregion

#pragma region SyceeManager
//--------------------------------------------------------------
void SyceeManager::setup()
{
	_bStartAuto = false;

	_SyceeImg.loadImage("Money/sycee.png");

	_bLeftCheck = _bRightCheck = false;
	_fCreateTimer = 1.0;
}

//--------------------------------------------------------------
void SyceeManager::update(float fDelta, SkeletonHandler& SkeletonHandler)
{	
	ofVec2f LeftHand_ = SkeletonHandler.getJoints(_JointType::JointType_HandLeft);
	ofVec2f RightHand_ = SkeletonHandler.getJoints(_JointType::JointType_HandRight);
	
	//Update all sycee
	for(auto& Iter_ : _SyceeList)
	{
		Iter_.update(fDelta);

		//check left hand
		if(_bLeftCheck && 
			Iter_.eState == eSYCEE_FALLING &&
			(LeftHand_.y > Iter_.Position.y) &&
			(LeftHand_.y - Iter_.Position.y) <  cSYCEE_CATCH_DISTANCE_Y &&
			abs(LeftHand_.x - Iter_.Position.x) < cSYCEE_CATCH_DISTANCE_X)
		{
			Iter_.eState = eSYCEE_CATCH;
			Iter_.AnimScale.animateFromTo(1.0, 0.0);
			//Trigger event
			string strMsg_ = "catch";
			ofNotifyEvent(_MoneyEvent, strMsg_, this);

			AudioMgr::GetInstance()->playAudio(AUDIO_NAME_MGR::A_MONEY_CATCH);
		}

		//check right hand
		if(_bRightCheck && 
			Iter_.eState == eSYCEE_FALLING &&
			(RightHand_.y >= Iter_.Position.y) &&
			(RightHand_.y - Iter_.Position.y) <  cSYCEE_CATCH_DISTANCE_Y &&
			abs(RightHand_.x - Iter_.Position.x) < cSYCEE_CATCH_DISTANCE_X)
		{
			Iter_.eState = eSYCEE_CATCH;
			Iter_.AnimScale.animateFromTo(1.0, 0.0);
			//Trigger event
			string strMsg_ = "catch";
			ofNotifyEvent(_MoneyEvent, strMsg_, this);

			AudioMgr::GetInstance()->playAudio(AUDIO_NAME_MGR::A_MONEY_CATCH);
		}
	}

	//Remove sycee
	auto Iter_ = _SyceeList.begin();
	while(Iter_ != _SyceeList.end())
	{
		if(Iter_->eState == eSYCEE_CAN_REMOVE)
		{
			Iter_ = _SyceeList.erase(Iter_);
		}
		else
		{
			Iter_++;
		}
	}

	//Auto
	if(_bStartAuto)
	{
		_fCreateTimer -= fDelta;
		if(_fCreateTimer <= 0.0)
		{
			_fCreateTimer = ofRandom(cSYCEE_CREATE_CURATION.first, cSYCEE_CREATE_CURATION.second);
			this->addSycee();
		}
	}
}

//--------------------------------------------------------------
void SyceeManager::draw()
{
	ofPushStyle();
	ofSetColor(255);
	ofEnableAlphaBlending();

	for(auto& Sycee_ : _SyceeList)
	{
		ofPushMatrix();
		{
			ofTranslate(Sycee_.Position);
			ofRotateZ(Sycee_.fDegree);

			float fScaleWidth_ = _SyceeImg.width * Sycee_.fScale;
			float fScaleHeight_ = _SyceeImg.height * Sycee_.fScale;

			_SyceeImg.draw(-fScaleWidth_/2, -fScaleHeight_/2, fScaleWidth_, fScaleHeight_);
		}
		ofPopMatrix();
	}

	ofPopStyle();
}

//--------------------------------------------------------------
void SyceeManager::clear()
{
	//clear Sycee manager
	_SyceeList.clear();

	//reset
	_fCreateTimer = 1.0;
}

//--------------------------------------------------------------
void SyceeManager::addSycee(float fPosX)
{
	stSyceePoint NewSycee_(ofVec2f(fPosX, 0));
	_SyceeList.push_back(NewSycee_);
}
#pragma endregion