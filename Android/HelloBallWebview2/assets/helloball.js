window.setTimeout(function () {
    'use strict';

    var svgroot = document.getElementById('svgmain');
    var canvas = document.getElementById('canvasfortext');
    var context = canvas.getContext("2d");

    var delegate = {
        displayListChanged: function () {},
        eventsPerSecond: function () {},
        ballCountChanged: function () {},
        phaseChanged: function () {},
        log: function (text) { console.log(text); },
        measureText: measureText.bind(null, context)
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


    var phaseStream = Rx.Observable.create(function (observer) {
        var handler = function (phase) {
            observer.onNext(phase);
        };
        delegate.phaseChanged = handler;
    }).subscribe(renderPhase.bind(undefined, svgroot));

    var rootRect = svgroot.getBoundingClientRect();
    var controller = initApp(rootRect.width, rootRect.height, delegate);

    var getOffset = function (e, touch) {
        var target = e.target || e.srcElement;
        var rect = svgroot.getBoundingClientRect();
        var offsetX = (e.clientX || e.touches[0].clientX) - rect.left;
        var offsetY = (e.clientY || e.touches[0].clientY) - rect.top;
        return {x: offsetX, y: offsetY};
    };

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
            var res = getOffset(ev);
            controller.mouseDown(res.x, res.y);
        });

    Rx.Observable.fromEvent(document, 'touchstart')
        .subscribe(function (ev) {
            var res = getOffset(ev, true);
            controller.mouseDown(res.x, res.y);
        });

    Rx.Observable.fromEvent(document, 'mousemove')
        .subscribe(function (mm) {
            var res = getOffset(mm);
            controller.mouseMove(res.x, res.y);
        });

    Rx.Observable.fromEvent(document, 'touchmove')
        .subscribe(function (mm) {
            var res = getOffset(mm, true);
            controller.mouseMove(res.x, res.y);
        });

    Rx.Observable.fromEvent(document, 'mouseup')
        .subscribe(function (mm) {
            controller.mouseUp()
        });

    Rx.Observable.fromEvent(document, 'touchend')
        .subscribe(function (mm) {
            controller.mouseUp()
        });

    Rx.Observable.fromEvent(document, 'touchcancel')
        .subscribe(function (mm) {
            controller.mouseUp()
        });

    setInterval(function () {
        controller.task();
    }, 30);

}, 1000);