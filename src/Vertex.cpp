#include "Vertex.h"

Vertex::Vertex()
{
	vdLoc.resize(3);
	vdLoc[0] = vdLoc[1] = vdLoc[2] = 0.0;
	iId = -1;
}

Vertex::~Vertex()
{

}

void Vertex::setPos(double dLocX, double dLocY, double dLocZ)
{
	vdLoc[0] = dLocX;
	vdLoc[1] = dLocY;
	vdLoc[2] = dLocZ;
}
