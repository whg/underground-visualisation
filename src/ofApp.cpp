#include "ofApp.h"

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

void ofApp::setup(){

    img.loadImage("empty_map_nocircles.png");
    
    timeline.setup();
    timeline.setDurationInSeconds(100);
    timeline.addCurves("time", ofRange(418, 600));
    timeline.addCurves("x trans", ofRange(-300, 300));
    timeline.addCurves("y trans", ofRange(0, 500));
    timeline.addCurves("x2 trans", ofRange(-400, 400));
    timeline.addCurves("y2 trans", ofRange(-400, 400));
    timeline.addCurves("z trans", ofRange(0, 1600));
    timeline.addCurves("z2 trans", ofRange(0, 1000));
    timeline.addCurves("rot", ofRange(0, 90));
    timeline.addCurves("point size", ofRange(0, 10));
    timeline.addCurves("img alpha", ofRange(0, 255));
    timeline.setSpacebarTogglePlay(true);
    
    
    int maj, min;
    glGetIntegerv(GL_MAJOR_VERSION, &maj);
    glGetIntegerv(GL_MINOR_VERSION, &min);
    printf("OpenGL version %d.%d\n", maj, min);
    
    cout << glGetString(GL_VERSION) << endl;
    
    int w = 512, h = 1024;
//    w = 1024;
    h = w;
    
    
    // load data into each of the textures
    char filename[50];
    for (int i = 0; i < NUM_TEXES; i++) {
        texes[i].allocate(w, h, GL_RGB32F);
    
        sprintf(filename, "t%d.data", i);
        float *data = getFileData(ofToDataPath(filename).c_str(), w*h*3 );
        texes[i].loadData(data, w, h, GL_RGB);
        free(data);
    }

    // load the colours
    cols.allocate(w, h, GL_RGB32F);
    float *data = getFileData( ofToDataPath( "cols.data" ).c_str(), w*h*3 );
    cols.loadData(data, w, h, GL_RGB);
    colordata = data;
    
    compute.allocate(w, h, GL_RGB32F);
    
    
    tex1.allocate(ofGetWidth(), ofGetHeight(), GL_RGB32F);
    tex2.allocate(w, h, GL_RGB32F);
    
    image.allocate(w, h, GL_RGB32F);
    
    fbo.allocate(w, h, GL_RGB32F);
    
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
    ofSetFrameRate(25);
    ofSetVerticalSync(false);
}

void ofApp::exit() {
    free(colordata);
    delete offsets;
}


void ofApp::update(){
        
}


void ofApp::draw(){

    ofDisableAntiAliasing();
    ofBackgroundHex(0xffffff);
    float time = 418 + ofGetFrameNum()/20.0;
    
    fbo.begin();
    shader.begin();
    shader.setUniform1f("time", timeline.getValue("time"));
    shader.setUniform2f("resolution", (float) ofGetWidth(), (float) ofGetHeight());
    
    char s[50];
    for (int i = 0; i < NUM_TEXES; i++) {
        sprintf(s, "tex%d", i);
        shader.setUniformTexture(string(s), texes[i], i+2);
    }
    tex2.draw(0, 0);
    shader.end();
    fbo.end();

    ofTexture fbotex = fbo.getTextureReference();
    ofFloatPixels pix;
    ofTextureData data = fbotex.getTextureData();
    pix.allocate(data.width, data.height, 3);
    fbotex.readToPixels(pix);
    
    float *fpix = pix.getPixels();
    
    
    float d = timeline.getValue("point size");
    points.clear();
    colours.clear();
    for (int i = 0; i < pix.size(); i+= 3) {
        if (fpix[i] > 1) {
            points.push_back(ofVec2f(fpix[i] + offsets[i], fpix[i+1] + offsets[i+1]));
            points.push_back(ofVec2f(fpix[i] + offsets[i], fpix[i+1] + offsets[i+1]+d));
            points.push_back(ofVec2f(fpix[i] + offsets[i]+d, fpix[i+1] + offsets[i+1]+d));
            points.push_back(ofVec2f(fpix[i] + offsets[i]+d, fpix[i+1] + offsets[i+1]));

            ofFloatColor col(colordata[i], colordata[i+1], colordata[i+2]);
            colours.push_back(col); colours.push_back(col);
            colours.push_back(col); colours.push_back(col);
        }
    }
    
    
    
    mainFbo.begin();

    ofTranslate(timeline.getValue("x trans"), timeline.getValue("y trans"), timeline.getValue("z trans"));
//    ofScale(0.5, 0.5);
//    ofScale(0.5, 0.5);
//    ofRotateX(timeline.getValue("rot"));
    ofBackgroundHex(0xffffff);
    
    ofSetColor(255, 255, 255, timeline.getValue("img alpha"));
    img.draw(d+5, d-5);
    
    vbo.setVertexData(&points[0], points.size(), GL_STREAM_DRAW);
    vbo.setColorData(&colours[0], colours.size(), GL_STREAM_DRAW);
    vbo.draw(GL_QUADS, 0, points.size());
    
    ofEnableAlphaBlending();
   

    mainFbo.end();
    
    ofSetColor(255, 255, 255, 255);
    
    mainFbo.setAnchorPercent(0.5, 0.5);
    
    ofPushMatrix();
    ofTranslate(ofGetWidth()*0.5, ofGetHeight()*0.5);
    ofTranslate(timeline.getValue("x2 trans"), timeline.getValue("y2 trans"), timeline.getValue("z2 trans"));
    ofRotateX(timeline.getValue("rot"));

    mainFbo.draw(0, 0, ofGetWidth(), ofGetHeight());
    ofPopMatrix();
    
//    ofSetHexColor(0x888888);
//    ofDrawBitmapString(ofToString(time), 10, 10);
//    ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, 20);
//    if (points.size() > maxjourneys) {
//        maxjourneys = points.size();
//    }
//    ofDrawBitmapString(ofToString(points.size()) + " journeys (max = " + ofToString(maxjourneys) + ")", 10, 30);

    if (showtimeline) {
        ofSetColor(0, 0, 0, 200);
        ofFill();
        ofRect(0, 0, ofGetWidth(), ofGetHeight());
        timeline.draw();
    }
    

}


void ofApp::keyPressed(int key){
//    time = 0;
    if(key == 's') showtimeline = !showtimeline;
}


void ofApp::keyReleased(int key){

}


void ofApp::mouseMoved(int x, int y){

}


void ofApp::mouseDragged(int x, int y, int button){

}


void ofApp::mousePressed(int x, int y, int button){

}


void ofApp::mouseReleased(int x, int y, int button){

}


void ofApp::windowResized(int w, int h){

}


void ofApp::gotMessage(ofMessage msg){

}


void ofApp::dragEvent(ofDragInfo dragInfo){ 

}