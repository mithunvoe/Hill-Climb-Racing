#include <SDL2/SDL.h>

int main(int argc, char *argv[])
{
    // Load the image into an SDL surface.
    SDL_Surface *surface = SDL_LoadBMP("image.bmp");
    if (surface == nullptr)
    {
        printf("Failed to load image.\n");
        return 1;
    }

    // Calculate the offset of the pixel we want to retrieve.
    int x = 100;
    int y = 50;
    int offset = (y * surface->pitch) + (x * surface->format->BytesPerPixel);

    // Retrieve the RGB value of the pixel at the specified offset.
    Uint8 r, g, b;
    SDL_GetRGB(surface->pixels[offset], surface->format, &r, &g, &b);

    // Print the RGB values to the console.
    printf("RGB values: (%d, %d, %d)\n", r, g, b);

    // Free the SDL surface.
    SDL_FreeSurface(surface);

    return 0;
}
