(function () {
    'use strict';

    var model;
    var svgroot = document.getElementById('svgmain');
    var selectionID;

    var populateModel = function (model) {
        var getRandInt = function (min, max) {
            return Math.floor(Math.random() * (max - min)) + min;
        };

        var ballNumber = 50;
        var ballMinRadius = 10;
        var ballMaxRadius = 40;
        var screenWidth = 800;
        var screenHeight = 600;
        var i, ball;

        for (i = 0; i < ballNumber; i += 1) {
            var radius = getRandInt(ballMinRadius, ballMaxRadius);
            ball = new Ball(
                getRandInt(radius, screenWidth - radius),
                getRandInt(radius, screenHeight - radius),
                radius,
                getRandInt(0, 16777215)
            );
            model.addBall(ball);
        }
    };

    var main = function () {

        model = new Model();
        populateModel(model);

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

        var mouseup = Rx.Observable.fromEvent(document, 'mouseup');
        var mousemove = Rx.Observable.fromEvent(document, 'mousemove');
        var mousedown = Rx.Observable.fromEvent(document, 'mousedown');

        var mousedrag = mousedown.selectMany(function (md) {
            var ballIDs = model.hitTest(md.offsetX, md.offsetY);
            if (!ballIDs.length) {
                return [];
            }
            selectionID = ballIDs[0];
            var target = document.getElementById('ball' + ballIDs[0]);

            var circleX = parseInt(target.getAttribute("cx"));
            var circleY = parseInt(target.getAttribute("cy"));

            // calculate offsets when mouse down
            var startX = md.offsetX, startY = md.offsetY;
            // Calculate delta with mousemove until mouseup
            return mousemove.map(function (mm) {
                (mm.preventDefault) ? mm.preventDefault() : event.returnValue = false;
                return {
                    targetID: ballIDs[0],
                    newX: mm.clientX - startX + circleX,
                    newY: mm.clientY - startY + circleY
                };
            }).takeUntil(mouseup);
        });

        // Update position
        mousedrag.subscribe(function (data) {
            model.setBallPosition(data.targetID, data.newX, data.newY);
        });

        var displayListStartStream = Rx.Observable.just(model.getDisplayList());
        var ballsChangedStream = Rx.Observable.create(function (observer) {
            var handler = function (ballIDs) {
                observer.onNext(ballIDs);
            };
            model.ballChanged = handler;
        });

        var displayListStream = ballsChangedStream.map(function (ballIDs) {
            return model.getDisplayList(ballIDs);
        }).merge(displayListStartStream);

        displayListStream.subscribe(renderDisplayList.bind(undefined, svgroot));
    };

    main();

}());