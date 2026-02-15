#include <iostream>
#include <cstring>

#include "mathpls.h" // github.com/Ninter6/mathpls

using namespace mathpls;

constexpr int WIDTH = 80, HEIGHT = 23;
constexpr float R0 = 1, R1 = .5, D0 = 72, D1 = 72;

constexpr std::string_view tb = ".-:+=*#";

const mat4 p = perspective(1.f, (float)WIDTH/HEIGHT/2, .1f,  10.f);
const vec3 ld = normalize(vec3{5, 4, 2});

char img[HEIGHT][WIDTH+1]{};
float dep[HEIGHT][WIDTH]{};

int main() {
    for (float t=0;;t+=.005f) {
        vec3 cam = quat({cos(t-.3),sin(t)*3.14, 0}, xyz) * vec3(2);
        auto vp = p * lookAt(cam, {}, {0,-1, 0});

        for (float i = 0; i < pi()*2; i += pi()/D0*2)
            for (float j = 0; j < pi()*2; j += pi()/D1*2) {
                vec3 u = {cos(j)*(R0+sin(i)*R1), sin(j)*(R0+sin(i)*R1), cos(i)*R1};
                auto q = vp * vec4(u, 1.f);
                q = q / q.w * .5f + .5f;
                int cy = round(q.y * (HEIGHT-1)), cx = round(q.x * (WIDTH-1));
                if (dep[cy][cx] < q.z) {
                    dep[cy][cx] = q.z;
                    auto N = normalize(u - vec3(vec2(u)).normalized()*R0);
                    img[cy][cx] = tb[(int)round((tb.size()-1)*pow(dot(N, ld)*.5+.5, 3))];
                }
            }

        std::cout << "\E[2J\E[H";
        for (auto& s : img)
            puts(s), std::memset(s, ' ',  WIDTH);
        std::memset(dep, 0, WIDTH*HEIGHT*sizeof(float));
    }
}