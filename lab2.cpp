#include <iostream>
#include <glut.h>

int n;
GLdouble** points;
int camera[3] = { 0,0,10 };

long double fact(int N)
{
    if(N < 0)
        return 0; 
    if (N == 0) 
        return 1; 
    else 
        return N * fact(N - 1); 
}

void axis()
{
	double axismatrix[6][3] = { {-1000, 0, 0},{1000, 0, 0},{0, -1000, 0},{0, 1000, 0},{0, 0, -1000},{0, 0, 1000} };//матрица осей
	glBegin(GL_LINES);
	int r = 0, g = 0, b = 0;
	for (int i = 0; i < 6; ++i)
	{
		if (i == 0)
		{
			r = 1;
		}
		else if (i == 2)
		{
			r = 0;
			g = 1;
		}
		else if (i == 4)
		{
			g = 0;
			b = 1;
		}
		glColor3f(r, g, b);
		glVertex3dv(axismatrix[i]);//связь образуется между двумя точками объявленными последовательно
	}
	glEnd();//закрываем бегин
}


double BezierEquation(GLdouble** points, double t, bool xaxis)
{
	double result = 0;
	if (xaxis)
		for (int i = 1; i <= n; ++i)
			result += points[i][0] * (fact(n) / (fact(i) * fact(n - i))) * pow(t, i) * pow((1 - t), (n - i));
	else
		for (int i = 1; i <= n; ++i)
			result += points[i][1] * (fact(n) / (fact(i) * fact(n - i))) * pow(t, i) * pow((1 - t), (n - i));
	
	return result;
}

void Bezier(GLdouble** points)
{
	glBegin(GL_POINTS);
	glColor3f(1, 0, 0);
		for (double t = 0; t < 1; t += 0.0001)
		{
			double x = BezierEquation(points,t,true);
			double y = BezierEquation(points, t,false);
			glVertex2f(x, y);
		}
	glEnd();
}
void specialKeyDown(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_F1:
		camera[2] += 1;
		break;
	case GLUT_KEY_F2:
		camera[2] -= 1;
		break;
	default:
		break;
	}
}
void KeyPoints(GLdouble** points) {
	glBegin(GL_POINTS);
	glColor3f(0, 0, 1);
	for (int i = 1; i <= n; ++i)
		glVertex2f(points[i][0],points[i][1]);
	glEnd();
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(camera[0], camera[1], camera[2], 0, 0, 0, 0, 1, 0);
	axis();//отрисовка осей

	glBegin(GL_LINES);
		glColor3f(0, 0, 0);
		for (int i = 1; i <= n - 1; i++)
		{
			glVertex2dv(points[i]);
			glVertex2dv(points[i + 1]);
		}
	glEnd();

	glBegin(GL_POINT);
		glPointSize(10.0);
		KeyPoints(points);
		glPointSize(1.0);
	glEnd();

	Bezier(points);
	glutSpecialFunc(specialKeyDown);
	glutSwapBuffers();
}


void View()//функция для создания окна, вызова отрисовки,
{
	
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(640, 480);//размер окна
	glutInitWindowPosition(200, 200);
	glutCreateWindow("Кривая Безье");//открыть окно с заголовком
	glutDisplayFunc(draw);
	glutIdleFunc(draw);
	glutSpecialFunc(specialKeyDown);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	gluPerspective(25.0, float(640) / float(480), 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glutMainLoop();
}



int main()
{
	setlocale(LC_ALL, "russian");
	std::cout << "Введите количество точек: ";
	std::cin >> n;
	points = new GLdouble * [n + 1];
	for (int i = 0; i < n + 1; ++i)
		points[i] = new GLdouble[2];
	for (int i = 1; i <= n; ++i)
	{
		std::cout << "точка " << i << std::endl;
		std::cout << "координата x = ";
		std::cin >> points[i][0];
		std::cout << "координата y = ";
		std::cin >> points[i][1];
	}
	std::cout << "F1 чтобы отдалить камеру если кривая не вмещается, F2 приблизить";
	View();
	return 0;
}
