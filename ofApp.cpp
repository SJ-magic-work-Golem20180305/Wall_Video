/************************************************************
************************************************************/
#include "ofApp.h"

/************************************************************
************************************************************/

/******************************
******************************/
ofApp::ofApp(int _BootMode)
: BootMode(BOOTMODE(_BootMode))
, Osc("127.0.0.1", 12347, 12346)
, MocapPos_x(0)
, MocapPos_y(0)
, MocapPos_z(0)
{
}

/******************************
******************************/
ofApp::~ofApp()
{
}

/******************************
******************************/
void ofApp::exit()
{
	printf("> Exit\n");
}


//--------------------------------------------------------------
void ofApp::setup(){
	/********************
	********************/
	ofSetWindowTitle("Wall:Video");
	ofSetVerticalSync(true);
	ofSetFrameRate(60); // if vertical sync is off, we can go a bit fast... this caps the framerate at 60fps.
	ofSetWindowShape(WIDTH, HEIGHT);
	ofSetEscapeQuitsApp(false);
	
	ofEnableAlphaBlending();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	// ofEnableBlendMode(OF_BLENDMODE_ADD);
	// ofEnableSmoothing();
	
	ofSetCircleResolution(50);
	
	/********************
	********************/
	SyphonTexture.setName("Screen Output");
	fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	
	/********************
	********************/
	video.load("video.mov");
	setup_video(video);
}

/******************************
******************************/
void ofApp::setup_video(ofxHapPlayer& video)
{
	video.setLoopState(OF_LOOP_NORMAL);
	// video.setLoopState(OF_LOOP_PALINDROME);
	
	video.setSpeed(1);
	// video.setVolume(0.0);
	video.play();
	
	if(BootMode != BOOTMODE_PLAY){
		video.setPaused(true);
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	/********************
	********************/
	while(Osc.OscReceive.hasWaitingMessages()){
		ofxOscMessage m_receive;
		Osc.OscReceive.getNextMessage(&m_receive);
		
		if(m_receive.getAddress() == "/HumanPose"){
			MocapPos_x = m_receive.getArgAsFloat(0);
			MocapPos_y = m_receive.getArgAsFloat(1);
			MocapPos_z = m_receive.getArgAsFloat(2);
		}
	}
	
	/********************
	********************/
	ofxOscMessage m;
	m.setAddress("/HumanPose");
	m.addFloatArg(MocapPos_x);
	m.addFloatArg(MocapPos_y);
	m.addFloatArg(MocapPos_z);
	
	Osc.OscSend.sendMessage(m);
	
	/********************
	********************/
	switch(BootMode){
		case BOOTMODE_PLAY:
			// nothing.
			break;
			
		case BOOTMODE_MOUSE:
			map_X_to_VideoTime(mouseX, 0, ofGetWidth(), 0, 1);
			break;
			
		case BOOTMODE_MOCAP:
			map_X_to_VideoTime(MocapPos_x, -1, 1, 0.35, 0.1);
			break;
	}
	
	/********************
	********************/
	video.update();
}

/******************************
******************************/
void ofApp::map_X_to_VideoTime(float x, float x_from, float x_to, float video_from, float video_duration)
{
	float video_to = video_from + video_duration;
	float VideoPos = ofMap(x, x_from, x_to, video_from, video_to);
	if(VideoPos < 0)		VideoPos = 0;
	else if(1 < VideoPos)	VideoPos = 1.0;
	
	video.setPosition(VideoPos);
}

//--------------------------------------------------------------
void ofApp::draw(){
	/********************
	********************/
	// Clear with alpha, so we can capture via syphon and composite elsewhere should we want.
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// ofClear(0, 0, 0, 0);
	
	/********************
	********************/
	fbo.begin();
		ofClear(0, 0, 0, 0);
		ofSetColor(255, 255, 255, 255);
		
		video.draw(0, 0, ofGetWidth(), ofGetHeight());
	fbo.end();
	
	/********************
	********************/
	ofSetColor(255, 255, 255, 200);
	fbo.draw(0, 0, ofGetWidth(), ofGetHeight());
	
	/********************
	Syphon Stuff
	********************/
	SyphonTexture.publishScreen();
	
	/********************
	********************/
	ofSetColor(255, 0, 0, 255);
	
	char buf[BUF_SIZE];
	sprintf(buf, "%7.4f", video.getPosition());
	ofDrawBitmapString(buf, 100, 50);
	
	if(BootMode == BOOTMODE_PLAY){
		float MousePos = ofMap(mouseX, 0, ofGetWidth(), 0, 1);
		sprintf(buf, "%7.4f", MousePos);
		ofDrawBitmapString(buf, 100, 70);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch(key){
		case ' ':
			ofSaveScreen("image.png");
			printf("image saved\n");
			break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	map_X_to_VideoTime(mouseX, 0, ofGetWidth(), 0, 1);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
