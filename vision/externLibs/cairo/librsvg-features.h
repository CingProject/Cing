#ifndef LIBRSVG_FEATURES_H
#define LIBRSVG_FEATURES_H 1

#define LIBRSVG_MAJOR_VERSION (2)
#define LIBRSVG_MINOR_VERSION (14)
#define LIBRSVG_MICRO_VERSION (4)
#define LIBRSVG_VERSION ""

extern const unsigned int librsvg_major_version, librsvg_minor_version, librsvg_micro_version;
extern const char *librsvg_version;

void librsvg_preinit(void *app, void *modinfo);
void librsvg_postinit(void *app, void *modinfo);
#endif
