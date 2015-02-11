// For an introduction to the Blank template, see the following documentation:
// http://go.microsoft.com/fwlink/?LinkID=392286
(function () {
    "use strict";

    var app = WinJS.Application;
    var activation = Windows.ApplicationModel.Activation;

    app.onactivated = function (args) {
        if (args.detail.kind === activation.ActivationKind.launch) {
            if (args.detail.previousExecutionState !== activation.ApplicationExecutionState.terminated) {
                // TODO: This application has been newly launched. Initialize
                // your application here.
                var svg = document.getElementById("balls");
                var r = svg.getClientRects()[0];
                window.addEventListener("mousedown", delegate.onMouseDown);
                window.addEventListener("mousemove", delegate.onMouseMove);
                window.addEventListener("mouseup", delegate.onMouseUp);
                controller = initApp(r.width, r.height, delegate);
            } else {
                // TODO: This application has been reactivated from suspension.
                // Restore application state here.
            }
            args.setPromise(WinJS.UI.processAll());
        }
    };

    app.oncheckpoint = function (args) {
        // TODO: This application is about to be suspended. Save any state
        // that needs to persist across suspensions here. You might use the
        // WinJS.Application.sessionState object, which is automatically
        // saved and restored across suspension. If you need to complete an
        // asynchronous operation before your application is suspended, call
        // args.setPromise().
    };

    function int2color(intColor) {
        var hexStr = (+intColor).toString(16);
        return '#' + '000000'.substring(0, 6 - hexStr.length) + hexStr;
    };

    function populateBalls(count) {
        var svg = document.getElementById("balls");
        var i;
        var ball;
        for (i = 0; i < count; i++) {
            ball = document.createElementNS("http://www.w3.org/2000/svg", "circle");
            ball.id = "ball" + i;
            ball.setAttributeNS(null, "cx", 10);
            ball.setAttributeNS(null, "cy", 10);
            ball.setAttributeNS(null, "r", 30);
            ball.setAttributeNS(null, "stroke-width", "3px");
            svg.appendChild(ball);
        }
    };

    function updateBall(ballIndex, displayData) {
        var ball = document.getElementById("ball" + ballIndex);
        ball.setAttributeNS(null, "cx", displayData.x);
        ball.setAttributeNS(null, "cy", displayData.y);
        ball.setAttributeNS(null, "r", displayData.radius);
        ball.setAttributeNS(null, "fill", int2color(displayData.color));
    };

    var controller = null;

    var delegate = {
        ballCountChanged: function (count) {
            var svg = document.getElementById("balls");
            var i;
            var ball;
            for (i = 0; i < count; i++) {
                ball = document.createElementNS("http://www.w3.org/2000/svg", "circle");
                ball.id = "ball" + i;
                ball.setAttributeNS(null, "stroke-width", "3px");
                svg.appendChild(ball);
            }
        },
        eventsPerSecond: function (eps) {
            var p = document.getElementById("eps");
            p.textContent = "EPS: " + Math.round(eps);
//            console.log("eps: " + eps);
        },
        displayListChanged: function (displayList) {
            for (var ballIndex in displayList) {
                var ball = document.getElementById("ball" + ballIndex);
                var data = displayList[ballIndex];
                ball.setAttributeNS(null, "cx", data.x);
                ball.setAttributeNS(null, "cy", data.y);
                ball.setAttributeNS(null, "r", data.radius);
                ball.setAttributeNS(null, "fill", int2color(data.color));
            }
        },
        log: function (str) {
            console.log(str);
        },
        onMouseDown: function (event) {
            controller.mouseDown(event.clientX, event.clientY);
        },
        onMouseMove: function (event) {
            controller.mouseMove(event.clientX, event.clientY);
        },
        onMouseUp: function (event) {
            controller.mouseUp(event.clientX, event.clientY);
        }
    };

    app.start();
})();