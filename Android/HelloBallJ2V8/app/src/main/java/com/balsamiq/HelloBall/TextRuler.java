package com.balsamiq.HelloBall;

import android.content.Context;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.Typeface;
import android.text.TextPaint;

import java.util.HashMap;

/**
 * Created by morsini on 04/03/15.
 */
public class TextRuler {

    HashMap<String, Typeface> fonts = new HashMap<String, Typeface>();
    Context _context;
    Paint paint;
    
    public TextRuler(Context context) {
        _context = context;
        paint = new TextPaint();
// useless        paint.setFlags(Paint.DEV_KERN_TEXT_FLAG);
// useless        paint.setAntiAlias(true);
// influences data       paint.setSubpixelText(true);
        paint.setLinearText(true);
    }

    public float measureTextWidth(String font, int fontSize, boolean bold, boolean italic, String text) {
        Typeface tf = findFont(font, bold, italic);
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
                    fontToLoad = "BalsamiqSansBoldItalic.otf";
                } else {
                    fontToLoad = "BalsamiqSansBold.otf";
                }
            } else if (italic) {
                fontToLoad = "BalsamiqSansItalic.otf";
            } else {
                fontToLoad = "BalsamiqSansRegular.otf";
            }

            if (fonts.containsKey(fontToLoad)) {
                return fonts.get(fontToLoad);
            } else {
                String fontPath = fontToLoad;
                Typeface tf = Typeface.createFromAsset(_context.getAssets(), fontPath);
                fonts.put(fontToLoad, tf);
                return tf;
            }
        }

        return null;
    }
}
