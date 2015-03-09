"use strict";

function measureText (context, fontFamily, fontSize, bold, italic, text) {
    var fontStr = "";
    if (bold) {
        fontStr += "Bold ";
    }
    if (italic) {
        fontStr += "Italic ";
    }
    fontStr += fontSize + "px ";
    fontStr += '"' + fontFamily + '"';
    context.font = fontStr;

    if (fontSize === 72) {
        context.fillText(text, 10, 140);
    }
    return context.measureText(text).width;
}