#include "BaseCharacter.h"

#pragma region Character Object Class

void CharacterObj::setup(string strName, string Filename, ofVec2f anchor, float fBodyScale)
{
	_Name = strName;
	_obj.loadImage(Filename);
	_anchor.set(anchor);
	_fBodyScale = fBodyScale;

	_DrawPos.set(0);
	_fRotate = .0;
	_fScaleWidth = _obj.getWidth();
	_fScaleHeight = _obj.getHeight();
	_ScaleAnchor.set(_anchor);
}

//--------------------------------------------------------------
void CharacterObj::update(ofPoint DrawPos, float fRotate)
{
	_DrawPos.set(DrawPos);
	_fRotate = fRotate;
}

//--------------------------------------------------------------
void CharacterObj::draw()
{
	ofPushStyle();
	ofPushMatrix();
	{
		ofSetColor(255);
		ofDisableBlendMode();
		ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);

		ofTranslate(_DrawPos);
		ofRotateZ(-_fRotate);
		_obj.draw(-_ScaleAnchor.x, -_ScaleAnchor.y, _fScaleWidth, _fScaleHeight);
	}
	ofPopMatrix();
	ofPopStyle();
}

//--------------------------------------------------------------
void CharacterObj::setSizebyBody(float fBody)
{
	_ScaleAnchor.set(_anchor);
	
	_fScaleHeight = _fBodyScale * fBody;
	float fScale_ = (_fScaleHeight / _obj.getHeight());
	

	_fScaleWidth = _obj.getWidth() * fScale_;
	_ScaleAnchor.x = _ScaleAnchor.x * fScale_;
	_ScaleAnchor.y = _ScaleAnchor.y * fScale_;
}

#pragma endregion

#pragma region IBaseCharacter
ofEvent<pair<string, string>> IBaseCharacter::CharacterEvent = ofEvent<pair<string, string>>();

//--------------------------------------------------------------
void IBaseCharacter::updateCharacter(float fDelta, SkeletonHandler& SkeletonHandler)
{
	if(!_bIsSetup || _eState == eCHARACTER_WAIT)
	{
		return;
	}

	//update Character object
	_bIsDisplay = SkeletonHandler.getHaveUser();
	if(_bIsDisplay)
	{
		for(auto& Obj_ : _ObjectList)
		{
			this->updateCharacterObj(Obj_, SkeletonHandler);
		}
	}

	switch(_eState)
	{
	case eCHARACTER_TEACHING:
		this->updateTeaching(fDelta, SkeletonHandler);
		break;
	case eCHARACTER_GAMING:
		this->updateGaming(fDelta, SkeletonHandler);
		break;
	}
}

//--------------------------------------------------------------
void IBaseCharacter::drawCharacter()
{
	if(!_bIsSetup || !_bIsDisplay)
	{
		return;
	}

	//draw character object
	for(auto& Obj_ : _ObjectList)
	{
		Obj_.draw();
	}

	switch(_eState)
	{
	case eCHARACTER_TEACHING:
		this->drawTeaching();
		break;
	case eCHARACTER_GAMING:
		this->drawGaming();
		break;
	}
}

//--------------------------------------------------------------
void IBaseCharacter::play()
{
	if(!_bIsSetup)
	{
		ofLog(OF_LOG_ERROR, "This character is not setup");
		return;
	}
	_eState = eCHARACTER_TEACHING;

	this->setupTeaching();
	this->setupGaming();
}

//--------------------------------------------------------------
void IBaseCharacter::stop()
{
	_eState = eCHARACTER_WAIT;
	this->reset();
}

//--------------------------------------------------------------
void IBaseCharacter::setScale(float fBody)
{
	for(auto& Obj_ : _ObjectList)
	{
		Obj_.setSizebyBody(fBody);
	}
}

//--------------------------------------------------------------
void IBaseCharacter::takePicture(float fDelta)
{
	//Take picture
	if(_iPictureCounter < cPICTURE_NUM)
	{
		_fPictureTimer -= fDelta;	
		if(_fPictureTimer > 0.0)
		{
			return;
		}

		pair<string, string> Event_ = make_pair(NAME_MGR::EVENT_TakePicture, "");
		ofNotifyEvent(IBaseCharacter::CharacterEvent, Event_);
		_iPictureCounter++;
		_fPictureTimer = _fPirecureInterval;
	}
}
#pragma endregion
