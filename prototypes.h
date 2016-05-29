int gprintf( int x, int y, char *fmt, ... );
int ClearStatus();
int Status();
void MakeCoords(double &x, double &y, signed int newangle, double dist);
void DrawFort(int fn);
void DrawBarrel(int fn);
int DropForts();
void initproj();
void initfortkeys();
void Collapse(int sn);
void Shoot(int sn, int fn);
int SwitchShots(int fn );
int Supervisor();
void DrawShot(int sn);
int Roll(int sn);
void init();
void Beep();
void Tick();
//void projectileclass::explode(int sn);
void DrawTerrain(int terraincolor);
void Highlight(int left, int top, int right, int bottom);
void Blacken(int left, int top, int right, int bottom);
void Instructions();
void Options();
int menu();

void UpdatePower(int fn);
void UpdateAngle(int fn);
void UpdateWeapon(int fn);

void Dig_Digger(int sn);


