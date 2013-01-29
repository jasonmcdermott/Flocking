#pragma once

class Boundary {
public:

    ofVec3f innerTopLeftBack, innerBotRightFront, outerTopLeftBack, outerBotRightFront;
    ofVec3f a,b,c,d,e,f,g,h;
    string type, form;
    ofVec3f offset, centre;
    ofVec3f front, back, left, right, top, bottom;
    float length = 0;
    float halfLength;
//    float front, back, left, right, top, bottom;

    Boundary() {

    }

    void setDims(ofVec3f centre_, float length_) {
        centre.set(centre_);
        length = length_;
        reset();
    }
    
    void updateGUI() {
        reset();
    }
    
    void draw() {
//        ofPushMatrix();
//        ofTranslate(offset);
        ofSetLineWidth(1);
        ofSetColor(255);
        
        // A
        ofLine(a,b);
        ofLine(a,d);
        ofLine(a,e);
            
        // B
        ofLine(b,c);
        ofLine(b,f);
        
        // C
        ofLine(c,d);
        ofLine(c,g);
        
        // D
        ofLine(d,h);
        
        // E
        ofLine(e,f);
        ofLine(e,h);
        
        // F
        ofLine(f,g);
        
        // G
        ofLine(g,h);
        
        ofSetColor(255,0,0);
        ofSphere(a,10);
        ofSetColor(0,255,0);
        ofSphere(d,10);
        


        
//        ofPopMatrix();
    }
    
    void reset() {
        halfLength = length / 2;        
        a.set(centre.x-halfLength,centre.y-halfLength,centre.z+halfLength);
        b.set(centre.x+halfLength,centre.y-halfLength,centre.z+halfLength);
        c.set(centre.x+halfLength,centre.y-halfLength,centre.z-halfLength);
        d.set(centre.x-halfLength,centre.y-halfLength,centre.z-halfLength);
        
        e.set(centre.x-halfLength,centre.y+halfLength,centre.z+halfLength);
        f.set(centre.x+halfLength,centre.y+halfLength,centre.z+halfLength);
        g.set(centre.x+halfLength,centre.y+halfLength,centre.z-halfLength);
        h.set(centre.x-halfLength,centre.y+halfLength,centre.z-halfLength);

        top.set(d);
        
        bottom.set(a);
        bottom.y += length;
        
        left.set(d);
        right.set(b);
        front.set(a);
        back.set(g);
        
//        top = -length;
//        bottom = length;
//        left = -length;
//        right = length;
//        front = length;
//        back = -length;
//        
//        a += offset;
//        b += offset;
//        c += offset;
//        d += offset;
//        e += offset;
//        f += offset;
//        g += offset;
//        h += offset;
        
    }
    
};




