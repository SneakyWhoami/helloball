(function () {
    'use strict';

    var svgroot = document.getElementById('svgmain');

    var delegate = {
        displayListChanged: function () {},
        eventsPerSecond: function () {},
        ballCountChanged: function () {}
    };

    var displayListStream = Rx.Observable.create(function (observer) {
        var handler = function (displayList) {
            observer.onNext(displayList);
        };
        delegate.displayListChanged = handler;
    }).subscribe(renderDisplayList.bind(undefined, svgroot));

    var epsStream = Rx.Observable.create(function (observer) {
        var handler = function (eps) {
            observer.onNext(eps);
        };
        delegate.eventsPerSecond = handler;
    }).subscribe(renderEps.bind(undefined, svgroot));

    var controller = initApp(800, 600, delegate);

    // Keyboard Events: UP and DOWN ARROWS only...
    Rx.Observable.fromEvent(window, 'keydown')
        .filter(function (kev) {
            return [38, 40].indexOf(kev.keyCode) !== -1 ? kev : null;
        })
        .subscribe(function (data) {
            if (data.keyCode === 38) {
                model.modBallRadius(selectionID, 4);
            } else {
                model.modBallRadius(selectionID, -4);
            }
        });

    Rx.Observable.fromEvent(document, 'mousedown')
        .subscribe(function (ev) {
            controller.mouseDown(ev.offsetX, ev.offsetY);
        });

    Rx.Observable.fromEvent(document, 'mousemove')
        .subscribe(function (mm) {
            controller.mouseMove(mm.clientX, mm.clientY)
        });

    Rx.Observable.fromEvent(document, 'mouseup')
        .subscribe(function (mm) {
            controller.mouseUp(mm.clientX, mm.clientY)
        });

}());