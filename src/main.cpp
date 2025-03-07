#include <citro2d.h>
#include "app.h"
#include "gfx.h"
#include "pad.h"
#include "scenes/inventory.h"


Terraeditor *app;

int main(void) {
	romfsInit();
	app = new Terraeditor();
	GFX::init();
	app->fontManager.init("romfs:/font/font.t3x");
	
    setScene(new InventoryScene());

	while(aptMainLoop()) {
		hidScanInput(); //to clean up
		Pad::Read();
		hidTouchRead(&app->touch); //to clean up
		GFX::clear(app->clearcol);
  		app->currentScene->update();  
		app->currentScene->draw();  
		GFX::flip();
	}

	// Deinit libs
	romfsExit();
	GFX::exit();
	app->fontManager.del();
	delete app;
	return 0;
}
