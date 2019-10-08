#include <stdlib.h>
#include <GL/glut.h>

GLfloat fAspect;

GLfloat PontodeControle[4][4][3] = {//Pontos de Contorle

      {{0.0, 0.3, 0.0}, {0.3, 0.5, -0.7}, {0.0, 0.7, 0.0}, {1.0, 0.0, 0.0}},

      {{0.0, 0.3, 0.3}, {0.3, 0.5, 0.3}, {0.0, 0.7, 0.3}, {1.0, 0.0, 0.3}},

      {{0.0, 0.3, 0.7}, {0.3, 0.5, 0.7}, {0.0, 0.7, 0.7}, {1.0, 0.0, 0.7}},

      {{0.0, 0.3, 1.0}, {0.3, 0.5, 1.0}, {0.0, 0.7, 1.0}, {1.0, 0.0, 1.0}}

};

void Luzes(){

	// Intensidades das luzes
	GLfloat posicaoLuz[4] = {0.0, 50.0, 50.0, 1.0};
	GLfloat luzAmbiente[4] = {0.2, 0.2, 0.2, 1.0};
    GLfloat luzDifusa[4] = {0.7, 0.7, 0.7, 1.0};
    GLfloat luzEspecular[4] = {1.0, 1.0, 1.0, 1.0};
    GLfloat brilho[] = {60.0};

	// Coeficiente de reflexão
    glMaterialfv(GL_FRONT, GL_DIFFUSE, luzDifusa);
    glMaterialfv(GL_FRONT, GL_SPECULAR, luzEspecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, brilho);

	// Intensidade da luz que vai predominar o ambiente.
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzDifusa);

	// Parametros da Luz 0.
    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
	glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);


}


void Gira(int butao, int estado , int x , int y){

	if(butao == GLUT_RIGHT_BUTTON && estado == GLUT_DOWN){

		// Rotação de 15 graus com o click
		glRotatef(15.0, 20.0, 0, 0);
		// Limpa buffer de cores e profundidade, para limpar a tela.
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	}

	if(butao == GLUT_LEFT_BUTTON && estado == GLUT_DOWN){

		glRotatef(-15.0, 0, 20.0, 0.0);
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	}

}


void SuperficieBezier(){

	glColor3f(1.0, 1.0, 1.0);
    glPointSize(3);
   glBegin(GL_POINTS);

	for(int i=0; i < 4; i++){
		for(int j=0; j < 4; j++){


			glVertex3f( PontodeControle[i][j+3][0], PontodeControle[i+2][j][1] ,PontodeControle[i][j][2] );


		}
	}
	glEnd();

    glColor3f(1.0, 0.0, 0.0);

    // Define qual modo será o sombreamento. GL Smooth deixa mais realista.
	glShadeModel(GL_SMOOTH);

	// GL_MAP2_VERTEX_3, GLfloat  u1, GLfloat  u2, GLint  ustride, GLint uorder, GLfloat  v1, GLfloat  v2, GLint  vstride, GLint  vorder,  GLfloat  *pontos

	glMap2f(GL_MAP2_VERTEX_3, 0, 0.6, 3, 4, 0, 0.6, 12, 4, &PontodeControle[0][0][0]);

	// Divide a superficie em grades.
    glMapGrid2f(20, 1.0, 0.0, 20, 1.0, 0.0);

    // Calcula a superficie e desenha ela.
    glEvalMesh2(GL_FILL, 0, 20, 0, 20);

    // Envia os comandos para o Hardware executar imediatamente.
    glFlush();




}

void Inicializa(){

	glClearColor (0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	// Calcula as cordenadas x, y e z para os traçados da superficie.
    glEnable(GL_MAP2_VERTEX_3);

	// Calcula automaticamente vetores normais para iluminação.
    glEnable(GL_AUTO_NORMAL);

    Luzes();

}

void EspecificaParametrosVisualizacao(){

	// Especifica sistema de coordenadas de projeção
	glMatrixMode(GL_PROJECTION);

	// Inicializa sistema de coordenadas de projeção
	glLoadIdentity();

	// Especifica a projeção perspectiva
	gluPerspective(2,fAspect,0.4,500);

	// Especifica sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);
	// Inicializa sistema de coordenadas do modelo
	glLoadIdentity();

	// Especifica posição do observador e do alvo
	gluLookAt(0,80,200, 0,0,0, 0,1,0);

}

// Função callback chamada quando o tamanho da janela é alterado
void AlteraTamanhoJanela(GLsizei w, GLsizei h){

	// Para previnir uma divisão por zero
	if ( h == 0 ){
		h = 1;
	}
	// Especifica o tamanho da viewport
	glViewport(0, 0, w, h);

	// Calcula a correção de aspecto
	fAspect = (GLfloat)w/(GLfloat)h;

	EspecificaParametrosVisualizacao();
}


int main(int argc, char **argv){


	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize (800, 800);
    glutInitWindowPosition (500, 100);
    glutCreateWindow("Superfice de Bezier");

    Inicializa();

    glutReshapeFunc(AlteraTamanhoJanela);
    glutDisplayFunc(SuperficieBezier);
    glutMouseFunc(Gira);


    glutMainLoop();
    return 0;

}

