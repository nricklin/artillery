void Highlight(int left, int top, int right, int bottom)
{
	for (int y = top;y<bottom;y++)
      	line(left,y,right,y);
}
void Blacken(int left, int top, int right, int bottom)
{
   
   setwritemode(COPY_PUT);
	int oldcolor = getcolor();
   setcolor(BLACK);
   Highlight(left,top,right,bottom);
   setcolor(oldcolor);
   setwritemode(XOR_PUT);
}
void wallstuff()
{
if (walls == WRAPPING)
            	outtextxy(300,220,"Wrap Around");
            if (walls == RUBBER)
            	outtextxy(300,220,"Rubber");
            if (walls == SPRING)
            	outtextxy(300,220,"Spring Loaded");
            if (walls == NO_WALLS)
            	outtextxy(300,220,"None");

}
void Instructions()
{
}


void Options()
{
   cleardevice();
	setwritemode(XOR_PUT);
	setcolor(WHITE);
   settextstyle(0,0,2);
   outtextxy(100,50,"Options");
   settextstyle(0,0,0);
   outtextxy(100,180,"Rounds");
   outtextxy(100,190,"Sound");
   outtextxy(100,200,"Terrain Bumpiness");
   outtextxy(100,210,"Max Wind Speed");
   outtextxy(100,220,"Wall Type");
   outtextxy(100,230,"Done");
   int y = 220;
   //int x = 300;
   gprintf( 300, 180, "%d", NumRounds );
   if (SoundIsOn)
   	gprintf( 300, 190, "%s", "On" );
   else
   	gprintf( 300, 190, "%s", "Off" );
   gprintf( 300, 200, "%d", bumpiness );
   gprintf( 300, 210, "%d", windmax);
   wallstuff();
   setcolor(RED);
   Highlight(100,y,240,y+8);



   while (1)
   {
   	unsigned int command = _bios_keybrd(_KEYBRD_READ);
      if (command == 0x4800)      // up arrow
         	{
            Highlight(100,y,240,y+8);
            y -=10;
            Highlight(100,y,240,y+8);
            }
      if (command == 0x5000)      // down arrow
         	{
            Highlight(100,y,240,y+8);
            y +=10;
            Highlight(100,y,240,y+8);
            }
      if (command == 0x4D00 && bumpiness < 45 && y == 200)  // right arrow
      		{
            bumpiness++;
      		Blacken(300,200,350,208);
            setcolor(WHITE);
            gprintf( 300, 200, "%d", bumpiness );
            setcolor(RED);
            }
      if (command == 0x4B00 && bumpiness > 1 && y == 200)   // left arrow
      		{
            bumpiness--;
            Blacken(300,200,350,208);
            setcolor(WHITE);
            gprintf( 300, 200, "%d", bumpiness );
            setcolor(RED);
            }
      if (command == 0x4B00 && windmax > 1 && y == 210)   // left arrow
      		{
            windmax--;
            Blacken(300,210,350,218);
            setcolor(WHITE);
            gprintf( 300, 210, "%d", windmax );
            setcolor(RED);
            }
      if (command == 0x4D00 && windmax < 50 && y == 210)  // right arrow
      		{
            windmax++;
      		Blacken(300,210,350,218);
            setcolor(WHITE);
            gprintf( 300, 210, "%d", windmax );
            setcolor(RED);
            }
      if (command == 0x4B00 && y == 220)   // left arrow
      		{
            walls ++;
            if (walls > NO_WALLS)
            	walls = WRAPPING;
            setcolor(WHITE);
            Blacken(300,220,450,228);
            wallstuff();
            setcolor(RED);
            }
      if (command == 0x4D00 && y == 220)  // right arrow
      		{
            walls --;
            if (walls < 0)
            	walls = NO_WALLS;
            setcolor(WHITE);
            Blacken(300,220,450,228);
            wallstuff();
            setcolor(RED);
            }
      if ((command == 0x4B00 || command == 0x4D00) && y == 190)   // sound toggle
      		{
            SoundIsOn = !SoundIsOn;
            Blacken(300,190,450,198);
            setcolor(WHITE);
            if (SoundIsOn)
   				gprintf( 300, 190, "%s", "On" );
   			else
   				gprintf( 300, 190, "%s", "Off" );
            setcolor(RED);
            }
      if (command == 0x4B00 && NumRounds > 1 && y == 180)   // left arrow   rounds
      		{
            NumRounds--;
            Blacken(300,180,350,188);
            setcolor(WHITE);
            gprintf( 300, 180, "%d", NumRounds );
            setcolor(RED);
            }
      if (command == 0x4D00 && NumRounds < 100 && y == 180)  // right arrow   rounds
      		{
            NumRounds++;
      		Blacken(300,180,350,188);
            setcolor(WHITE);
            gprintf( 300, 180, "%d", NumRounds );
            setcolor(RED);
            }

      if (command == 0x1C0D && y == 230)      // enter on done
         	break;
   }



cleardevice();
}

int menu()
{
		int y = 200;
      bool YouPlayed = true;
		while (1)
		{

            if (YouPlayed)
            {
            	setwritemode(XOR_PUT);
            	setcolor(WHITE);
   				settextstyle(0,0,2);
   				outtextxy(100,50,"Welcome to Artillery");

      			settextstyle(0,0,0);
               outtextxy(100,70,"By Nathan Ricklin");
      			outtextxy(100,200,"Start Game");
               outtextxy(100,210,"Options");
     		 		outtextxy(100,220,"Quit");
    		  		setcolor(RED);
      			Highlight(100,y,200,y + 8);
            }
            YouPlayed = false;
      		unsigned int command = 0x352F;



				command = _bios_keybrd(_KEYBRD_READ);

         if (command == 0x4800)      // up arrow
         	{
            Highlight(100,y,200,y+8);
            y -=10;
            Highlight(100,y,200,y+8);
            }
         if (command == 0x5000)      // down arrow
         	{
            Highlight(100,y,200,y+8);
            y +=10;
            Highlight(100,y,200,y+8);
            }
         if (command == 0x1C0D && y == 200)      // enter on start game
         	{
            fort[0].score = 0;
            fort[1].score = 0;

               for (int r = 1; r <= NumRounds; r++)
   					{
                  cleardevice();
                  setcolor(WHITE);
                  gprintf(300,200,"%s %d","Round",r);
                  gprintf(200,300,"Player 1: %d    Player 2: %d",fort[0].score,fort[1].score);
                  getch();
                  Supervisor();   // main loop
                  }


               YouPlayed = true;
      		}
         if (command == 0x1C0D && y == 220)      // enter on quit
         	break;
         if (command == 0x1C0D && y == 210)      // enter on options
         	{
            Options();
            YouPlayed = true;
            }
      command = 0x352F;



      }
 	return(0);
}
