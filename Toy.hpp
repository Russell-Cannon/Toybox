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
        return "vec3(1.0, 0, 1.0)";
    }
};
class LitUV : public Toy {
    std::string GenerateGLSL() {return "vec3(gl_FragCoord.xy / iResolution, 0.0)";}
};
class LitTime : public Toy {
    std::string GenerateGLSL() {return "vec3(time)";}
};
class LitNumber : public Toy {
    std::string GenerateGLSL() {
        return "vec3(" + std::to_string(Value) + ")";
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
        return "fract("+children[0]->GenerateGLSL()+")";
    }
};
class Sin : public Toy {
    std::string GenerateGLSL() {
        if (children.empty()) {
            std::cerr << "Sin generated with no children\n";
            AddChild(new LitError());
        }
        return "sin("+children[0]->GenerateGLSL()+")";
    }
};
class Cos : public Toy {
    std::string GenerateGLSL() {
        if (children.empty()) {
            std::cerr << "Cos generated with no children\n";
            AddChild(new LitError());
        }
        return "cos("+children[0]->GenerateGLSL()+")";
    }
};
class Tan : public Toy {
    std::string GenerateGLSL() {
        if (children.empty()) {
            std::cerr << "Tan generated with no children\n";
            AddChild(new LitError());
        }
        return "tan("+children[0]->GenerateGLSL()+")";
    }
};
class Texture : public Toy {
    std::string GenerateGLSL() {
        if (children.empty()) {
            std::cerr << "Texture generated with no children\n";
            AddChild(new LitError());
        }
        std::string childOutput = children[0]->GenerateGLSL();
        return "texture(u_texture, vec2(" + childOutput + ".x, -" + childOutput + ".y)).xyz";
    }
};
class Ceiling : public Toy {
    std::string GenerateGLSL() {
        if (children.empty()) {
            std::cerr << "Ceiling generated with no children\n";
            AddChild(new LitError());
        }
        return "ceil(" + children[0]->GenerateGLSL() + ")";
    }
};
class Floor : public Toy {
    std::string GenerateGLSL() {
        if (children.empty()) {
            std::cerr << "Floor generated with no children\n";
            AddChild(new LitError());
        }
        return "floor(" + children[0]->GenerateGLSL() + ")";
    }
};
class Round : public Toy {
    std::string GenerateGLSL() {
        if (children.empty()) {
            std::cerr << "Round generated with no children\n";
            AddChild(new LitError());
        }
        return "round(" + children[0]->GenerateGLSL() + ")";
    }
};
class Negate : public Toy {
    std::string GenerateGLSL() {
        if (children.empty()) {
            std::cerr << "Negate generated with no children\n";
            AddChild(new LitError());
        }
        return "(-" + children[0]->GenerateGLSL() + ")";
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
        return "(" + children[0]->GenerateGLSL() + " * "+children[1]->GenerateGLSL() + ")";
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
        return "(" + children[0]->GenerateGLSL() + " / " +children[1]->GenerateGLSL() + ")";
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
        return "(" + children[0]->GenerateGLSL() + " + "+children[1]->GenerateGLSL() + ")";
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
        return "(" + children[0]->GenerateGLSL() + " - " + children[1]->GenerateGLSL() + ")";
    }
};
class Modulus : public Toy {
    std::string GenerateGLSL() {
        if (children.empty()) {
            std::cerr << "Modulus generated with no children\n";
            AddChild(new LitError());
            AddChild(new LitError());
        } else if (children.size() == 1) {
            std::cerr << "Modulus generated with only one child\n";
            AddChild(new LitError());
        }
        return "mod(" + children[0]->GenerateGLSL() + ", " + children[1]->GenerateGLSL() + ")";
    }
};
////Trinary
class Combine : public Toy {
    std::string GenerateGLSL() {
        if (children.empty()) {
            std::cerr << "Combine generated with no children\n";
            AddChild(new LitError());
            AddChild(new LitError());
            AddChild(new LitError());
        } else if (children.size() == 2) {
            std::cerr << "Combine generated only two children\n";
            AddChild(new LitError());
        } else if (children.size() == 1) {
            std::cerr << "Combine generated with only one child\n";
            AddChild(new LitError());
            AddChild(new LitError());
        }
        return "vec3(" + children[0]->GenerateGLSL() + ".x, " + children[1]->GenerateGLSL() + ".y, " + children[2]->GenerateGLSL() + ".z)";
    }
};

class Document : public Toy {
public: 
    std::string GenerateGLSL() {
        if (children.empty()) {
            std::cerr << "Document generated with no children\n";
            AddChild(new LitError());
        }
        return "outColor = vec4(" + children[0]->GenerateGLSL() + ", 1.0);\n";
    }
};
