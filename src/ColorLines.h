
#ifndef COLORlINES_H_
#define COLORlINES_H_



#include<windows.h>
#include<vector>
#include "Particle.h"

//Get width and height of the screen
const int WIN_WIDTH=GetSystemMetrics(SM_CXSCREEN);
const int WIN_HEIGHT=GetSystemMetrics(SM_CYSCREEN);

struct ColorLines{	
	int numOfLines,lineLength,numOfDrawingLines;
    float rotLineDegree;
	Particle hole,hole2;
    HPEN pen,pen2;
    DWORD dCounter;
	std::vector<Particle> dotHead,dotTail,
	                      dotHead2,dotTail2;
	//------------------------------------------
    void Init();
	void Render(HDC& hdc);
	void Update();
	void Clean();
};


//------------------------------------------------------------------------------
void ColorLines::Init()
{
	srand(GetTickCount());	
	dCounter=GetTickCount();

	numOfLines=1500;
	lineLength=20;
	numOfDrawingLines=0;
	//straight line
	rotLineDegree=1;
	
	//init two green pens
	pen=pen2=CreatePen(PS_SOLID,1,RGB(0,255,0));
	//set holes to center of the screen
	hole.x=hole2.x=WIN_WIDTH/2;
	hole.y=hole2.y=WIN_HEIGHT/2;
	hole.vel=hole2.vel=3;
	
    for(int i=0;i<numOfLines;i++){
        Particle dot(rand()%WIN_WIDTH,rand()%WIN_HEIGHT,1); 
	    dotHead.push_back(dot);
	    dotHead2.push_back(dot);	
	    dotTail.push_back(dot);
	    dotTail2.push_back(dot);   
	}
}


//------------------------------------------------------------------------------
void ColorLines::Render(HDC& hdc)
{	
    //Drawing lines from heads to tails    
	for(int i=0;i<numOfDrawingLines;++i){
		SelectObject(hdc,pen);				
		MoveToEx(hdc,(int)dotHead[i].x,(int)dotHead[i].y,0);
        LineTo(hdc,(int)dotTail[i].x,(int)dotTail[i].y);
           
        SelectObject(hdc,pen2);       
        MoveToEx(hdc,(int)dotHead2[i].x,(int)dotHead2[i].y,0);
        LineTo(hdc,(int)dotTail2[i].x,(int)dotTail2[i].y);
	}
}


//------------------------------------------------------------------------------
void ColorLines::Update()
{  
	if(numOfDrawingLines<numOfLines)  numOfDrawingLines++;

	if(GetTickCount()-dCounter>(4+rand()%4)*1000){
		dCounter=GetTickCount();
		hole.x=rand()%WIN_WIDTH;
		hole.y=rand()%WIN_HEIGHT;
		hole2.x=WIN_WIDTH-hole.x;
		hole2.y=WIN_HEIGHT-hole.y;
		
        if((rand()%50)<10)  rotLineDegree=1;
	    else                rotLineDegree=0.13;
		
		DeleteObject(pen);
		DeleteObject(pen2);	
		pen=CreatePen(PS_SOLID,1,RGB(255,rand()%255,0));
        pen2=CreatePen(PS_SOLID,1,RGB(rand()%255,255,0));

        //the value of vel has a chance to be minus so that the tails would go
        //in an oppsite direction away from heads
		int velo=7-rand()%10;
	  	while(velo==0)  velo=7-rand()%10;	  	
	  	//update velocity of each point with new value
	  	for(int i=0;i<numOfLines;++i)		
			dotHead[i].vel=dotTail[i].vel=dotHead2[i].vel=dotTail2[i].vel=velo;
	}
	
	//hole to hole rotating
	hole.RotateTo(hole2.x,hole2.y);   
	hole2.RotateTo(hole.x,hole.y);

	for(int i=0;i<numOfLines;++i){	
        //Heads rotate to holes	
        if(dotHead[i].RotateTo(hole.x,hole.y,rotLineDegree)){
			dotHead[i].x=dotTail[i].x=rand()%WIN_WIDTH;
            dotHead[i].y=dotTail[i].y=rand()%WIN_HEIGHT;
		}		
		if(dotHead2[i].RotateTo(hole2.x,hole2.y,rotLineDegree)){
			dotHead2[i].x=dotTail2[i].x=rand()%WIN_WIDTH;
            dotHead2[i].y=dotTail2[i].y=rand()%WIN_HEIGHT;
		}
		
		//Tails follow heads 
	    dotTail[i].ChasePoint(dotHead[i].x,dotHead[i].y,lineLength);				 
	    dotTail2[i].ChasePoint(dotHead2[i].x,dotHead2[i].y,lineLength);
	}
}

//------------------------------------------------------------------------------

void ColorLines::Clean()
{
	DeleteObject(pen);
	DeleteObject(pen2);
	
	dotHead.clear();
	dotHead2.clear();
	dotTail.clear();
	dotTail2.clear();
}

#endif
//------------------------------------------------------------------------------
