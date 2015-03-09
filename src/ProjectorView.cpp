#include "ProjectorView.h"

void ProjectorView::setup()
{
	ofBackground(0);
	ofSetVerticalSync(true);

	//Kinect
	if(!this->initKinect())
	{
		ofLog(OF_LOG_ERROR, "Initial Kinect failed");
		getchar();
		std::exit(1);
	}

	//Connector
	_Connector.initConnector("127.0.0.1", 2233, 5566);
	ofAddListener(_Connector.AirwavesConnectorEvent, this, &ProjectorView::onConnectorEvent);

	_SkeletonHandler.setDisplay(true);
}

//--------------------------------------------------------------
void ProjectorView::update()
{
	//Kinect
	this->updateKinect();

	_Connector.updateConnector();
}

//--------------------------------------------------------------
void ProjectorView::draw()
{
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
	case 's':
		{
			_Connector.sendCMD(eCONNECTOR_CMD::eP2D_TEACHING_END, "P2D");
			break;
		}

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
	case '1':
		if( (fScale_ - 0.05) >= .0)
		{
			_SkeletonHandler.setScale(fScale_ - 0.05);
		}
		break;
	case '2':
		_SkeletonHandler.setScale(fScale_ + 0.05);
		break;
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

#pragma region Connector

void ProjectorView::onConnectorEvent(string& e)
{
	cout<<e<<endl;
}

#pragma endregion

