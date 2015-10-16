#include <iostream>
#include <glut.h>
#include <sstream>
#include <string.h>
#include <stdio.h> 
#include <math.h>
using namespace std;


#define jumpVelocity 22
#define doubleJumpVelocity 40
#define exitX 0
#define exitY 350
#define exitW 20
#define exitH 120
#define sObjH 50
#define sObjW 20
#define sObjY 250
#define sObjX 200
#define fObjH 30
#define fObjW 20
#define fObjY 50
#define fObjX 350

void Display(void);
void gameLoop(int);
void Timer(void);
void Timer(int value);
void render(void);
void mouse(int,int,int,int);
void Switch_Key(int, int, int);
void doubleJump();
void displayText();
bool Level();


double leftBoundary= 0;
double rightBoundary= 700;
double upperBoundary = 200;
double lowerBoundary = 50;
double levelRightBoundary = 520;
bool gameOver=0;

class Hero
{
public:
	double positionX, positionY;     // Position of the character
	double velocityX, velocityY;     // Velocity of the character
	double acc;
	bool onGround;
	bool dJump;
	int playerT;
	bool level;

	Hero::Hero(){
		acc=0;
		positionX=50;
		positionY=50;
		velocityX=5;
		velocityY=0;
		onGround=true;
		dJump=false;
		playerT=0;
		level=false;

	}

	bool inRec(int x, int y,int w, int h, int rx, int ry, int rw, int rh){
		return (x+w >= rx && x < (rx+rw) && y+h >= ry && y<(ry+rh));
	}

	void StartJump()
	{
		if(onGround)
		{
			acc=-2;
			velocityY = jumpVelocity;
			update();
			dJump=true;
			onGround=false;
		}
	}

	void doubleJump()
	{
		acc=-2;
		velocityY =doubleJumpVelocity;
		update();
		onGround = false;
		dJump=false;
		Level();
	}

	void drawString(void * font,const char* s, float x, float y, float z)
	{
		unsigned int i;
		glRasterPos3f(x,y,z);

		for(i=0;i< strlen(s); i++)
			glutBitmapCharacter(font,s[i]);
	}


	void render()
	{
		glBegin(GL_QUADS);
		glVertex2f(positionX, positionY);
		glVertex2f(positionX,positionY+50);
		glVertex2f(positionX+50,positionY+50);
		glVertex2f(positionX+50,positionY);
		glEnd();
	}

	bool Level(){
		if(positionX < levelRightBoundary && positionY > 250 && positionY < 270 && (velocityX>0 || velocityX<0) )
		{
			acc=0;
			velocityY = 0;
			positionY = 265;
			onGround = 1;
			return 1;
		}else{
			if( positionX+50 > levelRightBoundary && positionY == 265 && velocityX>0){
				velocityY = -9;
				positionY+=velocityY+acc;
				update();
				onGround = 0;
			}
		}
	}

	void setOrthographicProjection() {
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, 700, 0,700);
		glScalef(1, -1, 1);
		glTranslatef(0, -700, 0);
		glMatrixMode(GL_MODELVIEW);
	}

	void resetPerspectiveProjection() {
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}

	void renderBitmapString(float x, float y, float z, void *font, char *string) {
		char *c;
		glRasterPos3f(x, y, z);
		for (c = string; *c != '\0'; c++) {
			glutBitmapCharacter(font, *c);
		}
	}

	char* toString(int i) {
		char buffer [33];
		sprintf(buffer,"%d",i);
		return buffer;
	}
	void update()
	{

		if(!gameOver){

			positionX += velocityX ;      // Apply horizontal velocity to X position

			if(positionY < 50){
				positionY=50;
				velocityY=0;
				acc=0;
				onGround=true;
			}
			else{
				if(velocityX>0 ||velocityX<0)
				{
					velocityY+=acc;		
					positionY+=velocityY;
				}

			}
			Level();

			//** Reverse the X movement and controlling the Y velocity
			if (velocityX>0) 
				positionX -= 0.1; 
			else 
				positionX += 0.1;

			if (positionX < leftBoundary) 
				velocityX*=-1; 
			else if (positionX+50 > rightBoundary) 
				velocityX*=-1;
		}
	}
};

Hero hero;

void render(int x, int y, int w, int h)
{
	glBegin(GL_QUADS);
	glVertex2f(x, y);
	glVertex2f(x,y+h);
	glVertex2f(x+w,y+h);
	glVertex2f(x+w,y);
	glEnd();
}


void main(int argc, char** argr)
{
	glutInit(&argc, argr);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("King of Thieves");
	glutDisplayFunc(Display);
	glClearColor(1, 1, 1, 0.0f);
	gluOrtho2D(0.0, 700.0, 0.0, 700.0);
	glutSpecialFunc(Switch_Key);
	Timer(0);
	glutMouseFunc(mouse);
	glutTimerFunc(100, gameLoop, 10);
	glutMainLoop();

}

void Display(void)
{

	glClear(GL_COLOR_BUFFER_BIT);

	//The object Drawing
	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslated(hero.positionX,hero.positionY,0);
	glPopMatrix();

	hero.render();
	hero.setOrthographicProjection();
	glPushMatrix();
	glLoadIdentity();
	displayText();
	glPopMatrix();
	hero.resetPerspectiveProjection();
	glutSwapBuffers();

	//The Exit Drawing
	glPushMatrix();
	glColor3f(1,69/255,0);
	render(exitX,exitY,exitW,exitH);
	glPopMatrix();

	//Draw Obsticle on level 1
	glPushMatrix();
	glColor3f(1,0,0);
	render(sObjX,sObjY,sObjW,sObjH);
	glPopMatrix();

	//Level Drawing
	glPushMatrix();
	glColor3f(0,0,0);
	render(0,250,520,20);
	glPopMatrix();

	//Draw Obsticle on base level
	glPushMatrix();
	glColor3f(1,0,0);
	render(fObjX,fObjY,fObjW,fObjH);
	glPopMatrix();

	//Base Drawing
	glPushMatrix();
	glColor3f(0,0,0);
	render(0,20,700,30);
	glPopMatrix();


	glFlush();
}

void gameLoop(int value)
{
	hero.update();
	Display();
	glutTimerFunc(30, gameLoop, 10);
}

void displayText(){
	hero.renderBitmapString(550, 100, 0, GLUT_BITMAP_HELVETICA_18, "Time :" );
	int number=hero.playerT;
	stringstream strs;
	strs << number;
	string temp_str = strs.str();
	char* char_type = (char*) temp_str.c_str();
	hero.renderBitmapString(610, 100, 0, GLUT_BITMAP_HELVETICA_18, char_type);


	if (hero.inRec(hero.positionX,hero.positionY,50,50,exitX,exitY,exitW,exitH)){
		hero.renderBitmapString(300, 100, 0, GLUT_BITMAP_HELVETICA_18, "Congratulations You Won");
		gameOver=true;
	}else{
		if (hero.inRec(hero.positionX,hero.positionY,50,50,fObjX,fObjY,fObjW,fObjH)){
			hero.renderBitmapString(300, 300, 0, GLUT_BITMAP_HELVETICA_18, "You Lost Try to avoid Obsticles to win");
			gameOver=true;
		}
		else{
			if (hero.inRec(hero.positionX,hero.positionY,50,50,sObjX,sObjY,sObjW,sObjH)){
				hero.renderBitmapString(300, 300, 0, GLUT_BITMAP_HELVETICA_18, "You Lost Try to avoid Obsticles to win");
				gameOver=true;
			}
		}
	}

}

void Switch_Key(int key, int x, int y)
{
	if(key==GLUT_KEY_UP && hero.dJump && hero.velocityX<0 && hero.positionX>rightBoundary-70)
		hero.doubleJump();

	if(key==GLUT_KEY_UP)
	{
		hero.StartJump();
	}
	glutPostRedisplay();
}

void Timer(int value){
	if(!gameOver){
		hero.playerT += 1;
		glutPostRedisplay();
		// 100 milliseconds
		glutTimerFunc(1000, Timer, 0);
	}
}

void mouse(int button, int state, int x, int y) 
{
	if(button==GLUT_LEFT_BUTTON && hero.dJump && hero.velocityX<0 && hero.positionX>rightBoundary-70)
		hero.doubleJump();

	if(button==GLUT_LEFT_BUTTON)
	{
		hero.StartJump();
	}
}
