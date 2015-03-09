#include "ofMain.h"
#include "ProjectorView.h"

//========================================================================
int main( ){
	ofSetupOpenGL(cWINDOW_WIDTH,cWINDOW_HEIGHT,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ProjectorView());

}
