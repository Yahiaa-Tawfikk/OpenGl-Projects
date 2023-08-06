#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <glut.h>

int WIDTH = 1280;
int HEIGHT = 720;
int moveZ = 0;
int moveX = 0;
int moveY = 0;

int BallZ;

GLuint tex;
char title[] = "3D Model Loader Sample";

// 3D Projection Options
GLdouble fovy = 45.0;
GLdouble aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT;
GLdouble zNear = 0.1;
GLdouble zFar = 100;

class Vector
{
public:
	GLdouble x, y, z;
	Vector() {}
	Vector(GLdouble _x, GLdouble _y, GLdouble _z) : x(_x), y(_y), z(_z) {}
	//================================================================================================//
	// Operator Overloading; In C++ you can override the behavior of operators for you class objects. //
	// Here we are overloading the += operator to add a given value to all vector coordinates.        //
	//================================================================================================//
	void operator +=(float value)
	{
		x += value;
		y += value;
		z += value;
	}
};

Vector Eye(20, 5, 20);
Vector At(0, 0, 0);
Vector Up(0, 1, 0);

int cameraZoom = 0;

// Model Variables
Model_3DS model_house;
Model_3DS model_tree;

// Textures
GLTexture tex_ground;

//=======================================================================
// Lighting Configuration Function
//=======================================================================
void InitLightSource()
{
	// Enable Lighting for this OpenGL Program
	glEnable(GL_LIGHTING);

	// Enable Light Source number 0
	// OpengL has 8 light sources
	glEnable(GL_LIGHT0);

	// Define Light source 0 ambient light
	GLfloat ambient[] = { 0.1f, 0.1f, 0.1, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	// Define Light source 0 diffuse light
	GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	// Define Light source 0 Specular light
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	// Finally, define light source 0 position in World Space
	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}


//Moving Function

void Mymove(int key, int x, int z) {
	switch (key) {
	case GLUT_KEY_UP:
		if (moveZ < 20) {
			moveZ += 1;
		}
		if (moveX + 4 == 7 && moveZ == 0) {
			BallZ = 20;
		}
	
		break;
	case GLUT_KEY_DOWN:
		if (moveZ > -20) {
			moveZ -= 1;
		}
		break;
	case GLUT_KEY_RIGHT:
		if (4 + moveX > -15) {
			moveX += 1;
		}
		break;
	case GLUT_KEY_LEFT:
		if (4 + moveX < 20) {
			moveX -= 1;
		}
		break;


	}
	glutPostRedisplay();


}

//=======================================================================
// Material Configuration Function
//======================================================================
void InitMaterial()
{
	// Enable Material Tracking
	glEnable(GL_COLOR_MATERIAL);

	// Sich will be assigneet Material Properties whd by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Set Material's Specular Color
	// Will be applied to all objects
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	// Set Material's Shine value (0->128)
	GLfloat shininess[] = { 96.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

//=======================================================================
// OpengGL Configuration Function
//=======================================================================
void myInit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(fovy, aspectRatio, zNear, zFar);
	//*******************************************************************************************//
	// fovy:			Angle between the bottom and top of the projectors, in degrees.			 //
	// aspectRatio:		Ratio of width to height of the clipping plane.							 //
	// zNear and zFar:	Specify the front and back clipping planes distances from camera.		 //
	//*******************************************************************************************//

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
	//*******************************************************************************************//
	// EYE (ex, ey, ez): defines the location of the camera.									 //
	// AT (ax, ay, az):	 denotes the direction where the camera is aiming at.					 //
	// UP (ux, uy, uz):  denotes the upward orientation of the camera.							 //
	//*******************************************************************************************//

	InitLightSource();

	InitMaterial();

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);
}

//=======================================================================
// Render Ground Function
//=======================================================================
void RenderGround()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_ground.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-20, 0, -20);
	glTexCoord2f(5, 0);
	glVertex3f(20, 0, -20);
	glTexCoord2f(5, 5);
	glVertex3f(20, 0, 20);
	glTexCoord2f(0, 5);
	glVertex3f(-20, 0, 20);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}

//=======================================================================
// Display Function
//=======================================================================
void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	GLfloat lightIntensity[] = { 0.7, 0.7, 0.7, 1.0f };
	GLfloat lightPosition[] = { 0.0f, 100.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);

	

	
	// Draw Ground

	RenderGround();

	//sky box
	glPushMatrix();
	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	glTranslated(50, 0, 0);
	glRotated(90, 1, 0, 1);
	glBindTexture(GL_TEXTURE_2D, tex);
	gluQuadricTexture(qobj, true);
	gluQuadricNormals(qobj, GL_SMOOTH);
	gluSphere(qobj, 100, 100, 100);
	gluDeleteQuadric(qobj);
	glPopMatrix();

	// Draw Ball
	glPushMatrix();
	glTranslatef(7, 1, 0+BallZ);
	glutSolidSphere(1, 25, 25);
	//glColor3f(0.0f, 1.0f, 0.6f);
	glPopMatrix();

	//Draw Player:
	//Draw Head
	glPushMatrix();
	glTranslatef(4+moveX, 7, 0+moveZ);
	glutSolidSphere(1, 25, 25);
	glPopMatrix();
	//Draw Body
	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslatef(4+moveX, 4.2, 0+moveZ);
	glScalef(1, 2, 1);
	glutSolidCube(2);
	glPopMatrix();
	//Draw Arm
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(5.25+moveX, 4.25, 0+moveZ);
	glRotatef(-30, 1, 1, 0);
	glScalef(0.75, 3, 1);
	glutSolidCube(1);
	glPopMatrix();
	//Draw Arm
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(2.75+moveX, 4.2, 0+moveZ);
	glRotatef(30, 1, 1, 0);
	glScalef(0.75, 3, 1);
	glutSolidCube(1);
	glPopMatrix();
	//Draw Leg
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(3.4+moveX, 1.65, 0+moveZ);
	glScalef(0.75, 2.5, 0.75);
	glutSolidCube(1);
	glPopMatrix();
	//Draw Leg
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(4.75+moveX, 1.65, 0+moveZ);
	glScalef(0.75, 2.5, 0.75);
	glutSolidCube(1);
	glPopMatrix();
	//Draw Post
	glPushMatrix();
	glColor3f(1,1,1);
	glTranslatef(10, 5, 20);
	glScalef(0.5, 10, 0.5);
	glutSolidCube(1);
	glPopMatrix();
	//Draw Post
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(-10, 5, 20);
	glScalef(0.5, 10, 0.5);
	glutSolidCube(1);
	glPopMatrix();
	//Draw Post
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(0, 10, 20);
	glScalef(20, 0.5, 0.5);
	glutSolidCube(1);
	glPopMatrix();
	//Draw Trophy
	glPushMatrix();
	glColor3f(0.9f,1,0.1f);
	glTranslatef(22, 5, 0);
	glRotatef(90, 1, 0, 0);
	glutSolidCone(1, 3.5, 25, 25);
	glPopMatrix();
	glPushMatrix();
	glColor3f(0.9f, 1, 0.1f);
	glTranslatef(22, 5.5, 0);
	glutSolidSphere(1, 25, 25);
	glPopMatrix();
	glPushMatrix();
	glColor3f(0, 1, 0);
	glTranslatef(22, 1.5, 0);
	glScalef(1, 0.05, 1);
	glutSolidSphere(1, 25, 25);
	glPopMatrix();
	glPushMatrix();
	glColor3f(0.9f, 1, 0.1f);
	glTranslatef(22, 1.2, 0);
	glScalef(1, 0.05, 1);
	glutSolidSphere(0.5, 25, 25);
	glPopMatrix();
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(22, 0.5, 0);
	glutSolidCube(1);
	glPopMatrix();

	// Draw Flags
	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(0, 5, -20);
	glScaled(0.25, 12, 0.5);
	glutSolidCube(1);
	glPopMatrix();
	glBegin(GL_QUADS);
	glColor3f(1, 0, 0);
	glVertex3f(0, 11, -20);
	glVertex3f(4, 11, -20);
	glVertex3f(4, 9, -20);
	glVertex3f(0, 9, -20);
	glColor3f(1, 1, 1);
	glVertex3f(0, 9, -20);
	glVertex3f(4, 9, -20);
	glVertex3f(4, 7, -20);
	glVertex3f(0, 7, -20);
	glEnd();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(5, 5, -20);
	glScaled(0.25, 12, 0.5);
	glutSolidCube(1);
	glPopMatrix();
	glBegin(GL_QUADS);
	glColor3f(0, 0, 1);
	glVertex3f(5, 11, -20);
	glVertex3f(9, 11, -20);
	glVertex3f(9, 9.67, -20);
	glVertex3f(5, 9.67, -20);
	glColor3f(1, 1, 1);
	glVertex3f(5, 9.67, -20);
	glVertex3f(9, 9.67, -20);
	glVertex3f(9, 8.33, -20);
	glVertex3f(5, 8.33, -20);
	glColor3f(1, 0, 0);
	glVertex3f(5, 8.33, -20);
	glVertex3f(9, 8.33, -20);
	glVertex3f(9, 7.01, -20);
	glVertex3f(5, 7.01, -20);
	glEnd();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(10, 5, -20);
	glScaled(0.25, 12, 0.5);
	glutSolidCube(1);
	glPopMatrix();
	glBegin(GL_QUADS);
	glColor3f(0, 0, 0);
	glVertex3f(10, 11, -20);
	glVertex3f(14, 11, -20);
	glVertex3f(14, 9.67, -20);
	glVertex3f(10, 9.67, -20);
	glColor3f(1, 0, 0);
	glVertex3f(10, 9.67, -20);
	glVertex3f(14, 9.67, -20);
	glVertex3f(14, 8.33, -20);
	glVertex3f(10, 8.33, -20);
	glColor3f(0.9f, 1, 0.1f);
	glVertex3f(10, 8.33, -20);
	glVertex3f(14, 8.33, -20);
	glVertex3f(14, 7.01, -20);
	glVertex3f(10, 7.01, -20);
	glEnd();

	// Draw Bench
	glPushMatrix();
	glColor3f(0.5f, 0.2f, 0);
	glTranslatef(-16, 1.25, -6);
	glScalef(1, 1, 5);
	glutSolidCube(2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.5f, 0.2f, 0);
	glTranslatef(-18, 2, -6);
	glScalef(1, 2, 5);
	glutSolidCube(2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.5f, 0.2f, 0);
	glTranslatef(-16, 1.25, 8);
	glScalef(1, 1, 5);
	glutSolidCube(2);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.5f, 0.2f, 0);
	glTranslatef(-18, 2, 8);
	glScalef(1, 2, 5);
	glutSolidCube(2);
	glPopMatrix();




	glutSwapBuffers();
}

//=======================================================================
// Keyboard Function
//=======================================================================
void myKeyboard(unsigned char button, int x, int y)
{
	switch (button)
	{
	case 'w':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 'r':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

//=======================================================================
// Motion Function
//=======================================================================
void myMotion(int x, int y)
{
	y = HEIGHT - y;

	if (cameraZoom - y > 0)
	{
		Eye.x += 0.1;
		Eye.z += -0.1;
	}
	else
	{
		Eye.x += -0.1;
		Eye.z += 0.1;
	}

	cameraZoom = y;

	glLoadIdentity();	//Clear Model_View Matrix

	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);	//Setup Camera with modified paramters

	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glutPostRedisplay();	//Re-draw scene 
}

//=======================================================================
// Mouse Function
//=======================================================================
void myMouse(int button, int state, int x, int y)
{
	y = HEIGHT - y;

	if (state == GLUT_DOWN)
	{
		cameraZoom = y;
	}
}

//=======================================================================
// Reshape Function
//=======================================================================
void myReshape(int w, int h)
{
	if (h == 0) {
		h = 1;
	}

	WIDTH = w;
	HEIGHT = h;

	// set the drawable region of the window
	glViewport(0, 0, w, h);

	// set up the projection matrix 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (GLdouble)WIDTH / (GLdouble)HEIGHT, zNear, zFar);

	// go back to modelview matrix so we can move the objects about
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
}

//=======================================================================
// Assets Loading Function
//=======================================================================
void LoadAssets()
{
	// Loading Model files
	model_house.Load("Models/house/house.3DS");
	model_tree.Load("Models/tree/Tree1.3ds");

	// Loading texture files
	tex_ground.Load("Textures/ground.bmp");
	loadBMP(&tex, "Textures/blu-sky-3.bmp", true);
}

//=======================================================================
// Main Function
//=======================================================================
void main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);

	glutInitWindowPosition(100, 150);

	


	glutCreateWindow(title);

	glutDisplayFunc(myDisplay);

	glutKeyboardFunc(myKeyboard);
	glutSpecialFunc(Mymove);
	glutMotionFunc(myMotion);

	glutMouseFunc(myMouse);

	glutReshapeFunc(myReshape);

	myInit();

	LoadAssets();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glutMainLoop();
}