#include <iostream>
#include <stdexcept>

#include <SDL2/SDL.h>

#include <vbte/core/engine.hpp>
#include <vbte/utils/stacktrace.hpp>

int main(int argc, char* argv[]) {
    try {
        vbte::utils::initStacktrace(argv[0]);
		vbte::core::engine e;
		e.run();
	} catch (std::exception& ex) {
		auto result = SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", ex.what(), nullptr);
		if (result < 0) {
			std::cout << ex.what() << std::endl;
		}
	}

	return 0;
}
