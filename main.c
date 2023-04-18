#include <string.h>
#include <argp.h>
#include "./logger.h"
#include "./analyser.h"

struct arg_config
{
    int log;
    char *file;
    int analyse;
    char *save;

};

const char *argp_program_version = "version 0.1";
const char *argp_program_bug_address = "test@example.com";
error_t argp_err_exit_status = 1;

static struct argp_option opt_config[] =
{
  { "log", 'l', 0, 0, "Log all clicks and save them to a file", 0},
  { "file", 'f', "LOG_FILE", 0, "File where the clicks are logged (Default: logs.txt)", 0},
  { "analyse", 'a', 0, 0, "Analyse the data and show it", 0},
  { "save", 's', "STATS_IMAGE", 0, "Save the data into a image file", 0},
  { 0 }
};

static error_t parse_config(int key, char* arg, struct argp_state* state)
{
    struct arg_config *config = state->input;

    switch(key)
    {
        case 'l':
            config->log = 1;
            break;
        case 'f':
            config->file = arg;
            break;
        case 'a':
            config->analyse = 1;
            break;
        case 's':
            config->save = arg;
            break;
        case ARGP_KEY_ARG:
            argp_error(state, "%s is not a valid command", arg);
         break;

        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp argp =
{
    opt_config,
    parse_config,
    "COMMAND",
    NULL,
    NULL,
    NULL,
    NULL
};

void cmd_config(int argc, char**argv)
{
    struct arg_config config = { 0, NULL, 0, NULL };
    argp_parse(&argp, argc, argv, ARGP_IN_ORDER, NULL, &config);
    if (config.log) {
        if (!config.file) {
            printf("Not log file set. (Default: 'logs.txt')\n");
            logger("logs.txt");
        }
        logger(config.file);
    } else if(config.analyse) {
        analyser();
    }
}

int main(int argc, char** argv)
{
    cmd_config(argc, argv);
    return 0;
}
