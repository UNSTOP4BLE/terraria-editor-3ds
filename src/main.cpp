#include <citro2d.h>
#include "app.h"
#include "gfx.h"
#include "items.h"
#include "items.h"

Terraeditor *app;

int main(void) {
	romfsInit();
	app = new Terraeditor();
	GFX::init();

	GFX::SpriteSheet spritesheet = C2D_SpriteSheetLoad("romfs:/sprites.t3x");
	GFX::Sprite2D *spr = new GFX::Sprite2D(spritesheet, 0);

	Terraria::LoadItemsList("romfs:/items.txt");

//    app->clearcol = C2D_Color32(51, 85, 153, 255);

//    setScreen(new TitleScreen(0, Intro));


	while(aptMainLoop()) {
		hidScanInput();
		hidTouchRead(&app->touch);
		if (app->currentScene == NULL) 	
			break;

		GFX::clear(app->clearcol);
  		app->currentScene->update();  
		app->currentScene->draw();  
		GFX::flip();
	}

	C2D_SpriteSheetFree(spritesheet);

	// Deinit libs
	romfsExit();
	GFX::exit();

	delete app;
	return 0;
}
