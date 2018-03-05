/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"
#include "ofxSyphon.h"
#include "ofxGui.h"
#include "ofxHapPlayer.h"

#include "sjCommon.h"
#include "sj_OSC.h"

/************************************************************
************************************************************/

class ofApp : public ofBaseApp{
private:
	/****************************************
	****************************************/
	enum BOOTMODE{
		BOOTMODE_PLAY,
		BOOTMODE_MOUSE,
		BOOTMODE_MOCAP,
	};
	enum{
		WIDTH = 1280,
		HEIGHT = 720,
	};
	
	/****************************************
	****************************************/
	BOOTMODE BootMode;
	
	ofxSyphonServer SyphonTexture;
	
	OSC_TARGET Osc;
	float MocapPos_x;
	float MocapPos_y;
	float MocapPos_z;
	
	ofxHapPlayer video;
	ofFbo fbo;
	

public:
	/****************************************
	****************************************/
	ofApp(int _BootMode);
	~ofApp();
	void exit();
	
	void setup();
	void update();
	void draw();
	
	void setup_video(ofxHapPlayer& video);
	void map_X_to_VideoTime(float x, float x_from, float x_to, float video_from, float video_duration);
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
};
