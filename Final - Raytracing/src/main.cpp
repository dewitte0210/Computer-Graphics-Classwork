#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){

	//Use ofGLFWWindowSettings for more options like multi-monitor fullscreen
	ofGLWindowSettings settings;
	settings.setSize(1200, 675);
	settings.windowMode = OF_WINDOW; //can also be OF_FULLSCREEN
	settings.setGLVersion(4, 1);

	auto window = ofCreateWindow(settings);

	ofRunApp(window, make_shared<ofApp>());
	ofRunMainLoop();

}
