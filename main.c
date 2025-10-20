#include "include.h"

auth_config opt;
toml_result_t config_file;

int main(int c, char *v[]) {
  pl_arg *help_arg = PL_A("--help", "show this dialog");
  pl_arg *arg_config_paths = PL_A("--config-path", "show config path");
  pl_arg *arg_config_path =
      PL_A("--config", "load a config file", .takes_value = 1, .type = "path");
  pl_r ret;

  // parse plib arguments
  if ((ret = pl_proc(c, v)) == PL_SUCCESS) {

		// if --help
    if (PL_R(help_arg)) {
      printf("--help called\n");
      goto help_exit;
    }

    // if config-path
    if (PL_R(arg_config_paths)) {
      printf("Default config path is: \"%s\"\n", DEFAULT_CONFIG_PATH);
      quit(SUCCESS);
    }
  } else goto help_exit;


  quit(SUCCESS);

// PLib error handling
help_exit:
  help();

  if (ret != PL_SUCCESS) {
    printf("error: '%s' from argument '%s'\n",
           PL_E(ret),    // error code stringify'd
           PL_LAST_ARG); // the last arg parsed by plib
  }

  quit(HELP_MENU_REF);
}
