#include "SetContext.h"
#include "SetContext.cpp"
#include "Toto2DEngine.h"
#include "Toto2DEngine.cpp"
#include "stb_image_aug.h"
#include "stb_image_aug.c"

int main(int, char**)
{
    // set the openGL context:
    SetContext setContext;
    setContext.setContext();

    // retrieve the screen size
    uint32_t screenWidth = setContext.getScreenWidth();
    uint32_t screenHeight = setContext.getScreenHeight();

    // init Toto2DEngine with the relevant infos
    Toto2DEngine toto2d;
    toto2d.init(screenWidth, screenHeight, setContext.getDisplay(), setContext.getSurface());
    
    // upload atlas
    int width, height, channels;
    void* pixels = stbi_load("/opt/vc/src/hello_pi/hello_toto/atlas.png", &width, &height, &channels, STBI_rgb_alpha );
    toto2d.uploadAtlas(0, pixels, width, height);
    
    // draw a star tiled pattern
    int idUV;
    toto2d.getRepeatTileBatcher().addUV(10, 190, 240, 230, idUV);
    toto2d.getRepeatTileBatcher().addTiles(idUV, 0.0f, 0.0f, screenWidth, screenHeight, 0.0f, 0.0f);
    
    // draw hello world sprite at center of screen with slight rotation
    glm::mat3 matrixTransform;
    float t2x, t2y, rot, sx, sy, t1x, t1y;
    t1x = -165.0f; // pivot x
    t1y = -90.0f; // pivot y
    sx = 1.0f; // scale x
    sy = 1.0f; // scale y
    rot = - 0.7f; // rotation
    t2x = screenWidth / 2.0f; // translation x
    t2y = screenHeight / 2.0f; // translation y
    Utils::mat3TRST(t2x, t2y, rot, sx, sy, t1x, t1y, matrixTransform);
    toto2d.getSpriteBatcher().addSprite(0, 0, 330, 180, matrixTransform);
    
    
    // show result
    toto2d.swap();
    
    // wait
    while (true)
    {
        
    }
}


