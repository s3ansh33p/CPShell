#include "calc.hpp"

extern void main2(); //in file main.cpp

//Global variables
#ifdef PC
	SDL_Window *win;
	SDL_Renderer *renderer;
	SDL_Texture *texture;
	int width;
	int height;
#else
	//uint16_t *vram; //this got moved to sdk/calc/calc.hpp
	uint8_t debugprintline = 0;
#endif


extern "C"
#ifdef PC
int  main(){
#else
void main(){
#endif
	//Initialisation
	#ifdef PC
		width  = 320;
		height = 528;
		SDL_Init(SDL_INIT_EVERYTHING);
		win = SDL_CreateWindow("CP3D", 100,100,width,height,SDL_WINDOW_SHOWN);
		renderer = SDL_CreateRenderer(win, -1, 0);
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderClear(renderer);
		texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,320,528);
	#else
		vram = LCD_GetVRAMAddress();
		LCD_GetSize(&width, &height);
		LCD_VRAMBackup(); //Stores the VRAM content
	#endif

	//The actual program
	main2();

	//Stopping everything
	#ifdef PC
		SDL_DestroyWindow(win);
		SDL_Quit();
	#else
		LCD_VRAMRestore(); //Restores the VRAM content
		LCD_Refresh();
	#endif
}

//println is printf for up to 4 arguments
void println(const char str[]                  ){println(str,0,0,0,0);}
void println(const char str[],int a            ){println(str,a,0,0,0);}
void println(const char str[],int a,int b      ){println(str,a,b,0,0);}
void println(const char str[],int a,int b,int c){println(str,a,b,c,0);}
void println(const char str[],int a,int b,int c,int d){
#ifdef PC
	printf(str,a,b,c,d);
	printf("\n");
#else
	Debug_Printf(0,debugprintline++,false,0,str,a,b,c,d);
	LCD_Refresh();
	if(debugprintline>42) debugprintline=0;
#endif
}

//Define LCD_Refresh for the pc (for the calc this is in debug.hpp)
#ifdef PC
void LCD_Refresh(){
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}
#endif

//delay (only on the computer for now)
void delay(uint32_t time){
	#ifdef PC
		SDL_Delay(time);
	#else
		(void)time; //Do nothing
	#endif	
}

//This is defined in sdk/calc/calc.cpp for the calc...
#ifdef PC
//Draw a line (bresanham line algorithm)
void line(int x1, int y1, int x2, int y2, uint16_t color){
	int8_t ix, iy;

	int dx = (x2>x1 ? (ix=1, x2-x1) : (ix=-1, x1-x2) );
	int dy = (y2>y1 ? (iy=1, y2-y1) : (iy=-1, y1-y2) );

	setPixel(x1,y1,color);
	if(dx>=dy){ //the derivative is less than 1 (not so steep)
		//y1 is the whole number of the y value
		//error is the fractional part (times dx to make it a whole number)
		// y = y1 + (error/dx)
		//if error/dx is greater than 0.5 (error is greater than dx/2) we add 1 to y1 and subtract dx from error (so error/dx is now around -0.5) 
		int error = 0;
		while (x1!=x2) {
			x1 += ix; //go one step in x direction
			error += dy;//add dy/dx to the y value.
			if (error>=(dx>>1)){ //If error is greater than dx/2 (error/dx is >=0.5)
				y1+=iy;
				error-=dx;
			}
			setPixel(x1,y1,color);
		}
	}else{ //the derivative is greater than 1 (very steep)
		int error = 0;
		while (y1!=y2) { //The same thing, just go up y and look at x
			y1 += iy; //go one step in y direction
			error += dx;//add dx/dy to the x value.
			if (error>=(dy>>1)){ //If error is greater than dx/2 (error/dx is >=0.5)
				x1+=ix;
				error-=dy;
			}
			setPixel(x1,y1,color);
		}
	}
}

void vline(int x, int y1, int y2, uint16_t color); //Defined further down.

//Draw a filled triangle.
void triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t colorFill, uint16_t colorLine){
//Filled triangles are a lot of vertical lines.
/*                                                               -
                        a   ___________----------P3              -
       P0 _________---------              ____---                -
          ---____               _____-----                       -
               b ----___  _-----   c                             -
                        P2                                       -
The triangle has three points P0, P1 and P2 and three lines a, b and c. We go from left to right, calculating the point on a and the point on b or c and then we draw a vertical line connecting these two.
*/
//Sort the points by x coordinate
	{int z;
	if(x0>x2){ z=x2; x2=x0; x0=z; z=y2; y2=y0; y0=z; }
	if(x1>x2){ z=x2; x2=x1; x1=z; z=y2; y2=y1; y1=z; }
	if(x0>x1){ z=x1; x1=x0; x0=z; z=y1; y1=y0; y0=z; }}

	int x = x0; //x is the variable that counts from left to right

	//Values for line a
	int ay = y0; //The point y for the current x on the line a
	int aiy; //The direction of line a
	int adx = (x2>x0 ? (       x2-x0) : (        x0-x2) );
	int ady = (y2>y0 ? (aiy=1, y2-y0) : (aiy=-1, y0-y2) );
	int aerr = 0; //The y value of a (fractional part). y is actually ay+(aerr/adx)

	//Values for line b
	int by = y0; //The point y for the current x on the line b
	int biy; //The direction of line b
	int bdx = (x1>x0 ? (       x1-x0) : (        x0-x1) );
	int bdy = (y1>y0 ? (biy=1, y1-y0) : (biy=-1, y0-y1) );
	int berr = 0;

	//Values for line c
	int cy = y1; //The point y for the current x on the line y (starting at P1)
	int ciy; //The direction of line c
	int cdx = (x2>x1 ? (       x2-x1) : (        x1-x2) );
	int cdy = (y2>y1 ? (ciy=1, y2-y1) : (ciy=-1, y1-y2) );
	int cerr = 0;

	//First draw area between a and b
	while (x<x1){
		x++;
		aerr+=ady;
		while(aerr>=adx >> 2){ //if aerr/adx >= 0.5
			aerr-=adx;
			ay+=aiy;
		}
		berr+=bdy;
		while(berr>=bdx >> 2){ //if berr/bdx >= 0.5
			berr-=bdx;
			by+=biy;
		}
		vline(x,ay,by,colorFill);
	}

	//Then draw area between a and c
	while (x<x2-1){ //we don't need x=x2
		x++;
		aerr+=ady;
		while(aerr>=adx >> 2){ //if aerr/adx >= 0.5
			aerr-=adx;
			ay+=aiy;
		}
		cerr+=cdy;
		while(cerr>=cdx >> 2){ //if berr/bdx >= 0.5
			cerr-=cdx;
			cy+=ciy;
		}
		vline(x,ay,cy,colorFill);
	}

	line(x0,y0,x1,y1,colorLine);
	line(x1,y1,x2,y2,colorLine);
	line(x2,y2,x0,y0,colorLine);
}

void vline(int x, int y1, int y2, uint16_t color){
	if (y1>y2) { int z=y2; y2=y1; y1=z;}
	for (int y=y1; y<=y2; y++)
		setPixel(x,y,color);
}

void fillScreen(uint16_t color){
	//#ifdef PC
		unsigned char pixels[4]; // { A, B, G, R }
		//Convert 565 colors to RGBA
		/*R*/ pixels[3] = (color >> 8) & 0b11111000;
		/*G*/ pixels[2] = (color >> 3) & 0b11111100;
		/*B*/ pixels[1] = (color << 3) & 0b11111000;
		/*A*/ pixels[0] = 0;

		//create an array with the whole screen filled with this color
		unsigned char screen[4*width*height] ;
		for(long i=0;i<(width*height);i++){
			screen[4*i + 0] = pixels[0];
			screen[4*i + 1] = pixels[1];
			screen[4*i + 2] = pixels[2];
			screen[4*i + 3] = pixels[3];
		}
		SDL_Rect rect;
		rect.x = 0; rect.y = 0; rect.w =width; rect.h = height;
		SDL_UpdateTexture(texture, &rect , (void*)screen, 4*width); //The last number defines the number of bytes per row. ( width * bytePerPixel )
	//#else
	//	const uint32_t size = width * height;
	//	for(uint32_t i = 0; i<size;i++)
	//		*((uint16_t*)( (uint32_t)vram + ( i*2 )  )) = color;
	//#endif
}

//for the pc getKey is written in c++, for the calculator this is written in asm in the file getKey.s
//#ifdef PC

//Add key to currently pressed keys (used in getKey on the pc)
inline void setKey(uint32_t *key1, uint32_t *key2, Keys1 key) {
	*key1 |= key; (void)key2; }
inline void setKey(uint32_t *key1, uint32_t *key2, Keys2 key) {
	*key2 |= key; (void)key1; }

//GetKey returns each bit of every key in 2 uint32_t variables.
//Emulate the calculator key matrix with sdl on the pc.
void getKey(uint32_t *key1, uint32_t *key2){
	*key1 = 0; *key2 = 0;
	SDL_PumpEvents();
	const uint8_t *state = SDL_GetKeyboardState(NULL);
	if (state[ SDL_SCANCODE_K			]) setKey(key1,key2, KEY_KEYBOARD	);
	if (state[ SDL_SCANCODE_LSHIFT		]) setKey(key1,key2, KEY_SHIFT		);
	if (state[ SDL_SCANCODE_RSHIFT		]) setKey(key1,key2, KEY_SHIFT		);
	if (state[ SDL_SCANCODE_BACKSPACE	]) setKey(key1,key2, KEY_BACKSPACE	);
	if (state[ SDL_SCANCODE_DELETE		]) setKey(key1,key2, KEY_CLEAR		);

	if (state[ SDL_SCANCODE_LEFT		]) setKey(key1,key2, KEY_LEFT		);
	if (state[ SDL_SCANCODE_A			]) setKey(key1,key2, KEY_LEFT		);
	if (state[ SDL_SCANCODE_RIGHT		]) setKey(key1,key2, KEY_RIGHT		);
	if (state[ SDL_SCANCODE_D			]) setKey(key1,key2, KEY_RIGHT		);
	if (state[ SDL_SCANCODE_UP			]) setKey(key1,key2, KEY_UP			);
	if (state[ SDL_SCANCODE_W			]) setKey(key1,key2, KEY_UP			);
	if (state[ SDL_SCANCODE_DOWN		]) setKey(key1,key2, KEY_DOWN		);
	if (state[ SDL_SCANCODE_S			]) setKey(key1,key2, KEY_DOWN		);

	if (state[ SDL_SCANCODE_0			]) setKey(key1,key2, KEY_0 			);
	if (state[ SDL_SCANCODE_KP_0		]) setKey(key1,key2, KEY_0 			);
	if (state[ SDL_SCANCODE_1			]) setKey(key1,key2, KEY_1 			);
	if (state[ SDL_SCANCODE_KP_1		]) setKey(key1,key2, KEY_1 			);
	if (state[ SDL_SCANCODE_2			]) setKey(key1,key2, KEY_2 			);
	if (state[ SDL_SCANCODE_KP_2		]) setKey(key1,key2, KEY_2 			);
	if (state[ SDL_SCANCODE_3			]) setKey(key1,key2, KEY_3 			);
	if (state[ SDL_SCANCODE_KP_3		]) setKey(key1,key2, KEY_3 			);
	if (state[ SDL_SCANCODE_4			]) setKey(key1,key2, KEY_4 			);
	if (state[ SDL_SCANCODE_KP_4		]) setKey(key1,key2, KEY_4 			);
	if (state[ SDL_SCANCODE_5			]) setKey(key1,key2, KEY_5 			);
	if (state[ SDL_SCANCODE_KP_5		]) setKey(key1,key2, KEY_5 			);
	if (state[ SDL_SCANCODE_6			]) setKey(key1,key2, KEY_6 			);
	if (state[ SDL_SCANCODE_KP_6		]) setKey(key1,key2, KEY_6 			);
	if (state[ SDL_SCANCODE_7			]) setKey(key1,key2, KEY_7 			);
	if (state[ SDL_SCANCODE_KP_7		]) setKey(key1,key2, KEY_7 			);
	if (state[ SDL_SCANCODE_8			]) setKey(key1,key2, KEY_8 			);
	if (state[ SDL_SCANCODE_KP_8		]) setKey(key1,key2, KEY_8 			);
	if (state[ SDL_SCANCODE_9			]) setKey(key1,key2, KEY_9 			);
	if (state[ SDL_SCANCODE_KP_9		]) setKey(key1,key2, KEY_9 			);
	if (state[ SDL_SCANCODE_PERIOD		]) setKey(key1,key2, KEY_DOT		);
	if (state[ SDL_SCANCODE_KP_DECIMAL	]) setKey(key1,key2, KEY_DOT		);

	if (state[ SDL_SCANCODE_E			]) setKey(key1,key2, KEY_EXP		);

	if (state[ SDL_SCANCODE_MINUS		]) setKey(key1,key2, KEY_NEGATIVE	); //The ß key on german keyboards
	if (state[ SDL_SCANCODE_COMMA		]) setKey(key1,key2, KEY_COMMA		);
	if (state[ SDL_SCANCODE_O			]) setKey(key1,key2, KEY_LEFT_BRACKET);//Key under the (
	if (state[ SDL_SCANCODE_LEFTBRACKET	]) setKey(key1,key2, KEY_LEFT_BRACKET);
	if (state[ SDL_SCANCODE_P			]) setKey(key1,key2, KEY_RIGHT_BRACKET);//Key under the )
	if (state[ SDL_SCANCODE_RIGHTBRACKET]) setKey(key1,key2, KEY_RIGHT_BRACKET);
	if (state[ SDL_SCANCODE_EQUALS		]) setKey(key1,key2, KEY_EQUALS		); //The ` ' key on german keyboards
	if (state[ SDL_SCANCODE_X			]) setKey(key1,key2, KEY_X			);
	if (state[ SDL_SCANCODE_Y			]) setKey(key1,key2, KEY_Y			);
	if (state[ SDL_SCANCODE_Z			]) setKey(key1,key2, KEY_Z			);
	if (state[ SDL_SCANCODE_GRAVE		]) setKey(key1,key2, KEY_POWER		); //The ^ key on german keyboards
	if (state[ SDL_SCANCODE_KP_DIVIDE	]) setKey(key1,key2, KEY_DIVIDE		);
	if (state[ SDL_SCANCODE_KP_MULTIPLY	]) setKey(key1,key2, KEY_MULTIPLY	);
	if (state[ SDL_SCANCODE_KP_MINUS	]) setKey(key1,key2, KEY_SUBTRACT	);
	if (state[ SDL_SCANCODE_KP_PLUS		]) setKey(key1,key2, KEY_ADD		);
	if (state[ SDL_SCANCODE_RETURN		]) setKey(key1,key2, KEY_EXE		);

	//if (state[ SDL_SCANCODE_			]) setKey(key1,key2, KEY_			);

	//Use Ctrl+C to close the program (Only when the program uses getKey)
	if (state[ SDL_SCANCODE_LCTRL		] && state[ SDL_SCANCODE_C ]) exit(0);
}
#endif
