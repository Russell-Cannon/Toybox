#include "Toy.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#define CLONE(Type)                                                  \
    std::shared_ptr<Toy> Type::Clone() {                             \
        std::shared_ptr<Toy> clone = std::shared_ptr<Toy>(new Type); \
        for (int i = 0; i < children.size(); i++) {                  \
            clone->AddChild(GetChild(i)->Clone());                   \
        }                                                            \
        return clone;                                                \
    }

Toy::Toy() {}
std::string Toy::Name() { return "Abstract Toy"; }
std::string Toy::GenerateGLSL() { return ""; }
void Toy::AddChild(std::shared_ptr<Toy> toy) { children.push_back(toy); }
void Toy::RemoveChild(std::shared_ptr<Toy> toy) {
    for (int i = 0; i < NumChildren(); i++) {
        if (toy.get() == GetChild(i).get()) {
            children.erase(children.begin() + i);
            return;
        }
    }
    std::cerr << "Could not remove toy from children of " << Name() << '\n';
}
std::shared_ptr<Toy> Toy::GetChild(int n) const { return children[n]; }
CLONE(Toy)
int Toy::NumChildren() const { return children.size(); }
bool Toy::Empty() const { return children.empty(); };
void Toy::Print(bool last, std::string pre) {
#ifdef _WIN32
    const char BAR = char(179) /*│*/, MIDDLE = char(195) /*├*/, LAST = char(192) /*└*/, DASH = char(196) /*─*/;
#elif __linux__ // WSL wouldn't print Extended ASCII characters right. They might work on an actual Linux operating system, I'm not sure.
    const char BAR = '|' /*│*/, MIDDLE = '>' /*├*/, LAST = 'L' /*└*/, DASH = '-' /*─*/;
#endif
    if (pre.empty())
        pre += BAR;

    // if the last character is a bar, override it with MIDDLE or LAST
    if (pre[pre.length() - 1] == BAR) {
        std::cout << pre.substr(0, pre.length() - 1);
    } else
        std::cout << pre;

    if (last) {
        std::cout << LAST;
        pre = pre.substr(0, pre.length() - 1) + ' ';
    } else
        std::cout << MIDDLE;

    std::cout << DASH << Name() << std::endl;

    for (int i = 0; i < NumChildren(); i++) {
        GetChild(i)->Print(i == NumChildren() - 1, pre + ' ' + BAR);
    }
}

// Literal Toys
std::string LitError::Name() { return "Error"; }
std::string LitError::GenerateGLSL() { return "vec3(1.0, 0, 1.0)"; }
CLONE(LitError)

std::string LitUV::Name() { return "UV"; }
std::string LitUV::GenerateGLSL() {
    return "vec3(gl_FragCoord.xy / iResolution, 0.0)";
}
CLONE(LitUV)

std::string LitTime::Name() { return "Time"; }
std::string LitTime::GenerateGLSL() { return "vec3(time)"; }
CLONE(LitTime)

std::string LitNumber::Name() { return "Number (" + std::to_string(value) + ")"; }
std::string LitNumber::GenerateGLSL() { return "vec3(" + std::to_string(value) + ")"; }
void LitNumber::SetNumber(const double _value) { value = _value; }
double LitNumber::GetNumber() const { return value; };
std::shared_ptr<Toy> LitNumber::Clone() {
    LitNumber* num = new LitNumber();
    num->SetNumber(GetNumber());
    std::shared_ptr<Toy> clone = std::shared_ptr<Toy>(num);
    for (int i = 0; i < children.size(); i++) {
        clone->AddChild(GetChild(i)->Clone());
    }
    return clone;
}

// other
std::string Document::Name() { return "Document"; }
std::string Document::GenerateGLSL() {
    if (children.empty()) {
        std::cerr << Name() << " generated with no children\n";
        AddChild(std::shared_ptr<Toy>(new LitError()));
    }
    return "outColor = vec4(" + children[0]->GenerateGLSL() + ", 1.0);\n";
}
CLONE(Document)

Parameter::Parameter(std::string _ID) { ID = _ID; }
std::string Parameter::Name() { return "Parameter (" + ID + ")"; }
std::string Parameter::GenerateGLSL() { return ""; }
std::shared_ptr<Toy> Parameter::Clone() {
    std::shared_ptr<Toy> clone = std::shared_ptr<Toy>(new Parameter(ID));
    for (int i = 0; i < children.size(); i++) {
        clone->AddChild(GetChild(i)->Clone());
    }
    return clone;
}

// Unary Operations
std::string Unary::Name() { return "Abstract Unary Operation"; }
std::string Unary::GenerateGLSL() {
    if (children.empty()) {
        std::cerr << Name() << " generated with no children\n";
        AddChild(std::shared_ptr<Toy>(new LitError()));
    }
    return "";
}
CLONE(Unary)

std::string Fract::Name() { return "Fract"; }
std::string Fract::GenerateGLSL() {
    Unary::GenerateGLSL();
    return "fract(" + children[0]->GenerateGLSL() + ")";
}
CLONE(Fract)

std::string SawTooth::Name() { return "Saw Tooth (Reflect)"; }
std::string SawTooth::GenerateGLSL() {
    Unary::GenerateGLSL();
    return "(1.0 - abs(fract(" + children[0]->GenerateGLSL() + "*0.5)*2.0-1.0))";
}
CLONE(SawTooth)

std::string Sin::Name() { return "Sine"; }
std::string Sin::GenerateGLSL() {
    Unary::GenerateGLSL();
    return "sin(" + children[0]->GenerateGLSL() + ")";
}
CLONE(Sin)

std::string Cos::Name() { return "Cosine"; }
std::string Cos::GenerateGLSL() {
    Unary::GenerateGLSL();
    return "cos(" + children[0]->GenerateGLSL() + ")";
}
CLONE(Cos)

std::string Tan::Name() { return "Tangent"; }
std::string Tan::GenerateGLSL() {
    Unary::GenerateGLSL();
    return "tan(" + children[0]->GenerateGLSL() + ")";
}
CLONE(Tan)

std::string Normalize::Name() { return "Normalize"; }
std::string Normalize::GenerateGLSL() {
    Unary::GenerateGLSL();
    return "normalize(" + children[0]->GenerateGLSL() + ")";
}
CLONE(Normalize)

std::string Length::Name() { return "Length"; }
std::string Length::GenerateGLSL() {
    Unary::GenerateGLSL();
    return "vec3(length(" + children[0]->GenerateGLSL() + "))";
}
CLONE(Length)

std::string Texture::Name() { return "Texture (" + textureName + ")"; }
std::string Texture::GenerateGLSL() {
    Unary::GenerateGLSL();
    std::string childOutput = children[0]->GenerateGLSL();
    return "texture(u_texture_" + textureName + ", vec2(" + childOutput + ".x, -" + childOutput + ".y)).xyz";
}
void Texture::SetTexture(std::string i) {
    textureName = i;
}
std::string Texture::GetTexture() const {
    return textureName;
}
std::shared_ptr<Toy> Texture::Clone() {
    Texture* text = new Texture();
    text->SetTexture(GetTexture());
    std::shared_ptr<Toy> clone = std::shared_ptr<Toy>(text);
    for (int i = 0; i < children.size(); i++) {
        clone->AddChild(GetChild(i)->Clone());
    }
    return clone;
}

std::string Ceiling::Name() { return "Ceiling"; }
std::string Ceiling::GenerateGLSL() {
    Unary::GenerateGLSL();
    return "ceil(" + children[0]->GenerateGLSL() + ")";
}
CLONE(Ceiling)

std::string Floor::Name() { return "Floor"; }
std::string Floor::GenerateGLSL() {
    Unary::GenerateGLSL();
    return "floor(" + children[0]->GenerateGLSL() + ")";
}
CLONE(Floor)

std::string Round::Name() { return "Round"; }
std::string Round::GenerateGLSL() {
    Unary::GenerateGLSL();
    return "round(" + children[0]->GenerateGLSL() + ")";
}
CLONE(Round)

std::string Negate::Name() { return "Negate"; }
std::string Negate::GenerateGLSL() {
    Unary::GenerateGLSL();
    return "(-" + children[0]->GenerateGLSL() + ")";
}
CLONE(Negate)

std::string Absolute::Name() { return "Absolute"; }
std::string Absolute::GenerateGLSL() {
    Unary::GenerateGLSL();
    return "abs(" + children[0]->GenerateGLSL() + ")";
}
CLONE(Absolute)

std::string X::Name() { return "X"; }
std::string X::GenerateGLSL() {
    Unary::GenerateGLSL();
    return "vec3(" + children[0]->GenerateGLSL() + ".x)";
}
CLONE(X)

std::string Y::Name() { return "Y"; }
std::string Y::GenerateGLSL() {
    Unary::GenerateGLSL();
    return "vec3(" + children[0]->GenerateGLSL() + ".y)";
}
CLONE(Y)

std::string Z::Name() { return "Z"; }
std::string Z::GenerateGLSL() {
    Unary::GenerateGLSL();
    return "vec3(" + children[0]->GenerateGLSL() + ".z)";
}
CLONE(Z)

std::string Screen::Name() { return "Screen Distance Field"; }
std::string Screen::GenerateGLSL() {
    Unary::GenerateGLSL();
    std::string st = "abs(mix(vec2(-1.0), vec2(1.0), " + children[0]->GenerateGLSL() + ".xy))";
    return "vec3(fract(1.0 - max(" + st + ".x, " + st + ".y)))";
}
CLONE(Screen)

std::string Random::Name() { return "Random Hash"; }
std::string Random::GenerateGLSL() {
    Unary::GenerateGLSL();
    std::string seed = children[0]->GenerateGLSL();
    return "vec3(hash(" + seed + ".xy + vec2(" + seed + ".z)))";
}
CLONE(Random)

std::string Noise::Name() { return "Simplex Noise"; }
std::string Noise::GenerateGLSL() {
    Unary::GenerateGLSL();
    std::string seed = children[0]->GenerateGLSL();
    return "vec3((snoise(" + seed + ".xy + vec2(" + seed + ".z)) + 1.0 ) / 2.0)";
}
CLONE(Noise)

// Binary Operations
std::string Binary::Name() { return "Abstract Binary Operation"; }
std::string Binary::GenerateGLSL() {
    if (children.empty()) {
        std::cerr << Name() << " generated with no children\n";
        AddChild(std::shared_ptr<Toy>(new LitError()));
        AddChild(std::shared_ptr<Toy>(new LitError()));
    } else if (children.size() == 1) {
        std::cerr << Name() << "generated with only one child\n";
        AddChild(std::shared_ptr<Toy>(new LitError()));
    }
    return "";
}
CLONE(Binary)

std::string Multiply::Name() { return "Multiply"; }
std::string Multiply::GenerateGLSL() {
    Binary::GenerateGLSL();
    return "(" + children[0]->GenerateGLSL() + " * " + children[1]->GenerateGLSL() + ")";
}
CLONE(Multiply)

std::string Divide::Name() { return "Divide"; }
std::string Divide::GenerateGLSL() {
    Binary::GenerateGLSL();
    return "(" + children[0]->GenerateGLSL() + " / " + children[1]->GenerateGLSL() + ")";
}
CLONE(Divide)

std::string Add::Name() { return "Add"; }
std::string Add::GenerateGLSL() {
    Binary::GenerateGLSL();
    return "(" + children[0]->GenerateGLSL() + " + " + children[1]->GenerateGLSL() + ")";
}
CLONE(Add)

std::string Subtract::Name() { return "Subtract"; }
std::string Subtract::GenerateGLSL() {
    Binary::GenerateGLSL();
    return "(" + children[0]->GenerateGLSL() + " - " + children[1]->GenerateGLSL() + ")";
}
CLONE(Subtract)

std::string Modulus::Name() { return "Modulus"; }
std::string Modulus::GenerateGLSL() {
    Binary::GenerateGLSL();
    return "mod(" + children[0]->GenerateGLSL() + ", " + children[1]->GenerateGLSL() + ")";
}
CLONE(Modulus)

std::string Dot::Name() { return "Dot Product"; }
std::string Dot::GenerateGLSL() {
    Binary::GenerateGLSL();
    return "vec3(dot(" + children[0]->GenerateGLSL() + ", " + children[1]->GenerateGLSL() + "))";
}
CLONE(Dot)

std::string Cross::Name() { return "Cross Product"; }
std::string Cross::GenerateGLSL() {
    Binary::GenerateGLSL();
    return "cross(" + children[0]->GenerateGLSL() + ", " + children[1]->GenerateGLSL() + ")";
}
CLONE(Cross)

std::string Distance::Name() { return "Distance Between"; }
std::string Distance::GenerateGLSL() {
    Binary::GenerateGLSL();
    return "vec3(length(" + children[0]->GenerateGLSL() + " - " + children[1]->GenerateGLSL() + "))";
}
CLONE(Distance)

std::string Direction::Name() { return "Direction From A to B"; }
std::string Direction::GenerateGLSL() {
    Binary::GenerateGLSL();
    return "normalize(" + children[1]->GenerateGLSL() + " - " + children[0]->GenerateGLSL() + ")";
}
CLONE(Direction)

std::string Step::Name() { return "Step"; }
std::string Step::GenerateGLSL() {
    Binary::GenerateGLSL();
    return "step(" + children[0]->GenerateGLSL() + ", " + children[1]->GenerateGLSL() + ")";
}
CLONE(Step)

std::string NGon::Name() { return "N-Gon"; }
std::string NGon::GenerateGLSL() {
    Binary::GenerateGLSL();
    // Credit https://thebookofshaders.com/07/ for the code to make regular polygons using polar functions
    std::string st = "((" + children[0]->GenerateGLSL() /*UV*/ + " * 2.0 - 1.0).xy)";
    std::string a = "(atan(" + st + ".x, " + st + ".y) + PI)";
    std::string r = "(TWO_PI/" + children[1]->GenerateGLSL() /*N*/ + ")";
    return "vec3(cos(floor(.5+" + a + "/" + r + ")*" + r + "-" + a + ")*length(" + st + "))";
}
CLONE(NGon)

std::string Circle::Name() { return "Circular Distance Field"; }
std::string Circle::GenerateGLSL() {
    Binary::GenerateGLSL();
    std::string uv = children[0]->GenerateGLSL();
    std::string point = children[1]->GenerateGLSL();
    return "vec3(length(" + point + ".xy - " + uv + ".xy)*2.0)";
}
CLONE(Circle)

// Trinary Operations
std::string Trinary::Name() { return "Abstract Trinary Operation"; }
std::string Trinary::GenerateGLSL() {
    if (children.empty()) {
        std::cerr << Name() << " generated with no children\n";
        AddChild(std::shared_ptr<Toy>(new LitError()));
        AddChild(std::shared_ptr<Toy>(new LitError()));
        AddChild(std::shared_ptr<Toy>(new LitError()));
    } else if (children.size() == 2) {
        std::cerr << Name() << " generated only two children\n";
        AddChild(std::shared_ptr<Toy>(new LitError()));
    } else if (children.size() == 1) {
        std::cerr << Name() << "generated with only one child\n";
        AddChild(std::shared_ptr<Toy>(new LitError()));
        AddChild(std::shared_ptr<Toy>(new LitError()));
    }
    return "";
}
CLONE(Trinary)

std::string Combine::Name() { return "Combine"; }
std::string Combine::GenerateGLSL() {
    Trinary::GenerateGLSL();
    return "vec3(" + children[0]->GenerateGLSL() + ".x, " + children[1]->GenerateGLSL() + ".y, " + children[2]->GenerateGLSL() + ".z)";
}
CLONE(Combine)

std::string Clamp::Name() { return "Clamp"; }
std::string Clamp::GenerateGLSL() {
    Trinary::GenerateGLSL();
    return "clamp(" + children[0]->GenerateGLSL() + ", " + children[1]->GenerateGLSL() + ", " + children[2]->GenerateGLSL() + ")";
}
CLONE(Clamp)

std::string Mix::Name() { return "Mix"; }
std::string Mix::GenerateGLSL() {
    Trinary::GenerateGLSL();
    return "mix(" + children[0]->GenerateGLSL() + ", " + children[1]->GenerateGLSL() + ", " + children[2]->GenerateGLSL() + ")";
}
CLONE(Mix)

std::string SmoothStep::Name() { return "SmoothStep"; }
std::string SmoothStep::GenerateGLSL() {
    Trinary::GenerateGLSL();
    return "smoothstep(" + children[0]->GenerateGLSL() + ", " + children[1]->GenerateGLSL() + ", " + children[2]->GenerateGLSL() + ")";
}
CLONE(SmoothStep)

std::string Line::Name() { return "Line"; }
std::string Line::GenerateGLSL() {
    Trinary::GenerateGLSL();
    // https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line#Line_defined_by_an_equation

    std::string uv = children[0]->GenerateGLSL();
    std::string p1 = children[1]->GenerateGLSL();
    std::string p2 = children[2]->GenerateGLSL();

    std::string x0 = "(" + uv + ").x";
    std::string y0 = "(" + uv + ").y";
    std::string x1 = "(" + p1 + ").x";
    std::string y1 = "(" + p1 + ").y";
    std::string x2 = "(" + p2 + ").x";
    std::string y2 = "(" + p2 + ").y";

    return "vec3(abs((" + y2 + " - " + y1 + ")*" + x0 + " - (" + x2 +
           " - " + x1 + ")*" + y0 + " + " + x2 + "*" + y1 + " - " + y2 +
           "*" + x1 + ")/length(" + p2 + " - " + p1 + ")*2.0)";
}
CLONE(Line)

std::string LineSegment::Name() { return "Line Segment"; }
std::string LineSegment::GenerateGLSL() {
    // generate the distance field for a line, using the base line class
    std::string d = Line::GenerateGLSL() + ".x";

    // fetch parameters, again
    std::string uv = children[0]->GenerateGLSL() + ".xy";
    std::string p1 = children[1]->GenerateGLSL() + ".xy";
    std::string p2 = children[2]->GenerateGLSL() + ".xy";

    // create a mask to clip the ends of the line off
    std::string mask = "(step(0.0, dot( normalize(" + p1 + " - " + p2 +
                       "), normalize(" + uv + " - " + p2 + ")))";
    mask += "* step(0.0, dot(normalize(" + p2 + " - " + p1 +
            "), normalize(" + uv + " - " + p1 + "))))";
    d = "(" + d + "*" + mask + " + (1.0 - " + mask + "))";

    // create two dots at either end of the line to make end caps
    std::string dp1 = "length(" + p1 + " - " + uv + ") * 2.0";
    std::string dp2 = "length(" + p2 + " - " + uv + ") * 2.0";

    // Min everything together
    return "vec3(min(min(" + dp1 + ", " + d + "), " + dp2 + "))";
}
CLONE(LineSegment)

// N-ary Operations
std::string Average::Name() { return "Average"; }
std::string Average::GenerateGLSL() {
    if (children.empty()) {
        std::cerr << Name() << " generated with no children\n";
        AddChild(std::shared_ptr<Toy>(new LitError()));
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
CLONE(Average)
