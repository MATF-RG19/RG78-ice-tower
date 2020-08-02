#define GL_GLEXT_PROTOTYPES /* potrebno za glGenerateMipmap(GL_TEXTURE_2D);*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include <string.h>
#include <stdbool.h>

#include "image.h"
#include "objekti.h"

#define maximumNumber 30

#define LOPTA "Teksture/lopta.bmp"

/*globalni brojaci*/
long int random = 0;
int currY = -5;
int currX = 0;
int stepY = 3; //razmak izmedju blokova po Y
int scaleX = 0;
double cameraY = 0.1;
bool start = false;
int Xblocks[maximumNumber];
int yBlocks[maximumNumber];
int lBlock[maximumNumber];
int score = 0; //broji rezultat
double scoreY = 0; // pozicija skora
int count = 0;
bool gameEnd = false;
bool startSad = false;

double yPerLower = -5;
double yPerUpper = 5;


bool fallingEffect = false;

/**/
void drawWall(void);/**/
void SetupLights(void);/*ne koristi se OBRISI*/
void generateRandomStep(int, int, int);/**/
void randomCases(void);/**/
void anim(void);/**/
void generateBlocks(void);/**/
static void on_keyboard(unsigned char, int, int);/**/
void print(int, int, char *); /**/
void keyboard(int, int, int);/**/
void checkCollision();/**/
void mouse(int, int, int, int);/**/
void on_display(void);
static void inicijalizacijaTekstura(void);



double ballX = 0;
double ballY = -3.5; //pocetna pozicija lopte
bool jumped = false;
double ballJump = 3; //visina skoka
bool gameOver = false;

/*static void drawCoordinates();*/
GLuint textures[1];


int main(int argc, char** argv) {
    random = rand() % 8 + 1;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    /*prozor*/
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    generateBlocks();
    glutCreateWindow("Ice Tower");
    glutFullScreen();


    glutDisplayFunc(on_display);
    glEnable(GL_DEPTH_TEST);
    glClearColor(1, 1, 1, 0);
    glutIdleFunc(anim);
	glutKeyboardFunc(on_keyboard);
	//glutSpecialFunc(keyboard);
	glutMouseFunc(mouse);

    glutMainLoop();
}


/*
static void drawCoordinates() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(2,2,-4,
                2,0,2,
                0.0,1.0,0.0);

    glBegin(GL_LINES);
        glColor3f(1,0,0);
        glVertex3f(0,0,0);
        glVertex3f(100,0,0);
        glVertex3f(0,0,0);
        glVertex3f(-100,0,0);

        glColor3f(0,1,0);
        glVertex3f(0,0,0);
        glVertex3f(0,100,0);
        glVertex3f(0,0,0);
        glVertex3f(0,-100,0);

        glColor3f(0,0,1);
        glVertex3f(0,0,0);
        glVertex3f(0,0,100);
        glVertex3f(0,0,0);
        glVertex3f(0,0,-100);
    glEnd();

}
*/

static void inicijalizacijaTekstura(void){
    /*funkcija sa sedmog casa vezbi koja koristi biblioteku image.c*/

    Image * image;
    glEnable(GL_DEPTH_TEST);

    /*ukljucivanje odredjenih flegova za lepljenje tekstura*/
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);

    /*inicijalizacija*/
    image = image_init(0, 0);
    glGenTextures(3, textures);

    /*Tekstura lopte*/
    image_read(image, LOPTA);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    /*Iskljucujemo aktivnu teksturu*/
    glBindTexture(GL_TEXTURE_2D, 0);
    image_done(image);
}

/*kontrole na karakterskoj tastaturi*/
static void on_keyboard(unsigned char key, int x, int y)
{
    /*ako ne bude radilo mora preko if-a*/
    switch (key) {
    case 27:
        exit(0);
        break;
    case 'r':
        ballX = 0;
        ballY = -3.5;
        cameraY = 0.1;
        start = false;
        random = 0;
        score = 0; //broji rezultat
        scoreY = 0; // pozicija skora
        count = 0;
        gameEnd = false;
        startSad = false;
        ballX = 0;
        ballY = -3.5;
        gameOver = false;
        		  glutDisplayFunc(on_display);
        break; 
    case 'p': /*pocinje*/
        if(gameOver == false && gameEnd == false) {
            start = true;
            ballY += ballJump;
            jumped = true;
            count++;
        }
        break;
    case 'd': /*skace desno*/
        if(gameOver == false && gameEnd == false) {
            start = true;
            ballY += ballJump;
            ballX -= 2;
            jumped = true;
            count++;
        }
        break;
    case 's': /*skace levo*/
        if(gameOver == false && gameEnd == false) {
            start = true;
            ballY += ballJump;
            ballX += 2;
            jumped = true;
            count++;
        }
        break;
    case 'f': /*veliki skok desno*/
        if(gameOver == false && gameEnd == false) {
            start = true;
            ballY += ballJump;
            ballX -= 5; /*probaj drugi koef*/
            jumped = true;
            count++;
        }
        break;
    case 'a': /*veliki skok u levo*/
        if(gameOver == false && gameEnd == false) {
            start = true;
            ballY+= ballJump;
            ballX += 5;
            jumped = true;
            count++;
        }
        break;
    case 'w': /*skok gore*/
        if(gameOver == false && gameEnd == false) {
            start = true;
            ballY+= ballJump;
            ballX += 0;
            jumped = true;
            count++;
        }
        break;
    }
    glutPostRedisplay();
}

/*SKACE NA TRELICE, MANJI SKOKOVI*/
/*static void on_keyboard(unsigned char key, int x, int y) {
    if(key == GLUT_KEY_LEFT && ballX <= 4.5 && gameOver == false && gameEnd == false) {
        ballX += 0.2;
    }
    if(key == GLUT_KEY_RIGHT && ballX <= 4.5 && gameOver == false && gameEnd == false) {
        ballX -= 0.2;
    }
    glutPostRedisplay();
}    
*/
/*ispisuje rezultat*/
void print(int x, int y, char *string)
{
	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}
/*probaj druge koeficijente*/
void randomCases() {
    random = rand() % 8 + 1;
    switch(random) {
        case 1: currX = 5; currY += stepY; scaleX = 3; break;
        case 2: currX = 3; currY += stepY; scaleX = 2; break;
        case 3: currX = 0; currY += stepY; scaleX = 5; break;
        case 4: currX = -2; currY += stepY; scaleX = 2; break;
        case 5: currX = 1; currY += stepY; scaleX = 5; break;
        case 6: currX = -3; currY += stepY; scaleX = 2; break;
        case 7: currX = 1; currY += stepY; scaleX = 2; break;
        case 8: currX = 4; currY += stepY; scaleX = 2; break;
    }
}
/*
static void on_reshape(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float) width / height, 1, 1500);
}
*/
void generateBlocks() {
    for(int i = 0; i < maximumNumber; i++) {
        randomCases();
        Xblocks[i] = currX;
        yBlocks[i] = currY;
        lBlock[i] = scaleX;
    }
}

void draw(){
	for (int i = 0; i < maximumNumber; i++) {
		generateRandomStep(Xblocks[i], yBlocks[i], lBlock[i]);
	}
}

void checkCollision() {
    int i = 0;
    for (i = 0; i < maximumNumber - 1; i++) {
        if (ballX >= (Xblocks[i] - (lBlock[i] / 2) - 0.5) && (ballX <= (Xblocks[i] + (lBlock[i] / 2) + 0.5))
            && (ballY >= yBlocks[i] + 1) && (ballY < yBlocks[i+1])) {
                jumped = false;
                score += 10;
                ballY -= ballY - (yBlocks[i] + 1);
                break;
        }
        else {
            if ((ballX < (Xblocks[i] - (lBlock[i] / 2) - 0.5) || (ballX >(Xblocks[i] + (lBlock[i] / 2) + 0.5)))
				&& ((ballY >= yBlocks[i] + 1) && (ballY < yBlocks[i + 1]))) {
                fallingEffect = true;
                start = false;
                startSad = true;
                break;
            }
        } 
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON || button == GLUT_RIGHT_BUTTON) {
		start = true;
	}
	glutPostRedisplay();
}

void generateRandomStep(int x, int y, int s) {
    randomCases();
    glPushMatrix();
        glColor3f(0.9f, 0.9f, 0.9f);
        glTranslated(x, y, -1);
        glScaled(s, 1, 1);
    /////////////////////////////////////////////
        GLUquadricObj* esphereTwo = gluNewQuadric();
        gluQuadricTexture(esphereTwo, true);
        gluQuadricNormals(esphereTwo, GLU_SMOOTH);
        
        glEnable(GL_CULL_FACE);
        glutSolidCube(1);
        
    glPopMatrix();
}

void anim() {
	if (start == true && (fallingEffect == false)) {
		cameraY += 0.001;
		yPerLower += 0.02;
		yPerUpper += 0.02;
		scoreY += 0.02;

		if (jumped == true) {
			checkCollision();
		}

		if (jumped == false && ((ballX < (Xblocks[count-1] - (lBlock[count-1] / 2) - 0.5)) || (ballX >(Xblocks[count-1] + (lBlock[count-1] / 2) + 0.5)))) {
			fallingEffect = true;
			start = false;
			startSad = true;
			gameOver = true;
		}

		if (count == maximumNumber) {
			gameEnd = true;
			start = false;
			ballY = ballY - ballJump;
		}


		if (ballY < yPerLower) {
			fallingEffect = true;
			start = false;
			gameOver = true;
			startSad = true;
		}
	}

	if (fallingEffect == true && (start == false) && yPerLower > -5){

		cameraY -= 0.001;
		yPerLower -= 0.02;
		yPerUpper -= 0.02;
		scoreY -= 0.02;
		if (ballY > -3.5)
			ballY -= 0.04;

		if (startSad == true) {
			startSad = false;
			gameOver = true;
		}

		if (yPerLower <= -5) {
			fallingEffect = false;
			
			glPushMatrix();
			glColor3d(0, 0, 0);
			glTranslated(0, scoreY, -5);
			char * stC[100];
			sprintf((char *)stC, "Score: % i", score);
			print(4, 4, (char *)stC);
			glPopMatrix();
		}
	}

	for (int i = 0; i < 1000000; i++);

	glutPostRedisplay();
}
void on_display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
/*  TODO lose osvetljenje, promeniti*/
/* gluLookAt( -2.4, 0.8, 0,
               2, 0.28, 0,
               0, 1, 0);
               probaj ovu kameru 
*/
   

    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-5, 5, yPerLower, yPerUpper, -5, 5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, -5, 
              0, 0, 0, 
              0.0, cameraY, 0.0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);	

    
	//BackWall
	glPushMatrix();
        glColor3f(1.0f, 1.0f, 0.0f);
        glTranslated(0, yPerLower, 0);
        glScaled(1, 10, 0.1);

        
        GLUquadricObj* esphereTwo = gluNewQuadric();
        gluQuadricTexture(esphereTwo, true);
        gluQuadricNormals(esphereTwo, GLU_SMOOTH);
        
        glEnable(GL_CULL_FACE);
        glutSolidCube(10);
        
	glPopMatrix();

	//ground
	glPushMatrix();
	    glTranslated(0, -4.5, -1);
        glScaled(1, 0.1, 0.1);
        glColor3f(1, 0, 0);
        glutSolidCube(10);
	glPopMatrix();


	//sphere
	glPushMatrix();
        glColor3f(0.0f, 1.0f, 1.0f);
        glTranslated(ballX, ballY, -1);
        
        GLUquadricObj* esphere = gluNewQuadric();
        glEnable(GL_TEXTURE_2D);
        gluQuadricDrawStyle(esphere, GLU_FILL);
        glBindTexture(GL_TEXTURE_2D, textures[1]);
        gluQuadricTexture(esphere, true);
        gluQuadricNormals(esphere, GLU_SMOOTH);
        gluSphere(esphere, 0.5, 100, 100);
        
        glDisable(GL_TEXTURE_2D);
   		glBindTexture(GL_TEXTURE_2D,0);
        //glEnable(GL_CULL_FACE);
        //gluDeleteQuadric(esphere);
        //glutSolidSphere(0.5, 10, 10);
	glPopMatrix();

	/*ploce*/
	glPushMatrix();
	    draw();
	glPopMatrix();

	/*result*/
	glPushMatrix();
        glColor3d(0, 0, 0);
        glTranslated(0, scoreY, -5);
        char * stC[100];
        sprintf((char *)stC, "Score: % i", score);
        print(5, 4.5, (char *)stC);
	glPopMatrix();

	/*end game :)*/
	if (gameOver == true) {
		glPushMatrix();
            glColor3d(0, 0, 0);
            glTranslated(1, scoreY, -5);
            char * stC[100];
            sprintf((char *)stC, "Game Over");
            print(0, 0, (char *)stC);
		glPopMatrix();
	}

	
	glutSwapBuffers();
}

