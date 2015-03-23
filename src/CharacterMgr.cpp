#include "CharacterMgr.h"

CharacterMgr::~CharacterMgr()
{
	for(auto& Iter_ : _CharacterMap)
	{
		delete Iter_.second;
	}

	_CharacterMap.clear();
}

//--------------------------------------------------------------
void CharacterMgr::setupCharacterMgr()
{
	//Roma
	CharacterRoma *pCharacterRoma_ = new CharacterRoma();
	pCharacterRoma_->setupCharacter();
	_CharacterMap.insert(make_pair(eCHARACTER_ROMA, pCharacterRoma_));

	//Angel
	CharacterAngel *pCharacterAngel_ = new CharacterAngel();
	pCharacterAngel_->setupCharacter();
	_CharacterMap.insert(make_pair(eCHARACTER_ANGEL, pCharacterAngel_));

	//Alien
	CharacterAlien *pCharacterAlien_ = new CharacterAlien();
	pCharacterAlien_->setupCharacter();
	_CharacterMap.insert(make_pair(eCHARACTER_ALIEN, pCharacterAlien_));

	//Money
	CharacterMoney *pCharacterMoney_ = new CharacterMoney();
	pCharacterMoney_->setupCharacter();
	_CharacterMap.insert(make_pair(eCHARACTER_MONEY, pCharacterMoney_));

	//Default character
	_NowCharacter = _CharacterMap.find(eCHARACTER_ROMA);
}

//--------------------------------------------------------------
void CharacterMgr::updateCharacterMgr(float fDelta, SkeletonHandler& Skeleton)
{
	if(!_bIsPlay)
	{
		return;
	}

	_NowCharacter->second->updateCharacter(fDelta, Skeleton);
}

//--------------------------------------------------------------
void CharacterMgr::drawCharacterMgr()
{
	if(!_bIsPlay)
	{
		return;
	}

	_NowCharacter->second->drawCharacter();
}

//--------------------------------------------------------------
void CharacterMgr::setCharacter(eCHARACTER_TYPE eType, float fBodySize)
{
	if(_bIsPlay)
	{
		this->stop();
	}

	_NowCharacter = _CharacterMap.find(eType);
	if(_NowCharacter == _CharacterMap.end())
	{
		ofLog(OF_LOG_ERROR, "Can't found character type :" + ofToString(eType));
		_NowCharacter = _CharacterMap.find(eCHARACTER_ROMA);
	}

	_NowCharacter->second->setScale(fBodySize);
}
	
//--------------------------------------------------------------
void CharacterMgr::play()
{
	_bIsPlay = true;
	_NowCharacter->second->play();
}

//--------------------------------------------------------------
void CharacterMgr::stop()
{
	_bIsPlay = false;
	_NowCharacter->second->stop();
}

//--------------------------------------------------------------
void CharacterMgr::timeoutTeachingMode()
{
	_NowCharacter->second->teachingTimeout();
}