#include "ProjectorView.h"

#pragma region Basic Functions
void ProjectorView::setup()
{
	ofBackground(0);
	ofSetVerticalSync(true);

	//Config
	configLoader::GetInstance()->load();

	//Kinect
	if(!this->initKinect())
	{
		ofLog(OF_LOG_ERROR, "Initial Kinect failed");
		getchar();
		std::exit(1);
	}
	_SkeletonHandler.setMirror(configLoader::GetInstance()->_exIsMirror);
	_Kinect.startThread();

	//Character
	_CharacterMgr.setupCharacterMgr();
	_eCharacterType = eCHARACTER_NUM;
	ofAddListener(IBaseCharacter::CharacterEvent, this, &ProjectorView::onCharacterEvent);

	//Connector
	
	_Connector.initConnector(configLoader::GetInstance()->_exDisplayIP, 2233, 5566);
	ofAddListener(_Connector.AirwavesConnectorEvent, this, &ProjectorView::onConnectorEvent);

	//Background
	_bDisplayLight = false;
	_BackgroundL.loadImage("background_l.jpg");
	_BackgroundD.loadImage("background_d.jpg");
	_AnimBackgroundFade.setDuration(1.0);
	_AnimBackgroundFade.reset(0.0);

	//Aduio & BGM
	this->setupAudioMgr();
	AudioMgr::GetInstance()->playAudio(AUDIO_NAME_MGR::BGM_WAITING);
	//AudioMgr::GetInstance()->playAudio(AUDIO_NAME_MGR::BGM_GAME);

	_SkeletonHandler.setDisplay(configLoader::GetInstance()->_exDisplaySkeleton);

	_bCtrlMode = false;
	ofHideCursor();
	ofToggleFullscreen();
	_fMainTimer = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void ProjectorView::update()
{
	float fDelta_ = ofGetElapsedTimef() - _fMainTimer;
	_fMainTimer += fDelta_;

	//Background
	this->updateBackground(fDelta_);

	//Kinect
	this->updateKinect();

	//Character
	_CharacterMgr.updateCharacterMgr(fDelta_, _SkeletonHandler);

	//Connector
	_Connector.updateConnector();
}

//--------------------------------------------------------------
void ProjectorView::draw()
{
	ofSetColor(255);
	
	//Background
	this->drawBackground();

	//Character
	_CharacterMgr.drawCharacterMgr();

	//-----------------------
	//Debug
	//_Human.draw(ofGetWindowWidth()/2 - _Human.width/2, ofGetWindowHeight() - _Human.height);

	//Kinect
	this->drawKinect();
}

//--------------------------------------------------------------
void ProjectorView::exit()
{
	this->stopKinect();
	AudioMgr::GetInstance()->Destroy();
	configLoader::GetInstance()->Destroy();
}

//--------------------------------------------------------------
void ProjectorView::keyPressed(int key)
{
	switch(key)
	{
	//Character
	case '1':
		{
			if(!_bHaveUser)
			{
				return;
			}
			_CharacterMgr.stop();
			_CharacterMgr.setCharacter(eCHARACTER_ROMA, _SkeletonHandler.getBodySize());
			_CharacterMgr.play();
		}
		break;
	case '2':
		{
			if(!_bHaveUser)
			{
				return;
			}
			_CharacterMgr.stop();
			_CharacterMgr.setCharacter(eCHARACTER_ALIEN, _SkeletonHandler.getBodySize());
			_CharacterMgr.play();
		}
		break;
	case '3':
		{
			if(!_bHaveUser)
			{
				return;
			}
			_CharacterMgr.stop();
			_CharacterMgr.setCharacter(eCHARACTER_ANGEL, _SkeletonHandler.getBodySize());
			_CharacterMgr.play();
		}
		break;
	//case '4':
	//	{
	//		if(!_bHaveUser)
	//		{
	//			return;
	//		}
	//		_CharacterMgr.setCharacter(eCHARACTER_MONEY, _SkeletonHandler.getBodySize());
	//		_CharacterMgr.play();
	//	}
	//	break;
	case 's':
		{
			if(!_bHaveUser)
			{
				return;
			}
			_CharacterMgr.startGaming();
		}
		break;
	case 'f':
		{
			_bCtrlMode ^= true;
			if(_bCtrlMode)
			{
				ofShowCursor();
			}
			else
			{
				ofHideCursor();
			}
			ofToggleFullscreen();
		}
		break;
	}

	//Kinect setting (only work on debug mode)
	this->settingKinect(key);
}
#pragma endregion

#pragma region Background
//--------------------------------------------------------------
void ProjectorView::updateBackground(float fDelta)
{
	_AnimBackgroundFade.update(fDelta);

	if(_AnimBackgroundFade.isAnimating())
	{
		return;
	}

	if(_bHaveUser && !_bDisplayLight)
	{
		//Change to light background
		_AnimBackgroundFade.animateTo(255);
		_bDisplayLight = true;
	}
	else if(!_bHaveUser && _bDisplayLight)
	{
		//Change to dark background
		_AnimBackgroundFade.animateTo(0.0);
		_bDisplayLight = false;
	}
}

//--------------------------------------------------------------
void ProjectorView::drawBackground()
{
	ofPushStyle();
	ofEnableAlphaBlending();

	ofSetColor(255);
	_BackgroundD.draw(0, 0);

	ofSetColor(255, 255, 255, _AnimBackgroundFade.getCurrentValue());
	_BackgroundL.draw(0, 0);
	ofPopStyle();
}

#pragma endregion

#pragma region Audio Manager
void ProjectorView::setupAudioMgr()
{
	AudioMgr::GetInstance()->addBGM(AUDIO_NAME_MGR::BGM_WAITING, "Audios/bgm_waiting.mp3");
	AudioMgr::GetInstance()->addBGM(AUDIO_NAME_MGR::BGM_GAME, "Audios/bgm_gaming.mp3");

	AudioMgr::GetInstance()->addAduio(AUDIO_NAME_MGR::A_ROMA_SHOOT, "Audios/roma_arrow.wav");
	AudioMgr::GetInstance()->addAduio(AUDIO_NAME_MGR::A_ROMA_HIT, "Audios/roma_hit.wav");
	AudioMgr::GetInstance()->addAduio(AUDIO_NAME_MGR::A_ROMA_DEFENCE, "Audios/roma_defence.wav");

	AudioMgr::GetInstance()->addAduio(AUDIO_NAME_MGR::A_ALIEN_GOT, "Audios/alien_got.wav");
	AudioMgr::GetInstance()->addAduio(AUDIO_NAME_MGR::A_ALIEN_TRANSPORT, "Audios/alien_transport.wav");

	AudioMgr::GetInstance()->addAduio(AUDIO_NAME_MGR::A_ANGEL_TOUCH, "Audios/angel_touch.wav");
	AudioMgr::GetInstance()->addAduio(AUDIO_NAME_MGR::A_ANGEL_FLY, "Audios/angel_fly.wav");
}
#pragma endregion

#pragma region Kinect
//--------------------------------------------------------------
bool ProjectorView::initKinect()
{
	bool bResult_ = _Kinect.initialKinectV2();
	
	if(bResult_)
	{
		_bHaveUser = false;
		_Kinect.enableSkeleton();
	}
	_SkeletonHandler.setStartPos(configLoader::GetInstance()->_exKinectStartPos);
	_SkeletonHandler.setScale(configLoader::GetInstance()->_exKinectScale);

	return bResult_;
}

//--------------------------------------------------------------
void ProjectorView::updateKinect()
{
	if(!_Kinect.isThreadRunning())
	{
		return;
	}

	_bHaveUser = _Kinect.getSkeleton(_Skeleton);
	_SkeletonHandler.updateSkeleton(_bHaveUser, _Skeleton.aJoints);
}

//--------------------------------------------------------------
void ProjectorView::drawKinect()
{
	//for Debug
	_SkeletonHandler.drawSkeleton();
}

//--------------------------------------------------------------
void ProjectorView::startKinect()
{
	if(!_Kinect.isThreadRunning())
	{
		_Kinect.startThread();
	}
}

//--------------------------------------------------------------
void ProjectorView::stopKinect()
{
	if(_Kinect.isThreadRunning())
	{
		_Kinect.stopThread();
		_bHaveUser = false;
	}
}

//--------------------------------------------------------------
void ProjectorView::settingKinect(int key)
{
	ofPoint StartPos_ = _SkeletonHandler.getStartPos();
	float fScale_ = _SkeletonHandler.getScale();
	switch(key)
	{
	case '=':
		{
			configLoader::GetInstance()->save(_SkeletonHandler.getStartPos(), _SkeletonHandler.getScale());
			break;
		}
	//Translate
	case OF_KEY_UP:
		StartPos_.set(StartPos_.x, StartPos_.y - 1);
		_SkeletonHandler.setStartPos(StartPos_);
		break;
	case OF_KEY_DOWN:
		StartPos_.set(StartPos_.x, StartPos_.y + 1);
		_SkeletonHandler.setStartPos(StartPos_);
		break;
	case OF_KEY_LEFT:
		StartPos_.set(StartPos_.x - 1, StartPos_.y);
		_SkeletonHandler.setStartPos(StartPos_);
		break;
	case OF_KEY_RIGHT:
		StartPos_.set(StartPos_.x + 1, StartPos_.y);
		_SkeletonHandler.setStartPos(StartPos_);
		break;

	//Scale
	case '[':
		if( (fScale_ - 0.05) >= .0)
		{
			_SkeletonHandler.setScale(fScale_ - 0.05);
		}
		break;
	case ']':
		_SkeletonHandler.setScale(fScale_ + 0.05);
		break;
	}
}
#pragma endregion

#pragma region Character
//--------------------------------------------------------------
void ProjectorView::onCharacterEvent(pair<string, string>& e)
{
	if(e.first == NAME_MGR::EVENT_TeachingCheck)
	{
		_Connector.sendCMD(eCONNECTOR_CMD::eP2D_TEACHING_CHECK, e.second);
	}
	else if(e.first == NAME_MGR::EVENT_TeachingFinish)
	{
		_Connector.sendCMD(eCONNECTOR_CMD::eP2D_TEACHING_END, e.second);
	}
	else if(e.first == NAME_MGR::EVENT_TakePicture)
	{
		_Connector.sendCMD(eCONNECTOR_CMD::eP2D_TAKE_PICTURE, e.second);
	}
}
#pragma endregion

#pragma region Connector
//--------------------------------------------------------------
void ProjectorView::onConnectorEvent(pair<eCONNECTOR_CMD, string>& e)
{
	switch(e.first)
	{
	case eD2P_SET_CHARACTOR:
		{	
			eCHARACTER_TYPE eType_ = (eCHARACTER_TYPE)ofToInt(e.second);

			if(eType_ < eCHARACTER_ANGEL || eType_ >= eCHARACTER_NUM)
			{
				ofLog(OF_LOG_WARNING, "Unkown character type : " + ofToString(eType_));
				_eCharacterType = eCHARACTER_ROMA; //default
			}
			else
			{
				_eCharacterType = eType_;
			}
			_CharacterMgr.stop();
			if(!_SkeletonHandler.getHaveUser())
			{
				ofLog(OF_LOG_WARNING, "Using default body size");
				_CharacterMgr.setCharacter(_eCharacterType, cDEFAULT_BODY_SIZE);
			}
			else
			{
				_CharacterMgr.setCharacter(_eCharacterType, _SkeletonHandler.getBodySize());
			}
			_CharacterMgr.play();
			AudioMgr::GetInstance()->stopAudio(AUDIO_NAME_MGR::BGM_WAITING);
			AudioMgr::GetInstance()->playAudio(AUDIO_NAME_MGR::BGM_GAME);
		}
		break;
	case eD2P_TEACHING_TIMEOUT:
		{
			_CharacterMgr.timeoutTeachingMode();
		}
		break;
	case eD2P_GAME_START:
		{
			_CharacterMgr.startGaming();
		}
		break;
	case eD2P_GAME_TIMEOUT:
		{
			_CharacterMgr.stop();
		}
		break;
	case eD2P_RESET:
		{
			AudioMgr::GetInstance()->stopAudio(AUDIO_NAME_MGR::BGM_GAME);
			AudioMgr::GetInstance()->playAudio(AUDIO_NAME_MGR::BGM_WAITING);
		}
		break;
	default :
		{
			ofLog(OF_LOG_WARNING, "Unkown protocol : " + ofToString(e.first));
		}
		break;
	}
}
#pragma endregion

#pragma region Config
////-------------------------------------------------
////Config
//void ProjectorView::loadConfig()
//{
//	ofxXmlSettings	xml_;
//	_exKinectStartPos.set(0, 0);
//	_exKinectScale = 1.0;
//	if(!xml_.loadFile("_config.xml"))
//	{
//		ofLog(OF_LOG_WARNING, "Can't found _config.xml, used default setting");
//		return;
//	}
//
//	_exKinectStartPos.x = xml_.getValue("KINECT_START_X", 0);
//	_exKinectStartPos.y = xml_.getValue("KINECT_START_Y", 1.0);
//	_exKinectScale = xml_.getValue("KINECT_SCALE", 1.0);
//	_exIsMirror = (xml_.getValue("MIRROR", 0) == 1);
//}
//
////-------------------------------------------------
//void ProjectorView::saveConfig()
//{
//	ofxXmlSettings	xml_;
//	xml_.setValue("KINECT_START_X", _SkeletonHandler.getStartPos().x);
//	xml_.setValue("KINECT_START_Y",_SkeletonHandler.getStartPos().y);
//	xml_.setValue("KINECT_SCALE", _SkeletonHandler.getScale());
//	xml_.setValue("MIRROR", _exIsMirror);
//	if(xml_.saveFile("_config.xml"))
//	{
//		ofLog(OF_LOG_NOTICE, "Save config file success");
//	}
//}
#pragma endregion