#include "AlienCatcher.h"

#pragma region stAlienPoint
void stAlienPoint::update(float fDelta, float UFOPosX, float fScaleLimit, float fminDist)
{
	AnimUp.update(fDelta);
	AnimScale.update(fDelta);
	AnimScaleFadeout.update(fDelta);
	AnimAlpah.update(fDelta);
	
	switch(eState)
	{
	case eALIEN_STAND:
		{
			if(abs(UFOPosX - Position.x) <= fminDist)
			{
				eState = eALIEN_FLY;
				AnimUp.animateFromTo(0, 1);
				AnimScale.animateFromTo(fScale, fScale*0.7);
			}
		}
		break;
	case eALIEN_FLY:
		{
			Position.y = cWINDOW_HEIGHT - (AnimUp.getCurrentValue() * cWINDOW_HEIGHT);
			Position.x = UFOPosX;
			fScale = AnimScale.getCurrentValue();
			if(Position.y < fScaleLimit)
			{
				AnimScale.pause();
				AnimScaleFadeout.animateFromTo(fScale, 0.0);
				eState = eALIEN_SCALE;
			}
		}
		break;
	case eALIEN_SCALE:
		{
			fScale = AnimScaleFadeout.getCurrentValue();
			Position.y = cWINDOW_HEIGHT - (AnimUp.getCurrentValue() * cWINDOW_HEIGHT);
			Position.x = UFOPosX;

			if(AnimUp.getPercentDone() == 1.0 && AnimUp.hasFinishedAnimating())
			{
				eState = eALIEN_CAN_REMOVE;
			}
		}
		break;
	}
}
#pragma endregion

#pragma region AlienCatcher
//------------------------------
//@CLASS AlienCathcer
void AlienCatcher::setup()
{
	//Load Image
	_UFO.loadImage("Alien/UFO_pusheen.png");
	_Alien.loadImage("Alien/Alien_pusheen.png");

	//Position
	_IntervalSize = (float)cWINDOW_WIDTH/cALIEN_INTERVAL_NUM;
	_IntervalSizeHalf = _IntervalSize/2;
	for(int idx_ = 0; idx_ < cALIEN_INTERVAL_NUM; idx_++)
	{
		_PositionList.push_back(idx_ * _IntervalSize);
		_IntervalCreateOrder.push_back(idx_);
	}
	random_shuffle(_IntervalCreateOrder.begin(), _IntervalCreateOrder.end());
	_iAlienCounter = 0;

	//UFO
	_iNowIntervalID = this->getIntervalID(cWINDOW_WIDTH/2.0);
	_AnimUFOPosX.setCurve(AnimCurve::LINEAR);
	_AnimUFOPosX.reset(_PositionList[_iNowIntervalID] + _IntervalSizeHalf);
	_AlienList.clear();

	_bStartAuto = false;
	_fCreateTimer = 5.0;
}

//--------------------------------------------------------------
void AlienCatcher::update(float fDelta, float PosX)
{
	_AnimUFOPosX.update(fDelta);
	float fUFOPosX_ = _AnimUFOPosX.getCurrentValue();

	//Move UFO
	if(PosX != -1)
	{
		int NewIntervalID_ = this->getIntervalID(PosX);
		if(NewIntervalID_ != _iNowIntervalID && NewIntervalID_ != -1)
		{
			int iNum_ = abs(NewIntervalID_ - _iNowIntervalID);
		
			_AnimUFOPosX.reset();
			_AnimUFOPosX.setDuration(iNum_ * cUFO_INTERVAL_DURATION);
			_AnimUFOPosX.animateFromTo(fUFOPosX_, _PositionList[NewIntervalID_] + _IntervalSizeHalf);
			_iNowIntervalID = NewIntervalID_;
		}
	}	

	//Update Alien
	auto AlienIter_ = _AlienList.begin();
	while(AlienIter_ != _AlienList.end())
	{
		AlienIter_->update(fDelta, fUFOPosX_, _UFO.height * 1.5, _IntervalSizeHalf);

		if(AlienIter_->eState == eALIEN_CAN_REMOVE)
		{
			if(_AlienList.size() == 1)
			{
				//Trigger event
				string strMsg_ = "lastone";
				ofNotifyEvent(_AlienEvent, strMsg_, this);
			}
			AlienIter_ = _AlienList.erase(AlienIter_);
		}
		else
		{
			++AlienIter_;
		}
	}

	//Add new Alien
	if(!_bStartAuto)
	{
		return;
	}
	
	_fCreateTimer -= fDelta;
	if(_fCreateTimer <= 0.0)
	{
		_fCreateTimer = ofRandom(cALIEN_CREATE_DURATION.first, cALIEN_CREATE_DURATION.second);
		this->addAlien();
	}
	
}

//--------------------------------------------------------------
void AlienCatcher::draw()
{
	ofPushStyle();

	ofEnableAlphaBlending();
	ofSetColor(255);

	//Alien
	for(auto& Iter_ : _AlienList)
	{
		ofSetColor(255, 255, 255, Iter_.AnimAlpah.getCurrentValue());
		ofPushMatrix();
		{
			ofTranslate(Iter_.Position);
			float fScaleWidth_ = _Alien.width * Iter_.fScale;
			float fScaleHeight_ = _Alien.height * Iter_.fScale;

			_Alien.draw(-fScaleWidth_/2, -fScaleHeight_, fScaleWidth_, fScaleHeight_);
		}
		ofPopMatrix();
	}

	//UFO
	ofSetColor(255);
	ofPushMatrix();
	{	
		ofTranslate(ofVec2f(_AnimUFOPosX.getCurrentValue(), 0));
		_UFO.draw(-_UFO.width/2, 0);
	}
	ofPopMatrix();
	ofPopStyle();
}

//--------------------------------------------------------------
void AlienCatcher::clear()
{
	//clear alien
	_iNowIntervalID = this->getIntervalID(cWINDOW_WIDTH/2.0);
	_AnimUFOPosX.reset(_PositionList[_iNowIntervalID] + _IntervalSizeHalf);
	_AlienList.clear();
	_bStartAuto = false;
	_fCreateTimer = 5.0;

	//Reset order
	_iAlienCounter = 0;
	random_shuffle(_IntervalCreateOrder.begin(), _IntervalCreateOrder.end());
}

//--------------------------------------------------------------
void AlienCatcher::addAlien(float fPosX)
{	
	stAlienPoint	NewAlien_(ofVec2f(fPosX, cWINDOW_HEIGHT));
	_AlienList.push_back(NewAlien_);
}

//--------------------------------------------------------------
void AlienCatcher::addAlien()
{	
	if(_iAlienCounter == cALIEN_INTERVAL_NUM)
	{
		_iAlienCounter = 0;
		random_shuffle(_IntervalCreateOrder.begin(), _IntervalCreateOrder.end());
	}

	int ID_ = _IntervalCreateOrder[_iAlienCounter];

	this->addAlien(ofRandom(_PositionList[ID_], _PositionList[ID_] + _IntervalSize));
	_iAlienCounter++;
}

#pragma endregion
