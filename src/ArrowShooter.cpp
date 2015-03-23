
#include "ArrowShooter.h"

#pragma region STRUCT Arrow Point
//--------------------------------------------------------------
//Arrow Point
ofEvent<bool> stArrowPoint::ArrowEvent = ofEvent<bool>();

void stArrowPoint::update(float fDelta, ofVec2f& LeftPos, ofVec2f& RightPos, ofRectangle& bodyRect)
{
	if(!bLive)
	{
		return;
	}

	Position += Velocity * fDelta;
	
	ofVec2f ActivePos_;
	float fDistHandX_ = 0.0;
	
	if(Velocity.x > 0)
	{
		ActivePos_ = LeftPos;
		fDistHandX_ = ActivePos_.x - Position.x;
	}
	else
	{
		ActivePos_ = RightPos;
		fDistHandX_ = Position.x - ActivePos_.x;
	}
		
	if(bodyRect.inside(Position))
	{
		//Hit the body
		bLive = false;
		bool bDefend_ = false;
		ofNotifyEvent(ArrowEvent, bDefend_);
	}

	if(fDistHandX_ > cARROW_TRIGGER_DISTANCE_X)
	{
		//Arrow isn't in trigger area
		return;
	}
	
	float fDistHandY_ = abs(Position.y - ActivePos_.y);
	if(fDistHandY_ <= cARROW_DEFEND_DISTANCE_Y)
	{
		//Defend success
		bLive = false;
		bool bDefend_ = true;
		ofNotifyEvent(ArrowEvent, bDefend_);
	}
}

#pragma endregion

#pragma region CLASS Arrow Shooter
//--------------------------------------------------------------
//@CLASS ArrowShooter
void ArrowShooter::setup()
{
	_ArrowImg.loadImage("Roma/arrow.jpg");
	_Anchor = ofVec2f(300, 30);

	_ShooterPos[eSHOOT_TOP] = 150;
	_ShooterPos[eSHOOT_MIDDLE] = 450;
	_ShooterPos[eSHOOT_BOTTOM] = 750;
}

//--------------------------------------------------------------
void ArrowShooter::update(float fDelta, SkeletonHandler& SkeletonHandler)
{
	ofVec2f LeftHand_ = SkeletonHandler.getJoints(_JointType::JointType_HandLeft);
	ofVec2f RightHand_ = SkeletonHandler.getJoints(_JointType::JointType_HandRight);

	ofVec2f Spine_ = SkeletonHandler.getJoints(_JointType::JointType_SpineMid);
	float BodyWidth_ = (SkeletonHandler.getJoints(_JointType::JointType_ShoulderLeft).x - SkeletonHandler.getJoints(_JointType::JointType_ShoulderRight).x);
	float BodyHeight_ = (SkeletonHandler.getJoints(_JointType::JointType_Head).y - SkeletonHandler.getJoints(_JointType::JointType_FootLeft).y);
	ofRectangle BodyRect_;
	BodyRect_.setFromCenter(Spine_, BodyWidth_, BodyHeight_);
	
	//update arrow
	auto Iter_ = _ArrowList.begin();
	while(Iter_ != _ArrowList.end())
	{
		Iter_->update(fDelta, LeftHand_, RightHand_, BodyRect_);

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
		this->shoot(Target_, eType_);
	}
}

//--------------------------------------------------------------
void ArrowShooter::draw()
{
	ofPushStyle();

	ofSetColor(255);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);
	for(auto& Iter_ : _ArrowList)
	{
		if(!Iter_.bLive)
		{
			continue;
		}
		
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