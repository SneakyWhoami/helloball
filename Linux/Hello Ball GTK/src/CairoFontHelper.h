#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H
#include <ftadvanc.h>
#include <ftsnames.h>
#include <tttables.h>
#ifndef __CairoFontHelper__
#define __CairoFontHelper__

#include <harfbuzz/hb.h>
#include <harfbuzz/hb-ft.h>
#include <harfbuzz/hb-glib.h>
#include <cairo/cairo-ft.h>
#include <assert.h>
#include <vector>
#include <memory>

#define CCODE_SPACE (0x3)
#define UNICODE_TAB (0x9)
#define TAB_PX (48.0)

using namespace std;

typedef pair<cairo_font_face_t *, shared_ptr<hb_buffer_t>> renderCharBuffer;
typedef pair<cairo_font_face_t *, hb_codepoint_t> renderChar;

typedef struct _fonts_t {
    FT_Face face;
    cairo_font_face_t *cairo_face;
    hb_font_t *hb_font;
    hb_face_t *hb_face;
    ~_fonts_t() {
        if (cairo_face) {
            cairo_font_face_destroy(cairo_face);
        }
        if (hb_font) {
            hb_font_destroy(hb_font);
        }
        if (hb_face) {
            hb_face_destroy(hb_face);
        }
        if (face) {
            FT_Done_Face(face);
        }
    }
} fonts_t;

class CairoFontHelper {

protected:
    FT_Library _ft_library;

    fonts_t _main_fonts;
    fonts_t _fallback_fonts;

    bool _useFallbackFont;
    string _fontFaceName;

    void setText(const string &str);
    void ftSetCharSize(double fontSize);
    vector<renderChar> *newRenderCharsFromUtf8String(const string &str);
    vector<renderCharBuffer> *newRenderCharBuffersFromRenderChars(const vector<renderChar> &faces);
    void fillFromBuffers(vector<renderCharBuffer> &buffers, cairo_t *context, double x, double y, double fontSize);
    double measureFromBuffers(vector<renderCharBuffer> &buffers, double fontsize);
    vector<renderCharBuffer> *newBidiRenderCharBuffersFromUtf8String(const string &str);
    string logToVisUtf8String(const string& str);
    shared_ptr<hb_buffer_t> newHbBuffer();

public:
    CairoFontHelper();
    ~CairoFontHelper();

    const string &fontFaceName();
    bool initWithPath(const string &face, const string &path, const string *path2 = NULL);
    double measureText(const string &str, double fontSize);
    void fillText(cairo_t *context, double x, double y, const string &str, double fontSize);

};


#endif
