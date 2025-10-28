#pragma once
#include <string>
#include "Types.hpp"

class Toy {
public: 
    Toy() { }
    Toy* child = nullptr;
    virtual std::string GenerateGLSL() {return "";};
    void AddChild(Toy* toy) {
        child = toy;
    }
};

class LitError : public Toy {
public: 
    std::string GenerateGLSL() {
        return "1.0, 0, 1.0";
    }
};

class Document : public Toy {
public: 
    std::string GenerateGLSL() {
        if (child == nullptr) {
            std::cerr << "Document generated with no children\n";
            child = new LitError();
        }
        return "outColor = vec4("+child->GenerateGLSL()+", 1.0);";
    }
};

class LitUV : public Toy {
    std::string GenerateGLSL() {return "gl_FragCoord.xy / iResolution, 0.0";}
};
class LitTime : public Toy {
    std::string GenerateGLSL() {return "time, time, time";}
};

class Fract : public Toy {
    std::string GenerateGLSL() {
        if (child == nullptr) {
            std::cerr << "Fract generated with no children\n";
            child = new LitError();
        }
        return "fract(vec3("+child->GenerateGLSL()+"))";
    }
};