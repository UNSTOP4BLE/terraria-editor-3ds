#include <citro2d.h>
#include "app.h"
#include "gfx.h"
#include "pad.h"
#include "scene.h"
#include "scenes/disclamer.h"
#include "fslib/FsLib.hpp"
#include "scenes/inventory.h"
#include <chrono>

Terraeditor *app;

void ErrMSG(const char *filename, const char *function, int line, const char *expr, const char *msg)
{
	char errstr[512];
    sprintf(errstr, "error\nmessage: %s\nexpression: %s\nfile: %s\nfunction: %s\nline %d", msg, expr, filename, function, line);
    while(aptMainLoop())
    {
		GFX::clear(app->clearcol);
        app->fontManager.print(app->screens->top, GFX::SCR_TOP_W, GFX::Left, 0, 0, errstr);
		GFX::flip();
    }
}

bool confirmScreen(void) {
    while(aptMainLoop())
    {
		Pad::Read();
		GFX::clear(app->clearcol);
        app->fontManager.print(app->screens->top, GFX::SCR_TOP_W, GFX::Center, GFX::SCR_TOP_W/2, GFX::SCR_TOP_H/2, "Are you sure?");
        app->fontManager.print(app->screens->bottom, GFX::SCR_BTM_W, GFX::Center, GFX::SCR_BTM_W/2, GFX::SCR_BTM_H/2, "A - Confirm\nB - Go back");
		if (Pad::Pressed(Pad::KEY_A))
			return true;
		else if (Pad::Pressed(Pad::KEY_B))
			return false;
		GFX::flip();
    }
	return false;
}

int main(void) {
	romfsInit();
	app = new Terraeditor();
	app->elapsed = 0;
	GFX::init();
	app->fontManager.init("romfs:/font/font.t3x");
	FsLib::Initialize();
	FsLib::Dev::InitializeSDMC();
    FsLib::OpenExtData(u"extdata", 0x000016A6);

    setScene(new DisclamerScene());
	checkSwapScene();

	std::chrono::time_point<std::chrono::system_clock> start = std::chrono::high_resolution_clock::now();

	while(aptMainLoop()) {
        std::chrono::time_point<std::chrono::system_clock> last = std::chrono::high_resolution_clock::now();
        
		Pad::Read();
		GFX::clear(app->clearcol);
		app->scenemgr.cur->update();
		app->scenemgr.cur->draw();  
		GFX::flip();

		std::chrono::time_point<std::chrono::system_clock> current = std::chrono::high_resolution_clock::now();
        app->deltatime = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(current - last).count());
        last = current;
		app->elapsed = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(current - start).count()) / 1000;
		checkSwapScene();
	}

	// Deinit libs
	romfsExit();
    FsLib::Exit();
	GFX::exit();
	app->fontManager.del();
	delete app;
	return 0;
}
