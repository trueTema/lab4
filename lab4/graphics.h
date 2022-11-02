#include <glut.h>
#include <math.h>
#include <string>
#include <iostream>

const int X_COORD = 100;
const int Y_COORD = 100;
const float coeff = X_COORD / Y_COORD;
const float ITERATIONS = 1;

float x_off = 6 * coeff;
float y_off = 6;
double* arr_x = new double[20];
double* arr_y = new double[20];
double y_max = 0;

void drawgrid(float, float);
void drawfunc(float, float, float);
void display();
void init();

void draw(float x, float y, std::string str, bool isBig) {
	glRasterPos2d(x, y);
	glColor3f(0.0, 0.0, 0.0);
	for (int i = 0; i < str.size(); i++) {
		glutBitmapCharacter(isBig ? GLUT_BITMAP_HELVETICA_12 : GLUT_BITMAP_HELVETICA_10, str[i]);
	}
}

void drawgrid(float SERIF_OFFSET, float SERIF_DISTANCE) {
	glBegin(GL_LINES);
	glColor3f(0.0, 0.0, 0.0);

	glVertex2f(x_off / 3, y_off);
	glVertex2f(X_COORD, y_off);

	glVertex2f(x_off, Y_COORD);
	glVertex2f(x_off, y_off / 3);


	glEnd();
}

void drawfunc(float r, float g, float b) {
	glBegin(GL_LINES);
	double j = 0;
	glColor3f(r, g, b);
	glVertex2f(x_off, y_off);
	for (int i = 0; i < 20 && arr_x[i] != -1; i++) {
		double x = (arr_x[i] / 30000) * 100;
		//for (int j = 0; j < 10; j++) {
		//	if (arr_y[j] > y_max) {
		//		y_max = arr_y[j];
		//	}
		//}
		double y = (arr_y[i] / y_max) * 100;
		//cout << x << " " << y << endl;
		glVertex2f(x_off + x, y_off + y);
		glVertex2f(x_off + x, y_off + y);
	}
	glEnd();
}

void draw_pos_hor(float x, int num) {
	glBegin(GL_LINES);
	glColor3f(255, 0, 0);
	glVertex2f(x_off + x, y_off + 0.5);
	glVertex2f(x_off + x, y_off - 0.5);
	glEnd();
}

void draw_pos_ver(float y, int num) {
	glBegin(GL_LINES);
	glColor3f(255, 0, 0);
	glVertex2f(x_off - 0.5, y_off + y);
	glVertex2f(x_off + 0.5, y_off + y);
	glEnd();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	drawgrid(0.25, 40);
	for (int j = 0; (j < 20) && (arr_y[j] != -1); j++) {
		if (arr_y[j] > y_max) {
			y_max = arr_y[j];
		}
	}
	for (int i = 5000; i < 30000; i += 5000) {
		float x = x_off + ((float)(i) / float(30000)) * (100 - x_off);
		draw(x, y_off - 3, std::to_string(i), false);
	}
	draw(96, y_off - 3, "items", true);
	int pw10 = pow(10, int(log10(int(y_max/ 15))) - 1);
	for (int i = 0; i < y_max; i += (int(y_max / 15) / pw10) * pw10) {
		float y = y_off + ((float)(i) / float(y_max)) * (100 - y_off);
		draw(x_off - 5, y, std::to_string(i), false);
	}
	draw(x_off - 3, 97, "mcs", true);
	for (int i = 5000; i < 30000; i += 5000) {
		draw_pos_hor((float)(i) / float(30000) * (100 - x_off), i);
	}
	for (int i = 0; i < y_max; i += (int(y_max / 15) / pw10) * pw10) {
		draw_pos_ver((float)(i) / float(y_max) * (100 - y_off), i);
	}

	drawfunc(0, 255, 0);
	glutSwapBuffers();
	glFlush();
}

void init() {
	
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(500, 200);
	glutCreateWindow("Graphics");
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, X_COORD, 0.0, Y_COORD, -1.0, 1.0);
	glutDisplayFunc(display);
	glutMainLoop();
}
