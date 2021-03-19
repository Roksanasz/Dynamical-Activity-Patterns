// g++ main.cpp -lGL -lglut -lGLU
#include <cstdio>
#include <string>
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <GL/glut.h>
#include "dumpgl2.h"


using namespace std;
const int L = 100;
const double pgc = 0.40;
const double pg2 = 0.8;
int tab[L][L];
int tab_new[L][L];
int licznikklatek = 0;

int init(void) {

    for (int j = 0; j < L; j++) {
        for (int i = 0; i < L; i++) {

            //int r = (int(floor((rand() / float(RAND_MAX + 1.0)) * 3)) - 1);
            /*float a = rand() / float(RAND_MAX);
            int r=0;
            if(a<0.02) r = 1;
            else if(a<0.1) r=-1;
           else r=0;

             */
           int r = 0;


            tab[i][j] = r;
            if (r != -1 && r != 0 && r != 1) { cout << r << endl; }
            // cout << "r: " << r << endl;

        }
    }

   for(int k=0; k<15; k++)
   {
        int x0 = 3+(L-6)*(rand()/float(RAND_MAX));
        int y0 = 3+(L-6)*(rand()/float(RAND_MAX));
        for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            tab[x0+i][y0+j] = 1;
   }
}


float WPRIM(float d) {                      //funkcja meksykañska
    float CE = 0.4, CI = 0.1, dE = 14.0, dI = 42.0;
    if (d == 0)
        return 0;

    float wynik = CE * exp(-(pow(d, 2.0) / dE)) - CI * exp(-(pow(d, 2.0) / dI));
    return wynik;
}

float dystans(int i1, int j1, int i2, int j2) {

    float d = sqrt(pow(i1 - i2, 2) + pow(j1 - j2, 2));
    return d;
}


void Wdo(int i, int j, float *wdoe, float *wdoi) {
    float d0 = 5.4;
    float dm = 15;
    float dx = 14;
    *wdoe = 0;
    *wdoi = 0;
    for (int jp = j - dx; jp <= j + dx; jp++) {
        for (int ip = i - dx; ip <= i + dx; ip++) {

            float d = dystans(i, j, ip, jp);


            if ((d <= dm) && (d >= d0) && (d != 0)) {
                *wdoi = *wdoi + WPRIM(d);
            } else if ((d < d0) && (d != 0)) {
                *wdoe = *wdoe + WPRIM(d);
            }
        }
    }


}

float funkcjaW(int i, int j, int ip, int jp) {

    // cout << wynik <<endl;
    float d0 = 5.4;
    float dm = 15;
    float WE = 68;
    float WI = -300;
    // float WE = 68;
    //float WI = -50;
    float dx = 14;
    float wynikFunkcjiW;


    float d = dystans(i, j, ip, jp);


    float Wp = WPRIM(d);
    float wdoe = 0;
    float wdoi = 0;


    if (d >= dm) {
        wynikFunkcjiW = 0;
    } else {
        Wdo(i, j, &wdoe, &wdoi);

        if ((d < d0)) {

            wynikFunkcjiW = (WE * Wp) / wdoe;
        } else if ((dm > d) && (d >= d0)) {
            wynikFunkcjiW = (WI * Wp) / wdoi;
        }
    }
    return wynikFunkcjiW;

}

float Heavy(float x) {
    int H;
    if (x < 0) { H = 0; }
    else { H = 1; }
    return H;
}


float I(int i, int j) {
    float I = 0;
    float dx = 14;
    for (int jp = j - dx; jp <= j + dx; jp++) {
        for (int ip = i - dx; ip <= i + dx; ip++) {

            float d = dystans(i, j, ip, jp);

            int ipp, jpp;

            if (ip < 0) { ipp = L + ip; }
            else if (ip >= L) { ipp = ip - L; }
            else { ipp = ip; }

            if (jp < 0) { jpp = L + jp; }
            else if (jp >= L) { jpp = jp - L; }
            else { jpp = jp; }

            int x = tab[ipp][jpp] - 1;

            if (d != 0)
                I = I + funkcjaW(i, j, ip, jp) * Heavy(x);
        }
    }
    return I;
}


int changestate(void) {


    for (int j = 0; j < L; j++) {
        cout << "a," << j << endl;

        for (int i = 0; i < L; i++) {

            if (tab[i][j] == 0) {

                int uth = 2;
                float x2 = (I(i, j) - uth);            // warunek I - dla stanu spoczynku = 0
             //   cout << I(i, j) << " " << x2 << endl;
                tab_new[i][j] = Heavy(x2);
            } else if (tab[i][j] == 1) {
                tab_new[i][j] = -1; //warunek II
            } else if (tab[i][j] == -1) { //warunek III - dla stanu refrakcji = -1
                double pg = ((rand() / float(RAND_MAX)));
                if (pg <= pgc) {
                    tab_new[i][j] = 0;
                } else {
                    tab_new[i][j] = -1;
                }
            }

        }
    }
    for (int j = 0; j < L; j++) {
        for (int i = 0; i < L; i++) {
            tab[i][j] = tab_new[i][j];
        }
    }
}


void changeSize(int w, int h) {
    float ratio = 1.0 * w / h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluOrtho2D(-1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}

void renderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glColor3f(1, 1, 1);
    /*glPointSize(5.0);
    glBegin(GL_POINTS);
        glVertex3f(px_cpu, py_cpu,0);
    glEnd();*/

    string workbuf;
    //sprintf(workbuf, "frame%04d.ppm");
    workbuf = "klatka" + to_string(licznikklatek) + ".ppm";
    ofstream plik(workbuf, ios::out);
    //cout << n/float(NX*NY) << " " << M/float(n) << endl;

    if (plik.good() == true) {
        //     std::cout << "Uzyskano dostep do pliku!" << std::endl;
        plik << "P3" << endl << L << " " << L << endl << "255" << endl;
        //tu operacje na pliku
    } //else std::cout << "Dostep do pliku zostal zabroniony!" << std::endl;

    for (int j = 0; j < L; j++)
        for (int i = 0; i < L; i++) {
            float x = 2.0 * (i / float(L)) - 1.0;        // mapowanie
            float y = 2.0 * (j / float(L)) - 1.0;        // mapowanie
            float delta = 2.0 / L;

            float r = 0, g = 0, b = 0;
            if (tab[i][j] == 0) { r = 0, g = 255, b = 0; }   //zielony
            if (tab[i][j] == 1) { r = 255, g = 0, b = 0; }   //czerwony
            if (tab[i][j] == -1) { r = 0; g = 0, b = 0; } //czarny

            plik << r << " " << g << " " << b << endl;


            glColor3f(r, g, b);

            glBegin(GL_QUADS);
            glVertex2f(x, y);
            glVertex2f(x + delta, y);
            glVertex2f(x + delta, y + delta);
            glVertex2f(x, y + delta);
            glEnd();
        }
    plik.close();
    licznikklatek++;
    glutSwapBuffers();
}

void idleFunction(void) {
    changestate();

    sg_glDumpWindow(L, L);
    glutPostRedisplay();
}

void timer(int v) {

    //px_cpu += 0.01;
    //if(px_cpu > 1) px_cpu=-1;
    glutTimerFunc(10, timer, 0);
}

void key(unsigned char key, int x, int y) {
    if (key == ' ') {
        changestate();
        sg_glDumpWindow(L, L);

    }
    if (key == 27)
        exit(0);
}

int main(int argc, char **argv) {

    //srand(1000);
    srand(time(NULL));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(320, 320);
    glutCreateWindow("GLUT, przyklad");


    init();


    // register callbacks
    glutDisplayFunc(renderScene);
    glutKeyboardFunc(key);
    glutReshapeFunc(changeSize);
    glutIdleFunc(idleFunction);
    glutTimerFunc(120, timer, 0);
    // enter GLUT event processing cycle
    glutMainLoop();
}
