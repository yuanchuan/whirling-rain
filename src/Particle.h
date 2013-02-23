#ifndef PARTICLE_H_
#define PARTICLE_H_


#include<windows.h>
#include<cmath>

struct Particle{
	float x,y,vel;
	
	Particle():x(0.0),y(0.0),vel(1.0){}
	Particle(float xx,float yy,float vvel)
              :x(xx),y(yy),vel(vvel){}
	
	bool MoveTo(float dx,float dy);	
	bool RotateTo(float cx,float cy,float degree=0.13);
	bool ChasePoint(float tx,float ty,float dist);
};


bool Particle::MoveTo(float dx,float dy)
{
	//If reached 
	//----------------------
	if(fabs(x-dx)<=vel && fabs(y-dy)<=vel)
	   return true;
	   
	// Vertical   ==90  ==270
	//----------------------
	if(x==dx){
		if(y<dy)  y+=vel;
		else      y-=vel;
		return false;
	}
	
	//Horizontal  ==180 ==360
	//-----------------------
	if(y==dy){
		if(x<dx)  x+=vel;
		else      x-=vel;
		return false;
	}
	
	//Others    <90    >90
	//----------------------
	 float ky=fabs(dy-y);
	 float kx=fabs(dx-x);
	 float k=ky/kx;
	 
	 if(kx>=ky){
		if(x<dx)  x+=vel;
		else      x-=vel;
		if(y<dy)  y+=vel*k;
		else      y-=vel*k;
	 }
	 else if(kx<ky){
		if(x<dx)  x+=vel/k;
		else      x-=vel/k;
		if(y<dy)  y+=vel;
		else      y-=vel;
	}
	
	return false;
}


//--------------------------------------------
bool Particle::RotateTo(float cx,float cy,float degree)
{
    static float PI=3.1415926535;
	float dx=cx-x,
	      dy=cy-y,
	      theta=2*PI/degree,
	      newX=dx*cos(-theta)-dy*sin(-theta)+cx,
	      newY=dx*sin(-theta)+dy*cos(-theta)+cy;

	return MoveTo(newX,newY);
}

//---------------------------------------------
bool Particle::ChasePoint(float tx,float ty,float dist)
{
    float squareA=fabs(tx-x)*fabs(tx-x),
          squareB=fabs(ty-y)*fabs(ty-y),
          squareC=dist*dist;
          
    if(squareA+squareB>squareC)
        return MoveTo(tx,ty);
}

#endif

//------------------------------------------------------------------------------
