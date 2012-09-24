/*
	Info
*/

class Node{
public:
	Node(int shapeNum, float x, float y, float d1, float d2,ci::Color8u col,int idNum);

	Node* next_;
	
	//Node* prev_;
	//not sure if i'm going to use this or not...

	bool hasSub_;
	Node* subList_;

	bool isSquare_;
	bool isCircle_;
	float xPos_;
	float yPos_;
	float width_;
	float height_;
	float radius_;
	
	float vx_;
	float vy_;

	float dr_;
	
	ci::Color8u shade_;

	int id_;

	//functions

	bool isInside(float x, float y);

}