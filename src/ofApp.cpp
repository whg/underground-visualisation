#include "ofApp.h"

const int NUM_TEXES = 8;
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
int maxjourneys = 0;
//--------------------------------------------------------------
void ofApp::setup(){

    img.loadImage("empty_map2.png");
    
    timeline.setup();
    timeline.setDurationInSeconds(60);
    timeline.addCurves("time", ofRange(418, 600));
    timeline.addCurves("x trans", ofRange(-200, 200));
    timeline.addCurves("y trans", ofRange(-200, 200));
    timeline.addCurves("rot", ofRange(0, 90));
    timeline.addCurves("point size", ofRange(0, 10));
    timeline.setSpacebarTogglePlay(true);
    
    
    int maj, min;
    glGetIntegerv(GL_MAJOR_VERSION, &maj);
    glGetIntegerv(GL_MINOR_VERSION, &min);
    printf("OpenGL version %d.%d\n", maj, min);
    
    cout << glGetString(GL_VERSION) << endl;
    
    int w = 512, h = 1024;
//    w = 1024;
    h = w;
    
    char filename[50];
    for (int i = 0; i < NUM_TEXES; i++) {
        texes[i].allocate(w, h, GL_RGB32F);
    
        sprintf(filename, "t%d.data", i);
        float *data = getFileData(ofToDataPath(filename).c_str(), w*h*3 );
        texes[i].loadData(data, w, h, GL_RGB);
        free(data);
    }

    cols.allocate(w, h, GL_RGB32F);
    float *data = getFileData( ofToDataPath( "cols.data" ).c_str(), w*h*3 );
    cols.loadData(data, w, h, GL_RGB);
    colordata = data;
//    free(data);
    
    compute.allocate(w, h, GL_RGB32F);
    
    
    tex1.allocate(ofGetWidth(), ofGetHeight(), GL_RGB32F);
    tex2.allocate(w, h, GL_RGB32F);
    
    image.allocate(w, h, GL_RGB32F);
    
    fbo.allocate(w, h, GL_RGB32F);
    
//    int l = w*h*3;
//    float *data = new float[l];
//    for(int i = 0; i < l; i++) {
//        data[i] = i/(float)l;
//    }
//    tex.loadData(data, w, h, GL_RGB);
//    
    shader.load("basic");
    
    fbo.begin();
    ofClear(0, 0, 0);
    fbo.end();
    
    mainFbo.allocate(ofGetWidth(), ofGetHeight());
    
    offsets = new float[w*h*2];
    for (int i = 0; i < 512*512*2; i++) {
        offsets[i] = (ofRandomf() * 2 - 1) * 2.5;
//        offsets[i] = generateGaussianNoise(4);
    }

    cout << endl;
    ofSetFrameRate(40);
    ofSetVerticalSync(false);
}

void ofApp::exit() {
    free(colordata);
    delete offsets;
}

//--------------------------------------------------------------
void ofApp::update(){
        
}

//--------------------------------------------------------------
void ofApp::draw(){
    
//    timeline.show();


    ofDisableAntiAliasing();
    ofBackgroundHex(0xffffff);
//    tex.draw(0, 0);
//
//    tex1.bind();

//    float time = 1000 + ofGetFrameNum()/50.0;
    float time = 418 + ofGetFrameNum()/20.0;
    
//    time = ofMap(mouseX, 0, ofGetWidth(), 418, 720, true);
    
    fbo.begin();
    shader.begin();
    shader.setUniform1f("time", timeline.getValue("time"));
    shader.setUniform2f("resolution", (float) ofGetWidth(), (float) ofGetHeight());
//    img.getTextureReference().bind();
//    shader.setUniformTexture("tex", cols, 1);
    
    char s[50];
    for (int i = 0; i < NUM_TEXES; i++) {
        sprintf(s, "tex%d", i);
        shader.setUniformTexture(string(s), texes[i], i+2);
    }

//    shader.setUniformTexture("tex0", texes[0], 2);
    tex2.draw(0, 0);
//    img.getTextureReference().unbind();
    shader.end();
    fbo.end();
//    tex1.unbind();
    
    ofTexture fbotex = fbo.getTextureReference();
    ofFloatPixels pix;
    ofTextureData data = fbotex.getTextureData();
    pix.allocate(data.width, data.height, 3);
    fbotex.readToPixels(pix);
    
    float *fpix = pix.getPixels();
    
    
//    cout << time << ": ";
//    for (int i = 0; i < 9; i++) {
//        cout << fpix[i] << ", ";
//    }
//    cout << ": ";
//    for (int i = 0; i < 9; i++) {
//        cout << fpix[pix.size() - 1 - i] << ", ";
//    }
//    cout << endl;
//    
//    cout << pix.size() << endl;
    
    int d = mouseX/100.0;
    d = timeline.getValue("point size");
    points.clear();
    colours.clear();
    for (int i = 0; i < pix.size(); i+= 3) {
        if (fpix[i] > 1) {
//            points.push_back(ofVec2f(fpix[i] + offsets[i], fpix[i+1] + offsets[i+1]));
            points.push_back(ofVec2f(fpix[i] + offsets[i], fpix[i+1] + offsets[i+1]));
            points.push_back(ofVec2f(fpix[i] + offsets[i], fpix[i+1] + offsets[i+1]+d));
            points.push_back(ofVec2f(fpix[i] + offsets[i]+d, fpix[i+1] + offsets[i+1]+d));
            points.push_back(ofVec2f(fpix[i] + offsets[i]+d, fpix[i+1] + offsets[i+1]));
            ofFloatColor col(colordata[i], colordata[i+1], colordata[i+2]);
//            colours.push_back(col);
            colours.push_back(col); colours.push_back(col);
            colours.push_back(col); colours.push_back(col);
        }
    }
    
    
    
//    ofNoFill();/
    mainFbo.begin();

    ofTranslate(timeline.getValue("x trans"), timeline.getValue("y trans"));
    ofScale(0.5, 0.5);
//    ofScale(0.5, 0.5);
    ofRotateX(timeline.getValue("rot"));
    ofBackgroundHex(0xffffff);
    vbo.setVertexData(&points[0], points.size(), GL_STREAM_DRAW);
    vbo.setColorData(&colours[0], colours.size(), GL_STREAM_DRAW);
    vbo.draw(GL_QUADS, 0, points.size());
    
    ofEnableAlphaBlending();
    ofSetColor(200, 200, 200, 200);
    img.draw(0, 0);

    mainFbo.end();
    
    ofSetColor(255, 255, 255, 255);
    
    mainFbo.setAnchorPercent(0.5, 0.5);
    
    ofPushMatrix();
//    ofScale(mouseX/10.0, mouseX/10.0);
//    ofTranslate(ofGetWidth()*0.66, ofGetHeight()*0.66);
//    ofRotateX(mouseY);
    mainFbo.draw(0, 0, ofGetWidth(), ofGetHeight());
    ofPopMatrix();
    
    ofSetHexColor(0x888888);
    ofDrawBitmapString(ofToString(time), 10, 10);
    
    ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, 20);
    
    if (points.size() > maxjourneys) {
        maxjourneys = points.size();
    }
    
    ofDrawBitmapString(ofToString(points.size()) + " journeys (max = " + ofToString(maxjourneys) + ")", 10, 30);

    if (showtimeline) {
        ofSetColor(0, 0, 0, 200);
        ofFill();
        ofRect(0, 0, ofGetWidth(), ofGetHeight());
        timeline.draw();
    }
    
//    texes[0].draw(0, 50);
    
//    
//    if (ofGetFrameNum() > 1) {
//        exitApp();
//    }
    

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
//    time = 0;
    if(key == 's') showtimeline = !showtimeline;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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