#include "Game.h"
#include "Handler.h"

int main(int argc, char* argv[])
{	
	Game game(SDL_WINDOW_SHOWN, "Miseries of the world", 100, 100);
	Handler handler(&game);

	game.setRgbOfWindow(0, 150, 150, SDL_ALPHA_OPAQUE);
	game.setFps(144);
	game.setActions(Type::LOOPBACK, [&handler]() {handler.loopBack();});
	game.setActions(Type::ACTIONS,  [&handler]() {handler.actions();});
	game.setActions(Type::OUTRO,    [&handler]() {handler.outro();});
	game.startLoop();

	return 0;
}
