#include "cado.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "portability.h" /* For vasprintf */
#include "verbose.h"

#define G(X) CADO_VERBOSE_PRINT_ ## X
#define F(X) (UINT64_C(1) << G(X))


const char * verbose_flag_list[] = 
{
    [G(CMDLINE)]                = "print-cmdline",
    [G(MODIFIED_FILES)]         = "print-modified-files",
    [G(COMPILATION_INFO)]       = "print-compilation-info",
    [G(BWC_DISPATCH_SLAVES)]    = "bwc-dispatch-slaves",
    [G(BWC_DISPATCH_MASTER)]    = "bwc-dispatch-master",
    [G(BWC_TIMING_GRIDS)]       = "bwc-timing-grids",
    [G(BWC_ITERATION_TIMINGS)]  = "bwc-iteration-timings",
    [G(BWC_CACHE_BUILD)]        = "bwc-cache-build",
    [G(BWC_DISPATCH_OUTER)]     = "bwc-dispatch-outer",
    [G(BWC_CPUBINDING)]         = "bwc-cpubinding",
    [G(BWC_CACHE_MAJOR_INFO)]   = "bwc-cache-major-info",
    [G(BWC_LOADING_MKSOL_FILES)]= "bwc-loading-mksol-files",
};

struct {
    const char * name;
    uint64_t mask;
} verbose_flag_groups[] = {
    { "all-cmdline",
            F(CMDLINE) |
            F(MODIFIED_FILES) |
            F(COMPILATION_INFO) },
    { "all-bwc-dispatch",
            F(BWC_DISPATCH_SLAVES) |
            F(BWC_DISPATCH_MASTER) |
            F(BWC_DISPATCH_OUTER) |
            F(BWC_CACHE_BUILD) },
    { "all-bwc-sub-timings", 
            F(BWC_TIMING_GRIDS) |
            F(BWC_ITERATION_TIMINGS) },
};


uint64_t verbose_flag_word;

/* This must be called in single-threaded context, preferably at program
 * start */
void verbose_set_enabled_flags(param_list pl)
{
    verbose_flag_word = ~0UL;

    const char * v = param_list_lookup_string(pl, "verbose_flags");
    if (!v) return;

    char * w = strdup(v);
    char * p = w;
    char * q;
    for( ; *p != '\0' ; p = q) {
        q = strchr(p, ',');
        if (q) {
            *q++ = '\0';
        } else {
            q = p + strlen(p);
        }
        int enabled = 1;
        if (strncmp(p, "no-", 3) == 0) { enabled = 0; p += 3; }
        else if (strncmp(p, "no", 2) == 0) { enabled = 0; p += 2; }
        else if (*p == '^') { enabled = 0; p += 1; }
        else if (*p == '!') { enabled = 0; p += 1; }

        uint64_t mask = 0;
        for(size_t i = 0 ; i < sizeof(verbose_flag_list) / sizeof(verbose_flag_list[0]) ; i++) {
            if (strcmp(p, verbose_flag_list[i]) == 0) {
                mask = UINT64_C(1) << (int) i;
                break;
            }
        }
        for(size_t i = 0; i < sizeof(verbose_flag_groups) / sizeof(verbose_flag_groups[0]) ; i++) {
            if (strcmp(p, verbose_flag_groups[i].name) == 0) {
                mask = verbose_flag_groups[i].mask;
                break;
            }
        }
        if (!mask) {
            fprintf(stderr, "Verbose flag not recognized: %s\n", p);
            abort();
        }
        if (enabled) {
            verbose_flag_word |= mask;
        } else {
            verbose_flag_word &= ~mask;
        }
    }
    free(w);
}

int verbose_decl_usage(param_list pl)
{
    param_list_decl_usage(pl, "verbose_flags", "fine grained control on which messages get printed");
    return 1;
}

/* returns true if the following verbose flag is enabled */
int verbose_enabled(int flag) {
    return verbose_flag_word & (UINT64_C(1) << flag);
}

int verbose_vfprintf(FILE * f, int flag, const char * fmt, va_list ap)
{
    if (verbose_enabled(flag)) {
        return vfprintf(f, fmt, ap);
    }
    return 1;
}

int verbose_vprintf(int flag, const char * fmt, va_list ap)
{
    return verbose_vfprintf(stdout, flag, fmt, ap);
}
int verbose_fprintf(FILE * f, int flag, const char * fmt, ...)
{
    va_list ap;
    int rc;
    va_start(ap, fmt);
    rc = verbose_vfprintf(f, flag, fmt, ap);
    va_end(ap);
    return rc;
}
int verbose_printf(int flag, const char * fmt, ...)
{
    va_list ap;
    int rc;
    va_start(ap, fmt);
    rc = verbose_vprintf(flag, fmt, ap);
    va_end(ap);
    return rc;
}

/*
  The program can initialise zero or more "channels".
  To each "channel", zero or more "outputs" (FILE handles) can be attached,
  each with an output verbosity value.
  Text can be printed to a channel, with a verbosity v, then the text is
  sent to each output attached to this channel for which the output verbosity
  is at least v.
  The default behaviour, before calling the verbose_output_init() function or
  after calling verbose_output_clear(), has 2 channels:
  channel 0 with 1 output, going to stdout with verbosity 1, and
  channel 1 with 1 output, going to stderr with verbosity 1.
*/

struct outputs_s {
    size_t nr_outputs;
    int *verbosity;
    FILE **outputs;
};

static void
init_output(struct outputs_s * const output)
{
    output->nr_outputs = 0;
    output->verbosity = NULL;
    output->outputs = NULL;
}

static void
clear_output(struct outputs_s * const output)
{
    free (output->outputs);
    free (output->verbosity);
    output->nr_outputs = 0;
    output->outputs = NULL;
    output->verbosity = NULL;
}

static int
add_output(struct outputs_s *output, FILE * const out, const int verbosity)
{
    const size_t new_nr = output->nr_outputs + 1;

    FILE ** const new_output = (FILE **) realloc(output->outputs, new_nr * sizeof(FILE *));
    if (new_output == NULL)
        return 0;
    output->outputs = new_output;

    int * const new_verbosity = (int *) realloc(output->verbosity, new_nr * sizeof(int));
    if (new_verbosity == NULL)
        return 0;
    output->verbosity = new_verbosity;

    output->nr_outputs = new_nr;
    output->outputs[new_nr - 1] = out;
    output->verbosity[new_nr - 1] = verbosity;
    return 1;
}

/* Print a string to each output attached to this channel whose verbosity
   is at least the "verbosity" parameter.
   If any output operation returns with an error, no further output is
   performed, and the error code of the failed operation is returned.
   Otherwise returns the return code of the last output operation.
   If no outputs are attached to this channel, returns 0. */
static int
print_output(const struct outputs_s * const output, const int verbosity,
             const char * const str)
{
    int rc = 0;
    /* For each output attached to this channel */
    for (size_t i = 0; i < output->nr_outputs; i++) {
        /* print string if output verbosity is at least "verbosity" */
        if (output->verbosity[i] >= verbosity) {
            rc = fprintf(output->outputs[i], "%s", str);
            if (rc < 0)
                return rc;
        }
    }
    return rc;
}

/* Static variables, the poor man's Singleton. */
static size_t _nr_channels = 0;
static struct outputs_s *_channel_outputs = NULL;

int
verbose_output_init(const size_t nr_channels)
{
    _channel_outputs = (struct outputs_s *) malloc(nr_channels * sizeof(struct outputs_s));
    if (_channel_outputs == NULL)
        return 0;
    _nr_channels = nr_channels;
    for (size_t i = 0; i < nr_channels; i++)
        init_output(&_channel_outputs[i]);
    return 1;
}

/* Reset channels back to the 2 default channels. */
void
verbose_output_clear()
{
    for (size_t i = 0; i < _nr_channels; i++)
        clear_output(&_channel_outputs[i]);
    free(_channel_outputs);
    _nr_channels = 0;
    _channel_outputs = NULL;
}

int
verbose_output_add(const size_t channel, FILE * const out, const int verbose)
{
    ASSERT_ALWAYS(channel < _nr_channels);
    return add_output(&_channel_outputs[channel], out, verbose);
}

int
verbose_output_print(const size_t channel, const int verbose,
                     const char * const fmt, ...)
{
    va_list ap;
    int rc = 0;

    va_start(ap, fmt);
    if (_channel_outputs == NULL) {
        /* Default behaviour: print to stdout or stderr */
        ASSERT_ALWAYS(channel < 2);
        if (verbose <= 1) {
            FILE *out[2] = {stdout, stderr};
            rc = vfprintf(out[channel], fmt, ap);
        }
    } else {
        ASSERT_ALWAYS(channel < _nr_channels);
        char *str;
        rc = vasprintf(&str, fmt, ap);
        if (rc != -1) {
            rc = print_output(&_channel_outputs[channel], verbose, str);
            free (str);
        }
    }
    va_end(ap);
    return rc;
}