function modelStart(viewWidth, viewHeight) {
    
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
    
    ModelObserver.modelController = initApp(viewWidth, viewHeight, ModelObserver.delegate);

    ModelObserver.appController = {};
    
    ModelObserver.appController.mouseDown = function(x,y) {
        ModelObserver.modelController.mouseDown(x,y);
    };

    ModelObserver.appController.task = function() {
        ModelObserver.modelController.task();
    };

    ModelObserver.appController.mouseUp = function(x,y) {
        ModelObserver.modelController.mouseUp(x,y);
    };
    
    ModelObserver.appController.mouseMove = function(x,y) {
        ModelObserver.modelController.mouseMove(x,y);
    };
}
