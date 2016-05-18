#ifndef _VECTOR_H_
#define _VECTOR_H_
#include <vector>
#endif

class Vertex
{
public:
	Vertex();
	~Vertex();

	// Accessors
	void setPos(double dLocX, double dLocY, double dLocZ);
	std::vector<double> getPos(){return vdLoc;}
	void setId(int iNewId){iId = iNewId;}
	int getId(){return iId;}
	double getX(){return vdLoc[0];}
	double getY(){return vdLoc[1];}
	double getZ(){return vdLoc[2];}
	void setX(double dNewX){vdLoc[0] = dNewX;}
	void setY(double dNewY){vdLoc[1] = dNewY;}
	void setZ(double dNewZ){vdLoc[2] = dNewZ;}

protected:
	// Variables
	std::vector<double> vdLoc;
	int iId;
};