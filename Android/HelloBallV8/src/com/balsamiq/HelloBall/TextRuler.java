package com.balsamiq.HelloBall;

import android.content.Context;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.Typeface;

import java.util.HashMap;

/**
 * Created by morsini on 04/03/15.
 */
public class TextRuler {

    HashMap<String, Typeface> fonts = new HashMap<String, Typeface>();
    Context _context;

    public TextRuler(Context context) {
        _context = context;
    }

    public int measureTextWidth(String font, int fontSize, boolean bold, boolean italic, String text) {
        Typeface tf = findFont(font, bold, italic);
        Paint paint = new Paint();
        Rect bounds = new Rect();
        paint.setTypeface(tf);
        paint.setTextSize(fontSize);
        float val = paint.measureText(text);
        paint.getTextBounds(text, 0, text.length(), bounds);
        return bounds.width();
    }

    protected Typeface findFont(String fontName, boolean bold, boolean italic) {
        
        if (fontName.compareTo("Balsamiq Sans") == 0) {

            String fontToLoad;
            if (bold) {
                if (italic) {
                    fontToLoad = "BalsamiqSansBoldItalic.ttf";
                } else {
                    fontToLoad = "BalsamiqSansBold.ttf";
                }
            } else if (italic) {
                fontToLoad = "BalsamiqSansItalic.ttf";
            } else {
                fontToLoad = "BalsamiqSansRegular.ttf";
            }

            if (fonts.containsKey(fontToLoad)) {
                return fonts.get(fontToLoad);
            } else {
                String fontPath = "fonts/" + fontToLoad;
                Typeface tf = Typeface.createFromAsset(_context.getAssets(), fontPath);
                fonts.put(fontToLoad, tf);
                return tf;
            }
        }

        return null;
    }
}
