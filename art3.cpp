#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <iostream.h>
#include <math.h>
#include <stdarg.h>
#include <bios.h>


#include "initgraph.h"           // graphics initialize
void DrawShot(int sn);
int Roll(int sn);

#include "objects.h"             // fort and projectile classes



int bumpiness = 21;   // good max is probably around 45.
#define gravity   .005    // projectiles fall this many pixels per tick per tick
#define wind      .01   // wind pushes projectiles this many pixels per tick per tick
#define resistance .998  // air resistance

double temp1, temp2;
//int maxy = getmaxy() - 20;


fortclass fort[2];

projectileclass shot[3];
int shotmax = 2;



int gprintf( int *xloc, int *yloc, char *fmt, ... )
{
  va_list  argptr;                      /* Argument list pointer        */
  char str[140];                        /* Buffer to build sting into   */
  int cnt;                              /* Result of SPRINTF for return */

  va_start( argptr, fmt );              /* Initialize va_ functions     */

  cnt = vsprintf( str, fmt, argptr );   /* prints string to buffer      */
  outtextxy( *xloc, *yloc, str );       /* Send string in graphics mode */
  //*yloc += textheight( "H" ) + 2;       /* Advance to next line         */

  va_end( argptr );                     /* Close va_ functions          */

  return( cnt );                        /* Return the conversion count  */

}
int ClearStatus()
{
   setcolor(BLACK);
	setwritemode(COPY_PUT);
   for (int y = getmaxy() - 18;y <= getmaxy() - 2;y++)
   	line(2,y,getmaxx()-2,y);
   setcolor(LIGHTGRAY);
   return(0);
}
int Status()
{
   setwritemode(COPY_PUT);
	setcolor(WHITE);
   rectangle(0,getmaxy()-20,getmaxx(),getmaxy());
   outtextxy( 2, getmaxy()-9, "Power :" );
   rectangle(65,getmaxy()-9,125,getmaxy() - 2);
   //int blah = 55;
   int x = 2;
   int y = getmaxy()-18;
   gprintf( &x, &y, "Angle : %d", fort[0].barrelangle );
   //outtextxy( 2, getmaxy()-19, test );
   return(0);
   
}
void MakeCoords(double &x, double &y, signed int newangle, double dist)  // creates new x and y motion coords from angle
{
   //const int pixels = 1 ;  // distance in pixels to move
   double Radians = double(newangle) * 0.01745;
	x =    (dist * cos(Radians));
   y =    (- dist * sin(Radians));
}

void DrawFort(int fn)//int x, int y, int color)
{
	for (int i=fort[fn].y-9;i<=fort[fn].y;i++)
   	{
      line(fort[fn].x-10,i,fort[fn].x+10,i);
      }
}
void DrawBarrel(int fn)//double x, double y, int angle)
{
	//setcolor(LIGHTGRAY);
   setlinestyle(0,0,3);
   //double temp1, temp2;

   MakeCoords(temp1,temp2,fort[fn].barrelangle,15);
   fort[fn].bx = temp1;
   fort[fn].by = temp2;
   line(fort[fn].x,fort[fn].y-10,fort[fn].x+temp1,fort[fn].y-10+temp2);

   setlinestyle(0,0,0);
}


int DropForts()         // drop and initialize forts
{
		

	fort[0].x = getmaxx()/7;
   fort[0].y = 14;
   fort[0].bx = 5;
   fort[0].by = 5;
   fort[0].color = BLUE;
   fort[0].barrelangle = 65;
   fort[0].power = 2;
   setwritemode(XOR_PUT);
   setcolor(fort[0].color);

   double dropvect = 0;


   DrawFort(0);            // draw

   while (getpixel(fort[0].x,fort[0].y + 2)==BLACK)
   	{

   	DrawFort(0);          // erase
   	dropvect += gravity;
      fort[0].y += dropvect;
      DrawFort(0);          // draw

      delay(5);
      }
   setwritemode(COPY_PUT);
   DrawFort(0);

   DrawBarrel(0);


   fort[1].x = 6 * getmaxx()/7;
   fort[1].y = 14;
   fort[1].color = RED;
   fort[1].barrelangle = 135;
   fort[1].power = 2;
   setwritemode(XOR_PUT);
   setcolor(fort[1].color);

   dropvect = 0;

   DrawFort(1);            // draw

   while (getpixel(fort[1].x,fort[1].y + 2)==BLACK)
   	{

   	DrawFort(1);          // erase
   	dropvect += gravity;
      fort[1].y += dropvect;
      DrawFort(1);          // draw

      delay(5);
      }
   setwritemode(COPY_PUT);
   DrawFort(1);
   DrawBarrel(1);
   return(0);
}

void initproj()
{
	shot[0].color = RED;
   shot[0].expradius = 6;        //regular shot
   shot[1].color = RED;
   shot[1].expradius = 30;       // dirt bomb
   shot[2].color = RED;
   shot[2].expradius = 8;       // roller
}
void initfortkeys()               // initialize keyboard controls
{
	fort[0].right = 0x2064;       //d
   fort[0].left = 0x1E61;        //a
   fort[0].powerup = 0x1265;     //e
   fort[0].powerdown = 0x1071;   //q
   fort[0].fire = 0x1177;        //w
   fort[0].swap = 0x1F73;        //s

	fort[1].right = 0x2064;       //d
   fort[1].left = 0x1E61;        //a
   fort[1].powerup = 0x1265;     //e
   fort[1].powerdown = 0x1071;   //q
   fort[1].fire = 0x1177;        //w
   fort[1].swap = 0x1F73;        //s
}


void Collapse(int sn)       // make dirt collapse back into hole.
{
   setwritemode(COPY_PUT);
	int cx = 0;
   int cy = 0;
   int top = 0;

for (cx = shot[sn].x - shot[sn].expradius;cx <= shot[sn].x + shot[sn].expradius;cx++)
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
   if (cy<getmaxy()-20)            // make sure it's not [at bottom if there was no overhang].
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

void Shoot(int sn, int fn)
{
   setcolor(RED);
	shot[sn].x = fort[fn].x + fort[fn].bx;
   shot[sn].y = fort[fn].y - 10 + fort[fn].by;
   shot[sn].angle = fort[fn].barrelangle;


   MakeCoords(temp1,temp2,shot[sn].angle,fort[fn].power);
   shot[sn].xmove = temp1;
   shot[sn].ymove = temp2;


   setwritemode(XOR_PUT);
   DrawShot(sn);
   unsigned int command = 0x352F;

   while (getpixel(shot[sn].x,shot[sn].y)!=GREEN && shot[sn].y < getmaxy()-20  && command!=fort[fn].fire)
   {

      if(_bios_keybrd(_KEYBRD_READY))
						command = _bios_keybrd(_KEYBRD_READ);
   	DrawShot(sn);
   	shot[sn].ymove += gravity;
      shot[sn].xmove *= resistance;
      shot[sn].x     += shot[sn].xmove;
      shot[sn].y     += shot[sn].ymove;
      DrawShot(sn);
      delay(5);
      
   }

   if (sn == 2 && command!=fort[fn].fire)  // roller
   	Roll(sn);
   shot[sn].explode(sn);
   Collapse(sn);

   setwritemode(COPY_PUT);
   setcolor(RED);
   DrawFort(1);
   setcolor(BLUE);
   DrawFort(0);
}


int SwitchShots(int & sn )
{
	sn++;
   if (sn > shotmax)
   	sn = 0;
   Beep();
   return(0);
}



#include "terrain.h"            // terrain drawer
int Supervisor()
{
   int fn = 0;    // fort number (current working fortress)
   int sn = 1;    // shot number



   setwritemode(COPY_PUT);
	unsigned int command=0x352F; // /
   while(command!=0x011B)  // ESC
   	{
      setwritemode(COPY_PUT);
      command=0x352F;
      //KeyboardStatus = *(unsigned short far *)MK_FP( 0x40, 0x17 );

   	if(_bios_keybrd(_KEYBRD_READY))
			command = _bios_keybrd(_KEYBRD_READ);



      if (command == fort[fn].left && fort[fn].barrelangle < 180)     // (a)
      	{
         	while (command != fort[fn].fire && command != fort[fn].right && fort[fn].barrelangle < 180)  // w d
         	{
            	if(_bios_keybrd(_KEYBRD_READY))
						command = _bios_keybrd(_KEYBRD_READ);
         		setcolor(BLACK);
         		DrawBarrel(fn);
         		fort[fn].barrelangle++;
         		setcolor(LIGHTGRAY);
         		DrawBarrel(fn);
               ClearStatus();
         		Status();
               delay(2);
        		}
            	if (command != fort[fn].fire)      // w
            command = 0x352F;
         }
      if (command == fort[fn].right && fort[fn].barrelangle > 0)    // d
      	{
         while (command != fort[fn].fire && command != fort[fn].left && fort[fn].barrelangle > 0) // w a
         	{
            	if(_bios_keybrd(_KEYBRD_READY))
						command = _bios_keybrd(_KEYBRD_READ);
         		setcolor(BLACK);
         		DrawBarrel(fn);
         		fort[fn].barrelangle--;
         		setcolor(LIGHTGRAY);
         		DrawBarrel(fn);
               ClearStatus();
         		Status();
               delay(2);
            }
            if (command != fort[fn].fire)  //w
            command = 0x352F;
         }
      if (command == fort[fn].fire)    //w
      	{
         Shoot(sn,fn);
         fn = !fn;
         }
      if (command == fort[fn].swap)
      	SwitchShots(sn);





      }
      return (0);
}


	


#include "menu.h"                // menu
int main()
{

	init();
   randomize();
   initproj();
   initfortkeys();

   menu();

   



   cleardevice();
   closegraph();

   //cout << fort.x << " " << fort.y << " " << fort.bx << " " << fort.by;
   //cout << shot.expradius;
   return (0);
}
void DrawShot(int sn)
{

   setlinestyle(0,0,3);
	line(shot[sn].x,shot[sn].y,shot[sn].x,shot[sn].y-2);
   if (shot[sn].y < 0 )                         // indicate where shot is
   	{
      setcolor(WHITE);
   	line(shot[sn].x,0,shot[sn].x,-2);
      setcolor(RED);
      }

   setlinestyle(0,0,0);

}
int Roll(int sn)
{
   enum Directions {left= -1, right= 1};
   Directions dir;
   DrawShot(sn);   // erase dot in ground
   // first make sure x,y is exactly on top of the ground
   shot[sn].x-=shot[sn].xmove;
   shot[sn].y-=shot[sn].ymove;
   while (getpixel(shot[sn].x,shot[sn].y) != BROWN)
   	shot[sn].y++;
   shot[sn].y--;

	// next determine direction to roll
   if (getpixel(shot[sn].x-1,shot[sn].y)!=BROWN && getpixel(shot[sn].x+1,shot[sn].y)!=BROWN)
   	{
      if (shot[sn].xmove > 0 )
      	dir = right;
      else
      	dir = left;
      }
   if (getpixel(shot[sn].x-1,shot[sn].y)==BROWN)
   	dir = right;
   if (getpixel(shot[sn].x+1,shot[sn].y)==BROWN)
   	dir = left;


   //putpixel(shot[sn].x,shot[sn].y,WHITE);
   setwritemode(XOR_PUT);
   while (1)  // Roll!
   {

      putpixel(shot[sn].x,shot[sn].y,WHITE);
      delay(10);
      putpixel(shot[sn].x,shot[sn].y,BLACK);
   	if (getpixel(shot[sn].x,shot[sn].y+1) == BLACK)
   		shot[sn].y++;
      else if (getpixel(shot[sn].x+dir,shot[sn].y) == BLACK)
      	shot[sn].x+=dir;
      else
      	break;
      delay(10);
      putpixel(shot[sn].x,shot[sn].y,BLACK);

   }
   setwritemode(COPY_PUT);
   return(0);


	

}