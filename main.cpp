
#include <stdlib.h>   
#include <glut.h>  
#include <stdio.h>    
#include <math.h>     
#include <string.h>
#include <windows.h>    
#include <io.h>
#include <cstdlib>

using namespace std;

// lighting
GLfloat LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightPosition[] = { 5.0f, 25.0f, 5.0f, 1.0f };
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };

int mouse_x, mouse_y, Win_x, Win_y, coordinates_clicked;

int spin, elements_to_spin;

//result = 1 -> player 1 wins
//result = -1 -> player 2 wins
//result = 2 -> draw
int player_turn, result, start_game;

void* font = GLUT_BITMAP_HELVETICA_18;

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

// 1 = x, -1 = O, 0 = nothing
int boxes[9];

//centers
int object_map[9][2] = { {-6,6},{0,6},{6,6},{-6,0},{0,0},{6,0},{-6,-6},{0,-6},{6,-6} };

void init_game(void)
{
	int i;

	// clear matrix
	for (i = 0; i < 9; i++) {
		boxes[i] = 0;
	}

	result = 0;
	start_game = 1;
	player_turn = 1;
}

int check_if_draw(void) {
	int i, t = 0;

	for (i = 0; i < 8; i++) {
		t = t + abs(boxes[box[i][0]]) + abs(boxes[box[i][1]]) + abs(boxes[box[i][2]]);
	}

	if (t == 24) return(2);

	return 0;
}

int check_move(void) {
	int i, t = 0;

	for (i = 0; i < 8; i++) {
		t = boxes[box[i][0]] + boxes[box[i][1]] + boxes[box[i][2]];
		if ((t == 3) || (t == -3)) {
			elements_to_spin = i;
			return(1);
		}
	}
	
	return(check_if_draw());
}

void printString(char* s, float x, float y) {
	unsigned int i;
	glRasterPos2f(x, y);
	for (i = 0; i < strlen(s); i++)
		glutBitmapCharacter(font, s[i]);
}


// This creates the spinning of the cube.
static void TimeEvent(int te) {

	spin++;  // increase cube rotation by 1
	if (spin > 360) spin = 0; // if over 360 degress, start back at zero.
	glutPostRedisplay();  // Update screen with new rotation data
	glutTimerFunc(10, TimeEvent, 1);  // Reset our timmer.
}


void init() {
	glClearColor(1.0, 1.0, 1.0, 0.0);  
	glEnable(GL_DEPTH_TEST);   

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);

	glEnable(GL_LIGHTING); 
	glEnable(GL_LIGHT1);    

	start_game = 0;
	result = 0;
}


void Draw_O(int x, int y, int z, int a)
{
	glColor3f(0.0, 0.0, 1.0);
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(a, 1, 1, 1);
	glutSolidTorus(0.7, 2.0, 8, 16);
	glPopMatrix();
}


void Draw_X(int x, int y, int z, int a)
{
	glColor3f(1.0, 1.0, 0.0);
	glPushMatrix();
	glTranslatef(x, y, z);

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
	int ix, iy;

	for (ix = 0; ix <= 3; ix++) {
		glPushMatrix();
		glLineWidth(14.0);
		glColor3f(0, 0, 1);
		glBegin(GL_LINES);
		glVertex2i(-9, -9 + ix * 6);
		glVertex2i(9, -9 + ix * 6);
		glEnd();
		glPopMatrix();
	}
	for (iy = 0; iy <= 3; iy++) {
		glPushMatrix();
		glLineWidth(14.0);
		glColor3f(0, 0, 1);
		glBegin(GL_LINES);
		glVertex2i(-9 + iy * 6, 9);
		glVertex2i(-9 + iy * 6, -9);
		glEnd();
		glPopMatrix();
	}
}

void display() {
	int i, j;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //Clear the screen

	glMatrixMode(GL_PROJECTION);  // Tell opengl that we are doing project matrix work
	glLoadIdentity();  // Clear the matrix
	glOrtho(-9.0, 9.0, -9.0, 9.0, 0.0, 130.0);  // Setup an Ortho view
	glMatrixMode(GL_MODELVIEW);  // Tell opengl that we are doing model matrix work. (drawing)
	glLoadIdentity(); // Clear the model matrix

	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHTING);
	glColor3f(0.0, 1.0, 1.0);
	if (result == 1) printString((char*)"player 1 wins", -1, 0);
	if (result == -1) printString( (char*)"player 2 wins", -1, 0);
	if (result == 2) printString( (char*)"It's a draw!", -1, 0);


	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);

	gluLookAt(0, 0, 25, 0, 0, 0, 0, 1, 0);

	draw_grid();

	glColorMaterial(GL_FRONT, GL_AMBIENT);
	glColor4f(0.5, 0.5, 0.5, 1.0);
	glColorMaterial(GL_FRONT, GL_EMISSION);
	glColor4f(0.0, 0.0, 0.0, 1.0);
	glColorMaterial(GL_FRONT, GL_SPECULAR);
	glColor4f(0.0, 0.35, 0.0, 1.0);
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glColor4f(0.8, 0.8, 0.0, 1.0);

	for (i = 0; i < 9; i++) {
		j = 0;
		if (abs(result) == 1) {
			if ((i == box[elements_to_spin][0]) || (i == box[elements_to_spin][1]) || (i == box[elements_to_spin][2])) {
				j = spin;
			}
			else j = 0;
		}
		if (result == 2) j = spin;
		if (boxes[i] == 1) Draw_X(object_map[i][0], object_map[i][1], -1, j);
		if (boxes[i] == -1) Draw_O(object_map[i][0], object_map[i][1], -1, j);
	}
	glutSwapBuffers();
}

void windowResize(int w, int h)
{
	Win_x = w;
	Win_y = h;
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

void click(int button, int state, int x, int y)
{
	// We convert windows mouse coords to out openGL coords
	mouse_x = (18 * (float)((float)x / (float)Win_x)) / 6;
	mouse_y = (18 * (float)((float)y / (float)Win_y)) / 6;

	// What square have they clicked in?
	coordinates_clicked = mouse_x + mouse_y * 3;

	if (start_game == 0) {
		if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {
			if (player_turn == 1) {
				player_turn = -1;
				init_game();
				return;
			}
			else {
				player_turn = 1;
				init_game();
				return;
			}
			
		}
	}

	if (start_game == 1) {
		if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {
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
					if (result == 1) {
						start_game = 0;
						return;
					}
				}
			}
		}
	}

	if (result == 2) {
		start_game = 0;
	}

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
	glutTimerFunc(10, TimeEvent, 1);
	glutMainLoop();
}
