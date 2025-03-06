#include <citro2d.h>
#include "app.h"
#include "gfx.h"
#include "scenes/inventory.h"

Terraeditor *app;

int main(void) {
	romfsInit();
	app = new Terraeditor();
	GFX::init();

    setScene(new InventoryScene());

	while(aptMainLoop()) {
		hidScanInput();
		hidTouchRead(&app->touch);
		GFX::clear(app->clearcol);
		#ifndef __3DS__
  		app->currentScene->update();  
		app->currentScene->draw();  
		#endif
		GFX::flip();
	}

	// Deinit libs
	romfsExit();
	GFX::exit();

	delete app;
	return 0;
}
