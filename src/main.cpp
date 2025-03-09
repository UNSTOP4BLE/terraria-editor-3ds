#include <citro2d.h>
#include "app.h"
#include "gfx.h"
#include "pad.h"
#include "scenes/inventory.h"

Terraeditor *app;

void ErrMSG(const char *filename, const char *function, int line, const char *expr, const char *msg)
{
	char errstr[512];
    sprintf(errstr, "error\nmessage: %s\nexpression: %s\nfile: %s\nfunction: %s\nline %d", msg, expr, filename, function, line);
    while(1)
    {
		GFX::clear(app->clearcol);
        app->fontManager.print(app->screens->top, GFX::Left, 0, 0, errstr);
		GFX::flip();
    }
}

int main(void) {
	romfsInit();
	app = new Terraeditor();
	GFX::init();
	app->fontManager.init("romfs:/font/font.t3x");
	
    setScene(new InventoryScene());

	while(aptMainLoop()) {
		Pad::Read();
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
