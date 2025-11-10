#pragma once
#include <string>
#include <vector>
#include "Types.hpp"

class Toy {
public: 
    Toy() { }
    std::vector<Toy*> children;
    virtual std::string Name() {return "Abstract Toy";}
    virtual std::string GenerateGLSL() {return "";};
    void AddChild(Toy* toy) {
        children.push_back(toy);
    }
};

//Literal Toys
class LitError : public Toy {
public: 
    std::string Name() {return "Error";}
    std::string GenerateGLSL() {
        return "vec3(1.0, 0, 1.0)";
    }
};
class LitUV : public Toy {
    std::string Name() {return "UV";}
    std::string GenerateGLSL() {return "vec3(gl_FragCoord.xy / iResolution, 0.0)";}
};
class LitTime : public Toy {
    std::string Name() {return "Time";}
    std::string GenerateGLSL() {return "vec3(time)";}
};
class LitNumber : public Toy {
    std::string Name() {return "Number";}
    std::string GenerateGLSL() {
        return "vec3(" + std::to_string(Value) + ")";
    }
public:
    float Value;
};

//Operations
////Unary
class Unary : public Toy {
public:
    std::string GenerateGLSL() {
        if (children.empty()) {
            std::cerr << Name() << " generated with no children\n";
            AddChild(new LitError());
        }
        return "";
    }
};
class Fract : public Unary {
    std::string Name() {return "Fact";}
    std::string GenerateGLSL() {
        Unary::GenerateGLSL();
        return "fract("+children[0]->GenerateGLSL()+")";
    }
};
class Sin : public Unary {
    std::string Name() {return "Sin";}
    std::string GenerateGLSL() {
        Unary::GenerateGLSL();
        return "sin("+children[0]->GenerateGLSL()+")";
    }
};
class Cos : public Unary {
    std::string Name() {return "Cos";}
    std::string GenerateGLSL() {
        Unary::GenerateGLSL();
        return "cos("+children[0]->GenerateGLSL()+")";
    }
};
class Tan : public Unary {
    std::string Name() {return "Tan";}
    std::string GenerateGLSL() {
        Unary::GenerateGLSL();
        return "tan("+children[0]->GenerateGLSL()+")";
    }
};
class Texture : public Unary {
    std::string Name() {return "Texture";}
    std::string GenerateGLSL() {
        Unary::GenerateGLSL();
        std::string childOutput = children[0]->GenerateGLSL();
        return "texture(u_texture, vec2(" + childOutput + ".x, -" + childOutput + ".y)).xyz";
    }
};
class Ceiling : public Unary {
    std::string Name() {return "Ceiling";}
    std::string GenerateGLSL() {
        Unary::GenerateGLSL();
        return "ceil(" + children[0]->GenerateGLSL() + ")";
    }
};
class Floor : public Unary {
    std::string Name() {return "Floor";}
    std::string GenerateGLSL() {
        Unary::GenerateGLSL();
        return "floor(" + children[0]->GenerateGLSL() + ")";
    }
};
class Round : public Unary {
    std::string Name() {return "Round";}
    std::string GenerateGLSL() {
        Unary::GenerateGLSL();
        return "round(" + children[0]->GenerateGLSL() + ")";
    }
};
class Negate : public Unary {
    std::string Name() {return "Negate";}
    std::string GenerateGLSL() {
        Unary::GenerateGLSL();
        return "(-" + children[0]->GenerateGLSL() + ")";
    }
};
class Absolute : public Unary {
    std::string Name() {return "Absolute";}
    std::string GenerateGLSL() {
        Unary::GenerateGLSL();
        return "abs(" + children[0]->GenerateGLSL() + ")";
    }
};
class X : public Unary {
    std::string Name() {return "X";}
    std::string GenerateGLSL() {
        Unary::GenerateGLSL();
        return "vec3(" + children[0]->GenerateGLSL() + ".x)";
    }
};
class Y : public Unary {
    std::string Name() {return "Y";}
    std::string GenerateGLSL() {
        Unary::GenerateGLSL();
        return "vec3(" + children[0]->GenerateGLSL() + ".y)";
    }
};
class Z : public Unary {
    std::string Name() {return "Z";}
    std::string GenerateGLSL() {
        Unary::GenerateGLSL();
        return "vec3(" + children[0]->GenerateGLSL() + ".z)";
    }
};

////Binary
class Binary : public Toy {
public:
    std::string GenerateGLSL() {
        if (children.empty()) {
            std::cerr << Name() << " generated with no children\n";
            AddChild(new LitError());
            AddChild(new LitError());
        } else if (children.size() == 1) {
            std::cerr << Name() << "generated with only one child\n";
            AddChild(new LitError());
        }
        return "";
    }
};
class Multiply : public Binary {
    std::string Name() {return "Multiply";}
    std::string GenerateGLSL() {
        Binary::GenerateGLSL();
        return "(" + children[0]->GenerateGLSL() + " * " + children[1]->GenerateGLSL() + ")";
    }
};
class Divide : public Binary {
    std::string Name() {return "Divide";}
    std::string GenerateGLSL() {
        Binary::GenerateGLSL();
        return "(" + children[0]->GenerateGLSL() + " / " +children[1]->GenerateGLSL() + ")";
    }
};
class Add : public Binary {
    std::string Name() {return "Add";}
    std::string GenerateGLSL() {
        Binary::GenerateGLSL();
        return "(" + children[0]->GenerateGLSL() + " + "+children[1]->GenerateGLSL() + ")";
    }
};
class Subtract : public Binary {
    std::string Name() {return "Subtract";}
    std::string GenerateGLSL() {
        Binary::GenerateGLSL();
        return "(" + children[0]->GenerateGLSL() + " - " + children[1]->GenerateGLSL() + ")";
    }
};
class Modulus : public Binary {
    std::string Name() {return "Modulus";}
    std::string GenerateGLSL() {
        Binary::GenerateGLSL();
        return "mod(" + children[0]->GenerateGLSL() + ", " + children[1]->GenerateGLSL() + ")";
    }
};
class Step : public Binary {
    std::string Name() {return "Step";}
    std::string GenerateGLSL() {
        Binary::GenerateGLSL();
        return "step(" + children[0]->GenerateGLSL() + ", " + children[1]->GenerateGLSL() + ")";
    }
};
////Trinary
class Trinary : public Toy {
public:
    std::string GenerateGLSL() {
        if (children.empty()) {
            std::cerr << Name() << " generated with no children\n";
            AddChild(new LitError());
            AddChild(new LitError());
            AddChild(new LitError());
        } else if (children.size() == 2) {
            std::cerr << Name() << " generated only two children\n";
            AddChild(new LitError());
        } else if (children.size() == 1) {
            std::cerr << Name() << "generated with only one child\n";
            AddChild(new LitError());
            AddChild(new LitError());
        }
        return "";
    }
};
class Combine : public Trinary {
    std::string Name() {return "Combine";}
    std::string GenerateGLSL() {
        Trinary::GenerateGLSL();
        return "vec3(" + children[0]->GenerateGLSL() + ".x, " + children[1]->GenerateGLSL() + ".y, " + children[2]->GenerateGLSL() + ".z)";
    }
};
class Clamp : public Trinary {
    std::string Name() {return "Clamp";}
    std::string GenerateGLSL() {
        Trinary::GenerateGLSL();
        return "clamp(" + children[0]->GenerateGLSL() + ", " + children[1]->GenerateGLSL() + ", " + children[2]->GenerateGLSL() + ")";
    }
};
class Mix : public Trinary {
    std::string Name() {return "Mix";}
    std::string GenerateGLSL() {
        Trinary::GenerateGLSL();
        return "mix(" + children[0]->GenerateGLSL() + ", " + children[1]->GenerateGLSL() + ", " + children[2]->GenerateGLSL() + ")";
    }
};
class SmoothStep : public Trinary {
    std::string Name() {return "SmoothStep";}
    std::string GenerateGLSL() {
        Trinary::GenerateGLSL();
        return "smoothstep(" + children[0]->GenerateGLSL() + ", " + children[1]->GenerateGLSL() + ", " + children[2]->GenerateGLSL() + ")";
    }
};

//n-ary
class Average : public Toy {
    std::string Name() {return "Average";}
    std::string GenerateGLSL() {
        if (children.empty()) {
            std::cerr << Name() << " generated with no children\n";
            AddChild(new LitError());
        }
        std::string output = "((";
        for (int i = 0; i < children.size(); i++) {
            output += children[i]->GenerateGLSL();
            if (i < children.size() - 1)
                output += " + ";
        }
        output += ") / vec3(" + std::to_string(children.size()) + ".0))";
        return output;
    }
};

class Document : public Toy {
public: 
    std::string Name() {return "Document";}
    std::string GenerateGLSL() {
        if (children.empty()) {
            std::cerr << Name() << " generated with no children\n";
            AddChild(new LitError());
        }
        return "outColor = vec4(" + children[0]->GenerateGLSL() + ", 1.0);\n";
    }
};
