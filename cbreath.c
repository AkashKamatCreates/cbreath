#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#define FPS 60
#define ASPECT_Y 0.5f
#define BORDER_THICKNESS 6

typedef enum {
    INHALE,
    HOLD_IN,
    EXHALE,
    HOLD_OUT
} Phase;

/* ---------- presets ---------- */

typedef struct {
    const char *name;
    int inhale, hold_in, exhale, hold_out;
} Preset;

static Preset presets[] = {
    {"box",      4, 4, 4, 4},
    {"478",      4, 7, 8, 0},
    {"relax",    5, 2, 7, 3},
    {"focus",    4, 0, 6, 0},
    {"calm",     6, 2, 8, 2},
    {"sleep",    4, 7, 8, 0},
    {"energize", 3, 0, 3, 0}
};

#define PRESET_COUNT (sizeof(presets) / sizeof(presets[0]))

/* ---------- calm text ---------- */

static const char *calm_lines[] = {
    "Forget the noise. Stay with the breath.",
    "Nothing else is required right now.",
    "Let the body breathe itself.",
    "There is nowhere else to be.",
    "This moment is enough."
};

#define CALM_LINE_COUNT (sizeof(calm_lines) / sizeof(calm_lines[0]))

/* ---------- global state ---------- */

static int paused   = 0;
static int sound_on = 1;

/* ---------- helpers ---------- */

static float ease(float t) {
    return t * t * (3.0f - 2.0f * t);
}

/* ---------- visuals ---------- */

void draw_circle(float r, int cx, int cy, int color, int fade) {
    attron(COLOR_PAIR(color));
    for (int t = 0; t < BORDER_THICKNESS; t++) {
        float rad = r + t * 0.9f;
        int outer = (t == BORDER_THICKNESS - 1);

        if (!outer) attron(A_BOLD);

        char ch = (outer && fade) ? '.' : 'o';
        for (float a = 0; a < 2 * M_PI; a += 0.045f) {
            int x = cx + (int)(cosf(a) * rad);
            int y = cy + (int)(sinf(a) * rad * ASPECT_Y);
            mvaddch(y, x, ch);
        }

        if (!outer) attroff(A_BOLD);
    }
    attroff(COLOR_PAIR(color));
}

void draw_config_info(
    int cx, int cy,
    const char *mode,
    int total,
    int inh, int hi, int ex, int ho
) {
    char l1[64], l2[64], l3[64];

    snprintf(l1, sizeof(l1), "Mode: %s", mode[0] ? mode : "manual");
    snprintf(l2, sizeof(l2), "Total: %ds", total);
    snprintf(l3, sizeof(l3),
        "Inhale %d · Hold %d · Exhale %d · Hold %d",
        inh, hi, ex, ho
    );

    mvprintw(cy - 14, cx - (int)strlen(l1) / 2, "%s", l1);
    mvprintw(cy - 13, cx - (int)strlen(l2) / 2, "%s", l2);
    mvprintw(cy - 12, cx - (int)strlen(l3) / 2, "%s", l3);
}

void draw_calm_line(int cx, int y) {
    const char *line =
        calm_lines[(time(NULL) / 10) % CALM_LINE_COUNT];
    mvprintw(y, cx - (int)strlen(line) / 2, "%s", line);
}

/* ---------- phase runner ---------- */

int run_phase(
    Phase phase,
    int seconds,
    const char *label,
    int cx, int cy,
    float *radius,
    time_t *end_time,
    const char *mode,
    int total,
    int inh, int hi, int ex, int ho
) {
    int frames = seconds * FPS;
    float min_r = 4.0f, max_r = 16.0f;

    int color = (phase == INHALE) ? 1 :
                (phase == EXHALE) ? 3 : 2;

    for (int i = 0; i < frames;) {
        int ch = getch();
        if (ch == 'q') return 0;
        if (ch == ' ') paused = !paused;

        if (paused) {
            usleep(20000);
            (*end_time)++;
            continue;
        }

        if (time(NULL) >= *end_time) return 0;

        clear();

        float t = (float)i / (frames - 1);
        if (phase == INHALE)
            *radius = min_r + ease(t) * (max_r - min_r);
        else if (phase == EXHALE)
            *radius = max_r - ease(t) * (max_r - min_r);

        draw_config_info(cx, cy, mode, total, inh, hi, ex, ho);
        draw_circle(*radius, cx, cy, color, phase == INHALE);

        char buf[64];
        snprintf(buf, sizeof(buf), "%s · %d", label, seconds - i / FPS);
        mvprintw(cy + 12, cx - (int)strlen(buf) / 2, "%s", buf);

        draw_calm_line(cx, cy + 14);

        refresh();
        usleep(1000000 / FPS);
        i++;
    }

    if (sound_on) beep();
    return 1;
}

void draw_end_screen(int cx, int cy) {
    const char *title = "Session complete";
    const char *line1 = "Take a moment.";
    const char *line2 = "Notice how you feel.";

    clear();

    mvprintw(cy - 2, cx - (int)strlen(title) / 2, "%s", title);
    refresh();
    usleep(700000);

    mvprintw(cy, cx - (int)strlen(line1) / 2, "%s", line1);
    refresh();
    usleep(700000);

    mvprintw(cy + 1, cx - (int)strlen(line2) / 2, "%s", line2);
    refresh();

    /* soft dots */
    for (int i = 0; i < 3; i++) {
        mvprintw(cy + 3, cx - 2 + i * 2, "●");
        refresh();
        usleep(500000);
    }

    /* wait up to 5 seconds or quit */
    time_t end = time(NULL) + 5;
    while (time(NULL) < end) {
        if (getch() == 'q')
            break;
        usleep(50000);
    }
}

void print_help(void) {
    printf(
        "cbreath — a calm, terminal-based breathing guide\n\n"
        "USAGE:\n"
        "  cbreath [MODE]\n"
        "  cbreath manual TOTAL INHALE HOLD_IN EXHALE HOLD_OUT\n"
        "  cbreath TOTAL INHALE HOLD_IN EXHALE HOLD_OUT\n"
        "  cbreath --help\n\n"
        "MODES:\n"
        "  box        Inhale 4 · Hold 4 · Exhale 4 · Hold 4\n"
        "  478        Inhale 4 · Hold 7 · Exhale 8\n"
        "  relax      Inhale 5 · Hold 2 · Exhale 7 · Hold 3\n"
        "  calm       Inhale 6 · Hold 2 · Exhale 8 · Hold 2\n"
        "  focus      Inhale 4 · Exhale 6\n"
        "  sleep      Inhale 4 · Hold 7 · Exhale 8\n"
        "  energize   Inhale 3 · Exhale 3\n\n"
        "KEY CONTROLS:\n"
        "  Space      Pause / Resume\n"
        "  q          Quit\n\n"
        "AUTHOR:\n"
        "  Written by Akash Kamat\n"
    );
}

void list_presets(void) {
    printf("Available breathing modes:\n\n");

    for (size_t i = 0; i < PRESET_COUNT; i++) {
        printf("  %-10s Inhale %d",
            presets[i].name,
            presets[i].inhale
        );

        if (presets[i].hold_in)
            printf(" · Hold %d", presets[i].hold_in);

        printf(" · Exhale %d", presets[i].exhale);

        if (presets[i].hold_out)
            printf(" · Hold %d", presets[i].hold_out);

        printf("\n");
    }

    printf("\nUse:\n  cbreath <mode>\n");
}

/* ---------- main ---------- */

int main(int argc, char **argv) {
    int total = 300;
    int inhale = 4, hold_in = 3, exhale = 5, hold_out = 3;
    const char *mode = "manual";

    /* ---------- CLI parsing (highest priority) ---------- */

    if (argc >= 2 && !strcmp(argv[1], "list")) {
        list_presets();
        return 0;
    }

    if (argc >= 2 && (!strcmp(argv[1], "--help") || !strcmp(argv[1], "help"))) {
        print_help();
        return 0;
    }

    if (argc >= 2) {
        /* manual mode */
        if (!strcmp(argv[1], "manual")) {
            if (argc < 7) {
                fprintf(stderr,
                    "Usage: cbreath manual TOTAL INHALE HOLD_IN EXHALE HOLD_OUT\n");
                return 1;
            }
            total    = atoi(argv[2]);
            inhale   = atoi(argv[3]);
            hold_in  = atoi(argv[4]);
            exhale   = atoi(argv[5]);
            hold_out = atoi(argv[6]);
            mode = "manual";
        }
        /* preset mode */
        else {
            int preset_found = 0;
            for (size_t i = 0; i < PRESET_COUNT; i++) {
                if (!strcmp(argv[1], presets[i].name)) {
                    inhale   = presets[i].inhale;
                    hold_in  = presets[i].hold_in;
                    exhale   = presets[i].exhale;
                    hold_out = presets[i].hold_out;
                    mode = presets[i].name;
                    preset_found = 1;
                    break;
                }
            }

            /* numeric shortcut */
            if (!preset_found) {
                total = atoi(argv[1]);
                if (argc > 2) inhale   = atoi(argv[2]);
                if (argc > 3) hold_in  = atoi(argv[3]);
                if (argc > 4) exhale   = atoi(argv[4]);
                if (argc > 5) hold_out = atoi(argv[5]);
                mode = "manual";
            }
        }
    }

    /* ---------- ncurses ---------- */

    initscr();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);

    start_color();
    use_default_colors();
    init_pair(1, COLOR_CYAN,  -1);
    init_pair(2, COLOR_BLUE,  -1);
    init_pair(3, COLOR_GREEN, -1);

    int maxx, maxy;
    getmaxyx(stdscr, maxy, maxx);
    int cx = maxx / 2;
    int cy = maxy / 2;

    float radius = 4.0f;
    time_t end_time = time(NULL) + total;

    while (time(NULL) < end_time) {
        if (!run_phase(INHALE, inhale, "Inhale", cx, cy, &radius, &end_time,
                       mode, total, inhale, hold_in, exhale, hold_out)) break;
        if (!run_phase(HOLD_IN, hold_in, "Hold", cx, cy, &radius, &end_time,
                       mode, total, inhale, hold_in, exhale, hold_out)) break;
        if (!run_phase(EXHALE, exhale, "Exhale", cx, cy, &radius, &end_time,
                       mode, total, inhale, hold_in, exhale, hold_out)) break;
        if (!run_phase(HOLD_OUT, hold_out, "Hold", cx, cy, &radius, &end_time,
                       mode, total, inhale, hold_in, exhale, hold_out)) break;
    }

    draw_end_screen(cx, cy);
    endwin();
    return 0;
}


