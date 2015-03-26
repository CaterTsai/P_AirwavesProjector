#include "HeartManager.h"

#pragma region STRUCT FloatHeartPoint
void stFloatHeartPoint::update(float fDelta)
{
	switch(eState)
	{
	case eHEART_FLOATING:
		{
			//update Position
			Position += Velocity * fDelta;

			//update velocity
			Velocity.x += ofRandom(-1, 1);
			Velocity.y += ofRandom(-1, 1);

			//edge check
			if(	Position.x < 0 )
			{
				Position.x = 0;
				Velocity.x = abs(Velocity.x);
			}
			else if(Position.x > cWINDOW_WIDTH)
			{
				Position.x = cWINDOW_WIDTH;
				Velocity.x = -abs(Velocity.x);
			}

			if(	Position.y < 0 )
			{
				Position.y = 0;
				Velocity.y = abs(Velocity.y);
			}
			else if(Position.y > cWINDOW_HEIGHT)
			{
				Position.y = cWINDOW_HEIGHT;
				Velocity.y = -abs(Velocity.y);
			}

			//update defgree
			if(fDegree < cFLOATING_HEART_DEGREE.first)
			{
				fDegree = cFLOATING_HEART_DEGREE.first;
				bClockwise = true;
			}
			else if(fDegree > cFLOATING_HEART_DEGREE.second)
			{
				fDegree = cFLOATING_HEART_DEGREE.second;
				bClockwise = false;
			}
			
			bClockwise?fDegree += 0.5 : fDegree -= 0.5;
			AnimFadeIn.update(fDelta);
		}
		break;
	case eHEART_TOUCHING:
		{
			AnimTouch.update(fDelta);
			AnimBigHeart.update(fDelta);

			fTouchScale = AnimTouch.getCurrentValue() * fScale;
			if(AnimBigHeart.getPercentDone() == 1.0 && AnimBigHeart.hasFinishedAnimating())
			{
				eState = eHEART_BIG_HEART;
				FadeoutTimer = cHEART_FADEOUT_TIME;
			}
		}
		break;
	case eHEART_BIG_HEART:
		{
			FadeoutTimer -= fDelta;
			if(FadeoutTimer <= 0)
			{
				eState = eHEART_FAREOUT;
				AnimFadeout.animateFromTo(255, 0);
			}
		}
		break;
	case eHEART_FAREOUT:
		{
			AnimFadeout.update(fDelta);
			if(AnimFadeout.getPercentDone() == 1.0 && AnimFadeout.hasFinishedAnimating())
			{
				eState = eHEART_CAN_REMOVE;
			}
		}
		break;
	case eHEART_CAN_REMOVE:
		{
		}
		break;
	}
}
#pragma endregion

#pragma region CLASS HeartManager
void HeartManager::setup()
{
	_FlyingHeartImg.loadImage("Angel/FlyHeart.jpg");
	_FloatHeartImg.loadImage("Angel/FloatingHeart.png");
	_BigHeartImg.loadImage("Angel/BigHeart.png");
}

//--------------------------------------------------------------
void HeartManager::update(float fDelta)
{
	//update flying heart
	auto FlyIter_ = _FlyingHeartList.begin();
	while(FlyIter_ != _FlyingHeartList.end())
	{
		FlyIter_->update(fDelta);

		if(FlyIter_->bLive)
		{
			FlyIter_++;
		}
		else
		{
			FlyIter_ = _FlyingHeartList.erase(FlyIter_);
		}
	}

	//update floating heart
	auto FloatIter_ = _FloatingHeartList.begin();
	while(FloatIter_ != _FloatingHeartList.end())
	{
		FloatIter_->update(fDelta);

		if(FloatIter_->eState == eHEART_CAN_REMOVE)
		{
			FloatIter_ = _FloatingHeartList.erase(FloatIter_);

			if(_FloatingHeartList.size() == 0)
			{
				//lastone event
				string strHit_ = "last hit";
				ofNotifyEvent(HeartEvent, strHit_, this);
			}
		}
		else
		{
			FloatIter_++;	
		}
	}

	//Check touch
	for(auto& FlyIter_ : _FlyingHeartList)
	{
		for(auto& FloatIter_ : _FloatingHeartList)
		{
			if(FloatIter_.eState != eHEART_FLOATING || !FlyIter_.bLive)
			{
				continue;
			}

			float fDist_ = FlyIter_.Position.distance(FloatIter_.Position);
			if(fDist_ > cHEART_TOUCH_DISTANCE)
			{
				continue;
			}

			FlyIter_.bLive = false;
			
			FloatIter_.eState = eHEART_TOUCHING;
			FloatIter_.AnimTouch.animateFromTo(1.0, 0.0);
			FloatIter_.AnimBigHeart.animateToAfterDelay(1.0, 0.1);
		}
	}
}

//--------------------------------------------------------------
void HeartManager::draw()
{
	ofPushStyle();
	
	ofSetColor(255);
	//Fly Heart
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);
	for(auto& Iter_ : _FlyingHeartList)
	{
		ofPushMatrix();
		ofTranslate(Iter_.Position);
		_FlyingHeartImg.draw(-_FlyingHeartImg.width/2, -_FlyingHeartImg.height/2);
		ofPopMatrix();
	}
	ofDisableBlendMode();

	//Float Heart
	ofEnableAlphaBlending();
	for(auto& Iter_ : _FloatingHeartList)
	{
		ofPushMatrix();
		ofTranslate(Iter_.Position);
		ofRotateZ(Iter_.fDegree);
		switch(Iter_.eState)
		{
		case eHEART_FLOATING:
			{
				ofSetColor(255, 255, 255, Iter_.AnimFadeIn.getCurrentValue());
				float fScaleWidth_ = _FloatHeartImg.width * Iter_.fScale;
				float fScaleHeight_ = _FloatHeartImg.height * Iter_.fScale;

				_FloatHeartImg.draw(-fScaleWidth_/2, -fScaleHeight_/2, fScaleWidth_, fScaleHeight_);
			}
			break;
		case eHEART_TOUCHING:
			{
				ofSetColor(255);
				float fScaleWidth_ = _FloatHeartImg.width * Iter_.fTouchScale;
				float fScaleHeight_ = _FloatHeartImg.height * Iter_.fTouchScale;
				float fBigHeartWidth_ = _BigHeartImg.width * Iter_.AnimBigHeart.getCurrentValue();
				float fBigHeartHeight_ = _BigHeartImg.height * Iter_.AnimBigHeart.getCurrentValue();

				_BigHeartImg.draw(-fBigHeartWidth_/2, -fBigHeartHeight_/2, fBigHeartWidth_, fBigHeartHeight_);
				_FloatHeartImg.draw(-fScaleWidth_/2, -fScaleHeight_/2, fScaleWidth_, fScaleHeight_);
				
			}
			break;
		case eHEART_BIG_HEART:
			{
				ofSetColor(255);
				_BigHeartImg.draw(-_BigHeartImg.width/2, -_BigHeartImg.height/2);
			}
			break;
		case eHEART_FAREOUT:
			{
				ofSetColor(255, 255, 255, Iter_.AnimFadeout.getCurrentValue());
				_BigHeartImg.draw(-_BigHeartImg.width/2, -_BigHeartImg.height/2);
			}
			break;
		}
		ofPopMatrix();
	}

	ofPopStyle();
}

//--------------------------------------------------------------
void HeartManager::clear()
{
	_FlyingHeartList.clear();
	_FloatingHeartList.clear();
	_bStartAutoFloat = false;
}

//--------------------------------------------------------------
void HeartManager::addFlying(ofVec2f Start, ofVec2f End, float fDuration)
{
	stFlyingHeartPoint NewPoint_(Start, End, fDuration);
	_FlyingHeartList.push_back(NewPoint_);
}

//--------------------------------------------------------------
void HeartManager::addFloting(ofVec2f Pos)
{
	stFloatHeartPoint NewPoint_(Pos);
	_FloatingHeartList.push_back(NewPoint_);
}

//--------------------------------------------------------------
int HeartManager::getFloatingNum()
{
	return _FloatingHeartList.size();
}
#pragma endregion