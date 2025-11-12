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
    std::string Name() {return "Number (" + std::to_string(Value) + ")";}
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
class SawTooth : public Unary {
    std::string Name() {return "Saw Tooth (Reflect)";}
    std::string GenerateGLSL() {
        Unary::GenerateGLSL();
        return "(1.0 - abs(fract("+children[0]->GenerateGLSL()+"*0.5)*2.0-1.0))";
    }
};
class Sin : public Unary {
    std::string Name() {return "Sine";}
    std::string GenerateGLSL() {
        Unary::GenerateGLSL();
        return "sin("+children[0]->GenerateGLSL()+")";
    }
};
class Cos : public Unary {
    std::string Name() {return "Cosine";}
    std::string GenerateGLSL() {
        Unary::GenerateGLSL();
        return "cos("+children[0]->GenerateGLSL()+")";
    }
};
class Tan : public Unary {
    std::string Name() {return "Tangent";}
    std::string GenerateGLSL() {
        Unary::GenerateGLSL();
        return "tan("+children[0]->GenerateGLSL()+")";
    }
};
class Normalize : public Unary {
    std::string Name() {return "Normalize";}
    std::string GenerateGLSL() {
        Unary::GenerateGLSL();
        return "normalize("+children[0]->GenerateGLSL()+")";
    }
};
class Length : public Unary {
    std::string Name() {return "Length";}
    std::string GenerateGLSL() {
        Unary::GenerateGLSL();
        return "vec3(length("+children[0]->GenerateGLSL()+"))";
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
class Screen : public Unary {
    std::string Name() { return "Screen Distance Field"; }
    std::string GenerateGLSL() {
        Unary::GenerateGLSL();
        std::string st = "abs(mix(vec2(-1.0), vec2(1.0), " + children[0]->GenerateGLSL() + ".xy))";
        return "vec3(fract(1.0 - max(" + st + ".x, " + st + ".y)))";
    }
};
class Random : public Unary {
    std::string Name() { return "Random Hash"; }
    std::string GenerateGLSL() {
        Unary::GenerateGLSL();
        std::string seed = children[0]->GenerateGLSL();
        return "vec3(hash("+seed+".xy + vec2("+seed+".z)))";
    }
};
class Noise : public Unary {
    std::string Name() { return "Simplex Noise"; }
    std::string GenerateGLSL() {
        Unary::GenerateGLSL();
        std::string seed = children[0]->GenerateGLSL();
        return "vec3((snoise("+seed+".xy + vec2("+seed+".z)) + 1.0 ) / 2.0)";
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
class Dot : public Binary {
    std::string Name() {return "Dot Product";}
    std::string GenerateGLSL() {
        Binary::GenerateGLSL();
        return "vec3(dot("+children[0]->GenerateGLSL()+", "+children[1]->GenerateGLSL()+"))";
    }
};
class Cross : public Binary {
    std::string Name() {return "Cross Product";}
    std::string GenerateGLSL() {
        Binary::GenerateGLSL();
        return "cross("+children[0]->GenerateGLSL()+", "+children[1]->GenerateGLSL()+")";
    }
};
class Distance : public Binary {
    std::string Name() {return "Distance Between";}
    std::string GenerateGLSL() {
        Binary::GenerateGLSL();
        return "vec3(length("+children[0]->GenerateGLSL()+" - "+children[1]->GenerateGLSL()+"))";
    }
};
class Direction : public Binary {
    std::string Name() {return "Direction From A to B";}
    std::string GenerateGLSL() {
        Binary::GenerateGLSL();
        return "normalize("+children[1]->GenerateGLSL()+" - "+children[0]->GenerateGLSL()+")";
    }
};

class Step : public Binary {
    std::string Name() {return "Step";}
    std::string GenerateGLSL() {
        Binary::GenerateGLSL();
        return "step(" + children[0]->GenerateGLSL() + ", " + children[1]->GenerateGLSL() + ")";
    }
};
class NGon : public Binary {
    std::string Name() {return "N-Gon";}
    std::string GenerateGLSL() {
        Binary::GenerateGLSL();
        //Credit https://thebookofshaders.com/07/ for the code to make regular polygons using polar functions
        std::string st = "((" + children[0]->GenerateGLSL() /*UV*/ + " * 2.0 - 1.0).xy)";
        std::string a = "(atan(" + st + ".x, " + st + ".y) + PI)";
        std::string r = "(TWO_PI/" + children[1]->GenerateGLSL() /*N*/ + ")";
        return "vec3(cos(floor(.5+" + a + "/" + r + ")*" + r + "-" + a + ")*length(" + st + "))";
    }
};
class Circle : public Binary {
    std::string Name() { return "Circular Distance Field"; }
    std::string GenerateGLSL() {
        Binary::GenerateGLSL();
        std::string uv = children[0]->GenerateGLSL();
        std::string point = children[1]->GenerateGLSL();
        return "vec3(length("+point+".xy - "+uv+".xy)*2.0)";
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
class Line : public Trinary {
    std::string Name() {return "Line";}
protected:
    std::string GenerateGLSL() {
        Trinary::GenerateGLSL();
        //Credit https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line#Line_defined_by_an_equation for equation

        std::string uv = children[0]->GenerateGLSL();
        std::string p1 = children[1]->GenerateGLSL();
        std::string p2 = children[2]->GenerateGLSL();

        std::string x0 = "(" + uv + ").x";
        std::string y0 = "(" + uv + ").y";
        std::string x1 = "(" + p1 + ").x";
        std::string y1 = "(" + p1 + ").y";
        std::string x2 = "(" + p2 + ").x";
        std::string y2 = "(" + p2 + ").y";

        return "vec3(abs(("+y2+" - "+y1+")*"+x0+" - ("+x2+" - "+x1+")*"+y0+" + "+x2+"*"+y1+" - "+y2+"*"+x1+")/length("+p2+" - "+p1+")*2.0)";
    }
};
class LineSegment : public Line {
    std::string Name() {return "Line Segment";}
    std::string GenerateGLSL() {
        //generate the distance field for a line, using the base line class
        std::string d = Line::GenerateGLSL() + ".x"; 

        //fetch parameters, again
        std::string uv = children[0]->GenerateGLSL() + ".xy";
        std::string p1 = children[1]->GenerateGLSL() + ".xy";
        std::string p2 = children[2]->GenerateGLSL() + ".xy";

        //create a mask to clip the ends of the line off
        std::string mask = "(step(0.0, dot( normalize("+p1+" - "+p2+"), normalize("+uv+" - "+p2+")))";
        mask += "* step(0.0, dot(normalize("+p2+" - "+p1+"), normalize("+uv+" - "+p1+"))))";
        d = "(" + d + "*"+mask+" + (1.0 - "+mask+"))";

        //create two dots at either end of the line to make end caps
        std::string dp1 = "length("+p1+" - "+uv+") * 2.0";
        std::string dp2 = "length("+p2+" - "+uv+") * 2.0";

        //Min everything together
        return "vec3(min(min("+dp1+", "+d+"), "+dp2+"))";
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
