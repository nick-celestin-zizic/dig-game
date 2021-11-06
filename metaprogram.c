#define NOBUILD_IMPLEMENTATION
#include "nobuild.h"

#define GAME_NAME "untitled"

#define CFLAGS "-std=c99","-Wall", "-Wextra", "-Wpedantic", "-Werror"

#define SERVER_EXE_NAME       CONCAT(GAME_NAME, "-server")
#define SERVER_EXE            PATH("bin", SERVER_EXE_NAME)
#define SERVER_ENTRY_POINT    PATH("src", "server", "main.c")
#define SERVER_CC             "gcc"
#define SERVER_DEBUG_FLAGS    "-g"
#define SERVER_RELEASE_FLAGS  "-o3"
#define SERVER_CFLAGS         CFLAGS

#define CLIENT_EXE_NAME       "index.html"//CONCAT(GAME_NAME, "-client", ".html")
#define CLIENT_EXE            PATH("bin", CLIENT_EXE_NAME)
#define CLIENT_ENTRY_POINT    PATH("src", "client", "main.c")
#define CLIENT_CC             "emcc"
#define CLIENT_DEBUG_FLAGS    "-g"
#define CLIENT_RELEASE_FLAGS  "-o3", "--minify", "0"
#define CLIENT_CFLAGS         CFLAGS, \
    "--shell-file", PATH("src", "client", "shell.html")

static inline void
usage (FILE *out) {
  fprintf(out,
          "usage: ./build <help | full | client [action] | server [action]>\n"
          "  NOTE -- Can only build the serevr on Linux, client builds with emcc which is cross platform\n"
          "  help      - prints this message and exits\n"
          "  full      - builds the client and server in release mode\n"
          "  client    - builds only the client, actions available are:\n"
          "    run     - compile and run client in internal mode\n"
          "  server    - builds only the server with given options"
          "    run     - compile and run client in internal mode\n"
          "    debug   - compile and run server in a debugger\n"
          "    term    - compile and run server in a terminal window\n"
          );
}

static inline void
build_server(const char *arg) {
  if (arg == NULL) {
    CMD(SERVER_CC, SERVER_CFLAGS, SERVER_DEBUG_FLAGS,
      SERVER_ENTRY_POINT, "-o", SERVER_EXE);
    return;
  }

  if (strcmp(arg, "release") == 0) {
    CMD(SERVER_CC, SERVER_CFLAGS, SERVER_RELEASE_FLAGS,
        SERVER_ENTRY_POINT, "-o", SERVER_EXE);
    return;
  }

  CMD(SERVER_CC, SERVER_CFLAGS, SERVER_DEBUG_FLAGS,
      SERVER_ENTRY_POINT, "-o", SERVER_EXE);
  
  if (strcmp(arg, "run") == 0) {
    CMD(SERVER_EXE);
  } else if (strcmp(arg, "debug") == 0) {
    CMD("st", "-e", "gdb", "--tui", SERVER_EXE);
  } else if (strcmp(arg, "term") == 0) {
    CMD("st", "-e", SERVER_EXE);
  } else {
    usage(stderr);
    exit(1);
  }
}

static inline void
build_client (const char *arg) {
  if (arg == NULL) {
    CMD(CLIENT_CC, CLIENT_CFLAGS, CLIENT_DEBUG_FLAGS,
        CLIENT_ENTRY_POINT, "-o", CLIENT_EXE);
  } else if (strcmp(arg, "release") == 0) {
    CMD(CLIENT_CC, CLIENT_CFLAGS, CLIENT_RELEASE_FLAGS,
        CLIENT_ENTRY_POINT, "-o", CLIENT_EXE);
  } else if (strcmp(arg, "run") == 0) {
    CMD(CLIENT_CC, CLIENT_CFLAGS, CLIENT_DEBUG_FLAGS,
      CLIENT_ENTRY_POINT, "-o", CLIENT_EXE);
    CMD(CLIENT_EXE);
  } else {
    usage(stderr);
    exit(1);
  }
}

int
main (int argc, char **argv) {
  GO_REBUILD_URSELF(argc, argv);
  
  if (argc == 1) {
    usage(stderr);
    return 1;
  } else if (argc >= 2) {
    if (strcmp(argv[1], "help") == 0) {
      usage(stdout);
    } else if (strcmp(argv[1], "full") == 0) {    
      build_server("release");
      build_client("release");
    } else if (strcmp(argv[1], "server") == 0) {
      build_server(argc == 2 ? NULL : argv[2]);
    } else if (strcmp(argv[1], "client") == 0) {
      build_client(argc == 2 ? NULL : argv[2]);
    } else {
      usage(stderr);
      return 1;
    }
  }

  return 0;
}
