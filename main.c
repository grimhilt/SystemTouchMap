#include <argp.h>
#include <stdlib.h>
#include <string.h>
#include "./analyser.h"
#include "./logger.h"

const char *argp_program_version = "version 1.0";
const char *argp_program_bug_address = "https://github.com/grimhilt/SystemTouchMap/issues";
error_t argp_err_exit_status = 1;

static struct argp_option opt_config[] = {
    {"log", 'l', 0, 0, "Log all clicks and save them to a file", 0},
    {"file", 'f', "LOG_FILE", 0,
     "File where the clicks are logged (Default: logs)", 0},
    {"analyse", 'a', 0, 0, "Analyse the data and show it", 0},
    {"save", 's', "STATS_IMAGE", 0, "Save the data into a PNG file", 0},
    {"row", 'r', "NB_ROW", 0, "The number of row the image will be divised in",
     0},
    {"column", 'c', "NB_COL", 0,
     "The number of column the image will be divised in", 0},
    {"width", 'w', "WIDTH_PX", 0, "The width of the image in pixel", 0},
    {"height", 'h', "HEIGHT_PX", 0, "The height of the image in pixel", 0},
    {0}};

static error_t parse_config(int key, char *arg, struct argp_state *state)
{
    struct arg_config *config = state->input;
    switch (key)
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
        case 'r':
            config->row = atoi(arg);
            break;
        case 'c':
            config->column = atoi(arg);
            break;
        case 'w':
            config->width = atoi(arg);
            break;
        case 'h':
            config->height = atoi(arg);
            break;
        case ARGP_KEY_ARG:
            argp_error(state, "%s is not a valid command", arg);
            break;

        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp argp = {opt_config, parse_config, "COMMAND", NULL,
                           NULL,       NULL,         NULL};

void cmd_config(int argc, char **argv)
{
    struct arg_config config = {0, "logs", 0, NULL, 0, 0, 0, 0, 0, 0};
    argp_parse(&argp, argc, argv, ARGP_IN_ORDER, NULL, &config);
    if (config.log)
    {
        logger(config.file);
    }
    else if (config.analyse)
    {
        analyser(config);
    }
}

int main(int argc, char **argv)
{
    cmd_config(argc, argv);
    return 0;
}
