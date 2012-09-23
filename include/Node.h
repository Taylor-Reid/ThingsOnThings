/*
	Info
*/

class Node{
public:
	Node(int shapeNum, int x, int y, int d1, int d2,ci::Color8u col);

	Node* next_;
	
	//Node* prev_;
	//not sure if i'm going to use this or not...

	bool hasSub_;
	Node* subList_;

	bool isSquare_;
	bool isCircle_;
	//bool isTriangle_;
	int xPos_;
	int yPos_;
	int width_;
	int height_;
	int radius_;
	/*
	int p1;
	int p2;
	int p3;
	*/
	ci::Color8u shade_;

	//functions

	bool isInside(float x, float y);

}