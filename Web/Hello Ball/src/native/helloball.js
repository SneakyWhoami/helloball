(function () {
    'use strict';

    var getRandInt = function (min, max) {
        return Math.floor(Math.random() * (max - min)) + min;
    };

    var ballNumber = 1000;
    var ballMinRadius = 2;
    var ballMaxRadius = 10;
    var screenWidth = 800;
    var screenHeight = 600;

    var model;
    var svgroot = document.getElementById('svgmain');
    var selectionID = -1;

    var main = function () {

        model = new Model();
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

        model.ballChanged = function (ballIDs) {
            renderDisplayList(svgroot, model.getDisplayList(ballIDs));
        };

        var mouseup = Rx.Observable.fromEvent(document, 'mouseup');
        var mousemove = Rx.Observable.fromEvent(document, 'mousemove');
        var mousedown = Rx.Observable.fromEvent(document, 'mousedown');
        var keypress = Rx.Observable.fromEvent(window, 'keydown');
        var arrowKeyPress = keypress.filter(function (kev) {
            return [38, 40].indexOf(kev.keyCode) !== -1 ? kev : null;
        });
        arrowKeyPress.subscribe(function (data) {
            if (data.keyCode === 38) {
                model.modBallRadius(selectionID, 4);
            } else {
                model.modBallRadius(selectionID, -4);
            }
        });

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
            return mousemove.select(function (mm) {
                (mm.preventDefault) ? mm.preventDefault() : event.returnValue = false;
                return {
                    x: mm.clientX - startX,
                    y: mm.clientY - startY,
                    target: target,
                    targetID: ballIDs[0],
                    circleX: circleX,
                    circleY:circleY
                };
            }).takeUntil(mouseup);
        });

        // Update position
        var subscription = mousedrag.subscribe(function (data) {
            model.setBallPosition(data.targetID, (data.x + data.circleX), (data.y + data.circleY));
        });

        renderDisplayList(svgroot, model.getDisplayList());
    };

    main();

}());