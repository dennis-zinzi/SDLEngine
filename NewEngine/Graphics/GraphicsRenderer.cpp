#include "GraphicsRenderer.h"


#define FRAME_RATE 60
#define WINDOW_HEIGHT 720
#define WINDOW_WIDTH 1200


GraphicsRenderer::GraphicsRenderer(){
	SDL_Init(SDL_INIT_EVERYTHING);
	screen = SDL_CreateWindow("SDL Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
	renderer = SDL_CreateRenderer(screen, -1, 0);
	context = SDL_GL_CreateContext(screen);

	//Set OpenGL attribs
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	//OpenGL
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, WINDOW_WIDTH / WINDOW_HEIGHT, 1.0f, 500.0f);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_ALPHA_TEST);

	quadric = gluNewQuadric();
}

GraphicsRenderer::~GraphicsRenderer(){
	//gluDeleteQuadric(quadric);
	SDL_Quit();
}


void GraphicsRenderer::OpenGLUpdate(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//glBegin(GL_TRIANGLES);
	//{
	//	glVertex3f(1.0f, 1.0f, -5.0f);
	//	glVertex3f(-1.0f, -1.0f, -5.0f);
	//	glVertex3f(1.0f, -1.0f, -5.0f);
	//}
	//glEnd();
}


bool GraphicsRenderer::CheckStillRunning(){
	SDL_Event event;
	if(SDL_PollEvent(&event)){
		switch(event.type){
			case SDL_QUIT:
				return false;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym){
					case SDLK_ESCAPE:
						return false;
					default:
						return true;
				}
		}
	}
	return true;
}

void GraphicsRenderer::UpdateScene(float msec){
	//SDL_SetRenderDrawColor(GraphicsRenderer, 79, 79, 79, 255);
	//SDL_RenderClear(GraphicsRenderer);

	//SDL update render	
	//SDL_RenderPresent(GraphicsRenderer);

	SDL_GL_SwapWindow(screen);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	if(1000 / FRAME_RATE > SDL_GetTicks() - msec){
		SDL_Delay((Uint32)((1000 / FRAME_RATE) - (SDL_GetTicks() - msec)));
	}
}


void GraphicsRenderer::Draw2DRect(int x, int y, int height, int width,
	int red, int green, int blue, int alpha){

	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.h = height;
	rect.w = width;

	SDL_SetRenderDrawColor(renderer, red, green, blue, alpha);
	SDL_RenderFillRect(renderer, &rect);
}


void GraphicsRenderer::DrawTextLabel(){
	TTF_Init();

	TTF_Font *font = TTF_OpenFont("../Assets/Fonts/UnifrakturCook-Bold.ttf", 200);

	SDL_Color textColor = {66, 244, 217};
	SDL_Surface *text = TTF_RenderText_Solid(font, "Welcome to the game engine", textColor);
	SDL_Texture *message = SDL_CreateTextureFromSurface(renderer, text);

	SDL_Rect textRect;
	textRect.x = 200;
	textRect.y = 200;
	textRect.h = 250;
	textRect.w = 720;

	SDL_RenderCopy(renderer, message, nullptr, &textRect);

	//SDL_SetRenderDrawColor(GraphicsRenderer, 66, 244, 217, 255);

	//SDL_BlitSurface(text, nullptr, SDL_GetWindowSurface(screen), nullptr);

	TTF_CloseFont(font);
}

void GraphicsRenderer::RenderPlane(float x, float y, float z, 
	float halfWidth, float halfHeight, float halfDepth, float matrix[16],
	int red, int green, int blue, int alpha){
	glColor4f(red / 255.0f, green / 255.0f, blue / 255.0f, alpha / 255.0f);

	glPushMatrix();
	{
		glMultMatrixf(matrix);
		glBegin(GL_QUADS);
		{
			//TODO - Need to think about this
			glVertex3f(x - halfWidth, y + halfHeight, z + halfDepth);
			glVertex3f(x - halfWidth, y + halfHeight, z - halfDepth);
			glVertex3f(x + halfWidth, y - halfHeight, z - halfDepth);
			glVertex3f(x + halfWidth, y - halfHeight, z + halfDepth);
		}
		glEnd();
	}
	glPopMatrix();
}

void GraphicsRenderer::RenderSphere(float radius, float matrix[16], int red, int green, int blue, int alpha){
	glColor4f(red / 255.0f, green / 255.0f, blue / 255.0f, alpha / 255.0f);

	GLUquadricObj *quadr = gluNewQuadric();
	glPushMatrix();
	{
		glMultMatrixf(matrix); //translation + rotation
		gluSphere(quadr, radius, 100.0f, 100.0f);
	}
	glPopMatrix();

	gluDeleteQuadric(quadr);
}


void GraphicsRenderer::RenderCylinder(float radius, float height, float matrix[16],
	int red, int green, int blue, int alpha){
	glColor4f(red / 255.0f, green / 255.0f, blue / 255.0f, alpha / 255.0f);

	GLUquadricObj *quadr = gluNewQuadric();
	glPushMatrix();
	{
		glMultMatrixf(matrix); //translation + rotation
		//glRotatef(deg, !!x-axis, !!y-axis, !!z-axis); 
		glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
		gluCylinder(quadr, radius, radius, height*2.0, 100.0f, 100.0f);
	}
	glPopMatrix();

	gluDeleteQuadric(quadr);
}


void GraphicsRenderer::RenderCone(float radius, float height, float matrix[16],
	int red, int green, int blue, int alpha){
	glColor4f(red / 255.0f, green / 255.0f, blue / 255.0f, alpha / 255.0f);

	GLUquadricObj *quadr = gluNewQuadric();
	glPushMatrix();
	{
		glMultMatrixf(matrix); //translation + rotation
		//glRotatef(deg, !!x-axis, !!y-axis, !!z-axis); 
		//glTranslatef(0, 0.f, 0);
		glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
		gluCylinder(quadr, 0, radius, height, 100.0f, 100.0f);
	}
	glPopMatrix();

	gluDeleteQuadric(quadr);
}


void GraphicsRenderer::RenderBox(float x, float y, float z,
	float halfWidth, float halfHeight, float halfDepth, float matrix[16],
	int red, int green, int blue, int alpha){

	glColor4f(red / 255.0f, green / 255.0f, blue / 255.0f, alpha / 255.0f);

	glPushMatrix();
	{
		glMultMatrixf(matrix);
		glBegin(GL_QUADS);
		{
			glVertex3f(x - halfWidth, y + halfHeight, z - halfDepth);
			glVertex3f(x - halfWidth, y - halfHeight, z - halfDepth);
			glVertex3f(x - halfWidth, y - halfHeight, z + halfDepth);
			glVertex3f(x - halfWidth, y + halfHeight, z + halfDepth);
		}
		glEnd();
		glBegin(GL_QUADS);
		{
			glVertex3f(x + halfWidth, y + halfHeight, z - halfDepth);
			glVertex3f(x + halfWidth, y - halfHeight, z - halfDepth);
			glVertex3f(x + halfWidth, y - halfHeight, z + halfDepth);
			glVertex3f(x + halfWidth, y + halfHeight, z + halfDepth);
		}
		glEnd();
		glBegin(GL_QUADS);
		{
			glVertex3f(x - halfWidth, y + halfHeight, z + halfDepth);
			glVertex3f(x - halfWidth, y - halfHeight, z + halfDepth);
			glVertex3f(x + halfWidth, y - halfHeight, z + halfDepth);
			glVertex3f(x + halfWidth, y + halfHeight, z + halfDepth);
		}
		glEnd();
		glBegin(GL_QUADS);
		{
			glVertex3f(x - halfWidth, y + halfHeight, z - halfDepth);
			glVertex3f(x - halfWidth, y - halfHeight, z - halfDepth);
			glVertex3f(x + halfWidth, y - halfHeight, z - halfDepth);
			glVertex3f(x + halfWidth, y + halfHeight, z - halfDepth);
		}
		glEnd();
		glBegin(GL_QUADS);
		{
			glVertex3f(x - halfWidth, y + halfHeight, z - halfDepth);
			glVertex3f(x - halfWidth, y + halfHeight, z + halfDepth);
			glVertex3f(x + halfWidth, y + halfHeight, z + halfDepth);
			glVertex3f(x + halfWidth, y + halfHeight, z - halfDepth);
		}
		glEnd();
		glBegin(GL_QUADS);
		{
			glVertex3f(x - halfWidth, y - halfHeight, z - halfDepth);
			glVertex3f(x - halfWidth, y - halfHeight, z + halfDepth);
			glVertex3f(x + halfWidth, y - halfHeight, z + halfDepth);
			glVertex3f(x + halfWidth, y - halfHeight, z - halfDepth);
		}
		glEnd();
	}
	glPopMatrix();
}