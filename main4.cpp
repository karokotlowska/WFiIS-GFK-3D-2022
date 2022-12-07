
#include <stdlib.h>   
#include <glut.h>  
#include <stdio.h>    
#include <math.h>     
#include <string.h>
#include <windows.h>    
#include <io.h>
#include <cstdlib>
#include <iostream>

using namespace std;

//result = 1 -> player 1 wins
//result = -1 -> player 2 wins
//result = 2 -> draw

//player_turn = 1 -> player's one turn
//player_turn = 2 -> player's two turn

//
int player_turn, result, start_game;

void* font = GLUT_BITMAP_HELVETICA_18;

// light
GLfloat LightPosition[] = { 2.0f, 15.0f, 5.0f, 1.0f };
GLfloat mat_shininess[] = { 50.0 };
GLfloat mat_specular[] = { 1.0, 16.0, 1.0, 1.0 };
GLfloat LightAmbient[] = { 0.5f, 0.6f, 0.4f, 1.0f };
GLfloat LightDiffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };

int mouse_x, mouse_y, coordinates_clicked;

int w, h; //window

int rotation, elements_to_spin;

static int box[8][3] = { 
	{0, 1, 2}, 
	{3, 4, 5}, 
	{6, 7, 8}, 
	{0, 3, 6},		
	{1, 4, 7}, 
	{2, 5, 8}, 
	{0, 4, 8}, 
	{2, 4, 6} 
};

//1 if X
//-1 if O
//0 if empty
int boxes[9];

//centers of cells
int centers[9][2] = { 
	{-6,6},
	{0,6},
	{6,6},
	{-6,0},
	{0,0},
	{6,0},
	{-6,-6},
	{0,-6},
	{6,-6} 
};

void init_game(void) {
	int i;

	for (i = 0; i < 9; i++) {
		boxes[i] = 0;
	}

	result = 0;
	start_game = 1;
	player_turn = 1;
}

void menu(int id) {
	switch (id) {
	case 1:
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		init_game();
		break;
	case 2:
		exit(0);
	}
}
void keyboard(unsigned char k, int i, int j) {
	switch (k) {
	case 'r':
		init_game();
		break;
	case 'q':
		exit(0);
	default:
		break;
	}
}

int check_if_draw(void) {
	int i, count = 0;
	int empty = 0;

	for (i = 0; i < 9; i++) {
		if (boxes[i] != 0)
			count += boxes[i];
		 else {
			empty++;
		}
	}
//	std::cout << empty << endl;
//	std::cout << count << endl;

	if (count == -1 && empty == 0) return 2;

	return 0;
}

int check_move(void) {
	int i, count = 0;

	for (i = 0; i < 8; i++) {
		count = boxes[box[i][0]] + boxes[box[i][1]] + boxes[box[i][2]];
		if (abs(count) == 3) {
			elements_to_spin = i;
		//	std::cout << i <<"\n";
			return 1;
		}
	}
	
	return(check_if_draw());
}

void print_string(char* s, float x, float y) {
	unsigned int i;
	glRasterPos2f(x, y);
	for (i = 0; i < strlen(s); i++)
		glutBitmapCharacter(font, s[i]);
}


static void spin(int i) {
	rotation++; 
	if (rotation > 360) rotation = 0;
	glutPostRedisplay(); 
	glutTimerFunc(5, spin, 10);
}


void init() {
	glClearColor(1.0, 1.0, 1.0, 0.0);  
	glEnable(GL_DEPTH_TEST);   

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);

	glEnable(GL_LIGHTING); 
	glEnable(GL_LIGHT1);    
	glEnable(GL_NORMALIZE);

	start_game = 0;
	result = 0;
}


void draw_O(int x, int y, int a) {
	glColor3f(0.0, 0.0, 1.0);
	glPushMatrix();
	glTranslatef(x, y, -2);
	glRotatef(a, 1, 1, 1);
	glutSolidTorus(0.7, 2.0, 40, 46);
	glPopMatrix();
}


void draw_X(int x, int y, int a) {
	glColor3f(1.0, 1.0, 0.0);
	glPushMatrix();
	glTranslatef(x, y, -2);

	glPushMatrix();
	glRotatef(a, 1, 1, 1);
	glRotatef(90, 0, 1, 0);
	glRotatef(45, 1, 0, 0);
	glTranslatef(0, 0, -2.5);
	glutSolidCone(1, 2.5, 9, 8);
	glPopMatrix();

	glPushMatrix();
	glRotatef(a, 1, 1, 1);
	glRotatef(90, 0, 1, 0);
	glRotatef(315, 1, 0, 0);
	glTranslatef(0, 0, -2.5);
	glutSolidCone(1, 2.5, 9, 8);
	glPopMatrix();

	glPushMatrix();
	glRotatef(a, 1, 1, 1);
	glRotatef(90, 0, 1, 0);
	glRotatef(135, 1, 0, 0);
	glTranslatef(0, 0, -2.5);
	glutSolidCone(1, 2.5, 9, 8);
	glPopMatrix();

	glPushMatrix();
	glRotatef(a, 1, 1, 1);
	glRotatef(90, 0, 1, 0);
	glRotatef(225, 1, 0, 0);
	glTranslatef(0, 0, -2.5);
	glutSolidCone(1, 2.5, 9, 8);
	glPopMatrix();

	glPopMatrix();

}

void draw_grid() {
	glDisable(GL_LIGHTING);
	int x, y;

	for (x = 0; x <= 3; x++) {
		glPushMatrix();
		glLineWidth(14.0);
		glColor3f(0.2, 0, 0.6);
		glBegin(GL_LINES);
		glVertex2i(-9, -9 + x * 6);
		glVertex2i(9, -9 + x * 6);
		glEnd();
		glPopMatrix();
	}
	for (y = 0; y <= 3; y++) {
		glPushMatrix();
		glLineWidth(14.0);
		glColor3f(0.2, 0, 0.6);
		glBegin(GL_LINES);
		glVertex2i(-9 + y * 6, 9);
		glVertex2i(-9 + y * 6, -9);
		glEnd();
		glPopMatrix();
	}
	glEnable(GL_LIGHTING);
}

void draw_elements() {
	int i, rotate;
	for (i = 0; i < 9; i++) {
		rotate = 0;
		if (abs(result) == 1) {
			if ((i == box[elements_to_spin][0]) || (i == box[elements_to_spin][1]) || (i == box[elements_to_spin][2])) {
				rotate = rotation;
			}
			else rotate = 0;
		}
		if (result == 2) rotate = rotation;
		if (boxes[i] == 1) draw_X(centers[i][0], centers[i][1], rotate);
		if (boxes[i] == -1) draw_O(centers[i][0], centers[i][1], rotate);
	}
}

void printResults() {
	if (result == 1) print_string((char*)"player 1 wins", -1, 0);
	if (result == -1) print_string((char*)"player 2 wins", -1, 0);
	if (result == 2) print_string((char*)"It's a draw!", -1, 0);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();  
	glOrtho(-9.0, 9.0, -9.0, 9.0, 0.0, 130.0);  
	glMatrixMode(GL_MODELVIEW);  
	glLoadIdentity(); 

	glColor3f(0.0, 1.0, 1.0);

	printResults();

	gluLookAt(0, 0, 25, 0, 0, 0, 0, 1, 0);

	glColorMaterial(GL_FRONT, GL_AMBIENT);
	glColor4f(0.1, 0.4, 0.6, 1.0);
	glColorMaterial(GL_FRONT, GL_EMISSION);
	glColor4f(0.0, 0.0, 0.2, 1.0);
	glColorMaterial(GL_FRONT, GL_SPECULAR);
	glColor4f(0.4, 0.4, 0.5, 1.0);
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glColor4f(0.6, 0.8, 0.2, 1.0);
	glColor3f(1.0, 1.0, 1.0);

	draw_grid();

	draw_elements();
	
	glutSwapBuffers();
}

void windowResize(int x, int y) {
	w = x;
	h = y;
	glViewport(0, 0, (GLsizei)x, (GLsizei)y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

void click(int button, int state, int x, int y) {
	mouse_x = (18*(float)((float)x/w))/6;
	mouse_y = (18*(float)((float)y/h))/6;

	coordinates_clicked = mouse_x + mouse_y * 3;
	cout << mouse_x<<endl;
	cout << mouse_y<<endl<<endl<<endl;

	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {
		if (start_game == 0) {
			init_game();
			return;
		}
		if (player_turn == 1) {
			player_turn = -1;
		}
		else {
			player_turn = 1;
		}
		if (result == 0) {
			if (boxes[coordinates_clicked] == 0) {
				boxes[coordinates_clicked] = player_turn;
				result = check_move();
				if (result == 1 && player_turn == 1) {
					result = -1;
					start_game = 0;
					return;
				}
				if (result == 1 && player_turn == -1) {
					result = 1;
					start_game = 0;
					return;
				}
			}
		}

	}

	if (result == 2) {
		start_game = 0;
	}
}

void main() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(768, 768);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("Tic Tac Toe");
	glutSetWindowTitle("Tic Tac Toe 3D");
	init();

	glutCreateMenu(menu);
	glutAddMenuEntry("Start again", 1);
	glutAddMenuEntry("Exit", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutDisplayFunc(display);
	glutReshapeFunc(windowResize);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(click);
	glutTimerFunc(10, spin, 1);
	glutMainLoop();
}
