
#include "ArrowShooter.h"

#pragma region STRUCT Arrow Point
//--------------------------------------------------------------
//Arrow Point
ofEvent<bool> stArrowPoint::ArrowEvent = ofEvent<bool>();

void stArrowPoint::update(float fDelta, ofVec2f& SpearStart, ofVec2f& SpearEnd, ofVec2f& RightPos, ofRectangle& bodyRect)
{
	if(!bLive)
	{
		return;
	}
	AnimAlpha.update(fDelta);
	if(bBroke)
	{
		if(AnimAlpha.getPercentDone() == 1.0 && AnimAlpha.hasFinishedAnimating())
		{
			bLive = false;
		}
		return;
	}
	
	Position += Velocity * fDelta;		
	if(bodyRect.inside(Position))
	{
		//Hit the body
		bLive = false;
		bool bDefend_ = false;
		ofNotifyEvent(ArrowEvent, bDefend_);
		
		//Audio
		AudioMgr::GetInstance()->playAudio(AUDIO_NAME_MGR::A_ROMA_HIT);
		return;
	}

	if(Velocity.x > 0)
	{
		//Spear
		ofVec2f v1_ = Position - SpearStart;
		ofVec2f v2_ = SpearEnd - SpearStart;
		float fRadian_ = v1_.angleRad(v2_);
		float fDistSpear_ = fabs(v1_.length() * v2_.length() * sin(fRadian_))/SpearStart.distance(SpearEnd);

		float fSpearMinY_ = min(SpearStart.y, SpearEnd.y);
		float fSpearMaxY_ = max(SpearStart.y, SpearEnd.y);

		if(fDistSpear_ <= cARROW_DEFEND_SPEAR_DIST && Position.y >= fSpearMinY_ && Position.y <= fSpearMaxY_)
		{
			//Defend success
			bBroke = true;
			AnimAlpha.animateFromTo(255, 0);
			bool bDefend_ = true;
			ofNotifyEvent(ArrowEvent, bDefend_);

			//Audio
			AudioMgr::GetInstance()->playAudio(AUDIO_NAME_MGR::A_ROMA_DEFENCE);
		}
	}
	else
	{
		//Shield
		float fDistHandX_ = abs(Position.x - RightPos.x);
		float fDistHandY_ = abs(Position.y - RightPos.y);

		if(fDistHandX_ <= cARROW_TRIGGER_SHIELD_X && fDistHandY_ <= cARROW_DEFEND_SHIELD_Y)
		{
			//Defend success
			bBroke = true;
			AnimAlpha.animateFromTo(255, 0);
			bool bDefend_ = true;
			ofNotifyEvent(ArrowEvent, bDefend_);

			//Audio
			AudioMgr::GetInstance()->playAudio(AUDIO_NAME_MGR::A_ROMA_DEFENCE);
		}
	}
}

#pragma endregion

#pragma region CLASS Arrow Shooter
//--------------------------------------------------------------
//@CLASS ArrowShooter
void ArrowShooter::setup()
{
	_ArrowImg.loadImage("Roma/arrow.png");
	_Anchor = ofVec2f(273, 32);

	_ShooterPos[eSHOOT_TOP] = 150;
	_ShooterPos[eSHOOT_MIDDLE] = 450;
	_ShooterPos[eSHOOT_BOTTOM] = 750;
}

//--------------------------------------------------------------
void ArrowShooter::update(float fDelta, SkeletonHandler& SkeletonHandler, float fSpearSize)
{
	ofVec2f RightHand_ = SkeletonHandler.getJoints(_JointType::JointType_HandRight);
	ofVec2f LeftHand_ = SkeletonHandler.getJoints(_JointType::JointType_HandLeft);
	ofVec2f SpearVec_ = (LeftHand_ - SkeletonHandler.getJoints(_JointType::JointType_ElbowLeft)).normalize();
	ofVec2f SpearStart_ = LeftHand_ + (SpearVec_ * (fSpearSize * cSPEAR_HAND_RATIO));
	ofVec2f SpearEnd_ = LeftHand_ + ((-SpearVec_) * (fSpearSize * (1 - cSPEAR_HAND_RATIO)));

	ofVec2f Spine_ = SkeletonHandler.getJoints(_JointType::JointType_SpineMid);
	float BodyWidth_ = (SkeletonHandler.getJoints(_JointType::JointType_ShoulderLeft).x - SkeletonHandler.getJoints(_JointType::JointType_ShoulderRight).x);
	float BodyHeight_ = (SkeletonHandler.getJoints(_JointType::JointType_Head).y - SkeletonHandler.getJoints(_JointType::JointType_FootLeft).y);
	ofRectangle BodyRect_;
	BodyRect_.setFromCenter(Spine_, BodyWidth_, BodyHeight_);
	
	//update arrow
	auto Iter_ = _ArrowList.begin();
	while(Iter_ != _ArrowList.end())
	{
		Iter_->update(fDelta, SpearStart_, SpearEnd_, RightHand_, BodyRect_);

		if(!Iter_->bLive)
		{
			Iter_ = _ArrowList.erase(Iter_);
		}
		else
		{
			++Iter_;
		}
	}
	
	
	//auto add new arrow
	if(!_bStartAutoShoot)
	{
		return;
	}
	_fShooterTimer -= fDelta;
	if(_fShooterTimer <= 0.0)
	{
		eSHOOT_TARGET_TYPE eType_ = static_cast<eSHOOT_TARGET_TYPE>(rand() % eSHOOT_NUM);
		ofVec2f Target_;
		switch(eType_)
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
		_fShooterTimer = ofRandom(cAUTO_SHOOT_TIME.first, cAUTO_SHOOT_TIME.second);
		Target_.y += ofRandom(cARROW_TARGET_RANGE_Y.first, cARROW_TARGET_RANGE_Y.second);
		this->shoot(Target_, eType_);
	}
}

//--------------------------------------------------------------
void ArrowShooter::draw()
{
	ofPushStyle();

	ofEnableAlphaBlending();
	
	for(auto& Iter_ : _ArrowList)
	{
		if(!Iter_.bLive)
		{
			continue;
		}
		
		ofSetColor(255, 255, 255, Iter_.AnimAlpha.getCurrentValue());
		ofPushMatrix();
		ofTranslate(Iter_.Position);
		ofRotateZ(-Iter_.fDegree);
		_ArrowImg.draw(-_Anchor);
		ofPopMatrix();
	}
	ofPopStyle();
}

//--------------------------------------------------------------
void ArrowShooter::clear()
{
	_ArrowList.clear();
	_bStartAutoShoot = false;
	_fShooterTimer = 0.0;
}

//--------------------------------------------------------------
void ArrowShooter::shoot(ofVec2f Start, ofVec2f End, float fDuration)
{
	stArrowPoint NewArrow_(Start, End, fDuration);
	_ArrowList.push_back(NewArrow_);
	AudioMgr::GetInstance()->playAudio(AUDIO_NAME_MGR::A_ROMA_SHOOT);
}

//--------------------------------------------------------------
void ArrowShooter::shoot(ofVec2f End, eSHOOT_TARGET_TYPE eType)
{
	if(_ShooterPos.size() == 0)
	{
		ofLog(OF_LOG_ERROR, "In ArrowShooter, No default position");
		return;
	}

	ofVec2f Start_;
	Start_.y = _ShooterPos[eType] + ofRandom(-cARROW_START_RANDOM, cARROW_START_RANDOM);

	if(ofRandom(-1, 1) > 0)
	{
		Start_.x = -cARROW_START_X;
	}
	else
	{
		Start_.x = cWINDOW_WIDTH + cARROW_START_X;
	}

	this->shoot(Start_, End, ofRandom(cAUTO_SHOOT_TIME.first, cAUTO_SHOOT_TIME.second));
}

//--------------------------------------------------------------
void ArrowShooter::shoot(ofVec2f End, eSHOOT_TARGET_TYPE eType, float fDuration, bool bFromLeft)
{
	if(_ShooterPos.size() == 0)
	{
		ofLog(OF_LOG_ERROR, "In ArrowShooter, No default position");
		return;
	}

	ofVec2f Start_;
	Start_.y = _ShooterPos[eType] + ofRandom(-cARROW_START_RANDOM, cARROW_START_RANDOM);

	if(bFromLeft)
	{
		Start_.x = -cARROW_START_X;
	}
	else
	{
		Start_.x = cWINDOW_WIDTH + cARROW_START_X;
	}

	this->shoot(Start_, End,  fDuration);
}
#pragma endregion