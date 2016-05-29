#define NO_OF_SHOTS 8   // the total number of weapons available

#include <graphics.h>
#include <stdlib.h>              // for random numbers
#include <stdio.h>               // for gprintf()
#include <conio.h>               // for getch()
#include <dos.h>                 // for sound
//#include <iostream.h>

// somehow the right side power bar draws around 290, -345 another bar
#include <math.h>                // for sin and cos and abs and sqrt
#include <stdarg.h>              // for gprintf()
#include <bios.h>                // for keyboard input

int NumRounds = 5;             // number of rounds to play

bool SoundIsOn = true;         // sound on or off
int fallen = 0;           // number of pixels a tank fell
int bumpiness = 21;       // good max is probably around 45.
enum walltypes {WRAPPING , RUBBER , SPRING, NO_WALLS };
walltypes walls = WRAPPING; 
#define gravity   .005    // projectiles fall this many pixels per tick per tick
double wind= 0;              // wind pushes projectiles this many pixels per tick per tick
int windmax = 50;              // max wind
#define resistance .998   // air resistance

#include "objects.h"             // fort and projectile classes

double temp1, temp2;
//int maxy = getmaxy() - 20;
fortclass fort[2];

projectileclass shot[NO_OF_SHOTS];
int shotmax = NO_OF_SHOTS-1;         // just one less than the number of shots

#include "prototypes.h"          // all function prototypes
#include "inits.h"               // initializations (graph, forts, shots)
#include "terrain.h"             // terrain drawer
#include "menu.h"                // menu






int gprintf( int x, int y, char *fmt, ... )
{
  va_list  argptr;                      /* Argument list pointer        */
  char str[140];                        /* Buffer to build sting into   */
  int cnt;                              /* Result of SPRINTF for return */

  va_start( argptr, fmt );              /* Initialize va_ functions     */

  cnt = vsprintf( str, fmt, argptr );   /* prints string to buffer      */
  outtextxy( x, y, str );       /* Send string in graphics mode */
  //*yloc += textheight( "H" ) + 2;       /* Advance to next line         */

  va_end( argptr );                     /* Close va_ functions          */

  return( cnt );                        /* Return the conversion count  */

}
int ClearStatus()
{
   setcolor(BLACK);
	setwritemode(COPY_PUT);
   for (int y = getmaxy() - 19;y <= getmaxy() - 1;y++)
   	line(1,y,getmaxx()-1,y);
   setcolor(LIGHTGRAY);
   return(0);
}
#define wsw 20  // wind status width
int Status()
{

   setwritemode(COPY_PUT);
	setcolor(WHITE);
   rectangle(0,getmaxy()-20,getmaxx(),getmaxy());            // big status box
   rectangle(getmaxx()/2-wsw,getmaxy()-20,getmaxx()/2+wsw,getmaxy());     // wind box
   //if (wind!=0)
   //{
   	rectangle(getmaxx()/2-wsw+8,getmaxy()-15,getmaxx()/2+wsw-8,getmaxy()-14);
      gprintf( getmaxx()/2-8, getmaxy()-8, "%d", abs(int(wind)) );
      if (wind>0)
      {
      	line(getmaxx()/2+wsw-8,getmaxy()-15,getmaxx()/2+wsw-13,getmaxy()-18);
         line(getmaxx()/2+wsw-8,getmaxy()-14,getmaxx()/2+wsw-13,getmaxy()-11);
      }
      if (wind<0)
      {
      	line(getmaxx()/2-wsw+8,getmaxy()-15,getmaxx()/2-wsw+13,getmaxy()-18);
         line(getmaxx()/2-wsw+8,getmaxy()-14,getmaxx()/2-wsw+13,getmaxy()-11);
      }

   //}
  
   
   for (int z = 0 ; z<=1;z++)

   {
   int p = z * (getmaxx()/2+wsw);
   UpdatePower(z);
   UpdateWeapon(z);
   UpdateAngle(z);


   outtextxy(144+p, getmaxy()-9, "Energy :");
   rectangle(215+p, getmaxy()-9, 267+p, getmaxy()-2); // energy box
   setcolor(LIGHTBLUE);
   	for (int j = 216+p; j <= fort[z].energy+ 216+p ; j++)
   		line(j,getmaxy()-7,j,getmaxy() - 4);
   setcolor(WHITE);
   
   outtextxy( 2+p, getmaxy()-9, "Power :" );
   rectangle(65+p,getmaxy()-9,135+p,getmaxy() - 2); // power box
  
   }
   return(0);
   
}
void UpdatePower(int fn)
{
   int oldcolor = getcolor();
   int p = fn * (getmaxx()/2+wsw);

   setcolor(BLACK);


		line(fort[fn].power+67+p,getmaxy()-7,fort[fn].power+67+p,getmaxy() - 4);
		setcolor(LIGHTBLUE);
   	for (int j = 66+p; j <= fort[fn].power+ 66+p ; j++)
   		line(j,getmaxy()-7,j,getmaxy() - 4);

   setcolor(oldcolor);
}
int MakeAngleAcute(int angle)    // return an angle between 0 and 90 for display purposes
{
	if (angle <= 90)
   	return(angle);
   else if (angle > 90 && angle <=180)
   	return(180 - angle);
   else return(666);
}
void UpdateAngle(int fn)
{
int oldcolor = getcolor();
int p = fn * (getmaxx()/2+wsw) ;
Blacken( 66+p,getmaxy()-18, 100+p, getmaxy()-10);
setcolor(WHITE);
gprintf( 2+p, getmaxy()-18, "Angle : %d", MakeAngleAcute(fort[fn].barrelangle) );
setcolor(oldcolor);
setwritemode(COPY_PUT);
}
void UpdateWeapon(int fn)
{
int oldcolor = getcolor();
int p = fn * (getmaxx()/2+wsw);
Blacken( 110+p,getmaxy()-18, 290+p, getmaxy()-10);
setcolor(WHITE);
gprintf( 110+p,getmaxy()-18, "Weapon : %s x%d", shot[fort[fn].weapon].name,fort[fn].number[fort[fn].weapon]);
setcolor(oldcolor);
setwritemode(COPY_PUT);
}




void MakeCoords(double &x, double &y, signed int newangle, double dist)  // creates new x and y motion coords from angle
{
   //const int pixels = 1 ;  // distance in pixels to move
   double Radians = double(newangle) * 0.01745;
	x =    (dist * cos(Radians));
   y =    (- dist * sin(Radians));
}
void EraseFort(int fn)
{
   setcolor(BLACK);
	for (int i=fort[fn].y-9;i<=fort[fn].y;i++)
   	{
      line(fort[fn].x-10,i,fort[fn].x+10,i);
      }
   DrawBarrel(fn);
}

void DrawFort(int fn)
{
	for (int i=fort[fn].y-9;i<=fort[fn].y;i++)
   	{
      line(fort[fn].x-10,i,fort[fn].x+10,i);
      }
   DrawBarrel(fn);
}
void DrawBarrel(int fn)
{
	int oldcolor = getcolor();
   if (oldcolor !=BLACK)
   	setcolor(LIGHTGRAY);
   setlinestyle(0,0,3);
   //double temp1, temp2;

   MakeCoords(temp1,temp2,fort[fn].barrelangle,15);
   fort[fn].bx = temp1;
   fort[fn].by = temp2;
   line(fort[fn].x,fort[fn].y-10,fort[fn].x+temp1,fort[fn].y-10+temp2);

   setlinestyle(0,0,0);
   setcolor(oldcolor);
}
/*class collapser
{
public:
int x;
int y;
};

void Collapse2(int sn)
{
int sx = shot[sn].x;
int sy = shot[sn].y;
int exp = shot[sn].expradius
collapser top[(sx+exp)-(sx-exp)];
if (getpixel(sy - 1)!=BROWN)

}
*/	



void Collapse(int sn, int left, int right)       // make dirt collapse back into hole.
{
   setwritemode(COPY_PUT);
	int cx = 0;
   int cy = 0;
   int top = 0;

   int ExpBottom = shot[sn].y + shot[sn].expradius;
	for (cx = left;cx <= right;cx++)
	{
   cy = 0;
   //while (getpixel(cx,cy)==BROWN)
   	//cy++;                           // now its on first black spot if there was a cavern.

   while (getpixel(cx,cy)!=BROWN)
   	cy++;
   top = cy;                     // now pointer is resting on first pixel of ground


   int grains = 0;
   while (getpixel(cx,cy)==BROWN)
   	{
      cy++;
      grains++;
      }                          // now it's on first pixel of overhang
if (cy <= ExpBottom+1)
{
   if (cy<getmaxy()-20)            // make sure it's not [at bottom if there was no overhang].
   {
      // this is where it actually collapses.  above was for positioning.

   	while (getpixel(cx,cy)!=BROWN && cy < getmaxy()-20)
   		cy++;                         // now its one above bottom of cavern
      cy--;
      
      setcolor(BROWN);
      line(cx,cy,cx,cy - grains + rand()%3 - 1);   // draw dirt at the bottom
      setcolor(BLACK);
      line(cx,cy-grains,cx,top);                   // erase dirt above
   }
	}
   	
   	
}  

	setwritemode(XOR_PUT);
}
//-------------------------------------------------------------------------
int Dist2Points(double x1, double y1, double x2, double y2)   // returns distance between 2 points
{
//cout << x1 <<" "<<x2<<" "<<y1<<" "<<y2<<" "<<sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2))<<":";
//getch();
return ( sqrt(  (x1-x2)*(x1-x2) +(y1-y2)*(y1-y2)  )  );
}
void InflictDamage(int sn)   // if something hits a fort inflict some damage to it.
{
   if (sn != 1)
   {
		for (int f = 0;f<=1;f++)
   	{
   		int dist1 = Dist2Points(fort[f].x-5,fort[f].y-5,shot[sn].x,shot[sn].y);
   		int dist2 = Dist2Points(fort[f].x+5,fort[f].y-5,shot[sn].x,shot[sn].y);
   		if (dist1<=shot[sn].expradius || dist2<=shot[sn].expradius)
   		{
     	 		fort[f].energy-=shot[sn].damage;
      		for (int i = 2000; i > 400; i = i-50)
      			{if (SoundIsOn){sound(i);delay(4);nosound();}}
      	}
   	}
   }
}
int Die()
{
	int somebodydied = false;
	for (int f = 0;f<=1;f++)
   {
   	if (fort[f].energy <= 0)
      {
         setcolor(RED);
         if (SoundIsOn)
      		sound(50);
         for (double r = 0;r<50;r+=.4)
         {
         	circle(fort[f].x,fort[f].y-5,r);
            delay(7);
         }
         setfillstyle(SOLID_FILL,BLACK);
         setcolor(BLACK);
         fillellipse(fort[f].x,fort[f].y-5,50,50);
         nosound();
         ClearStatus();
         Status();
         somebodydied = f;
         return(somebodydied);
      }
   }
   return 5;  // arbitrary number, anything but 1 or 0

}



   




void Shoot(int sn, int fn)
{
   int hops = 0;

   setcolor(RED);

   if (sn != 0)
   	fort[fn].number[sn]--;
	shot[sn].x = fort[fn].x + fort[fn].bx;
   shot[sn].y = fort[fn].y - 10 + fort[fn].by;
   shot[sn].angle = fort[fn].barrelangle;


   MakeCoords(temp1,temp2,shot[sn].angle,fort[fn].power/25);
   shot[sn].xmove = temp1;
   shot[sn].ymove = temp2;

   double randwind = wind + (rand()%9) - 5;    // to make the wind fluctuate a little


BigTest:

   setwritemode(XOR_PUT);
   DrawShot(sn);
   unsigned int command = 0x352F;
         //  fire key not pushed  &&   shot not at bottom      &&     shot not hit ground               &&      shot not hit something blue (fort)
   double bouncy = 0;   // for bounciness in rubber or spring walls
   int bounces = 0;     // number of bounces off walls
   while ( command!=fort[fn].fire && shot[sn].y < getmaxy()-20 && getpixel(shot[sn].x,shot[sn].y)!=GREEN && getpixel(shot[sn].x,shot[sn].y)!=MAGENTA  && bounces < 5)
   {

   	if (getpixel(shot[sn].x,shot[sn].y)==BLACK && shot[sn].y > 1) // hits red fort
      	break;

      if(_bios_keybrd(_KEYBRD_READY))
						command = _bios_keybrd(_KEYBRD_READ);
   	DrawShot(sn);
   	shot[sn].ymove += gravity;

      shot[sn].xmove *= resistance;
      shot[sn].xmove += randwind/20000; // wind 

      shot[sn].x     += shot[sn].xmove;
      shot[sn].y     += shot[sn].ymove;

      if (walls == NO_WALLS)
      {
      	if (shot[sn].x > getmaxx() || shot[sn].x < 0)
         	break;
      }
      if (walls == WRAPPING)
      {
      	if (shot[sn].x > getmaxx())
         	shot[sn].x = 0;
         if (shot[sn].x < 0 )
         	shot[sn].x = getmaxx();
      }


      if (walls == SPRING)
      	bouncy = 1.3;
      if (walls == RUBBER)
      	bouncy = 1;


      if (walls == SPRING || walls == RUBBER)
      {
      	if (shot[sn].x > getmaxx())
         	{
         	shot[sn].xmove = -shot[sn].xmove*bouncy;
            shot[sn].x = getmaxx();
            bounces++;
            }
         if (shot[sn].x < 0 )
         	{
         	shot[sn].xmove = -shot[sn].xmove*bouncy;
            shot[sn].x = 0;
            bounces++;
            }
         if (shot[sn].y < 0)
         	{
            shot[sn].ymove = -shot[sn].ymove*bouncy;
            shot[sn].y = 0;
            bounces++;
            }
         if (shot[sn].y > getmaxy()-20)
         	{
            shot[sn].ymove = -shot[sn].ymove*bouncy;
            shot[sn].y = getmaxy()-21;
            bounces++;
            }

      }

      DrawShot(sn);
      delay(5);

      
      	

      
   }

   if (shot[sn].y < 0)
   	DrawShot(sn);

   if ((sn == 3 || sn == 2 || sn == 6) && command!=fort[fn].fire)  // roller
   	Roll(sn);
   if ( (sn == 5 || sn == 6) && command != fort[fn].fire)
   	Dig_Digger(sn);

   if (sn != 5 || fort[fn].fire == command)  // so diggers don't explode again
   		shot[sn].explode(sn);
   InflictDamage(sn);                               // inflict possible damage to forts
   


   Collapse(sn,shot[sn].x - shot[sn].expradius,shot[sn].x + shot[sn].expradius);
   setwritemode(COPY_PUT);
   for (int b = 0; b<=1;b++)                        // make forts fall if they can
      {EraseFort(b);
      DropFort(b);
      fort[b].energy-=fallen/4;}


   if (fort[fn].number[fort[fn].weapon]<=0)         // if your out of ammo switch
   	SwitchShots(fn);
   ClearStatus();                                   // redraw status bar
   Status();   
   setwritemode(COPY_PUT);
   setcolor(RED);
   DrawFort(1);
   setcolor(BLUE);
   DrawFort(0);

   if (sn == 7 && hops < 2)
   {
   	shot[sn].ymove = - 1.5;//shot[sn].ymove;
      setcolor(RED);
      hops ++;
   	goto BigTest;
   }
}


int SwitchShots(int fn )
{
   do
	{
   	fort[fn].weapon++;
   	if (fort[fn].weapon > shotmax)
   		fort[fn].weapon = 0;
   } while(fort[fn].number[fort[fn].weapon]<=0);
   Beep();
   return(0);
}




int Supervisor()
{
   int whodied = 5;
   int fn = rand()%2;    // fort number (current working fortress)
   //int sn = 1;    // shot number

   wind = (rand()%(2 * windmax) - windmax);

   setwritemode(COPY_PUT);
   cleardevice();

   DrawTerrain(BROWN);
   initforts();
   for (int b = 0; b<=1;b++)
      DropFort(b);
   Status();



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

         		UpdateAngle(fn);
               delay(4);
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
               UpdateAngle(fn);
               delay(4);
            }
            if (command != fort[fn].fire)  //w
            command = 0x352F;
         }
      if (command == fort[fn].fire)    //w
      	{
         Shoot(fort[fn].weapon,fn);
         fn = !fn;
         }

      whodied = Die();


      	if (whodied == 1)
      		{
         	fort[0].score++;
         	getch();
            break;
            }
         if (whodied == 0)
      		{
         	fort[1].score++;
         	getch();
            break;
            }

      if (command == fort[fn].swap)
      	{
      	SwitchShots(fn);//fort[fn].weapon);
         UpdateWeapon(fn);
         }
      if (command == fort[fn].powerup && fort[fn].power <= 67)
      	{
         fort[fn].power++;
         UpdatePower(fn);
         }
      if (command == fort[fn].powerdown && fort[fn].power >= 0)
      	{
         fort[fn].power--;
         UpdatePower(fn);
         }





      }

       //test = getpixel(291,-345);
    //test2 = getpixel(282,-344);
    //test3 = getpixel(5,5);



      setwritemode(XOR_PUT);
      cleardevice();
      return (0);
}


	



int main()
{

	init();
   randomize();
   initproj();
   initfortkeys();

   menu();

   



   cleardevice();
   closegraph();

   //cout << shot[0].x << " " << shot[0].y<<endl;
   //cout << BLUE << " " << test << " " << test2 << " " <<test3;
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

int uptoggle = 0;
int ups = 0;


int Roll(int sn)
{
   double upmax;
   if (sn == 3)      // crawler
   	upmax = 10;
   if (sn == 2 || sn == 6)      // roller   and digger
   	upmax = 2;



   ups = 0;     // number of pixels it's rolled up


   enum Directions {left= -1, right= 1};
   Directions dir;
   DrawShot(sn);   // erase dot in ground
   // first make sure x,y is exactly on top of the ground
   shot[sn].x-=shot[sn].xmove;
   shot[sn].y-=shot[sn].ymove;

   int z = 0;
   while (1)
   {
   	z++;
      if (getpixel(shot[sn].x+z,shot[sn].y) == BROWN)
      	{
         dir = right;
         break;
         }
      if (getpixel(shot[sn].x-z,shot[sn].y) == BROWN)
      	{
         dir = left;
         break;
         }
   }

     
      
   while ((getpixel(shot[sn].x,shot[sn].y) != BROWN && shot[sn].y < getmaxy() - 20))
   	{
      shot[sn].y++;
      shot[sn].x += dir;
      }
   shot[sn].y--;
   shot[sn].x -= dir;  // x,y is on top of ground exactly

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
   unsigned int command;
   while (shot[sn].y < getmaxy() - 20)  // Roll!
   {

      if(_bios_keybrd(_KEYBRD_READY))
			command = _bios_keybrd(_KEYBRD_READ);
      putpixel(shot[sn].x,shot[sn].y,WHITE);
      delay(10);
      putpixel(shot[sn].x,shot[sn].y,BLACK);
   	if (uptoggle==0 && getpixel(shot[sn].x,shot[sn].y+1) == BLACK && shot[sn].y<getmaxy() - 20)      // move down if...
   		{
         shot[sn].y++;
         if (sn == 3)      // give crawler momentum
   			upmax += .5;
         }
      else if (getpixel(shot[sn].x+dir,shot[sn].y) == BLACK && shot[sn].x > 1 && shot[sn].x < getmaxx()-1)            // move sideways if...
      	{
         shot[sn].x+=dir;
         uptoggle = 0;
         }
      else if (ups <= upmax && getpixel(shot[sn].x,shot[sn].y-1) == BLACK && getpixel(shot[sn].x+dir,shot[sn].y) == BROWN)
      	{
         ups++;
         shot[sn].y--;
         uptoggle = 1;
         if (ups > 3)            // sound for crawling up
         	{
            if (SoundIsOn)
            {sound(60);
            delay(5);
            nosound();}
            }
         }

      else
      	break;
      delay(10);
      putpixel(shot[sn].x,shot[sn].y,BLACK);

   }

   return(0);
}
void Dig_Digger(int sn)
{

int left = 0, right = 0, bottom = shot[sn].y;
if (shot[sn].xmove > 0)
	left = shot[sn].x - shot[sn].expradius;
else
   right = shot[sn].x + shot[sn].expradius;


int count = 0;
	if (shot[sn].ymove > 1)
   	shot[sn].ymove = 1;
   if (shot[sn].ymove < -1)
   	shot[sn].ymove = -1;
   if (shot[sn].xmove > 1)
   	shot[sn].xmove = 1;
   if (shot[sn].xmove < -1)
   	shot[sn].xmove = -1;
while (count < 10 && shot[sn].y < getmaxy()-20)
	{
   shot[sn].explode(sn);
   if (sn == 6)
   	shot[sn].y +=5;
   else
   {
   	shot[sn].y += shot[sn].ymove*5;
   	shot[sn].x += shot[sn].xmove*5;
   }
   count ++;
   }

   if (left == 0)
		left = shot[sn].x - shot[sn].expradius;
	else
   	right = shot[sn].x + shot[sn].expradius;

   if (shot[sn].y + shot[sn].expradius < bottom)
   	shot[sn].y = bottom;
   Collapse(sn,left,right);


}
