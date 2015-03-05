'use strict';

var BALL_COUNT = 100;
var PENETRATION_TEST_ENABLED = true;

var Ball = function (x, y, radius) {
    this.x = x;
    this.y = y;
    this.radius = radius;
};

Ball.prototype.setPosition = function (x, y) {
    this.x = x;
    this.y = y;
};

Ball.prototype.setRadius = function (radius) {
    this.radius = radius;
};

var Model = function () {
    this.balls = [];
    this.viewWidth = 0;
    this.viewHeight = 0;
};

Model.prototype.addBall = function (ball) {
    this.balls.push(ball);
};

Model.prototype.setBallPosition = function (ballIndex, x, y) {
    x = Math.max(Math.min(x, this.viewWidth), 0);
    y = Math.max(Math.min(y, this.viewHeight), 0);
    this.balls[ballIndex].setPosition(x, y);
};

Model.prototype.setBallRadius = function (ballIndex, radius) {
    this.balls[ballIndex].setRadius(radius);
};

Model.prototype.populate = function (viewWidth, viewHeight) {
    this.viewWidth = viewWidth;
    this.viewHeight = viewHeight;
    
    var getRandInt = function (min, max) {
        return Math.floor(Math.random() * (max - min)) + min;
    };
    
    var ballCount = BALL_COUNT;
    var ballMinRadius = 18;
    var ballMaxRadius = 44;
    var i, ball;
    
    for (i = 0; i < ballCount; i += 1) {
        var radius = getRandInt(ballMinRadius, ballMaxRadius);
        ball = new Ball(
                        getRandInt(radius, viewWidth - radius),
                        getRandInt(radius, viewHeight - radius),
                        radius
                        );
        this.addBall(ball);
    }
};

var hitTest = function (balls, x, y) {
    var ballsHit = [];
    var i;
    for (i = 0; i < balls.length; i++) {
        if (Math.pow((x - balls[i].x), 2) + Math.pow((y - balls[i].y), 2) < Math.pow(balls[i].radius, 2)) {
            ballsHit.push({ index:i, offsetX: (x - balls[i].x), offsetY: (y - balls[i].y) });
        }
    }
    return ballsHit;
};

var penetrationTest = function (balls) {
    var results = [];
    var i, j;
    for (i = 0; i < balls.length; i++) {
        results.push([]);
    }
    for (i = 0; i < balls.length - 1; i++) {
        for (j = i + 1; j < balls.length; j++) {
            if (Math.pow((balls[i].x - balls[j].x), 2) + Math.pow((balls[i].y - balls[j].y), 2) < Math.pow(balls[i].radius + balls[j].radius, 2)) {
                results[i].push(j);
                results[j].push(i);
            }
        }
    }
    return results;
};

var EventsCounter = function (delegate) {
    this.eventsCount = 0;
    this.lastTime = 0;
    this.delegate = delegate;
};

EventsCounter.prototype.countEvent = function () {
    this.eventsCount += 1;
    var diffTime = Date.now() - this.lastTime;
    if (diffTime > 200) {
        var eps = this.eventsCount * 1000 / diffTime;
        this.delegate.eventsPerSecond(eps);
        this.lastTime = Date.now();
        this.eventsCount = 0;
    }
};

var DisplayList = function (ballCount) {
    this.items = [];
    this.itemsChanged = {};
    var i = 0;
    for (i = 0; i < ballCount; i++) {
        this.items.push({ x: -1, y: -1, radius: -1, color: -1 });
    }
};

DisplayList.prototype.setBall = function (ballIndex, x, y, radius, color) {
    var newObj = { x: x, y: y, radius: radius, color: color };
    var oldObj = this.items[ballIndex];
    for (var key in newObj) {
        if (newObj[key] != oldObj[key]) {
            this.items[ballIndex] = newObj;
            this.itemsChanged[ballIndex] = true;
            break;
        }
    }
};

DisplayList.prototype.getChangedItems = function () {
    var result = {};
    for (var ballIndex in this.itemsChanged) {
        result[ballIndex] = this.items[ballIndex];
    }
    this.itemsChanged = {};
    return result;
};

var Controller = function (model, delegate) {
    this.delegate = delegate;
    this.model = model;
    this.selectedBall = null;
    this.mouseIsDown = false;
    this.eps = new EventsCounter(delegate);
    this.displayList = new DisplayList(this.model.balls.length);
};

Controller.prototype.mouseDown = function (x, y) {
    var ballsHit = hitTest(this.model.balls, x, y);
    if (ballsHit.length > 0) {
        this.selectedBall = ballsHit[ballsHit.length - 1];
    } else {
        this.selectedBall = -1;
    }
    this.mouseIsDown = true;
    this.makeDisplayList();
    this.eps.countEvent();
};

Controller.prototype.mouseMove = function (x, y) {
    var currentTime = Date.now();
    if (this.mouseIsDown && this.selectedBall) {
        this.model.setBallPosition(this.selectedBall.index, x - this.selectedBall.offsetX, y - this.selectedBall.offsetY);
        this.makeDisplayList();
    }
    this.eps.countEvent();
};

Controller.prototype.mouseUp = function (x, y) {
    this.mouseIsDown = false;
    if (this.selectedBall) {
        this.makeDisplayList();
    }
    this.eps.countEvent();
};

Controller.prototype.makeDisplayList = function () {
    var penetrationResults = PENETRATION_TEST_ENABLED ? penetrationTest(this.model.balls) : null;
    
    var i;
    var color;
    var numHits;
    for (i = 0; i < this.model.balls.length; i++) {
        if (this.selectedBall && this.selectedBall.index == i) {
            if (this.mouseIsDown) {
                color = 0x44FF44;
            } else {
                color = 0x00BB00;
            }
        } else {
            numHits = PENETRATION_TEST_ENABLED ? penetrationResults[i].length : 0;
            if (numHits == 0) {
                color = 0x0000FF;
            } else if (numHits == 1) {
                color = 0xFF0000;
            } else if (numHits == 2) {
                color = 0xFF6666;
            } else {
                color = 0xFFBBBB;
            }
        }
        this.displayList.setBall(i, this.model.balls[i].x, this.model.balls[i].y, this.model.balls[i].radius, color);
    }

    this.delegate.displayListChanged(this.displayList.getChangedItems());
};

Controller.prototype.doText = function () {
	var m = this.delegate.measureText("Balsamiq Sans", 18, false, false, "AVAVAVAVA");
	this.delegate.log("text width: " + m);
};

Controller.prototype.task = function () {
    var time = Date.now();
    var isOddSecond = (Math.floor(time / 1000) % 2) == 1;
    var phase = (time % 1000) / 1000;
    if (isOddSecond) {
        phase = 1 - phase;
    }
    this.delegate.phaseChanged(phase);
};

var initApp = function (viewWidth, viewHeight, delegate) {
    var model = new Model();
    model.populate(viewWidth, viewHeight);
    delegate.ballCountChanged(model.balls.length);
    var controller = new Controller(model, delegate);
    controller.makeDisplayList();
    controller.doText();
    return controller;
};
