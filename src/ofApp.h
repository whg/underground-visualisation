#pragma once

#include "ofMain.h"
#include "ofxTimeline.h"


const int NUM_TEXES = 8;

class ofApp : public ofBaseApp{
	public:
		void setup();
    void exit();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
private:
    ofxTimeline timeline;
    
    
    ofTexture texes[NUM_TEXES];
    ofTexture cols;
    ofTexture compute;
    ofTexture tex1, tex2, image;
    ofShader shader;
    ofImage img;
    ofFbo fbo, mainFbo;
    ofVbo vbo;
    float *colordata;
    float *offsets;
    vector<ofVec2f> points;
    vector<ofFloatColor> colours;
    bool showtimeline = false;
    
    int maxjourneys = 0;
};


float* getFileData(const char *filename, long size) {
    
    FILE *fp;
    
    if((fp=fopen(filename, "rb")) == NULL) {
        cout << "can't open" << endl;
        return NULL;
    }
    
    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    cout << "size = " << fsize/4 << endl;
    
    float *f = (float *)calloc(size, sizeof(float));
    if(f==NULL)
    {
        fclose(fp);
        cout << "can't alloc" << endl;
        return NULL;
    }
    
    size_t res = fread(f, sizeof(float), size/4, fp);
    if(res != size/4)
    {
        fclose(fp);
        cout << "no data, size = " << size << ", res = " << res << endl;
    }
    
    
    for (int i = 0; i < 4 * 3; i++) {
        cout << f[i] << ", ";
    }
    cout << endl;
    
    fclose(fp);
    
    return f;
}

