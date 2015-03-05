#include "CairoFontHelper.h"
#include <vector>
#include <algorithm>
#include <fribidi.h>
#include <math.h>

CairoFontHelper::CairoFontHelper()
{
    FT_Init_FreeType(&_ft_library);
    _main_fonts = { NULL, NULL, NULL, NULL };
    _fallback_fonts = { NULL, NULL, NULL, NULL };
    _useFallbackFont = false;
}

CairoFontHelper::~CairoFontHelper()
{

}

const string &CairoFontHelper::fontFaceName()
{
    return _fontFaceName;
}

bool CairoFontHelper::initWithPath(const string &face, const string &path, const string *path2)
{
    _fontFaceName = face;
    if (FT_New_Face(_ft_library, path.c_str(), 0, &_main_fonts.face)) {
        cout << "FontFamilyHelper: cannot load font face: " << path << endl;
        return false;
    }

    _main_fonts.cairo_face = cairo_ft_font_face_create_for_ft_face(_main_fonts.face, 0);
    _main_fonts.hb_font = hb_ft_font_create(_main_fonts.face, nullptr);
    _main_fonts.hb_face = hb_ft_face_create(_main_fonts.face, NULL);

    if (path2 != NULL) {
        _useFallbackFont = true;
        FT_Error err = FT_New_Face(_ft_library, path2->c_str(), 0, &_fallback_fonts.face);
        if (err) {
            cout << "FontFamilyHelper: cannot load font face: " << *path2 << endl;
            return false;
        }
        _fallback_fonts.cairo_face = cairo_ft_font_face_create_for_ft_face(_fallback_fonts.face, 0);
        _fallback_fonts.hb_font = hb_ft_font_create(_fallback_fonts.face, NULL);
        _fallback_fonts.hb_face = hb_ft_face_create(_fallback_fonts.face, NULL);
    }

    return true;
}

shared_ptr<hb_buffer_t> CairoFontHelper::newHbBuffer()
{
    hb_buffer_t *hb_buf = hb_buffer_create();
    hb_buffer_clear_contents(hb_buf);
    hb_buffer_set_unicode_funcs(hb_buf, hb_glib_get_unicode_funcs());
    hb_buffer_set_direction(hb_buf, HB_DIRECTION_LTR);
    hb_buffer_set_script(hb_buf, HB_SCRIPT_LATIN);

    return shared_ptr<hb_buffer_t>(hb_buf, hb_buffer_destroy);
}

double CairoFontHelper::measureText(const string &str, double fontSize)
{
    unique_ptr<vector<renderCharBuffer>> buffers(newBidiRenderCharBuffersFromUtf8String(str));

    return measureFromBuffers(*buffers, fontSize);
}

void CairoFontHelper::fillText(cairo_t *context, double x, double y, const string &str, double fontSize)
{
    unique_ptr<vector<renderCharBuffer>> buffers(newBidiRenderCharBuffersFromUtf8String(str));

    fillFromBuffers(*buffers, context, x, y, fontSize);
}

string CairoFontHelper::logToVisUtf8String(const string& str)
{
    FriBidiCharSet char_set = fribidi_parse_charset("UTF-8");
    FriBidiChar unicodeStr[str.length() * 2 + 1];
    int len = fribidi_charset_to_unicode(char_set, str.c_str(), str.length(),  unicodeStr);

    FriBidiParType base = FRIBIDI_PAR_ON;
    FriBidiChar *visual = (FriBidiChar *) malloc(sizeof(FriBidiChar)*(len + 1));
    FriBidiLevel lev = fribidi_log2vis(unicodeStr, len, &base, visual, NULL, NULL, NULL);

    char converted[str.length() + 1];
    fribidi_unicode_to_charset(char_set, visual, len, converted);

    return converted; // Copy buffer up to terminator
}

// Get a vector with a pointer to a Ft_Face for each glyph in the string
vector<renderChar> *CairoFontHelper::newRenderCharsFromUtf8String(const string &str)
{
    // If no fallback font is set return NULL
    if (_fallback_fonts.face == NULL) {
        return NULL;
    }

    // Create a buffer
    shared_ptr<hb_buffer_t> hb_buf = newHbBuffer();
    hb_buffer_add_utf8(hb_buf.get(), str.c_str(), str.length(), 0, str.length());

    unsigned int glyph_count;
    hb_glyph_info_t *glyph_info = hb_buffer_get_glyph_infos(hb_buf.get(), &glyph_count);
    unsigned int i = 0;
    vector<renderChar> *faces = new vector<renderChar>();
    for (i = 0; i < glyph_count; i++) {
        int ccode = FT_Get_Char_Index(_main_fonts.face, glyph_info[i].codepoint);
        fonts_t *chosen_fonts;
        if(!ccode) {
            chosen_fonts = &_fallback_fonts;
        } else {
            chosen_fonts = &_main_fonts;
        }
        faces->push_back(std::make_pair(chosen_fonts->cairo_face, glyph_info[i].codepoint));
    }

    return faces;
}

// Group renderchars with the same font in hb_buffers-fontface pairs
vector<renderCharBuffer> *CairoFontHelper::newRenderCharBuffersFromRenderChars(const vector<renderChar> &faces)
{
    vector<renderCharBuffer> *buffers = new vector<renderCharBuffer>();

    if (faces.size() == 0) {
        return buffers;
    }

    // Group adjacent codepoints with the same font in the same hb_buffer
    cairo_font_face_t *curr_face = faces[0].first;
    shared_ptr<hb_buffer_t> curr_hb_buf = newHbBuffer();
    for_each(faces.begin(), faces.end(), [&] (const renderChar &renderchar){
        if (renderchar.first != curr_face) {
            buffers->push_back(std::make_pair(curr_face, curr_hb_buf));
            curr_hb_buf = newHbBuffer();
            curr_face = renderchar.first;
        }
        hb_buffer_add_utf32(curr_hb_buf.get(), &renderchar.second, 1, 0, -1);
    });
    buffers->push_back(std::make_pair(curr_face, curr_hb_buf));

    return buffers;
}

double CairoFontHelper::measureFromBuffers(vector<renderCharBuffer> &buffers, double fontSize)
{
    ftSetCharSize(fontSize);

    std::vector<renderCharBuffer>::size_type i = 0;
    double string_width_in_pixels = 0;
    for (i = 0; i < buffers.size(); i++) {
        cairo_font_face_t *face = buffers[i].first;
        shared_ptr<hb_buffer_t> buffer = buffers[i].second;
        hb_shape(face == _fallback_fonts.cairo_face ? _fallback_fonts.hb_font : _main_fonts.hb_font, buffer.get(), NULL, 0);

        unsigned int         glyph_count;
        hb_glyph_position_t *glyph_pos = hb_buffer_get_glyph_positions(buffer.get(), &glyph_count);

        for (unsigned int j=0; j < glyph_count; ++j) {
            string_width_in_pixels += glyph_pos[j].x_advance/64.;
        }
    }

    return string_width_in_pixels;
}

void CairoFontHelper::fillFromBuffers(vector<renderCharBuffer> &buffers, cairo_t *context, double x, double y, double fontSize)
{
    ftSetCharSize(fontSize);

    std::vector<renderCharBuffer>::size_type i = 0;
    for (i = 0; i < buffers.size(); i++) {

        cairo_font_face_t *face = buffers[i].first;
        fonts_t *chosen_fonts = (face == _main_fonts.cairo_face) ? &_main_fonts : &_fallback_fonts;

        shared_ptr<hb_buffer_t> buffer = buffers[i].second;

        // Copy codepoints before they get transformed to ccodes
        unsigned int glyph_count;
        hb_glyph_info_t *glyph_info = hb_buffer_get_glyph_infos(buffer.get(), &glyph_count);
        hb_glyph_info_t *glyph_info_codepoints = (hb_glyph_info_t *) malloc(sizeof(hb_glyph_info_t) * glyph_count);
        memcpy(glyph_info_codepoints, glyph_info, sizeof(hb_glyph_info_t) * glyph_count);

        // Shape buffers
        hb_shape(chosen_fonts->hb_font, buffer.get(), NULL, 0);
        hb_glyph_position_t *glyph_pos    = hb_buffer_get_glyph_positions(buffer.get(), &glyph_count);
        cairo_glyph_t       *cairo_glyphs = (cairo_glyph_t *)malloc(sizeof(cairo_glyph_t) * glyph_count);

        for (unsigned int j=0; j < glyph_count; ++j) {
            double advance_x;
            if (glyph_info_codepoints[j].codepoint == UNICODE_TAB) { // Tab
                cairo_glyphs[j].index = CCODE_SPACE; // Space
                advance_x = TAB_PX - fmod(x, TAB_PX);
            } else {
                cairo_glyphs[j].index = FT_Get_Char_Index(chosen_fonts->face, glyph_info_codepoints[j].codepoint);
                advance_x = glyph_pos[j].x_advance/64.;
            }

            cairo_glyphs[j].x = (int) (x + (glyph_pos[j].x_offset/64.));
            cairo_glyphs[j].y = (int) (y - (glyph_pos[j].y_offset/64.));
            x += advance_x;
            y -= glyph_pos[j].y_advance/64.;
        }

        cairo_set_font_face(context, chosen_fonts->cairo_face);
        cairo_set_font_size(context, fontSize);
        cairo_show_glyphs(context, cairo_glyphs, glyph_count);

        free(cairo_glyphs);
        free(glyph_info_codepoints);
    }

    return;
}

vector<renderCharBuffer> *CairoFontHelper::newBidiRenderCharBuffersFromUtf8String(const string &str)
{
    string &&bidiStr = logToVisUtf8String(str);

    vector<renderCharBuffer> *buffers;
    if (_useFallbackFont) {
        // Get vector with face-codepoint pairs for each character
        unique_ptr<vector<renderChar>> faces(newRenderCharsFromUtf8String(bidiStr));
        // Group adjacent codepoints with the same font in the same hb_buffer
        buffers = newRenderCharBuffersFromRenderChars(*(faces.get()));
    } else {
        // NO FALLBACK Create one buffer with one face
        buffers = new vector<renderCharBuffer>();
        shared_ptr<hb_buffer_t> buffer = newHbBuffer();
        hb_buffer_add_utf8(buffer.get(), str.c_str(), str.length(), 0, -1);
        buffers->push_back(std::make_pair(_main_fonts.cairo_face, buffer));
    }

    return buffers;
}

void CairoFontHelper::ftSetCharSize(double fontSize)
{
    FT_Set_Char_Size(_main_fonts.face, 0, fontSize * 64, 72, 72);
    if (_useFallbackFont) {
        FT_Set_Char_Size(_fallback_fonts.face, 0, fontSize * 64, 72, 72);
    }
}
