#include <iostream>
#include <cstring>

#include "mathpls.h" // github.com/Ninter6/mathpls

using namespace mathpls;

constexpr int WIDTH = 80;
constexpr int HEIGHT = 23;

constexpr float R0 = 1, R1 = .5;
constexpr float D0 = 12, D1 = 72;

constexpr std::string_view tb = ".-:+=*#";

char img[HEIGHT][WIDTH+1]{};
float dep[HEIGHT][WIDTH]{};

mat4 vp;
vec3 cam{0, 0, -4};

const auto ld = normalize(vec3{5, 4, 2});
auto lum(const vec3& u) {
    auto N = normalize(u - vec3(vec2(u)).normalized()*R0);
    return tb[(int)round((tb.size()-1)*pow(dot(N, ld)*.5+.5, 3))];
}

void pt(const vec3& u) {
    auto q = vp * vec4(u, 1.f);
    q = q / q.w * .5f + .5f;
    int cy = round(q.y * (HEIGHT-1)), cx = round(q.x * (WIDTH-1));
    if (dep[cy][cx] < q.z) {
        dep[cy][cx] = q.z;
        img[cy][cx] = lum(u);
    }
}

void gen() {
    for (float h = 0; h <= .785f; h += .785f/D0) {
        const auto shr = sin(h)*R1, chr = cos(h)*R1;
        for (float t = 0; t < pi()*2; t += pi()/D1*2) {
            const auto ct = cos(t), st = sin(t);
            pt({ct*(R0-shr), st*(R0-shr), chr});
            pt({ct*(R0+shr), st*(R0+shr), chr});
            pt({ct*(R0-shr), st*(R0-shr),-chr});
            pt({ct*(R0+shr), st*(R0+shr),-chr});
            pt({ct*(R0-chr), st*(R0-chr), shr});
            pt({ct*(R0+chr), st*(R0+chr), shr});
            pt({ct*(R0-chr), st*(R0-chr),-shr});
            pt({ct*(R0+chr), st*(R0+chr),-shr});
        }
    }
}

int main() {
    for (float t=0;;t+=.006f) {
        vp = perspective(1.f, (float)WIDTH/HEIGHT/2, .1f,  10.f) *
            lookAt(cam, {}, {0,-1, 0});
        gen();

        std::cout << "\E[2J\E[H";
        for (auto& s : img)
            puts(s), std::memset(s, ' ',  WIDTH);
        std::memset(dep, 0, WIDTH*HEIGHT*sizeof(float));

        cam = quat({cos(t-.3),sin(t)*3.14, 0}, xyz) * vec3(2);
    }
}