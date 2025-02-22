#include "Texture.h"

SDL_Renderer* Texture::renderer = nullptr;

Texture::Texture()
{
	texture = nullptr;
	ancho = 0;
	alto = 0;
}

Texture::~Texture()
{
	free();
}

bool Texture::loadFromImage(string path, Uint8 r, Uint8 g, Uint8 b)
{
	//Liberar textura anterior
	free();

	//Comprobar si el renderizador est� inicializado
	if (Texture::renderer == nullptr) {
		printf("Render no inicializado %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		return false;
	}

	//Cargar imagen a la superficie
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == nullptr) {
		printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		return false;
	}

	//Establecer colores
	SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, r, g, b));

	//Crea textura a partir de la superficie
	texture = SDL_CreateTextureFromSurface(Texture::renderer, loadedSurface);
	if (texture == nullptr) {
		printf("Unable to create texture from surface %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		return false;
	}

	//Establecer alto y ancho de la textura
	ancho = loadedSurface->w;
	alto = loadedSurface->h;

	//Liberar la superficie
	SDL_FreeSurface(loadedSurface);

	return true;
}

bool Texture::loadFromRenderedText(TTF_Font* font, string text, SDL_Color textColor)
{
	// Free the previous texture
	free();

	// Return if the renderer was not set
	if (Texture::renderer == nullptr)
		return false;

	// Render the text using SDL_ttf library
	SDL_Surface* loadedSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
	if (loadedSurface == nullptr) {
		printf("Unable to render text! SDL_ttf Error: %s\n", TTF_GetError());
		return false;
	}

	// Create a texture from generated surface
	texture = SDL_CreateTextureFromSurface(Texture::renderer, loadedSurface);
	if (texture == nullptr) {
		printf("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	// Set width and height of the texture
	ancho = loadedSurface->w;
	alto = loadedSurface->h;

	// Free the surface
	SDL_FreeSurface(loadedSurface);

	return true;
}

void Texture::render(SDL_Rect* renderQuad, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip renderFlip)
{
	//Comprobar si el renderizador est� inicializado
	if (Texture::renderer == nullptr)
		return;

	//Renderizar la imagen en pantalla
	SDL_RenderCopyEx(Texture::renderer, texture, clip, renderQuad, angle, center, renderFlip);
}

void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	SDL_SetTextureColorMod(texture, red, green, blue);
}

void Texture::setBlendMode(SDL_BlendMode blendMode)
{
	SDL_SetTextureBlendMode(texture, blendMode);
}

void Texture::setAlpha(Uint8 alpha)
{
	SDL_SetTextureAlphaMod(texture, alpha);
}

void Texture::free()
{
	if (texture != nullptr) {
		// Free the texture and set its pointer to NULL
		SDL_DestroyTexture(texture);
		texture = nullptr;

		ancho = 0;
		alto = 0;
	}
}