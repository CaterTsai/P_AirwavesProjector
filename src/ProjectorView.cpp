#include "ProjectorView.h"

void ProjectorView::setup()
{
	ofBackground(0);
	ofSetVerticalSync(true);

	//Config
	this->loadConfig();

	//Kinect
	if(!this->initKinect())
	{
		ofLog(OF_LOG_ERROR, "Initial Kinect failed");
		getchar();
		std::exit(1);
	}

	//Character
	_CharacterMgr.setupCharacterMgr();
	ofAddListener(IBaseCharacter::CharacterEvent, this, &ProjectorView::onCharacterEvent);

	//Connector
	_Connector.initConnector("127.0.0.1", 2233, 5566);
	ofAddListener(_Connector.AirwavesConnectorEvent, this, &ProjectorView::onConnectorEvent);

	_Background.loadImage("background_1.jpg");
	_Human.loadImage("human_2.png");

	_SkeletonHandler.setDisplay(true);

	_fMainTimer = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void ProjectorView::update()
{
	float fDelta_ = ofGetElapsedTimef() - _fMainTimer;
	_fMainTimer += fDelta_;

	//Kinect
	this->updateKinect();

	//Character
	_CharacterMgr.updateCharacterMgr(fDelta_, _SkeletonHandler);

	_Connector.updateConnector();
}

//--------------------------------------------------------------
void ProjectorView::draw()
{
	ofSetColor(255);

	_Background.draw(0, 0);
	
	//Character
	_CharacterMgr.drawCharacterMgr();

	//Debug
	_Human.draw(ofGetWindowWidth()/2 - _Human.width/2, ofGetWindowHeight() - _Human.height);

	//Kinect
	this->drawKinect();
}

//--------------------------------------------------------------
void ProjectorView::exit()
{
	this->stopKinect();
}

//--------------------------------------------------------------
void ProjectorView::keyPressed(int key)
{
	ofPoint StartPos_ = _SkeletonHandler.getStartPos();
	float fScale_ = _SkeletonHandler.getScale();

	switch(key)
	{
	//Character
	case '1':
		{
			if(!_bHaveUser)
			{
				return;
			}
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
			_CharacterMgr.setCharacter(eCHARACTER_ANGEL, _SkeletonHandler.getBodySize());
			_CharacterMgr.play();
		}
		break;
	case '4':
		{
			if(!_bHaveUser)
			{
				return;
			}
			_CharacterMgr.setCharacter(eCHARACTER_MONEY, _SkeletonHandler.getBodySize());
			_CharacterMgr.play();
		}
		break;
	//Kinect ctrl
	case 'k':
		{
			if(_Kinect.isThreadRunning())
			{
				_Kinect.stopThread();
			}
			else
			{
				_Kinect.startThread();
			}
			break;
		}
	//case 's':
	//	{
	//		this->saveConfig();
	//		break;
	//	}
	////Translate
	//case OF_KEY_UP:
	//	StartPos_.set(StartPos_.x, StartPos_.y - 1);
	//	_SkeletonHandler.setStartPos(StartPos_);
	//	break;
	//case OF_KEY_DOWN:
	//	StartPos_.set(StartPos_.x, StartPos_.y + 1);
	//	_SkeletonHandler.setStartPos(StartPos_);
	//	break;
	//case OF_KEY_LEFT:
	//	StartPos_.set(StartPos_.x - 1, StartPos_.y);
	//	_SkeletonHandler.setStartPos(StartPos_);
	//	break;
	//case OF_KEY_RIGHT:
	//	StartPos_.set(StartPos_.x + 1, StartPos_.y);
	//	_SkeletonHandler.setStartPos(StartPos_);
	//	break;

	////Scale
	//case '1':
	//	if( (fScale_ - 0.05) >= .0)
	//	{
	//		_SkeletonHandler.setScale(fScale_ - 0.05);
	//	}
	//	break;
	//case '2':
	//	_SkeletonHandler.setScale(fScale_ + 0.05);
	//	break;
	}
}

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
	_SkeletonHandler.setStartPos(_exKinectStartPos);
	_SkeletonHandler.setScale(_exKinectScale);

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
#pragma endregion

#pragma region Character
//--------------------------------------------------------------
void ProjectorView::onCharacterEvent(string& e)
{
	cout<<e<<endl;
}
#pragma endregion

#pragma region Connector
//--------------------------------------------------------------
void ProjectorView::onConnectorEvent(string& e)
{
	cout<<e<<endl;
}
#pragma endregion

#pragma region Config
//-------------------------------------------------
//Config
void ProjectorView::loadConfig()
{
	ofxXmlSettings	xml_;
	_exKinectStartPos.set(0, 0);
	_exKinectScale = 1.0;
	if(!xml_.loadFile("_config.xml"))
	{
		ofLog(OF_LOG_WARNING, "Can't found _config.xml, used default setting");
		return;
	}

	
	_exKinectStartPos.x = xml_.getValue("KINECT_START_X", 0);
	_exKinectStartPos.y = xml_.getValue("KINECT_START_Y", 1.0);
	_exKinectScale = xml_.getValue("KINECT_SCALE", 1.0);
}

//-------------------------------------------------
void ProjectorView::saveConfig()
{
	ofxXmlSettings	xml_;
	xml_.setValue("KINECT_START_X", _SkeletonHandler.getStartPos().x);
	xml_.setValue("KINECT_START_Y",_SkeletonHandler.getStartPos().y);
	xml_.setValue("KINECT_SCALE", _SkeletonHandler.getScale());

	if(xml_.saveFile("_config.xml"))
	{
		ofLog(OF_LOG_NOTICE, "Save config file success");
	}
}
#pragma endregion
