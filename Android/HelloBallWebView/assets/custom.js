function modelStart(viewWidth, viewHeight) {

    var setContextTextStyle = function (p_context, p_fontFamily, p_fontSize, p_bold, p_italic) {
        var fontStr = "";
        if (p_bold) {
            fontStr += "Bold ";
        }
        if (p_italic) {
            fontStr += "Italic ";
        }
        fontStr += p_fontSize + "px ";
        fontStr += '"' + p_fontFamily + '"';
        p_context.font = fontStr;
    };

    var measureText = function(p_fontFamily, p_fontSize, p_bold, p_italic, text) {
        var canvas = document.getElementById("textMeterCanvas");
        var g = canvas.getContext("2d");
        setContextTextStyle(g, p_fontFamily, p_fontSize, p_bold, p_italic);
        return g.measureText(text).width;
    };

    ModelObserver.delegate = {};
    ModelObserver.delegate.displayListChanged = function(dlObject)
    {
        ModelObserver.displayListChanged(JSON.stringify(dlObject));
    };
    
    ModelObserver.delegate.ballCountChanged = function(count)
    {
        ModelObserver.ballCountChanged(count);
    };

    ModelObserver.delegate.eventsPerSecond = function(eventsPerSecond)
    {
        ModelObserver.eventsPerSecond(eventsPerSecond);
    };

    ModelObserver.delegate.log = function(message)
    {
        ModelObserver.log(message);
    };
    
    ModelObserver.delegate.phaseChanged = function(value)
    {
        ModelObserver.phaseChanged(value);
    };
    
    ModelObserver.delegate.measureText = measureText;

    ModelObserver.controller = initApp(viewWidth, viewHeight, ModelObserver.delegate);
}
