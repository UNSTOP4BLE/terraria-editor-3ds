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

	while(aptMainLoop()) {
		hidScanInput();
		hidTouchRead(&app->touch);

		GFX::clear(app->clearcol);
		spr->pos = {(int)app->touch.px, (int)app->touch.py, spr->pos.w, spr->pos.h};
		spr->draw(app->screens->bottom);


		GFX::flip();
	}

	C2D_SpriteSheetFree(spritesheet);

	// Deinit libs
	romfsExit();
	GFX::exit();

	delete app;
	return 0;
}
