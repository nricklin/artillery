#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <iostream.h>
#include <math.h>

#include "initgraph.h"           // graphics initialize
#include "objects.h"             // fort and projectile classes


#define bumpiness 21   // good max is probably around 45.
#define gravity   .005    // projectiles fall this many pixels per tick per tick
#define wind      .01   // wind pushes projectiles this many pixels per tick per tick
#define resistance .998  // air resistance

double temp1, temp2;


fortclass fort[2];

projectileclass shot[];




void MakeCoords(double &x, double &y, signed int newangle, double dist)  // creates new x and y motion coords from angle
{
   //const int pixels = 1 ;  // distance in pixels to move
   double Radians = double(newangle) * 0.01745;
	x =    (dist * cos(Radians));
   y =    (- dist * sin(Radians));
}

void DrawFort(int x, int y, int color)
{
	for (int i=y-9;i<=y;i++)
   	{
      line(x-10,i,x+10,i);
      }
}
void DrawBarrel(double x, double y, int angle)
{
	setcolor(LIGHTGRAY);
   setlinestyle(0,0,3);
   //double temp1, temp2;

   MakeCoords(temp1,temp2,angle,15);
   fort.bx = temp1;
   fort.by = temp2;
   line(x,y,x+temp1,y+temp2);

   setlinestyle(0,0,0);
}


int DropForts()         // drop and initialize forts
{
		

	fort.x = getmaxx()/7;
   fort.y = 14;
   fort.bx = 5;
   fort.by = 5;
   fort.color = BLUE;
   fort.barrelangle = 65;
   fort.power = 2;
   setwritemode(XOR_PUT);
   setcolor(fort.color);

   double dropvect = 0;


   DrawFort(fort.x,fort.y,fort.color);            // draw

   while (getpixel(fort.x,fort.y + 2)==BLACK)
   	{

   	DrawFort(fort.x,fort.y,fort.color);          // erase
   	dropvect += gravity;
      fort.y += dropvect;
      DrawFort(fort.x,fort.y,fort.color);          // draw

      delay(5);
      }
   setwritemode(COPY_PUT);
   DrawFort(fort.x,fort.y,fort.color);

   DrawBarrel(fort.x,fort.y-10,fort.barrelangle);


   fort2.x = 6 * getmaxx()/7;
   fort2.y = 14;
   fort2.color = RED;
   fort2.barrelangle = 135;
   fort2.power = 0;
   setwritemode(XOR_PUT);
   setcolor(fort2.color);

   dropvect = 0;

   DrawFort(fort2.x,fort2.y,fort2.color);            // draw

   while (getpixel(fort2.x,fort2.y + 2)==BLACK)
   	{

   	DrawFort(fort2.x,fort2.y,fort2.color);          // erase
   	dropvect += gravity;
      fort2.y += dropvect;
      DrawFort(fort2.x,fort2.y,fort2.color);          // draw

      delay(5);
      }
   setwritemode(COPY_PUT);
   DrawFort(fort2.x,fort2.y,fort2.color);
   return(0);
}

void initproj()
{
	shot.color = RED;
   shot.expradius = 6;        /////////////////////////////////////////////////////////
}

void DrawShot()
{

   setlinestyle(0,0,3);
	line(shot.x,shot.y,shot.x,shot.y-2);
   if (shot.y < 0 )                         // indicate where shot is
   	{
      setcolor(WHITE);
   	line(shot.x,0,shot.x,-2);
      setcolor(RED);
      }

   setlinestyle(0,0,0);

}
void Collapse()       // make dirt collapse back into hole.
{
   setwritemode(COPY_PUT);
	int cx = 0;
   int cy = 0;
   int top = 0;

for (cx = shot.x - shot.expradius;cx <= shot.x + shot.expradius;cx++)
{
   cy = 0;
   while (getpixel(cx,cy)!=BROWN)
   	cy++;
   top = cy;                     // now pointer is resting on first pixel of ground
   int grains = 0;
   while (getpixel(cx,cy)==BROWN)
   	{
      cy++;
      grains++;
      }                          // now it's on first pixel of overhang
   if (cy<getmaxy())            // make sure it's not [at bottom if there was no overhang].
   {
      

   	while (getpixel(cx,cy)!=BROWN)
   		cy++;                         // now its one above bottom of cavern
      cy--;
      
      setcolor(BROWN);
      line(cx,cy,cx,cy - grains + rand()%3 - 1);
      setcolor(BLACK);
      line(cx,cy-grains,cx,top);
   }
}
   	
   	
  

setwritemode(XOR_PUT);
}

void Shoot()
{
   setcolor(RED);
	shot.x = fort.x + fort.bx;
   shot.y = fort.y - 10 + fort.by;
   shot.angle = fort.barrelangle;


   MakeCoords(temp1,temp2,shot.angle,fort.power);
   shot.xmove = temp1;
   shot.ymove = temp2;


   setwritemode(XOR_PUT);
   DrawShot();
   int command = 'f';

   while (getpixel(shot.x,shot.y)!=GREEN && shot.y < getmaxy()  && command!='s')
   {

      if( kbhit() )
				command=getch();
   	DrawShot();
   	shot.ymove += gravity;
      shot.xmove *= resistance;
      shot.x += shot.xmove;
      shot.y += shot.ymove;
      DrawShot();
      delay(5);
      
   }

   shot.explode();
   Collapse();
}
   	


#include "terrain.h"            // terrain drawer
int Supervisor()
{
   setwritemode(XOR_PUT);
	int command='f';
   while(command!='q')
   	{
      command='f';
      //KeyboardStatus = *(unsigned short far *)MK_FP( 0x40, 0x17 );

   	if( kbhit() )
				command=getch();

      if (command == 'a' && fort.barrelangle < 180)
      	{
         DrawBarrel(fort.x,fort.y-10,fort.barrelangle);
         fort.barrelangle++;
         DrawBarrel(fort.x,fort.y-10,fort.barrelangle);
         }
      if (command == 'z' && fort.barrelangle > 0)
      	{
         DrawBarrel(fort.x,fort.y-10,fort.barrelangle);
         fort.barrelangle--;
         DrawBarrel(fort.x,fort.y-10,fort.barrelangle);
         }
      if (command == 's')
      	Shoot();


      }
      return (0);
}





int main()
{

	init();
   randomize();

   DrawTerrain(BROWN);
getch();

   DropForts();

   initproj();

	Supervisor();   // main loop







   cleardevice();
   closegraph();

   //cout << fort.x << " " << fort.y << " " << fort.bx << " " << fort.by;
   //cout << shot.expradius;
   return (0);
}