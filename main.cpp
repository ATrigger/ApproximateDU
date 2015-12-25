#include <iostream>
#include <fstream>
#include <matplotpp/matplotpp.h>
#include <sstream>
#include <thread>

typedef std::vector<double> coords;
typedef std::pair<coords, std::pair<coords, coords>> plot3D; // x -> (y1,y2);

std::function<double(double, double, double)> f[2] =
        {[](double x, double y1, double y2) { return sin(x * y2); },
         [](double x, double y1, double y2) { return x * cos(x * y1); }};
plot3D generate3DPlotEuler() {
    double a = 0; //начало
    double b = 2; //конец
    double _y1 = 0; // y1(a)
    double _y2 = 0; // y2(a)
    double h = 0.1; // шаг
    plot3D plot;
    std::fstream out("outputEuler.txt", fstream::out);
    plot.first.push_back(a);
    plot.second.first.push_back(_y1);
    plot.second.second.push_back(_y2);
    for (double x = a + h; x <= b; x += h) {
        plot.first.push_back(x);
        double ny1 = plot.second.first.back() + h * f[0](x, plot.second.first.back(), plot.second.second.back()),
                ny2 = plot.second.second.back() + h * f[1](x, plot.second.first.back(), plot.second.second.back());
        out << "x = " << x << " : y1 = " << ny1 << ", y2 = " << ny2 << std::endl;
        plot.second.first.push_back(
                ny1
        );
        plot.second.second.push_back(
                ny2
        );
    }
    return plot;
};
std::function<double(double, double, double)> RGKf[2] =
        {[](double x, double y1, double y2) { return 2*sqrt(3*x*x + y1*y1 + y1); },
         [](double x, double y1, double y2) { return sqrt(x*x + y1*y1 + y2); }};
plot3D generate3DPlotRGK() {
    plot3D plot;
    double a = 0; //начало
    double b = 2; //конец
    double _y1 = 0.5; // y1(a)
    double _y2 = 1.2; // y2(a)
    double h = 0.1; // шаг
    std::fstream out("outputRGK.txt", fstream::out);
    plot.first.push_back(a);
    plot.second.first.push_back(_y1);
    plot.second.second.push_back(_y2);
    for (double x = a + h; x <= b; x += h) {
        plot.first.push_back(x);
        double oldy[2];
        double ny[2];
        oldy[0] = plot.second.first.back();
        oldy[1] = plot.second.second.back();
        double k[4][2];
        for (int i = 0; i < 2; i++) {
            k[0][i] = RGKf[i](x, oldy[0], oldy[1]);
            k[1][i] = RGKf[i](x + h / 2, oldy[0] + (h * k[0][i] / 2), oldy[1] + (h * k[0][i]) / 2);
            k[2][i] = RGKf[i](x + h / 2, oldy[0] + (h * k[1][i] / 2), oldy[1] + (h * k[1][i]) / 2);
            k[3][i] = RGKf[i](x + h, oldy[0] + (h * k[2][i]), oldy[1] + (h * k[2][i]));
            ny[i] = oldy[i] + (h / 6) * (k[0][i] + 2 * k[1][i] + 2 * k[2][i] + k[3][i]);
        }
        out << "x = " << x << " : y1 = " << ny[0] << ", y2 = " << ny[1] << std::endl;
        plot.second.first.push_back(
                ny[0]
        );
        plot.second.second.push_back(
                ny[1]
        );
    }
    return plot;
};

class MP : public MatPlot {
    plot3D target;
    plot3D target2;
public:
    void rebuild() {
        target = generate3DPlotEuler();
        target2 = generate3DPlotRGK();
    }

private:
    void DISPLAY() {
        std::string title_str = "dy1/dx = sin(x*y2),dy2/dx=x+cos(xy1,y2)";
        layer(std::string(std::string("EULER:")+title_str), 1);
        plot( target.second.first, target.second.second);
        set("Color","m");
        layer(std::string(std::string("RGK:")+title_str), 0);
        plot(target2.second.first, target2.second.second);
        set("Color","r");
    }
} mp;

void display() { mp.display(); }

void reshape(int w, int h) { mp.reshape(w, h); }

void idle(void) {
    glutPostRedisplay();
    usleep(10000);
}

void mouse(int button, int state, int x, int y) { mp.mouse(button, state, x, y); }

void motion(int x, int y) { mp.motion(x, y); }

void passive(int x, int y) { mp.passivemotion(x, y); }

void keyboard(unsigned char key, int x, int y) { mp.keyboard(key, x, y); }


int main(int argc, char *argv[]) {
    //mp.debug1();
    mp.rebuild();
    glutInit(&argc, argv);
    glutCreateWindow(100, 100, 640, 768);
    glutSetWindowTitle("Graph");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutMotionFunc(motion);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(passive);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
