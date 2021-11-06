#include <stdio.h>
//#include <emscripten/emscripten.h>
#include <emscripten/html5.h>

int
main (void) {
  emscripten_set_canvas_element_size("canvas", 900+225, 600+150);
  printf("Hello WASM!\n");
  printf("Hello WASM!\n");
  return 0;
}
