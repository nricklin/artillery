void Beep()
{
   if (SoundIsOn)
	{sound(600);
   delay(40);
   nosound(); }
}
void Tick()
{
   if (SoundIsOn)
	{sound(2000);
   delay(3);
   nosound();}
}





class projectileclass
{
	public:
   int exptime;        // speed of explosion (delay time)
   int expfreq;        // sound frequency of explosion
   char *name;         // name of projectile eg. "missile"
   int damage;         // damage given by projectile
   double x;               // position
   double y;
   double xmove;           // xmove and ymove make up relative motion vector
   double ymove;
   int angle;           // angle of motion(not used yet)
   int expradius;       // explosion radius (make sure it encompasses whole object)
   int color;           // color of the point (x,y).  (for detecting if it hits stuff)
   							// NOTE: make sure you make (x,y) this color
   void explode(int sn);

};

void projectileclass::explode(int sn)
{
   

   

   int i;
	setcolor(RED);
   if (SoundIsOn)
   	sound(expfreq);
	for (i=1;i<=expradius;i++)
   	{
 
      circle(x,y,i);
      delay(exptime);
      }
   setcolor(BLACK);
   if (sn == 1 && y > 0)
   	setcolor(BROWN);
   nosound();
   if (sn == 1)
   	setfillstyle(SOLID_FILL,BROWN);


   else
   	setfillstyle(SOLID_FILL,BLACK);
   fillellipse(x,y,expradius,expradius);

   
}
//---------------------------------------------------------------------------
class fortclass
{
	public:
   double x,y;              // position  (x,y) should be bottom center of fort
   int barrelangle;      	 // angle of barrel of gun
   double bx, by;           // relative coord of tip of gun
   double power;            // power of shot
   int color;            	 // color of (x,y)
   int weapon;          	 // selected weapon
   int energy;          	 // life energy health left in percent/2
   int number[NO_OF_SHOTS];           // number of each weapon this fort has


   int score;              // score of each fort

   unsigned int right;          // keyboard configuration
   unsigned int left;
   unsigned int powerup;
   unsigned int powerdown;
   unsigned int fire;
   unsigned int swap;

};








