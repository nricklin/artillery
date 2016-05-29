void init()                    // initialize graphics
{ /* request auto detection */
   int gdriver = DETECT, gmode, errorcode;

   /* initialize graphics mode */
   initgraph(&gdriver, &gmode, "");

   /* read result of initialization */
   errorcode = graphresult();

   if (errorcode != grOk)  /* an error occurred */
   {
      printf("Graphics error: %s\n", grapherrormsg(errorcode));
      printf("Press any key to halt:");
      getch();
      exit(1);             /* return with error code */
   }




}
void initforts()
{
	fort[0].x = getmaxx()/7;
   fort[0].y = 14;
   fort[0].color = BLUE;
   fort[0].barrelangle = 65;
   fort[0].power = 10;
   fort[0].weapon = 0;
   fort[0].energy = 50;
   for (int i = 0;i<=NO_OF_SHOTS-1;i++)
   	fort[0].number[i] = 5;
   fort[0].number[4] = 1;

   fort[1].x = 6 * getmaxx()/7;
   fort[1].y = 14;
   fort[1].color = RED;
   fort[1].barrelangle = 115;
   fort[1].power = 10;
   fort[1].weapon = 0;
   fort[1].energy = 50;
   for (int i = 0;i<=NO_OF_SHOTS-1;i++)
   	fort[1].number[i] = 5;
   fort[1].number[4] = 1;
}

int DropFort(int fn)         // drop and initialize forts
{
		
   fallen = 0;
   int oldy =  fort[fn].y;
   setwritemode(XOR_PUT);
   setcolor(fort[fn].color);

   double dropvect = 1;


   DrawFort(fn);            // draw
   
   while (getpixel(fort[fn].x,fort[fn].y+1)==BLACK && fort[fn].y <= getmaxy()-20)
   	{

   	DrawFort(fn);          // erase
   	dropvect += gravity;
      fort[fn].y += dropvect;
      DrawFort(fn);          // draw

      delay(5);
      }
   setwritemode(COPY_PUT);
   DrawFort(fn);
   fallen =  int(fort[fn].y) - oldy;

   return(0);
}

void initproj()
{
	shot[0].color = RED;
   shot[0].expradius = 6;
   shot[0].name = "Bullet";
   shot[0].exptime = 12;
   shot[0].expfreq = 60;
   shot[0].damage = 5;

   shot[1].color = RED;
   shot[1].expradius = 30;
   shot[1].name = "Dirt Blob";
   shot[1].exptime = 5;
   shot[1].expfreq = 40;
   shot[1].damage = 0;
   
   shot[2].color = RED;
   shot[2].expradius = 8;
   shot[2].name = "Roller";
   shot[2].exptime = 12;
   shot[2].expfreq = 60;
   shot[2].damage = 10;

   shot[4].color = RED;
   shot[4].expradius = 50;
   shot[4].name = "Nuke";
   shot[4].exptime = 12;
   shot[4].expfreq = 30;
   shot[4].damage = 20;

   shot[3].color = RED;
   shot[3].expradius = 8;
   shot[3].name = "Crawler";
   shot[3].exptime = 12;
   shot[3].expfreq = 60;
   shot[3].damage = 5;

   shot[5].color = RED;
   shot[5].expradius = 4;
   shot[5].name = "Burrower";
   shot[5].exptime = 12;
   shot[5].expfreq = 100;
   shot[5].damage = 0;

   shot[6].color = RED;
   shot[6].expradius = 5;
   shot[6].name = "Digger";
   shot[6].exptime = 12;
   shot[6].expfreq = 50;
   shot[6].damage = 0;

   shot[7].color = RED;
   shot[7].expradius = 10;
   shot[7].name = "Hopper";
   shot[7].exptime = 12;
   shot[7].expfreq = 50;
   shot[7].damage = 7;
}
void initfortkeys()               // initialize keyboard controls
{
	fort[0].right = 0x2064;       //d
   fort[0].left = 0x1E61;        //a
   fort[0].powerup = 0x1265;     //e
   fort[0].powerdown = 0x1071;   //q
   fort[0].fire = 0x1177;        //w
   fort[0].swap = 0x1F73;        //s

	fort[1].right = 0x266C;       //l
   fort[1].left = 0x246A;        //j
   fort[1].powerup = 0x186F;     //o
   fort[1].powerdown = 0x1675;   //u
   fort[1].fire = 0x1769;        //i
   fort[1].swap = 0x256B;        //k
}
