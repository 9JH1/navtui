#include "include.h"

int main(const int c, const char *v[]) {
  pl_arg *arg_help = PL_P("--help", "show this dialog");
  pl_arg *arg_config_paths = PL_P("--config-path", "show config path");
	pl_arg *arg_config_path = PL_P("--config","load a config file",.takes_value = 1);
  pl_r ret;

  if ((ret = pl_proc(c, v)) == PL_SUCCESS) {
    if (PL_R(arg_help))
      goto help_exit;
  }

	// help dialog code	
	help_exit:
  help();

  // print error details and exit
  if (ret != PL_SUCCESS) {
    printf("error: '%s' from argument '%s'\n",
           PL_E(ret),    // error code stringify'd
           PL_LAST_ARG); // the last arg parsed by plib
  }

  quit(HELP_MENU_REF);
}
