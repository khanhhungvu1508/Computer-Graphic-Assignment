/**
 * 	Computer Graphic Assignment
 *	Ten: 	Vu Khanh Hung
 * 	MSSV:	1910232
 * 	Lop:	L02
 */

#include <math.h>
#include <iostream>
#include <windows.h>
#include "GL.H"
#include "glut.h"
#include <cmath>
#include <stdlib.h>     
#include <time.h>
#include <stdio.h>
#include <random>
#include <limits>

#define PI 3.14159265358979323846
#define COLORNUM 14
#define NUM 36

using namespace std;

#pragma region
class Point3
{
public:
	float x, y, z;
	void set(float dx, float dy, float dz)
	{
		x = dx;
		y = dy;
		z = dz;
	}
	void set(Point3& p)
	{
		x = p.x;
		y = p.y;
		z = p.z;
	}
	Point3() { x = y = z = 0; }
	Point3(float dx, float dy, float dz)
	{
		x = dx;
		y = dy;
		z = dz;
	}
};
class Color3
{
public:
	float r, g, b;
	void set(float red, float green, float blue)
	{
		r = red;
		g = green;
		b = blue;
	}
	void set(Color3& c)
	{
		r = c.r;
		g = c.g;
		b = c.b;
	}
	Color3() { r = g = b = 0; }
	Color3(float red, float green, float blue)
	{
		r = red;
		g = green;
		b = blue;
	}
};
class Point2
{
public:
	Point2() { x = y = 0.0f; } 
	Point2(float xx, float yy)
	{
		x = xx;
		y = yy;
	} 
	void set(float xx, float yy)
	{
		x = xx;
		y = yy;
	}
	float getX() { return x; }
	float getY() { return y; }
	void draw()
	{
		glBegin(GL_POINTS);
		glVertex2f((GLfloat)x, (GLfloat)y);
		glEnd();
	}

private:
	float x, y;
};
class IntRect
{
public:
	IntRect()
	{
		l = 0;
		r = 100;
		b = 0;
		t = 100;
	} 
	IntRect(int left, int right, int bottom, int top)
	{
		l = left;
		r = right;
		b = bottom;
		t = top;
	}
	void set(int left, int right, int bottom, int top)
	{
		l = left;
		r = right;
		b = bottom;
		t = top;
	}
	void draw()
	{
		glRecti(l, b, r, t);
		glFlush();
	}
	int getWidth() { return (r - l); }
	int getHeight() { return (t - b); }

private:
	int l, r, b, t;
};

class RealRect
{
public:
	RealRect()
	{
		l = 0;
		r = 100;
		b = 0;
		t = 100;
	} 
	RealRect(float left, float right, float bottom, float top)
	{
		l = left;
		r = right;
		b = bottom;
		t = top;
	}
	void set(float left, float right, float bottom, float top)
	{
		l = left;
		r = right;
		b = bottom;
		t = top;
	}
	float getWidth() { return (r - l); }
	float getHeight() { return (t - b); }
	void draw()
	{
		glRectf(l, b, r, t);
		glFlush();
	}; 
private:
	float l, r, b, t;
};

class Vector3
{
public:
	float x, y, z;
	void set(float dx, float dy, float dz)
	{
		x = dx;
		y = dy;
		z = dz;
	}
	void set(Vector3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}
	void flip()
	{
		x = -x;
		y = -y;
		z = -z;
	}
	void normalize();
	Vector3() { x = y = z = 0; }
	Vector3(float dx, float dy, float dz)
	{
		x = dx;
		y = dy;
		z = dz;
	}
	Vector3(Vector3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}
	Vector3 cross(Vector3 b);
	float dot(Vector3 b);
};

Vector3 Vector3::cross(Vector3 b)
{
	Vector3 c(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x);
	return c;
}
float Vector3::dot(Vector3 b)
{
	return x * b.x + y * b.y + z * b.z;
}
void Vector3::normalize()
{
	float temp = sqrt(x * x + y * y + z * z);
	x = x / temp;
	y = y / temp;
	z = z / temp;
}
#pragma endregion

// Mesh
#pragma region
class VertexID
{
public:
	int vertIndex;
	int colorIndex;
};

class Face
{
public:
	int nVerts;
	VertexID* vert;
	Vector3 facenorm; 

	Face()
	{
		nVerts = 0;
		vert = NULL;
	}
	~Face()
	{
		if (vert != NULL)
		{
			delete[] vert;
			vert = NULL;
		}
		nVerts = 0;
	}
};

class Mesh
{
public:
	int numVerts;
	Point3* pt;

	int numFaces;
	Face* face;

	float slideX, slideY, slideZ;
	float rotateX, rotateY, rotateZ;
	float scaleX, scaleY, scaleZ;

public:
	Mesh()
	{
		numVerts = 0;
		pt = NULL;
		numFaces = 0;
		face = NULL;
		// Movement variables
		slideX = 0;
		slideY = 0;
		slideZ = 0;
		rotateX = 0;
		rotateY = 0;
		rotateZ = 0;
		scaleX = 0;
		scaleY = 0;
		scaleZ = 0;
	}
	~Mesh()
	{
		if (pt != NULL)
		{
			delete[] pt;
		}
		if (face != NULL)
		{
			delete[] face;
		}
		numVerts = 0;
		numFaces = 0;
	}

	void DrawWireframe();
	void DrawColor();
	void SetColor(int colorIdx);
	void CreateTetrahedron();
	void CreateCube(float fSize);
	void CreateCuboid(float fSizeX, float fSizeY, float fSizeZ);
	void OvalHole(int N, float bigger_radius, float smaller_radius, float length, float height);
	void drawU(float length_biggerU, float length_smallerU, float width_biggerU, float width_smallerU, float height);
	void gouged_semicircle(int N, float width, float height, float larger_radius, float smaller_radius);
	void CreateCylinder(int N, float base1Height, float base1Radius);
	void CalculateFacesNorm();
	void Draw();
	void setupMaterial(float ambient[], float diffuse[], float specular[], float shiness);
};

float ColorArr[COLORNUM][3] = {
	{1.0, 0.0, 0.0}, //0
	{0.0, 1.0, 0.0}, //1
	{0.0, 0.0, 1.0}, //2
	{1.0, 1.0, 0.0}, //3
	{1.0, 0.0, 1.0}, //4
	{0.0, 1.0, 1.0}, //5
	{0.3, 0.3, 0.3}, //6
	{0.5, 0.5, 0.5}, //7
	{0.9, 0.9, 0.9}, //8
	{1.0, 0.5, 0.5}, //9
	{0.5, 1.0, 0.5}, //10
	{0.5, 0.5, 1.0}, //11
	{0.0, 0.0, 0.0}, //12
	{1.0, 1.0, 1.0} }; //13

void Mesh::DrawWireframe()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for (int f = 0; f < numFaces; f++)
	{
		glBegin(GL_POLYGON);
		for (int v = 0; v < face[f].nVerts; v++)
		{
			int iv = face[f].vert[v].vertIndex;
			glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
		}
		glEnd();
	}
}

void Mesh::DrawColor()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	for (int f = 0; f < numFaces; f++)
	{
		glBegin(GL_POLYGON);
		for (int v = 0; v < face[f].nVerts; v++)
		{
			int iv = face[f].vert[v].vertIndex;
			int ic = face[f].vert[v].colorIndex;
			glColor3f(ColorArr[ic][0], ColorArr[ic][1], ColorArr[ic][2]);
			glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
		}
		glEnd();
	}
}

void Mesh::SetColor(int colorIdx)
{
	for (int f = 0; f < numFaces; f++)
	{
		for (int v = 0; v < face[f].nVerts; v++)
		{
			face[f].vert[v].colorIndex = colorIdx;
		}
	}
}

void Mesh::CalculateFacesNorm()
{
	for (int f = 0; f < numFaces; f++)
	{
		float mx = 0, my = 0, mz = 0;
		for (int v = 0; v < face[f].nVerts; v++)
		{
			int iv = face[f].vert[v].vertIndex;
			int next = face[f].vert[(v + 1) % face[f].nVerts].vertIndex;
			mx += (pt[iv].y - pt[next].y) * (pt[iv].z + pt[next].z);
			my += (pt[iv].z - pt[next].z) * (pt[iv].x + pt[next].x);
			mz += (pt[iv].x - pt[next].x) * (pt[iv].y + pt[next].y);
		}
		face[f].facenorm.set(mx, my, mz);
		face[f].facenorm.normalize();
	}
}

void Mesh::Draw()
{
	for (int f = 0; f < numFaces; f++)
	{
		glBegin(GL_POLYGON);
		for (int v = 0; v < face[f].nVerts; v++)
		{
			int iv = face[f].vert[v].vertIndex;
			glNormal3f(face[f].facenorm.x, face[f].facenorm.y, face[f].facenorm.z);
			glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
		}
		glEnd();
	}
}

void Mesh::setupMaterial(float ambient[], float diffuse[], float specular[], float shiness)
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiness);
}

void Mesh::CreateCube(float fSize)
{
	int i;

	numVerts = 8;
	pt = new Point3[numVerts];
	pt[0].set(-fSize, fSize, fSize);
	pt[1].set(fSize, fSize, fSize);
	pt[2].set(fSize, fSize, -fSize);
	pt[3].set(-fSize, fSize, -fSize);
	pt[4].set(-fSize, -fSize, fSize);
	pt[5].set(fSize, -fSize, fSize);
	pt[6].set(fSize, -fSize, -fSize);
	pt[7].set(-fSize, -fSize, -fSize);

	numFaces = 6;
	face = new Face[numFaces];

	//Left face
	face[0].nVerts = 4;
	face[0].vert = new VertexID[face[0].nVerts];
	face[0].vert[0].vertIndex = 1;
	face[0].vert[1].vertIndex = 5;
	face[0].vert[2].vertIndex = 6;
	face[0].vert[3].vertIndex = 2;
	for (i = 0; i < face[0].nVerts; i++)
		face[0].vert[i].colorIndex = 0;

	//Right face
	face[1].nVerts = 4;
	face[1].vert = new VertexID[face[1].nVerts];
	face[1].vert[0].vertIndex = 0;
	face[1].vert[1].vertIndex = 3;
	face[1].vert[2].vertIndex = 7;
	face[1].vert[3].vertIndex = 4;
	for (i = 0; i < face[1].nVerts; i++)
		face[1].vert[i].colorIndex = 1;

	//top face
	face[2].nVerts = 4;
	face[2].vert = new VertexID[face[2].nVerts];
	face[2].vert[0].vertIndex = 0;
	face[2].vert[1].vertIndex = 1;
	face[2].vert[2].vertIndex = 2;
	face[2].vert[3].vertIndex = 3;
	for (i = 0; i < face[2].nVerts; i++)
		face[2].vert[i].colorIndex = 2;

	//bottom face
	face[3].nVerts = 4;
	face[3].vert = new VertexID[face[3].nVerts];
	face[3].vert[0].vertIndex = 7;
	face[3].vert[1].vertIndex = 6;
	face[3].vert[2].vertIndex = 5;
	face[3].vert[3].vertIndex = 4;
	for (i = 0; i < face[3].nVerts; i++)
		face[3].vert[i].colorIndex = 3;

	//near face
	face[4].nVerts = 4;
	face[4].vert = new VertexID[face[4].nVerts];
	face[4].vert[0].vertIndex = 4;
	face[4].vert[1].vertIndex = 5;
	face[4].vert[2].vertIndex = 1;
	face[4].vert[3].vertIndex = 0;
	for (i = 0; i < face[4].nVerts; i++)
		face[4].vert[i].colorIndex = 4;

	//Far face
	face[5].nVerts = 4;
	face[5].vert = new VertexID[face[5].nVerts];
	face[5].vert[0].vertIndex = 3;
	face[5].vert[1].vertIndex = 2;
	face[5].vert[2].vertIndex = 6;
	face[5].vert[3].vertIndex = 7;
	for (i = 0; i < face[5].nVerts; i++)
		face[5].vert[i].colorIndex = 5;
}

void Mesh::CreateTetrahedron()
{
	int i;
	numVerts = 4;
	pt = new Point3[numVerts];
	pt[0].set(0, 0, 0);
	pt[1].set(1, 0, 0);
	pt[2].set(0, 1, 0);
	pt[3].set(0, 0, 1);

	numFaces = 4;
	face = new Face[numFaces];

	face[0].nVerts = 3;
	face[0].vert = new VertexID[face[0].nVerts];
	face[0].vert[0].vertIndex = 1;
	face[0].vert[1].vertIndex = 2;
	face[0].vert[2].vertIndex = 3;
	for (i = 0; i < face[0].nVerts; i++)
		face[0].vert[i].colorIndex = 0;

	face[1].nVerts = 3;
	face[1].vert = new VertexID[face[1].nVerts];
	face[1].vert[0].vertIndex = 0;
	face[1].vert[1].vertIndex = 2;
	face[1].vert[2].vertIndex = 1;
	for (i = 0; i < face[1].nVerts; i++)
		face[1].vert[i].colorIndex = 1;

	face[2].nVerts = 3;
	face[2].vert = new VertexID[face[2].nVerts];
	face[2].vert[0].vertIndex = 0;
	face[2].vert[1].vertIndex = 3;
	face[2].vert[2].vertIndex = 2;
	for (i = 0; i < face[2].nVerts; i++)
		face[2].vert[i].colorIndex = 2;

	face[3].nVerts = 3;
	face[3].vert = new VertexID[face[3].nVerts];
	face[3].vert[0].vertIndex = 1;
	face[3].vert[1].vertIndex = 3;
	face[3].vert[2].vertIndex = 0;
	for (i = 0; i < face[3].nVerts; i++)
		face[3].vert[i].colorIndex = 3;
}

void Mesh::CreateCuboid(float fSizeX, float fSizeY, float fSizeZ)
{
	int i;
	numVerts = 8;
	pt = new Point3[numVerts];

	pt[0].set(-fSizeX / 2, -fSizeY / 2, -fSizeZ / 2);
	pt[1].set(fSizeX / 2, -fSizeY / 2, -fSizeZ / 2);
	pt[2].set(fSizeX / 2, fSizeY / 2, -fSizeZ / 2);
	pt[3].set(-fSizeX / 2, fSizeY / 2, -fSizeZ / 2);
	pt[4].set(-fSizeX / 2, -fSizeY / 2, fSizeZ / 2);
	pt[5].set(fSizeX / 2, -fSizeY / 2, fSizeZ / 2);
	pt[6].set(fSizeX / 2, fSizeY / 2, fSizeZ / 2);
	pt[7].set(-fSizeX / 2, fSizeY / 2, fSizeZ / 2);

	numFaces = 6;
	face = new Face[numFaces];

	face[0].nVerts = 4;
	face[0].vert = new VertexID[face[0].nVerts];
	face[0].vert[0].vertIndex = 0;
	face[0].vert[1].vertIndex = 4;
	face[0].vert[2].vertIndex = 7;
	face[0].vert[3].vertIndex = 3;
	for (i = 0; i < face[0].nVerts; i++)
		face[0].vert[i].colorIndex = 0;

	face[1].nVerts = 4;
	face[1].vert = new VertexID[face[1].nVerts];
	face[1].vert[0].vertIndex = 3;
	face[1].vert[1].vertIndex = 7;
	face[1].vert[2].vertIndex = 6;
	face[1].vert[3].vertIndex = 2;
	for (i = 0; i < face[1].nVerts; i++)
		face[1].vert[i].colorIndex = 1;

	face[2].nVerts = 4;
	face[2].vert = new VertexID[face[2].nVerts];
	face[2].vert[0].vertIndex = 1;
	face[2].vert[1].vertIndex = 2;
	face[2].vert[2].vertIndex = 6;
	face[2].vert[3].vertIndex = 5;
	for (i = 0; i < face[2].nVerts; i++)
		face[2].vert[i].colorIndex = 2;

	face[3].nVerts = 4;
	face[3].vert = new VertexID[face[3].nVerts];
	face[3].vert[0].vertIndex = 0;
	face[3].vert[1].vertIndex = 1;
	face[3].vert[2].vertIndex = 5;
	face[3].vert[3].vertIndex = 4;
	for (i = 0; i < face[3].nVerts; i++)
		face[3].vert[i].colorIndex = 3;

	face[4].nVerts = 4;
	face[4].vert = new VertexID[face[4].nVerts];
	face[4].vert[0].vertIndex = 6;
	face[4].vert[1].vertIndex = 7;
	face[4].vert[2].vertIndex = 4;
	face[4].vert[3].vertIndex = 5;
	for (i = 0; i < face[4].nVerts; i++)
		face[4].vert[i].colorIndex = 4;

	face[5].nVerts = 4;
	face[5].vert = new VertexID[face[5].nVerts];
	face[5].vert[0].vertIndex = 2;
	face[5].vert[1].vertIndex = 1;
	face[5].vert[2].vertIndex = 0;
	face[5].vert[3].vertIndex = 3;
	for (i = 0; i < face[5].nVerts; i++)
		face[5].vert[i].colorIndex = 5;
}

void Mesh::CreateCylinder(int N, float base1Height, float base1Radius)
{
	/*
	* N: Số đỉnh trên hình tròn
	* base1Height: Chiều cao của hình trụ
	* base1Radius: bán kính của hình tròn
	*/
	int i;
	numVerts = 2 * N + 2;
	GLfloat angle = 2 * PI / N;
	pt = new Point3[numVerts];
	for (i = 0; i < N; i++)
		pt[i].set(base1Radius * sin((double)i * angle), base1Height / 2, -base1Radius * cos((double)i * angle));
	for (i = 0; i < N; i++)
		pt[i + N].set(base1Radius * sin((double)i * angle), -base1Height / 2, -base1Radius * cos((double)i * angle));
	pt[2 * N].set(0, base1Height / 2, 0);
	pt[2 * N + 1].set(0, -base1Height / 2, 0);
	numFaces = 3 * N;
	face = new Face[numFaces];
	for (i = 0; i < numFaces; i++)
	{
		if (i < N - 1)
		{
			face[i].nVerts = 3;
			face[i].vert = new VertexID[face[i].nVerts];

			face[i].vert[0].vertIndex = i;
			face[i].vert[1].vertIndex = i + 1;
			face[i].vert[2].vertIndex = 2 * N;
		}
		else if (i == N - 1)
		{
			face[i].nVerts = 3;
			face[i].vert = new VertexID[face[i].nVerts];

			face[i].vert[0].vertIndex = i;
			face[i].vert[1].vertIndex = 0;
			face[i].vert[2].vertIndex = 2 * N;
		}
		else if (i < 2 * N - 1)
		{
			face[i].nVerts = 3;
			face[i].vert = new VertexID[face[i].nVerts];

			face[i].vert[0].vertIndex = i;
			face[i].vert[1].vertIndex = 2 * N + 1;
			face[i].vert[2].vertIndex = i + 1;
		}
		else if (i == 2 * N - 1)
		{
			face[i].nVerts = 3;
			face[i].vert = new VertexID[face[i].nVerts];

			face[i].vert[0].vertIndex = i;
			face[i].vert[1].vertIndex = 2 * N + 1;
			face[i].vert[2].vertIndex = N;
		}
		else if (i < 3 * N - 1)
		{
			face[i].nVerts = 4;
			face[i].vert = new VertexID[face[i].nVerts];

			face[i].vert[0].vertIndex = i - 2 * N + 1;
			face[i].vert[1].vertIndex = i - 2 * N;
			face[i].vert[2].vertIndex = i - N;
			face[i].vert[3].vertIndex = i - N + 1;
		}
		else
		{
			face[i].nVerts = 4;
			face[i].vert = new VertexID[face[i].nVerts];

			face[i].vert[0].vertIndex = 0;
			face[i].vert[1].vertIndex = N - 1;
			face[i].vert[2].vertIndex = 2 * N - 1;
			face[i].vert[3].vertIndex = N;
		}
		for (int j = 0; j < face[i].nVerts; j++)
			face[i].vert[j].colorIndex = i;
	}
}

void Mesh::OvalHole(int N, float bigger_radius, float smaller_radius, float width, float height)
{
	// N: So dinh tren mot nua hinh tron
	GLfloat angle = 2 * PI / (2 * (N - 1));

	int i;
	numVerts = N * 8;
	pt = new Point3[numVerts];

	for (i = 0; i < N; i++)
		pt[i].set(-width / 2 - bigger_radius * sin((double)i * angle), height / 2, -bigger_radius * cos((double)i * angle));

	for (i = 0; i < N; i++)
		pt[i + N].set(-width / 2 - smaller_radius * sin((double)i * angle), height / 2, -smaller_radius * cos((double)i * angle));

	for (i = 0; i < N; i++)
		pt[i + N * 2].set(-width / 2 - bigger_radius * sin((double)i * angle), -height / 2, -bigger_radius * cos((double)i * angle));

	for (i = 0; i < N; i++)
		pt[i + N * 3].set(-width / 2 - smaller_radius * sin((double)i * angle), -height / 2, -smaller_radius * cos((double)i * angle));

	for (i = 0; i < N; i++)
		pt[i + N * 4].set(width / 2 + bigger_radius * sin((double)i * angle), height / 2, -bigger_radius * cos(-(double)i * angle));

	for (i = 0; i < N; i++)
		pt[i + N * 5].set(width / 2 + smaller_radius * sin((double)i * angle), height / 2, -smaller_radius * cos(-(double)i * angle));

	for (i = 0; i < N; i++)
		pt[i + N * 6].set(width / 2 + bigger_radius * sin((double)i * angle), -height / 2, -bigger_radius * cos(-(double)i * angle));

	for (i = 0; i < N; i++)
		pt[i + N * 7].set(width / 2 + smaller_radius * sin((double)i * angle), -height / 2, -smaller_radius * cos(-(double)i * angle));

	numFaces = (N - 1) * 8 + 8;
	face = new Face[numFaces];
	int color = 0;
	for (i = 0; i < N - 1; i++)
	{
		face[i].nVerts = 4;
		face[i].vert = new VertexID[face[i].nVerts];
		face[i].vert[0].vertIndex = i;
		face[i].vert[1].vertIndex = i + 1;
		face[i].vert[2].vertIndex = i + N + 1;
		face[i].vert[3].vertIndex = i + N;
		for (int j = 0; j < face[i].nVerts; j++)
			face[i].vert[j].colorIndex = color;
		color++;
	}

	for (i = 0; i < N - 1; i++)
	{
		face[i + (N - 1)].nVerts = 4;
		face[i + (N - 1)].vert = new VertexID[face[i + N - 1].nVerts];
		face[i + (N - 1)].vert[0].vertIndex = i + N * 2 + 1;
		face[i + (N - 1)].vert[1].vertIndex = i + N * 2;
		face[i + (N - 1)].vert[2].vertIndex = i + N * 3;
		face[i + (N - 1)].vert[3].vertIndex = i + N * 3 + 1;
		for (int j = 0; j < face[i + N - 1].nVerts; j++)
			face[i + N - 1].vert[j].colorIndex = color;
		color++;
	}

	for (i = 0; i < N - 1; i++)
	{
		face[i + (N - 1) * 2].nVerts = 4;
		face[i + (N - 1) * 2].vert = new VertexID[face[i + (N - 1) * 2].nVerts];
		face[i + (N - 1) * 2].vert[0].vertIndex = i + 1;
		face[i + (N - 1) * 2].vert[1].vertIndex = i;
		face[i + (N - 1) * 2].vert[2].vertIndex = i + N * 2;
		face[i + (N - 1) * 2].vert[3].vertIndex = i + N * 2 + 1;
		for (int j = 0; j < face[i + (N - 1) * 2].nVerts; j++)
			face[i + (N - 1) * 2].vert[j].colorIndex = color;
		color++;
	}

	for (i = 0; i < (N - 1); i++)
	{
		face[i + (N - 1) * 3].nVerts = 4;
		face[i + (N - 1) * 3].vert = new VertexID[face[i + (N - 1) * 3].nVerts];
		face[i + (N - 1) * 3].vert[0].vertIndex = i + N;
		face[i + (N - 1) * 3].vert[1].vertIndex = i + N + 1;
		face[i + (N - 1) * 3].vert[2].vertIndex = i + N * 3 + 1;
		face[i + (N - 1) * 3].vert[3].vertIndex = i + N * 3;
		for (int j = 0; j < face[i + (N - 1) * 3].nVerts; j++)
			face[i + (N - 1) * 3].vert[j].colorIndex = color;
		color++;
	}

	for (i = 0; i < (N - 1); i++)
	{
		face[i + (N - 1) * 4].nVerts = 4;
		face[i + (N - 1) * 4].vert = new VertexID[face[i + (N - 1) * 4].nVerts];

		face[i + (N - 1) * 4].vert[0].vertIndex = i + N * 4 + 1;
		face[i + (N - 1) * 4].vert[1].vertIndex = i + N * 4;
		face[i + (N - 1) * 4].vert[2].vertIndex = i + N * 5;
		face[i + (N - 1) * 4].vert[3].vertIndex = i + N * 5 + 1;

		for (int j = 0; j < face[i + (N - 1) * 4].nVerts; j++)
			face[i + (N - 1) * 4].vert[j].colorIndex = color;
		color++;
	}

	for (i = 0; i < (N - 1); i++)
	{
		face[i + (N - 1) * 5].nVerts = 4;
		face[i + (N - 1) * 5].vert = new VertexID[face[i + (N - 1) * 5].nVerts];
		face[i + (N - 1) * 5].vert[0].vertIndex = i + N * 6;
		face[i + (N - 1) * 5].vert[1].vertIndex = i + N * 6 + 1;
		face[i + (N - 1) * 5].vert[2].vertIndex = i + N * 7 + 1;
		face[i + (N - 1) * 5].vert[3].vertIndex = i + N * 7;

		for (int j = 0; j < face[i + (N - 1) * 5].nVerts; j++)
			face[i + (N - 1) * 5].vert[j].colorIndex = color;
		color++;
	}

	for (i = 0; i < (N - 1); i++)
	{
		face[i + (N - 1) * 6].nVerts = 4;
		face[i + (N - 1) * 6].vert = new VertexID[face[i + (N - 1) * 6].nVerts];
		face[i + (N - 1) * 6].vert[0].vertIndex = i + N * 4;
		face[i + (N - 1) * 6].vert[1].vertIndex = i + N * 4 + 1;
		face[i + (N - 1) * 6].vert[2].vertIndex = i + N * 6 + 1;
		face[i + (N - 1) * 6].vert[3].vertIndex = i + N * 6;
		for (int j = 0; j < face[i + (N - 1) * 6].nVerts; j++)
			face[i + (N - 1) * 6].vert[j].colorIndex = color;
		color++;
	}

	for (i = 0; i < (N - 1); i++)
	{
		face[i + (N - 1) * 7].nVerts = 4;
		face[i + (N - 1) * 7].vert = new VertexID[face[i + (N - 1) * 7].nVerts];
		face[i + (N - 1) * 7].vert[0].vertIndex = i + N * 5 + 1;
		face[i + (N - 1) * 7].vert[1].vertIndex = i + N * 5;
		face[i + (N - 1) * 7].vert[2].vertIndex = i + N * 7;
		face[i + (N - 1) * 7].vert[3].vertIndex = i + N * 7 + 1;
		for (int j = 0; j < face[i + (N - 1) * 7].nVerts; j++)
			face[i + (N - 1) * 7].vert[j].colorIndex = color;
		color++;
	}

	i = (N - 1) * 8;
	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];
	face[i].vert[0].vertIndex = N;
	face[i].vert[1].vertIndex = N * 3;
	face[i].vert[2].vertIndex = N * 7;
	face[i].vert[3].vertIndex = N * 5;

	for (int j = 0; j < face[i].nVerts; j++)
		face[i].vert[j].colorIndex = color;
	color++;

	i = (N - 1) * 8 + 1;
	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];
	face[i].vert[0].vertIndex = 0;
	face[i].vert[1].vertIndex = N;
	face[i].vert[2].vertIndex = N * 5;
	face[i].vert[3].vertIndex = N * 4;

	for (int j = 0; j < face[i].nVerts; j++)
		face[i].vert[j].colorIndex = color;
	color++;

	i = (N - 1) * 8 + 2;
	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];
	face[i].vert[0].vertIndex = 0;
	face[i].vert[1].vertIndex = N * 4;
	face[i].vert[2].vertIndex = N * 6;
	face[i].vert[3].vertIndex = N * 2;

	for (int j = 0; j < face[i].nVerts; j++)
		face[i].vert[j].colorIndex = color;
	color++;

	i = (N - 1) * 8 + 3;
	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];
	face[i].vert[0].vertIndex = N * 2;
	face[i].vert[1].vertIndex = N * 6;
	face[i].vert[2].vertIndex = N * 7;
	face[i].vert[3].vertIndex = N * 3;

	for (int j = 0; j < face[i].nVerts; j++)
		face[i].vert[j].colorIndex = color;
	color++;

	i = (N - 1) * 8 + 4;
	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];
	face[i].vert[0].vertIndex = (N - 1) + N * 0;
	face[i].vert[1].vertIndex = (N - 1) + N * 2;
	face[i].vert[2].vertIndex = (N - 1) + N * 6;
	face[i].vert[3].vertIndex = (N - 1) + N * 4;

	for (int j = 0; j < face[i].nVerts; j++)
		face[i].vert[j].colorIndex = color;
	color++;

	i = (N - 1) * 8 + 5;
	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];
	face[i].vert[0].vertIndex = (N - 1) + N * 1;
	face[i].vert[1].vertIndex = (N - 1) + N * 0;
	face[i].vert[2].vertIndex = (N - 1) + N * 4;
	face[i].vert[3].vertIndex = (N - 1) + N * 5;

	for (int j = 0; j < face[i].nVerts; j++)
		face[i].vert[j].colorIndex = color;
	color++;

	i = (N - 1) * 8 + 6;
	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];
	face[i].vert[0].vertIndex = (N - 1) + N * 1;
	face[i].vert[1].vertIndex = (N - 1) + N * 5;
	face[i].vert[2].vertIndex = (N - 1) + N * 7;
	face[i].vert[3].vertIndex = (N - 1) + N * 3;

	for (int j = 0; j < face[i].nVerts; j++)
		face[i].vert[j].colorIndex = color;
	color++;

	i = (N - 1) * 8 + 7;
	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];
	face[i].vert[0].vertIndex = (N - 1) + N * 2;
	face[i].vert[1].vertIndex = (N - 1) + N * 3;
	face[i].vert[2].vertIndex = (N - 1) + N * 7;
	face[i].vert[3].vertIndex = (N - 1) + N * 6;

	for (int j = 0; j < face[i].nVerts; j++)
		face[i].vert[j].colorIndex = color;
	color++;
}

void Mesh::drawU(float length_biggerU, float length_smallerU, float width_biggerU, float width_smallerU, float height)
{
	int i;
	numVerts = 20;
	pt = new Point3[numVerts];

	pt[0].set(0, 0, 0);
	pt[1].set(length_biggerU, 0, 0);
	pt[2].set(length_biggerU, height, 0);
	pt[3].set(0, height, 0);
	pt[4].set(0, height, width_biggerU);
	pt[5].set(0, 0, width_biggerU);
	pt[6].set((length_biggerU - length_smallerU) / 2, 0, width_biggerU);
	pt[7].set((length_biggerU - length_smallerU) / 2, height, width_biggerU);
	pt[8].set((length_biggerU - length_smallerU) / 2, height, width_biggerU - width_smallerU);
	pt[9].set((length_biggerU - length_smallerU) / 2, 0, width_biggerU - width_smallerU);
	pt[10].set((length_biggerU + length_smallerU) / 2, 0, width_biggerU - width_smallerU);
	pt[11].set((length_biggerU + length_smallerU) / 2, height, width_biggerU - width_smallerU);
	pt[12].set((length_biggerU + length_smallerU) / 2, height, width_biggerU);
	pt[13].set((length_biggerU + length_smallerU) / 2, 0, width_biggerU);
	pt[14].set(length_biggerU, 0, width_biggerU);
	pt[15].set(length_biggerU, height, width_biggerU);
	pt[16].set((length_biggerU - length_smallerU) / 2, height, 0);
	pt[17].set((length_biggerU - length_smallerU) / 2, 0, 0);
	pt[18].set((length_biggerU + length_smallerU) / 2, 0, 0);
	pt[19].set((length_biggerU + length_smallerU) / 2, height, 0);

	numFaces = 14;
	face = new Face[numFaces];

	// Base face 1
	face[0].nVerts = 4;
	face[0].vert = new VertexID[face[0].nVerts];
	face[0].vert[0].vertIndex = 0;
	face[0].vert[1].vertIndex = 1;
	face[0].vert[2].vertIndex = 2;
	face[0].vert[3].vertIndex = 3;
	for (i = 0; i < face[0].nVerts; i++)
		face[0].vert[i].colorIndex = 0;

	// Base face 2
	face[1].nVerts = 4;
	face[1].vert = new VertexID[face[1].nVerts];
	face[1].vert[0].vertIndex = 4;
	face[1].vert[1].vertIndex = 5;
	face[1].vert[2].vertIndex = 6;
	face[1].vert[3].vertIndex = 7;
	for (i = 0; i < face[1].nVerts; i++)
		face[1].vert[i].colorIndex = 1;
	
	// Base face 3
	face[2].nVerts = 4;
	face[2].vert = new VertexID[face[2].nVerts];
	face[2].vert[0].vertIndex = 8;
	face[2].vert[1].vertIndex = 9;
	face[2].vert[2].vertIndex = 10;
	face[2].vert[3].vertIndex = 11;
	for (i = 0; i < face[2].nVerts; i++)
		face[2].vert[i].colorIndex = 2;
	
	// Base face 4
	face[3].nVerts = 4;
	face[3].vert = new VertexID[face[3].nVerts];
	face[3].vert[0].vertIndex = 12;
	face[3].vert[1].vertIndex = 13;
	face[3].vert[2].vertIndex = 14;
	face[3].vert[3].vertIndex = 15;
	for (i = 0; i < face[3].nVerts; i++)
		face[3].vert[i].colorIndex = 3;
	
	// Left face 1
	face[4].nVerts = 4;
	face[4].vert = new VertexID[face[4].nVerts];
	face[4].vert[0].vertIndex = 0;
	face[4].vert[1].vertIndex = 3;
	face[4].vert[2].vertIndex = 4;
	face[4].vert[3].vertIndex = 5;
	for (i = 0; i < face[4].nVerts; i++)
		face[4].vert[i].colorIndex = 4;
	
	// Right face 1
	face[5].nVerts = 4;
	face[5].vert = new VertexID[face[5].nVerts];
	face[5].vert[0].vertIndex = 6;
	face[5].vert[1].vertIndex = 7;
	face[5].vert[2].vertIndex = 16;
	face[5].vert[3].vertIndex = 17;
	for (i = 0; i < face[5].nVerts; i++)
		face[5].vert[i].colorIndex = 5;
	
	// Left face 2
	face[6].nVerts = 4;
	face[6].vert = new VertexID[face[6].nVerts];
	face[6].vert[0].vertIndex = 18;
	face[6].vert[1].vertIndex = 19;
	face[6].vert[2].vertIndex = 12;
	face[6].vert[3].vertIndex = 13;
	for (i = 0; i < face[6].nVerts; i++)
		face[6].vert[i].colorIndex = 6;
	
	// Right face 2
	face[7].nVerts = 4;
	face[7].vert = new VertexID[face[7].nVerts];
	face[7].vert[0].vertIndex = 1;
	face[7].vert[1].vertIndex = 2;
	face[7].vert[2].vertIndex = 15;
	face[7].vert[3].vertIndex = 14;
	for (i = 0; i < face[7].nVerts; i++)
		face[7].vert[i].colorIndex = 7;

	// Near face 1
	face[8].nVerts = 4;
	face[8].vert = new VertexID[face[8].nVerts];
	face[8].vert[0].vertIndex = 3;
	face[8].vert[1].vertIndex = 4;
	face[8].vert[2].vertIndex = 7;
	face[8].vert[3].vertIndex = 16;
	for (i = 0; i < face[8].nVerts; i++)
		face[8].vert[i].colorIndex = 8;
	
	// Near face 2
	face[9].nVerts = 4;
	face[9].vert = new VertexID[face[9].nVerts];
	face[9].vert[0].vertIndex = 16;
	face[9].vert[1].vertIndex = 8;
	face[9].vert[2].vertIndex = 11;
	face[9].vert[3].vertIndex = 19;
	for (i = 0; i < face[9].nVerts; i++)
		face[9].vert[i].colorIndex = 9;
	
	// Near face 3
	face[10].nVerts = 4;
	face[10].vert = new VertexID[face[10].nVerts];
	face[10].vert[0].vertIndex = 2;
	face[10].vert[1].vertIndex = 15;
	face[10].vert[2].vertIndex = 12;
	face[10].vert[3].vertIndex = 19;
	for (i = 0; i < face[10].nVerts; i++)
		face[10].vert[i].colorIndex = 10;

	// Far face 1
	face[11].nVerts = 4;
	face[11].vert = new VertexID[face[11].nVerts];
	face[11].vert[0].vertIndex = 0;
	face[11].vert[1].vertIndex = 5;
	face[11].vert[2].vertIndex = 6;
	face[11].vert[3].vertIndex = 17;
	for (i = 0; i < face[11].nVerts; i++)
		face[11].vert[i].colorIndex = 11;

	// Far face 2
	face[12].nVerts = 4;
	face[12].vert = new VertexID[face[12].nVerts];
	face[12].vert[0].vertIndex = 17;
	face[12].vert[1].vertIndex = 9;
	face[12].vert[2].vertIndex = 10;
	face[12].vert[3].vertIndex = 18;
	for (i = 0; i < face[11].nVerts; i++)
		face[12].vert[i].colorIndex = 12;
	
	// Far face 3
	face[13].nVerts = 4;
	face[13].vert = new VertexID[face[13].nVerts];
	face[13].vert[0].vertIndex = 1;
	face[13].vert[1].vertIndex = 14;
	face[13].vert[2].vertIndex = 13;
	face[13].vert[3].vertIndex = 18;
	for (i = 0; i < face[11].nVerts; i++)
		face[13].vert[i].colorIndex = 13;

}

void Mesh::gouged_semicircle(int N, float width, float height, float larger_radius, float smaller_radius)
{
	// N: so diem tren mot nua hinh tron
	int M = (N - 1) * 2; // M: So diem tren mot hinh tron
	GLfloat angle = 2 * PI / M;

	int i = 0;
	numVerts = M * 2 + N * 2 + N * 2 + 4;
	pt = new Point3[numVerts];

	for (int j = 0; j < M; j++)
	{
		pt[i].set(smaller_radius * cos((double)j * angle), height + smaller_radius * sin((double)j * angle), width / 2);
		i++;
	}

	for (int j = 0; j < M; j++)
	{
		pt[i].set(smaller_radius * cos((double)j * angle), height + smaller_radius * sin((double)j * angle), -width / 2);
		i++;
	}

	for (int j = 0; j < N; j++)
	{
		pt[i].set(larger_radius * cos((double)j * angle), height + larger_radius * sin((double)j * angle), width / 2);
		i++;
	}

	for (int j = 0; j < N; j++)
	{
		pt[i].set(larger_radius * cos((double)j * angle), height + larger_radius * sin((double)j * angle), -width / 2);
		i++;
	}

	for (int j = 0; j < N; j++)
	{
		pt[i].set(smaller_radius * cos((double)j * angle), 0, width / 2);
		i++;
	}

	for (int j = 0; j < N; j++)
	{
		pt[i].set(smaller_radius * cos((double)j * angle), 0, -width / 2);
		i++;
	}

	pt[i].set(larger_radius, 0, width / 2);
	i++;
	pt[i].set(larger_radius, 0, -width / 2);
	i++;
	pt[i].set(-larger_radius, 0, -width / 2);
	i++;
	pt[i].set(-larger_radius, 0, width / 2);
	i++;

	numFaces = M + N - 1 + N - 1 + N - 1 + N - 1 + N - 1 + 1 + 1 + 1 + 1 + 1 + 1;
	face = new Face[numFaces];
	int color = 0;
	i = 0;

	for (int j = 0; j < (M - 1); j++)
	{
		face[i].nVerts = 4;
		face[i].vert = new VertexID[face[i].nVerts];

		face[i].vert[0].vertIndex = j;
		face[i].vert[1].vertIndex = j + 1;
		face[i].vert[2].vertIndex = j + M + 1;
		face[i].vert[3].vertIndex = j + M;

		for (int k = 0; k < face[i].nVerts; k++)
			face[i].vert[k].colorIndex = color;
		color++;
		i++;
	}
	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];
	face[i].vert[0].vertIndex = (M - 1);
	face[i].vert[1].vertIndex = 0;
	face[i].vert[2].vertIndex = M;
	face[i].vert[3].vertIndex = 2 * M - 1;

	for (int k = 0; k < face[i].nVerts; k++)
		face[i].vert[k].colorIndex = color;
	color++;
	i++;

	for (int j = 0; j < (N - 1); j++)
	{
		face[i].nVerts = 4;
		face[i].vert = new VertexID[face[i].nVerts];
		face[i].vert[0].vertIndex = j + 2 * M;
		face[i].vert[1].vertIndex = j + 2 * M + 1;
		face[i].vert[2].vertIndex = j + 1;
		face[i].vert[3].vertIndex = j;
		for (int k = 0; k < face[i].nVerts; k++)
			face[i].vert[k].colorIndex = color;
		color++;
		i++;
	}

	for (int j = 0; j < (N - 1); j++)
	{
		face[i].nVerts = 4;
		face[i].vert = new VertexID[face[i].nVerts];
		face[i].vert[0].vertIndex = j + 2 * M + N;
		face[i].vert[1].vertIndex = j + 2 * M + N + 1;
		face[i].vert[2].vertIndex = j + 2 * M + 1;
		face[i].vert[3].vertIndex = j + 2 * M;

		for (int k = 0; k < face[i].nVerts; k++)
			face[i].vert[k].colorIndex = color;
		color++;
		i++;
	}

	for (int j = 0; j < (N - 1); j++)
	{
		face[i].nVerts = 4;
		face[i].vert = new VertexID[face[i].nVerts];
		face[i].vert[0].vertIndex = j + 2 * M + N + 1;
		face[i].vert[1].vertIndex = j + 2 * M + N;
		face[i].vert[2].vertIndex = j + M;
		face[i].vert[3].vertIndex = j + M + 1;

		for (int k = 0; k < face[i].nVerts; k++)
			face[i].vert[k].colorIndex = color;
		color++;
		i++;
	}

	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];
	face[i].vert[0].vertIndex = 0;
	face[i].vert[1].vertIndex = (M - 1);
	face[i].vert[2].vertIndex = 2 * M + 2 * N + 1;
	face[i].vert[3].vertIndex = 2 * M + 2 * N;

	for (int k = 0; k < face[i].nVerts; k++)
		face[i].vert[k].colorIndex = color;
	color++;
	i++;
	for (int j = 1; j < (N - 1); j++)
	{
		face[i].nVerts = 4;
		face[i].vert = new VertexID[face[i].nVerts];
		face[i].vert[0].vertIndex = M - j;
		face[i].vert[1].vertIndex = M - j - 1;
		face[i].vert[2].vertIndex = j + 2 * M + 2 * N + 1;
		face[i].vert[3].vertIndex = j + 2 * M + 2 * N;

		for (int k = 0; k < face[i].nVerts; k++)
			face[i].vert[k].colorIndex = color;
		color++;
		i++;
	}
	// Mat ben phai, nua duoi hinh tron
	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];
	face[i].vert[0].vertIndex = M;
	face[i].vert[1].vertIndex = 2 * M - 1;
	face[i].vert[2].vertIndex = 2 * M + 3 * N + 1;
	face[i].vert[3].vertIndex = 2 * M + 3 * N;

	for (int k = 0; k < face[i].nVerts; k++)
		face[i].vert[k].colorIndex = color;
	color++;
	i++;
	for (int j = 1; j < (N - 1); j++)
	{
		face[i].nVerts = 4;
		face[i].vert = new VertexID[face[i].nVerts];
		face[i].vert[0].vertIndex = 2 * M - j;
		face[i].vert[1].vertIndex = 2 * M - j - 1;
		face[i].vert[2].vertIndex = j + 2 * M + 3 * N + 1;
		face[i].vert[3].vertIndex = j + 2 * M + 3 * N;

		for (int k = 0; k < face[i].nVerts; k++)
			face[i].vert[k].colorIndex = color;
		color++;
		i++;
	}

	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];
	face[i].vert[0].vertIndex = 2 * M;
	face[i].vert[1].vertIndex = 0;
	face[i].vert[2].vertIndex = 2 * M + 2 * N;
	face[i].vert[3].vertIndex = 2 * M + 4 * N;

	for (int k = 0; k < face[i].nVerts; k++)
		face[i].vert[k].colorIndex = color;
	color++;
	i++;

	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];
	face[i].vert[0].vertIndex = M;
	face[i].vert[1].vertIndex = 2 * M + N;
	face[i].vert[2].vertIndex = 2 * M + 4 * N + 1;
	face[i].vert[3].vertIndex = 2 * M + 3 * N;

	for (int k = 0; k < face[i].nVerts; k++)
		face[i].vert[k].colorIndex = color;
	color++;
	i++;

	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];
	face[i].vert[0].vertIndex = 2 * M + 2 * N - 1;
	face[i].vert[1].vertIndex = M + (N - 1);
	face[i].vert[2].vertIndex = 2 * M + 4 * N - 1;
	face[i].vert[3].vertIndex = 2 * M + 4 * N + 2;

	for (int k = 0; k < face[i].nVerts; k++)
		face[i].vert[k].colorIndex = color;
	color++;
	i++;

	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];
	face[i].vert[0].vertIndex = 0 + (N - 1);
	face[i].vert[1].vertIndex = 2 * M + N - 1;
	face[i].vert[2].vertIndex = 2 * M + 4 * N + 3;
	face[i].vert[3].vertIndex = 2 * M + 3 * N - 1;

	for (int k = 0; k < face[i].nVerts; k++)
		face[i].vert[k].colorIndex = color;
	color++;
	i++;

	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];
	face[i].vert[0].vertIndex = 2 * M + N;
	face[i].vert[1].vertIndex = 2 * M;
	face[i].vert[2].vertIndex = 2 * M + 4 * N;
	face[i].vert[3].vertIndex = 2 * M + 4 * N + 1;

	for (int k = 0; k < face[i].nVerts; k++)
		face[i].vert[k].colorIndex = color;
	color++;
	i++;

	face[i].nVerts = 4;
	face[i].vert = new VertexID[face[i].nVerts];
	face[i].vert[0].vertIndex = 2 * M + N - 1;
	face[i].vert[1].vertIndex = 2 * M + 2 * N - 1;
	face[i].vert[2].vertIndex = 2 * M + 4 * N + 2;
	face[i].vert[3].vertIndex = 2 * M + 4 * N + 3;

	for (int k = 0; k < face[i].nVerts; k++)
		face[i].vert[k].colorIndex = color;
	color++;
	i++;
}
#pragma endregion

// Parameter
#pragma region
// The size of screen
int screenWidth = 600;
int screenHeight = 600;

// The size of base 1
float base1X = 4;
float base1Y = 0.2;
float base1Z = 1.6;

// The size of base 2
float base2X = 1;
float base2Y = 3.1;
float base2Z = 0.2;

// The size of base 3
float base3X = 4;
float base3Y = 1.5;
float base3Z = 0.2;

// The size of base of trestle
float baseTrestleX = 0.2;
float baseTrestleY = 0.8;
float baseTrestleZ = 0.2;

// The size of trestle
float trestle_width = 0.2;
float trestle_height = 0.5;
float trestle_larger_radius = 0.3;
float trestle_smaller_radius = 0.2;

// The size of scroll bar
float scrollbar_height = 6;
float scrollbar_radius = 0.2;

// The size of turntable
float turntable_height = 0.2;
float turntable_radius = 0.6;

// The size of crank
float crank_radius = 0.1;
float crank_length = 0.55;
float crank_height = 0.2;

// The size of latch 3
float latch3_height = 0.75;
float latch3_radius = 0.1;

// The size of U bar
float ubar_length_bigger = 0.6;
float ubar_length_smaller = 0.2;
float ubar_width_bigger = 1.2;
float ubar_width_smaller = 1.1;
float ubar_height = 0.15;

// The size of latch 1 and latch 2 cube
float latch12_cube_size = 0.1;

// The size of latch 1 cylinder
float latch1_cylinder_height = 0.25;
float latch1_cylinder_radius = 0.05;

// The size of latch 2 cylinder
float latch2_cylinder_height = 0.7;
float latch2_cylinder_radius = 0.05;

// The gouged semicircle size of connected bar 1
float connectedbar1_width_semicircle = 0.15;
float connectedbar1_height_semicircle = 0.51;
float connectedbar1_larger_radius_semicircle = 0.23;
float connectedbar1_smaller_radius_semicircle = 0;

// The cuboid size of connected bar 
float connectedbarX_cuboid = 0.15;
float connectedbarY_cuboid = 1.5;
float connectedbarZ_cuboid = 0.15;

// The gouged semicircle size of connected bar 2
float connectedbar2_width_semicircle = 0.15;
float connectedbar2_height_semicircle = 0.51;
float connectedbar2_larger_radius_semicircle = 0.23;
float connectedbar2_smaller_radius_semicircle = 0.1;

int view_state = 0;

// Mesh
Mesh base1;
Mesh base2;
Mesh base3;
Mesh base1Trestle;
Mesh base2Trestle;
Mesh trestle1;
Mesh trestle2;
Mesh scrollbar;
Mesh turntable;
Mesh crank;
Mesh latch3;
Mesh ubar;
Mesh latch1Cube;
Mesh latch2Cube;
Mesh latch1Cylinder;
Mesh latch2Cylinder;
Mesh connectedbarSemicircle1;
Mesh connectedbarSemicircle2;
Mesh connectedbarCuboid1;
Mesh connectedbarCuboid2;

double angle = 0;
bool bWireFrame = false;
bool bLight1 = true;

float camera_angle;
float camera_height;
float camera_dis;
float camera_X, camera_Y, camera_Z;
float lookAt_X, lookAt_Y, lookAt_Z;

bool bCamera = true;

#pragma endregion

void mySpecialKeyboard(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		camera_height += 0.5;
		break;
	case GLUT_KEY_DOWN:
		camera_height -= 0.5;
		if (camera_height < 0)
			camera_height = 0;
		break;
	case GLUT_KEY_LEFT:
		camera_angle += 5;
		break;
	case GLUT_KEY_RIGHT:
		camera_angle -= 5;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void myKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '1':
		crank.rotateY += 5;
		if (crank.rotateY >= 360)
			crank.rotateY -= 360;
		turntable.rotateY += 5;
		if (turntable.rotateY >= 360)
			turntable.rotateY -= 360;
		break;

	case '2':
		crank.rotateY -= 5;
		if (crank.rotateY < 0)
			crank.rotateY += 360;
		turntable.rotateY -= 5;
		if (turntable.rotateY < 0)
			turntable.rotateY += 360;
		break;

	case '3':
		latch2Cube.slideZ += 0.05;
		if (latch2Cube.slideZ > 0.45)
			latch2Cube.slideZ = 0.45;
		latch2Cylinder.slideZ += 0.05;
		if (latch2Cylinder.slideZ > 0.45)
			latch2Cylinder.slideZ = 0.45;
		break;

	case '4':
		latch2Cube.slideZ -= 0.05;
		if (latch2Cube.slideZ < 0.1)
			latch2Cube.slideZ = 0.1;
		latch2Cylinder.slideZ -= 0.05;
		if (latch2Cylinder.slideZ < 0.1)
			latch2Cylinder.slideZ = 0.1;
		break;

	case 'd':
	case 'D':
		bLight1 = !bLight1;
		break;

	case '+':
		camera_dis += 0.5;
		break;

	case '-':
		camera_dis -= 0.5;
		break;

	case 'v':
	case 'V':
		view_state = abs(view_state - 1);
		break;

	case 'w':
	case 'W':
		bWireFrame = !bWireFrame;
		break;

	default:
		break;
	}
	glutPostRedisplay();
}

#pragma region

void drawTruchetTile(float x, float y, float z)
{
	int sel = rand() % 4;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
	glColor4f(0.6040, 0.6353, 0.9686, 1.0);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-30, y, -30);
	glVertex3f(30, y, -30);
	glVertex3f(30, y, 30);
	glVertex3f(-30, y, 30);
	glEnd();

	glColor4f(0.0, 0.0, 0.0, 1.0);
	glNormal3f(0.0f, 1.0f, 0.0f);
	
	if (sel == 0)
	{
		for (float R = 0.55; R >= 0.46; R -= 0.01)
		{
			glBegin(GL_LINE_LOOP);
			for (float angle = 0.0; angle <= 90 * PI / 180; angle += PI / 180)
				glVertex3f(x + R * cos(angle), y, z + R * sin(angle));
			glVertex3f(x, y, z + R - 0.01);
			for (float angle = 90 * PI / 180; angle >= 0; angle -= PI / 180)
				glVertex3f(x + (R - 0.01) * cos(angle), y, z + (R - 0.01) * sin(angle));
			glVertex3f(x + R - 0.01, y, z);
			glEnd();
		}
		for (float R = 0.45; R <= 0.54; R += 0.01)
		{
			glBegin(GL_LINE_LOOP);
			for (float angle = 0.0; angle <= 90 * PI / 180; angle += PI / 180)
				glVertex3f(x + 1 - R * cos(angle), y, z + 1 - R * sin(angle));
			glVertex3f(x + 1, y, z + 1 - R - 0.01);
			for (float angle = 90 * PI / 180; angle >= 0; angle -= PI / 180)
				glVertex3f(x + 1 - (R + 0.01) * cos(angle), y, z + 1 - (R + 0.01) * sin(angle));
			glVertex3f(x + 1 - R - 0.01, y, z + 1);
			glEnd();
		}
	}
	else if (sel == 1)
	{
		for (float R = 0.55; R >= 0.46; R -= 0.01)
		{
			glBegin(GL_LINE_LOOP);
			for (float angle = 0.0; angle <= 90 * PI / 180; angle += PI / 180)
				glVertex3f(x + R * cos(angle), y, z + 1 - R * sin(angle));
			glVertex3f(x, y, z + 1 - R + 0.01);
			for (float angle = 90 * PI / 180; angle >= 0; angle -= PI / 180)
				glVertex3f(x + (R - 0.01) * cos(angle), y, z + 1 - (R - 0.01) * sin(angle));
			glVertex3f(x + R, y, z + 1);
			glEnd();
		}

		for (float R = 0.45; R <= 0.54; R += 0.01)
		{
			glBegin(GL_LINE_LOOP);
			for (float angle = 0.0; angle <= 90 * PI / 180; angle += PI / 180)
				glVertex3f(x + 1 - R * cos(angle), y, z + R * sin(angle));
			glVertex3f(x + 1, y, z + R + 0.01);
			for (float angle = 90 * PI / 180; angle >= 0; angle -= PI / 180)
				glVertex3f(x + 1 - (R + 0.01) * cos(angle), y, z + (R + 0.01) * sin(angle));
			glVertex3f(x + 1 - R, y, z);
			glEnd();
		}
	}
	else if (sel == 2)
	{
		glBegin(GL_POLYGON);
		glVertex3f(x + 0.45, y, z);
		glVertex3f(x + 0.55, y, z);
		glVertex3f(x + 0.55, y, z + 1);
		glVertex3f(x + 0.45, y, z + 1);
		glEnd();

		glBegin(GL_POLYGON);
		glVertex3f(x, y, z + 0.45);
		glVertex3f(x, y, z + 0.55);
		glVertex3f(x + 1, y, z + 0.55);
		glVertex3f(x + 1, y, z + 0.45);
		glEnd();
	}
	else if (sel == 3)
	{
		float big_radius = sqrtf(0.3 * 0.3 + 0.05 * 0.05);
		glBegin(GL_POLYGON);
		glVertex3f(x + 0.45, y, z + 0.2);
		glVertex3f(x + 0.45, y, z);
		glVertex3f(x + 0.55, y, z);
		glVertex3f(x + 0.55, y, z + 0.2);
		glEnd();

		glBegin(GL_POLYGON);
		glVertex3f(x + 0.8, y, z + 0.45);
		glVertex3f(x + 1, y, z + 0.45);
		glVertex3f(x + 1, y, z + 0.55);
		glVertex3f(x + 0.8, y, z + 0.55);
		glEnd();

		glBegin(GL_POLYGON);
		glVertex3f(x + 0.55, y, z + 0.8);
		glVertex3f(x + 0.55, y, z + 1);
		glVertex3f(x + 0.45, y, z + 1);
		glVertex3f(x + 0.45, y, z + 0.8);
		glEnd();

		glBegin(GL_POLYGON);
		glVertex3f(x + 0.2, y, z + 0.55);
		glVertex3f(x, y, z + 0.55);
		glVertex3f(x, y, z + 0.45);
		glVertex3f(x + 0.2, y, z + 0.45);
		glEnd();

		glBegin(GL_LINES);
		for (float angle = 0; angle <= 360 * PI / 180; angle += PI / 180)
		{
			glVertex3f(x + 0.5 + big_radius / 2 * cos(angle), y, z + 0.5 + big_radius / 2 * sin(angle));
			glVertex3f(x + 0.5 + big_radius * cos(angle), y, z + 0.5 + big_radius * sin(angle));
		}
		glEnd();
	}
}

void drawFloor()
{	
	float y = 0;
	glDisable(GL_LIGHTING);
	glColor3f(1.0f, 1.0f, 1.0f);
	int i = 0;
	for (float x = -10; x < 10; x += 1)
	{
		for (float z = -10; z < 10; z += 1)
			drawTruchetTile(x, y, z);
	}
	glEnable(GL_LIGHTING);
}

void drawAxis()
{
	glColor3f(0, 0, 1);
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0); //x
	glVertex3f(4, 0, 0);

	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0); //y
	glVertex3f(0, 4, 0);

	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0); //z
	glVertex3f(0, 0, 4);
	glEnd();
}

void drawBase1()
{
	glPushMatrix();

	glTranslated(0, 0.1, 0);
	glRotatef(0, 0, 1, 0);

	// He so cua vat the
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 0.2, 0.2, 1.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	base1.setupMaterial(ambient, diffuse, specular, shininess);

	if (bWireFrame)
		base1.DrawWireframe();
	else
		base1.Draw();

	glPopMatrix();
}

void drawBase2()
{
	glPushMatrix();

	glTranslated(0.05, 1.7, 0);
	glRotatef(1, 0, 0, 0);

	// He so cua vat the
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 0.2, 0.2, 1.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	base2.setupMaterial(ambient, diffuse, specular, shininess);

	if (bWireFrame)
		base2.DrawWireframe();
	else
		base2.Draw();

	glPopMatrix();
}

void drawBase3()
{
	glPushMatrix();

	glTranslated(0, 4, 0);

	// He so cua vat the
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 0.2, 0.2, 1.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	base3.setupMaterial(ambient, diffuse, specular, shininess);

	if (bWireFrame)
		base3.DrawWireframe();
	else
		base3.Draw();

	glPopMatrix();
}

void drawBase1Trestle()
{
	glPushMatrix();
	glTranslated(-1.7, 4, 0.1);

	// He so cua vat the
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	base1Trestle.setupMaterial(ambient, diffuse, specular, shininess);

	if (bWireFrame)
		base1Trestle.DrawWireframe();
	else
		base1Trestle.Draw();

	glPopMatrix();
}

void drawBase2Trestle()
{
	glPushMatrix();
	glTranslated(1.7, 4, 0.1);

	// He so cua vat the
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	base2Trestle.setupMaterial(ambient, diffuse, specular, shininess);

	if (bWireFrame)
		base2Trestle.DrawWireframe();
	else
		base2Trestle.Draw();

	glPopMatrix();
}

void drawTrestle1()
{
	glPushMatrix();

	glTranslated(-1.7, 4, 0.2);
	glRotatef(90, 0, 1, 0);
	glRotatef(90, 0, 0, 1);

	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	trestle1.setupMaterial(ambient, diffuse, specular, shininess);

	if (bWireFrame)
		trestle1.DrawWireframe();
	else
		trestle1.Draw();

	glPopMatrix();
}

void drawTrestle2()
{
	glPushMatrix();

	glTranslated(1.7, 4, 0.2);
	glRotatef(90, 0, 1, 0);
	glRotatef(90, 0, 0, 1);

	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	trestle2.setupMaterial(ambient, diffuse, specular, shininess);

	if (bWireFrame)
		trestle2.DrawWireframe();
	else
		trestle2.Draw();

	glPopMatrix();
}

void drawScrollbar()
{
	glPushMatrix();

	float rotateX_cuboid = latch2Cylinder.slideZ * sin(crank.rotateY * PI / 180);
	float rotateY_cuboid = latch2Cylinder.slideZ * cos(crank.rotateY * PI / 180);
	float distanceCrank_Latch3 = sqrtf(1.2 * 1.2 + 0.1 * 0.1);
	float angle_rotate = atan(rotateX_cuboid / (rotateY_cuboid + distanceCrank_Latch3));

	glTranslated(0, 4, 0.7);
	glRotatef(90, 0, 0, 1);

	glTranslated(0, -0.025 + 2.5 * sin(angle_rotate) + 0.025 * cos(angle_rotate), 0);

	GLfloat diffuse[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	scrollbar.setupMaterial(ambient, diffuse, specular, shininess);

	if (bWireFrame)
		scrollbar.DrawWireframe();
	else
		scrollbar.Draw();

	glPopMatrix();
}

void drawTurntable()
{
	glPushMatrix();

	glTranslated(0.05, 1.7, 0.2);
	glRotatef(90, 0, 1, 0);
	glRotatef(90, 0, 0, 1);
	glRotatef(turntable.rotateY, 0, 1, 0);

	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	turntable.setupMaterial(ambient, diffuse, specular, shininess);

	if (bWireFrame)
		turntable.DrawWireframe();
	else
		turntable.Draw();

	glPopMatrix();
}

void drawCrank()
{
	glPushMatrix();

	/*
	glTranslated(0.05, 2, 0.4);
	glRotatef(90, 0, 1, 0);
	glRotatef(90, 0, 0, 1);
	*/

	// crank.rotateY is the angle rotate!
	glTranslated(0.05 - 0.275 * sin(crank.rotateY * PI / 180), 1.8 + 0.275 * cos(crank.rotateY * PI / 180), 0.4);
	glRotatef(90, 0, 1, 0);
	glRotatef(90, 0, 0, 1);
	glRotatef(crank.rotateY, 0, 1, 0);

	GLfloat ambient[] = { 1.9, 0.6, 0.6, 1.0 };
	GLfloat diffuse[] = { 1.9, 0.6, 0.6, 1.0 };
	GLfloat specular[] = { 1.9, 0.6, 0.6, 1.0 };
	GLfloat shininess = 40.0;
	crank.setupMaterial(ambient, diffuse, specular, shininess);

	if (bWireFrame)
		crank.DrawWireframe();
	else
		crank.Draw();

	glPopMatrix();
}

void drawLatch3()
{
	glPushMatrix();

	glTranslated(0.05, 0.6, 0.3);
	glRotatef(90, 0, 1, 0);
	glRotatef(90, 0, 0, 1);

	GLfloat diffuse[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	latch3.setupMaterial(ambient, diffuse, specular, shininess);

	if (bWireFrame)
		latch3.DrawWireframe();
	else
		latch3.Draw();

	glPopMatrix();
}

void drawUbar()
{
	glPushMatrix();

	glTranslated(-0.27, 3.8, 0.6);
	glRotatef(90, 1, 0, 0);

	float rotateX_cuboid = latch2Cylinder.slideZ * sin(crank.rotateY * PI / 180);
	float rotateY_cuboid = latch2Cylinder.slideZ * cos(crank.rotateY * PI / 180);
	float distanceCrank_Latch3 = sqrtf(1.2 * 1.2 + 0.1 * 0.1);
	float angle_rotate = atan(rotateX_cuboid / (rotateY_cuboid + distanceCrank_Latch3));

	glTranslated(0.025 - 2.5 * sin(angle_rotate) - 0.025 * cos(angle_rotate), 0, 0);

	GLfloat diffuse[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	ubar.setupMaterial(ambient, diffuse, specular, shininess);

	if (bWireFrame)
		ubar.DrawWireframe();
	else
		ubar.Draw();

	glPopMatrix();
}

void drawLatch1Cube()
{
	glPushMatrix();

	float rotateX_cuboid = latch2Cylinder.slideZ * sin(crank.rotateY * PI / 180);
	float rotateY_cuboid = latch2Cylinder.slideZ * cos(crank.rotateY * PI / 180);
	float distanceCrank_Latch3 = sqrtf(1.2 * 1.2 + 0.1 * 0.1);
	float angle_rotate = atan(rotateX_cuboid / (rotateY_cuboid + distanceCrank_Latch3));

	glTranslated(0.05 - 2.5 * sin(angle_rotate) - 0.025 * cos(angle_rotate),
				 0.6 + 2.5 * cos(angle_rotate) - 0.025 * sin(angle_rotate), 0.7);
	glRotatef(90, 1, 0, 0);

	// He so cua vat the
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	latch1Cube.setupMaterial(ambient, diffuse, specular, shininess);

	if (bWireFrame)
		latch1Cube.DrawWireframe();
	else
		latch1Cube.Draw();

	glPopMatrix();
}

void drawLatch2Cube()
{
	glPushMatrix();

	glTranslated(0.06, 1.8, 0.6);
	glRotatef(90, 1, 0, 0);

	glTranslated(-latch2Cube.slideZ * sin(crank.rotateY * PI / 180), 0, 
				 -latch2Cube.slideZ * cos(crank.rotateY * PI / 180));

	float rotateX_cuboid = latch2Cylinder.slideZ * sin(crank.rotateY * PI / 180);
	float rotateY_cuboid = latch2Cylinder.slideZ * cos(crank.rotateY * PI / 180);
	float distanceCrank_Latch3 = sqrtf(1.2 * 1.2 + 0.1 * 0.1);
	float angle_rotate = atan(rotateX_cuboid / (rotateY_cuboid + distanceCrank_Latch3));

	glRotatef(angle_rotate * 180 / PI, 0, 1, 0);

	// He so cua vat the
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat diffuse[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	latch2Cube.setupMaterial(ambient, diffuse, specular, shininess);

	if (bWireFrame)
		latch2Cube.DrawWireframe();
	else
		latch2Cube.Draw();

	glPopMatrix();
}

void drawLatch1Cylinder()
{
	glPushMatrix();

	float rotateX_cuboid = latch2Cylinder.slideZ * sin(crank.rotateY * PI / 180);
	float rotateY_cuboid = latch2Cylinder.slideZ * cos(crank.rotateY * PI / 180);
	float distanceCrank_Latch3 = sqrtf(1.2 * 1.2 + 0.1 * 0.1);
	float angle_rotate = atan(rotateX_cuboid / (rotateY_cuboid + distanceCrank_Latch3));

	glTranslated(0.05 - 2.5 * sin(angle_rotate) - 0.015 * cos(angle_rotate),
				 0.6 + 2.5 * cos(angle_rotate) - 0.015 * sin(angle_rotate), 0.7);
	glRotatef(90, 1, 0, 0);

	GLfloat diffuse[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	latch1Cylinder.setupMaterial(ambient, diffuse, specular, shininess);

	if (bWireFrame)
		latch1Cylinder.DrawWireframe();
	else
		latch1Cylinder.Draw();

	glPopMatrix();
}

void drawLatch2Cylinder()
{
	glPushMatrix();

	glTranslated(0.05, 1.8, 0.4);
	glRotatef(90, 1, 0, 0);

	glTranslated(-latch2Cylinder.slideZ * sin(crank.rotateY * PI / 180), 0,
		         -latch2Cylinder.slideZ * cos(crank.rotateY * PI / 180));

	GLfloat diffuse[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	latch2Cylinder.setupMaterial(ambient, diffuse, specular, shininess);

	if (bWireFrame)
		latch2Cylinder.DrawWireframe();
	else
		latch2Cylinder.Draw();

	glPopMatrix();
}

void drawConnectedbarSemicircle1()
{
	glPushMatrix();

	//glTranslated(0.05, 2.6, 0.55);
	//glRotatef(90, 1, 0, 0);

	float rotateX_cuboid = latch2Cylinder.slideZ * sin(crank.rotateY * PI / 180);
	float rotateY_cuboid = latch2Cylinder.slideZ * cos(crank.rotateY * PI / 180);
	float distanceCrank_Latch3 = sqrtf(1.2 * 1.2 + 0.1 * 0.1);
	float angle_rotate = atan(rotateX_cuboid / (rotateY_cuboid + distanceCrank_Latch3));

	glTranslated(0.05 - 2 * sin(angle_rotate), 0.6 + 2 * cos(angle_rotate), 0.55);

	glRotatef(angle_rotate * 180 / PI, 0, 0, 1);

	//glTranslated(-2.01 * sin(angle_rotate), -2.01 + 2.01 * cos(angle_rotate), 0);

	GLfloat diffuse[] = { 0.0, 1.0, 0.0, 1.0 };
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	connectedbarSemicircle1.setupMaterial(ambient, diffuse, specular, shininess);

	if (bWireFrame)
		connectedbarSemicircle1.DrawWireframe();
	else
		connectedbarSemicircle1.Draw();

	glPopMatrix();
}

void drawConnectedbarCuboid1()
{
	glPushMatrix();

	float rotateX_cuboid = latch2Cylinder.slideZ * sin(crank.rotateY * PI / 180);
	float rotateY_cuboid = latch2Cylinder.slideZ * cos(crank.rotateY * PI / 180);
	float distanceCrank_Latch3 = sqrtf(1.2 * 1.2 + 0.1 * 0.1);
	float angle_rotate = atan(rotateX_cuboid / (rotateY_cuboid + distanceCrank_Latch3));

	glTranslated(0.05 - 1.26 * sin(angle_rotate) - 0.15 * cos(angle_rotate),
		0.6 + 1.26 * cos(angle_rotate) - 0.15 * sin(angle_rotate), 0.55);

	glRotatef(angle_rotate * 180 / PI, 0, 0, 1);

	GLfloat diffuse[] = { 0.0, 1.0, 0.0, 1.0 };
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	connectedbarCuboid1.setupMaterial(ambient, diffuse, specular, shininess);

	if (bWireFrame)
		connectedbarCuboid1.DrawWireframe();
	else
		connectedbarCuboid1.Draw();

	glPopMatrix();
}

void drawConnectedbarCuboid2()
{
	glPushMatrix();

	float rotateX_cuboid = latch2Cylinder.slideZ * sin(crank.rotateY * PI / 180);
	float rotateY_cuboid = latch2Cylinder.slideZ * cos(crank.rotateY * PI / 180);
	float distanceCrank_Latch3 = sqrtf(1.2 * 1.2 + 0.1 * 0.1);
	float angle_rotate = atan(rotateX_cuboid / (rotateY_cuboid + distanceCrank_Latch3));

	glTranslated(0.05 - 1.25 * sin(angle_rotate) + 0.15 * cos(angle_rotate),
				 0.6 + 1.25 * cos(angle_rotate) + 0.15 * sin(angle_rotate), 0.55);

	glRotatef(angle_rotate * 180 / PI, 0, 0, 1);


	GLfloat diffuse[] = { 0.0, 1.0, 0.0, 1.0 };
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	connectedbarCuboid2.setupMaterial(ambient, diffuse, specular, shininess);

	if (bWireFrame)
		connectedbarCuboid2.DrawWireframe();
	else
		connectedbarCuboid2.Draw();

	glPopMatrix();
}

void drawConnectedbarSemicircle2()
{
	glPushMatrix();

	float rotateX_cuboid = latch2Cylinder.slideZ * sin(crank.rotateY * PI / 180);
	float rotateY_cuboid = latch2Cylinder.slideZ * cos(crank.rotateY * PI / 180);
	float distanceCrank_Latch3 = sqrtf(1.2 * 1.2 + 0.1 * 0.1);
	float angle_rotate = atan(rotateX_cuboid / (rotateY_cuboid + distanceCrank_Latch3));

	glTranslated(0.05 - 0.51 * sin(angle_rotate), 0.59 + 0.51 * cos(angle_rotate), 0.55);
	glRotatef(180, 0, 0, 1);
	glRotatef(angle_rotate * 180 / PI, 0, 0, 1);

	GLfloat diffuse[] = { 0.0, 1.0, 0.0, 1.0 };
	GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat shininess = 40.0;
	connectedbarSemicircle2.setupMaterial(ambient, diffuse, specular, shininess);

	if (bWireFrame)
		connectedbarSemicircle2.DrawWireframe();
	else
		connectedbarSemicircle2.Draw();

	glPopMatrix();
}

#pragma endregion

void drawAll()
{
	drawBase1();
	drawBase2();
	drawBase3();
	drawBase1Trestle();
	drawBase2Trestle();
	drawTrestle1();
	drawTrestle2();
	drawScrollbar();
	drawTurntable();
	drawCrank();
	drawLatch3();
	drawUbar();
	drawLatch1Cube();
	drawLatch2Cube();
	drawLatch1Cylinder();
	drawLatch2Cylinder();
	drawConnectedbarSemicircle1();
	drawConnectedbarCuboid1();
	drawConnectedbarCuboid2();
	drawConnectedbarSemicircle2();
}

void myDisplay()
{	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	GLfloat light_position0[] = { 10.0, 10.0, 10.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);

	if (bLight1 == true)
	{
		glEnable(GL_LIGHT1);
		GLfloat diffuse1[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat specular1[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat ambient1[] = { 0.0, 0.0, 0.0, 1.0 };
		GLfloat position1[] = { -10.0, 10.0, -10.0, 0.0 };

		glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);
		glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1);
		glLightfv(GL_LIGHT1, GL_SPECULAR, specular1);
		glLightfv(GL_LIGHT1, GL_POSITION, position1);
	}
	else
		glDisable(GL_LIGHT1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	camera_X = camera_dis * sinf(camera_angle * PI / 180);
	camera_Y = camera_height;
	camera_Z = camera_dis * cosf(camera_angle * PI / 180);

	if (camera_dis == 0)
		gluLookAt(camera_X, camera_Y, camera_Z, lookAt_X, lookAt_Y, lookAt_Z, sinf(camera_angle * PI / 180), 0, cosf(camera_angle * PI / 180));
	else
		gluLookAt(camera_X, camera_Y, camera_Z, lookAt_X, lookAt_Y, lookAt_Z, 0, 1, 0);

	glViewport(0, 0, screenWidth, screenHeight);

	// Clear the stencil buffers
	glClearStencil(0);
	// Clear depth
	glClearDepth(1.0f);
	// Draw
	drawAll();
	/* Don't update color or depth. */
	glDisable(GL_DEPTH_TEST);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	/* Draw 1 into the stencil buffer. */
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	/* Re-enable update of color and depth. */
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glEnable(GL_DEPTH_TEST);

	/* Now, only render where stencil is set to 1. */
	glStencilFunc(GL_EQUAL, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	//Draw the cube, reflected vertically, at all PIxels where the stencil
	//buffer is 1
	glPushMatrix();
	glScalef(1, -1, 1);
	drawAll();
	glPopMatrix();
	glDisable(GL_STENCIL_TEST);

	// Blend the floor onto the screen
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	drawFloor();
	glDisable(GL_BLEND);

	const float ar = (float)screenWidth / (float)screenHeight;

	if (view_state == 0)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glFrustum(-ar, ar, -1.0, 1.0, 1.5, 50.0);
	}
	else if (view_state == 1)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glFrustum(-ar, ar, 0, 0.5, 0.5, 50.0);
	}

	glFlush();
	glutSwapBuffers();

}

void processTimer(int value)
{
	glutTimerFunc(10, processTimer, 0);
	glutPostRedisplay();
}

void myInit()
{
	camera_angle = -15;  // Góc quay camera xung quanh trục Oy
	camera_height = 5.5; // Chiều cao camera so với mặt phẳng xOz
	camera_dis = 6.5;	// Khoảng cách đến trục Oy

	lookAt_X = 0;
	lookAt_Y = 1;
	lookAt_Z = 0;

	float fHalfSize = 4;

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);

	glutTimerFunc(10, processTimer, 0);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
	glDepthFunc(GL_LEQUAL);
	//glEnable(GL_COLOR_MATERIAL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat lmodel_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	GLfloat light_ambient0[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_diffuse0[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular0[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular0);
}

void create(int N)
{
	int M = 2 * (N - 1);

	// Create first base
	base1.CreateCuboid(base1X, base1Y, base1Z);
	base1.CalculateFacesNorm();

	// Create second base
	base2.CreateCuboid(base2X, base2Y, base2Z);
	base2.CalculateFacesNorm();

	// Create third base
	base3.CreateCuboid(base3X, base3Y, base3Z);
	base3.CalculateFacesNorm();

	// Create first base of trestle (gia do)
	base1Trestle.CreateCuboid(baseTrestleX, baseTrestleY, baseTrestleZ);
	base1Trestle.CalculateFacesNorm();

	// Create second base of trestle (gia do)
	base2Trestle.CreateCuboid(baseTrestleX, baseTrestleY, baseTrestleZ);
	base2Trestle.CalculateFacesNorm();

	// Create first trestle (gia do)
	trestle1.gouged_semicircle(N, trestle_width, trestle_height, trestle_larger_radius, trestle_smaller_radius);
	trestle1.CalculateFacesNorm();

	// Create second trestle (gia do)
	trestle2.gouged_semicircle(N, trestle_width, trestle_height, trestle_larger_radius, trestle_smaller_radius);
	trestle2.CalculateFacesNorm();

	// Create second trestle (gia do)
	trestle2.gouged_semicircle(N, trestle_width, trestle_height, trestle_larger_radius, trestle_smaller_radius);
	trestle2.CalculateFacesNorm();

	// Create scroll bar (thanh truot)
	scrollbar.CreateCylinder(M, scrollbar_height, scrollbar_radius);
	scrollbar.CalculateFacesNorm();

	// Create turntable (ban quay)
	turntable.CreateCylinder(M, turntable_height, turntable_radius);
	turntable.CalculateFacesNorm();

	// Create crank (tay quay)
	crank.OvalHole(N, crank_radius, 0, crank_length, crank_height);
	crank.CalculateFacesNorm();

	// Create latch3 (chot 3)
	latch3.CreateCylinder(M, latch3_height, latch3_radius);
	latch3.CalculateFacesNorm();

	// Create U bar (thanh U)
	ubar.drawU(ubar_length_bigger, ubar_length_smaller, ubar_width_bigger, ubar_width_smaller, ubar_height);
	ubar.CalculateFacesNorm();

	// Create latch1 cube (Phan hinh lap phuong trong latch 1)
	latch1Cube.CreateCube(latch12_cube_size);
	latch1Cube.CalculateFacesNorm();

	// Create latch1 cylinder (Phan hinh tru trong latch 1)
	latch1Cylinder.CreateCylinder(M, latch1_cylinder_height, latch1_cylinder_radius);
	latch1Cylinder.CalculateFacesNorm();

	// Create latch2 cube (Phan hinh lap phuong trong latch 2)
	latch2Cube.CreateCube(latch12_cube_size);
	latch2Cube.CalculateFacesNorm();

	// Create latch2 cylinder (Phan hinh tru trong latch 2)
	latch2Cylinder.CreateCylinder(M, latch2_cylinder_height, latch2_cylinder_radius);
	latch2Cylinder.CalculateFacesNorm();

	// Create connected bar Semicircle 1 (Hinh ban nguyet thu 1 cua thanh lien ket)
	connectedbarSemicircle1.gouged_semicircle(N, connectedbar1_width_semicircle, connectedbar1_height_semicircle, 
						    connectedbar1_larger_radius_semicircle, connectedbar1_smaller_radius_semicircle);
	connectedbarSemicircle1.CalculateFacesNorm();

	// Create connected bar Cuboid 1 (Hinh hop chu nhat thu 1 cua thanh lien ket)
	connectedbarCuboid1.CreateCuboid(connectedbarX_cuboid, connectedbarY_cuboid, connectedbarZ_cuboid);
	connectedbarCuboid1.CalculateFacesNorm();

	// Create connected bar Cuboid 2 (Hinh hop chu nhat thu 2 cua thanh lien ket)
	connectedbarCuboid2.CreateCuboid(connectedbarX_cuboid, connectedbarY_cuboid, connectedbarZ_cuboid);
	connectedbarCuboid2.CalculateFacesNorm();

	// Create connected bar Semicircle 2 (Hinh ban nguyet thu 2 cua thanh lien ket)
	connectedbarSemicircle2.gouged_semicircle(N, connectedbar2_width_semicircle, connectedbar2_height_semicircle,
		connectedbar2_larger_radius_semicircle, connectedbar2_smaller_radius_semicircle);
	connectedbarSemicircle2.CalculateFacesNorm();

}

void print()
{
	cout << "1     : Xoay ban quay nguoc chieu kim dong ho" << endl
		 << "2         : Xoay ban quay cung chieu kim dong ho" << endl
		 << "3, 4  : Dieu chinh vi tri cua chot 2" << endl
		 << "W, w  : Chuyen doi qua lai giua che do khung day va to mau" << endl
		 << "V, v: Chuyen doi qua lai giua hai che do nhin khac nhau" << endl
		 << "D, d: Bat/tat nguon sang thu hai" << endl
		 << "+   : Tang khoang cach camera" << endl
		 << "-   : Giam khoang cach camera" << endl
		 << "up arrow  : Tang chieu cao camera" << endl
		 << "down arrow: Giam chieu cao camera" << endl
		 << "<-        : Quay camera theo chieu kim dong ho" << endl
		 << "->        : Quay camera nguoc chieu kim dong ho" << endl;
}

int main(int argc, char* argv[])
{
	glutInit(&argc, (char**)argv);									//initialize the tool kit
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);		//set the display mode
	glutInitWindowSize(screenWidth, screenHeight);					//set window size
	glutInitWindowPosition(100, 100);								// set window position on screen
	glutCreateWindow("Assignment - Vu Khanh Hung (1910232)");	// open the screen window

	print();

	int N = NUM / 2 + 1;		// So dinh tren mot nua hinh tron
	create(N);

	glutKeyboardFunc(myKeyboard);
	myInit();
	glutSpecialFunc(mySpecialKeyboard);
	glutDisplayFunc(myDisplay);
	glutMainLoop();
	return 0;
}