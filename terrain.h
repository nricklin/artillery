void DrawTerrain(int terraincolor)
{
	setcolor(terraincolor);
	int maxx = getmaxx();
   int maxy = getmaxy()- 20;
   
   int xstart = rand()%maxx;
   double xmove, ymove;
   double y= 2 * maxy / 4;
   double x= xstart;
   int angle=0;

   while ( x <= maxx +1)
   {
      putpixel(x,y,terraincolor);
      line(x,y,x,maxy);


      angle = angle + ((rand()%bumpiness) - bumpiness/2);
      MakeCoords(xmove,ymove,angle,1);
      x+=xmove;
      y+=ymove;
      if (y > maxy - 50)
         {
      	y--;
         angle=0;
         }
      if (y < maxy/5)
      	{
         y++;
         angle=0;
         }
      if (angle > 90)
      	angle-=5;
      if (angle < -90)
      	angle+=5;
   }

   y= 2 * maxy / 4;
   x= xstart;
   angle= 180;
   

   while ( x >= 0 )
   {
      putpixel(x,y,terraincolor);
      line(x,y,x,maxy);


      angle = angle + ((rand()%bumpiness) - bumpiness/2);
      MakeCoords(xmove,ymove,angle,1);
      x+=xmove;
      y+=ymove;
      if (y > maxy - 50)
         {
      	y--;
         angle=180;
         }
      if (y < maxy/5)
      	{
         y++;
         angle=180;
         }
      if (angle < 90)
      	angle+=5;
      if (angle > 270)
      	angle-=5;
   }


}


