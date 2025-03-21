#include <citro2d.h>
#include "app.h"
#include "gfx.h"
#include "pad.h"
#include "scenes/disclamer.h"
#include <chrono>

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
	app->elapsed = 0;
	GFX::init();
	app->fontManager.init("romfs:/font/font.t3x");
	
    setScene(new DisclamerScene());

	std::chrono::time_point<std::chrono::system_clock> start = std::chrono::high_resolution_clock::now();
	while(aptMainLoop()) {
        std::chrono::time_point<std::chrono::system_clock> last = std::chrono::high_resolution_clock::now();
        
		Pad::Read();
		GFX::clear(app->clearcol);
  		app->currentScene->update();  
		app->currentScene->draw();  
		GFX::flip();

		std::chrono::time_point<std::chrono::system_clock> current = std::chrono::high_resolution_clock::now();
        app->deltatime = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(current - last).count());
        last = current;
		app->elapsed = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(current - start).count()) / 1000;
	}

	// Deinit libs
	romfsExit();
	GFX::exit();
	app->fontManager.del();
	delete app;
	return 0;
}
