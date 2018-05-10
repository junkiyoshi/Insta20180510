#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofBackground(239);
	ofSetWindowTitle("Insta");

	ofSetColor(39);

	// Initialize ofxBox2d
	this->box2d.init();
	this->box2d.setGravity(0, 0);
	this->box2d.createBounds();
	this->box2d.setFPS(60);
	this->box2d.registerGrabbing();

	// Add Circle
	for (int i = 0; i < 255; i++) {

		float radius = 10;
		auto circle = make_shared<ofxBox2dCircle>();
		circle->setPhysics(1.0, 0.63, 0.1);
		circle->setup(this->box2d.getWorld(), ofRandom(ofGetWidth() / 2), ofRandom(ofGetHeight() / 2), radius);

		this->circles.push_back(circle);
	}

	// Add Field of Force
	this->force_fields.push_back(ofPoint(ofGetWidth() / 4, ofGetHeight() / 4));
	this->force_fields.push_back(ofPoint(ofGetWidth() / 4 * 3, ofGetHeight() / 4));
	this->force_fields.push_back(ofPoint(ofGetWidth() / 4 * 3, ofGetHeight() / 4 * 3));
	this->force_fields.push_back(ofPoint(ofGetWidth() / 4, ofGetHeight() / 4 * 3));

	this->force_field_radius = 150;
}

//--------------------------------------------------------------
void ofApp::update() {

	// Field of Force
	for (int i = 0; i < this->circles.size(); i++) {

		shared_ptr<ofxBox2dCircle> circle = this->circles[i];
		ofPoint point = circle->getPosition();
		for (int field_index = 0; field_index < this->force_fields.size(); field_index++) {

			float distance = point.distance(this->force_fields[field_index]);
			if (distance < this->force_field_radius) {

				ofPoint p = ofPoint(this->force_field_radius * cos((field_index * 90 + 10) * DEG_TO_RAD), this->force_field_radius * sin((field_index * 90 + 10) * DEG_TO_RAD));
				circle->addForce(p, ofMap(distance, 0, this->force_field_radius, 0.1, 0.01));
			}
		}
	}

	// Compute box2d
	this->box2d.update();
}


//--------------------------------------------------------------
void ofApp::draw() {

	// Draw Circles
	for (int i = 0; i < this->circles.size(); i++) {

		float radius = this->circles[i]->getRadius();
		ofPoint point = this->circles[i]->getPosition();
		ofPoint velocity = this->circles[i]->getVelocity();

		ofPushMatrix();
		ofTranslate(point);

		if (velocity.length() < radius) {

			ofDrawCircle(ofPoint(), radius);
		}
		else {

			float velocity_deg = atan2f(velocity.y, velocity.x) * RAD_TO_DEG + 180;
			ofBeginShape();
			for (int deg = velocity_deg + 90; deg < velocity_deg + 270; deg++) {

				ofVertex(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD));
			}
			ofVertex(-velocity * 2);
			ofEndShape(true);
		}

		ofPopMatrix();
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}