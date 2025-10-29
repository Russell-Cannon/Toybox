#pragma once
#include <string>
#include <vector>
#include "Types.hpp"

class Toy {
public: 
    Toy() { }
    std::vector<Toy*> children;
    virtual std::string GenerateGLSL() {return "";};
    void AddChild(Toy* toy) {
        children.push_back(toy);
    }
};

//Literal Toys
class LitError : public Toy {
public: 
    std::string GenerateGLSL() {
        return "1.0, 0, 1.0";
    }
};
class LitUV : public Toy {
    std::string GenerateGLSL() {return "gl_FragCoord.xy / iResolution, 0.0";}
};
class LitTime : public Toy {
    std::string GenerateGLSL() {return "time, time, time";}
};
class LitNumber : public Toy {
    std::string GenerateGLSL() {
        std::string floatToString = std::to_string(Value);
        return floatToString + ", " + floatToString + ", " + floatToString;
    }
public:
    float Value;
};

//Operations
////Unary
class Fract : public Toy {
    std::string GenerateGLSL() {
        if (children.empty()) {
            std::cerr << "Fract generated with no children\n";
            AddChild(new LitError());
        }
        return "fract(vec3("+children[0]->GenerateGLSL()+"))";
    }
};
class Sin : public Toy {
    std::string GenerateGLSL() {
        if (children.empty()) {
            std::cerr << "Sin generated with no children\n";
            AddChild(new LitError());
        }
        return "sin(vec3("+children[0]->GenerateGLSL()+"))";
    }
};
class Cos : public Toy {
    std::string GenerateGLSL() {
        if (children.empty()) {
            std::cerr << "Cos generated with no children\n";
            AddChild(new LitError());
        }
        return "cos(vec3("+children[0]->GenerateGLSL()+"))";
    }
};
class Tan : public Toy {
    std::string GenerateGLSL() {
        if (children.empty()) {
            std::cerr << "Tan generated with no children\n";
            AddChild(new LitError());
        }
        return "tan(vec3("+children[0]->GenerateGLSL()+"))";
    }
};
class Texture : public Toy {
    std::string GenerateGLSL() {
        if (children.empty()) {
            std::cerr << "Texture generated with no children\n";
            AddChild(new LitError());
        }
        std::string childOutput = children[0]->GenerateGLSL();
        return "texture(u_texture, vec2(vec3(" + childOutput + ").x, -vec3(" + childOutput + ").y)).xyz";
    }
};

////Binary
class Multiply : public Toy {
    std::string GenerateGLSL() {
        if (children.empty()) {
            std::cerr << "Multiply generated with no children\n";
            AddChild(new LitError());
            AddChild(new LitError());
        } else if (children.size() == 1) {
            std::cerr << "Multiply generated with only one child\n";
            AddChild(new LitError());
        }
        return "vec3(" + children[0]->GenerateGLSL() + ") * vec3("+children[1]->GenerateGLSL() + ")";
    }
};
class Divide : public Toy {
    std::string GenerateGLSL() {
        if (children.empty()) {
            std::cerr << "Divide generated with no children\n";
            AddChild(new LitError());
            AddChild(new LitError());
        } else if (children.size() == 1) {
            std::cerr << "Divide generated with only one child\n";
            AddChild(new LitError());
        }
        return "vec3(" + children[0]->GenerateGLSL() + ") / vec3("+children[1]->GenerateGLSL() + ")";
    }
};
class Add : public Toy {
    std::string GenerateGLSL() {
        if (children.empty()) {
            std::cerr << "Add generated with no children\n";
            AddChild(new LitError());
            AddChild(new LitError());
        } else if (children.size() == 1) {
            std::cerr << "Add generated with only one child\n";
            AddChild(new LitError());
        }
        return "vec3(" + children[0]->GenerateGLSL() + ") + vec3("+children[1]->GenerateGLSL() + ")";
    }
};
class Subtract : public Toy {
    std::string GenerateGLSL() {
        if (children.empty()) {
            std::cerr << "Subtract generated with no children\n";
            AddChild(new LitError());
            AddChild(new LitError());
        } else if (children.size() == 1) {
            std::cerr << "Subtract generated with only one child\n";
            AddChild(new LitError());
        }
        return "vec3(" + children[0]->GenerateGLSL() + ") - vec3("+children[1]->GenerateGLSL() + ")";
    }
};

class Document : public Toy {
public: 
    std::string GenerateGLSL() {
        if (children.empty()) {
            std::cerr << "Document generated with no children\n";
            AddChild(new LitError());
        }
        return "outColor = vec4("+children[0]->GenerateGLSL()+", 1.0);\n";
    }
};
