#include "BaseCharacter.h"

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
		_obj.draw(-_anchor.x, -_anchor.y);
	}
	ofPopMatrix();
	ofPopStyle();
}

//--------------------------------------------------------------
void CharacterObj::setSizebyBody(float fBody)
{
	_fScaleHeight = _fBodyScale * fBody;
	_fScaleWidth = _obj.getWidth() * (_fScaleHeight / _obj.getHeight());
}