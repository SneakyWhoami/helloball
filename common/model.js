'use strict';

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
};

Model.prototype.addBall = function (ball) {
    this.balls.push(ball);
};

Model.prototype.setBallPosition = function (ballIndex, x, y) {
    this.balls[ballIndex].setPosition(x, y);
};

Model.prototype.setBallRadius = function (ballIndex, radius) {
    this.balls[ballIndex].setRadius(radius);
};

Model.prototype.hitTest = function (x, y) {
    var ballsHit = [];
    var i;
    for (i = 0; i < this.balls.length; i++) {
        if (Math.pow((x - this.balls[i].x), 2) + Math.pow((y - this.balls[i].y), 2) < Math.pow(this.balls[i].radius, 2)) {
            ballsHit.push(i);
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
}

var makeDisplayList = function (balls, penetrationResults, selectedBallIndex, mouseDown) {
    var displayList = [];
    var i;
    var color;
    var numHits;
    for (i = 0; i < balls.length; i++) {
        if (selectedBallIndex == i) {
            if (mouseDown) {
                color = 0x44FF44;
            } else {
                color = 0x00BB00;
            }
        } else {
            numHits = penetrationResults[i].length;
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
        displayList.push({
                         x: balls[i].x,
                         y: balls[i].y,
                         radius: balls[i].radius,
                         color: color
        });
    }
    return displayList;
}

var EventsCounter = function (delegate) {
    this.eventsCount = 0;
    this.lastTime = 0;
    this.delegate = delegate;
}

EventsCounter.prototype.countEvent = function () {
    this.eventsCount += 1;
    var diffTime = Date.now() - this.lastTime;
    if (diffTime > 200) {
        if (this.delegate) {
            var eps = this.eventsCount * 1000 / diffTime;
            this.delegate.eventsPerSecond(eps);
        }
        this.lastTime = Date.now();
        this.eventsCount = 0;
    }
}

var Controller = function (model, delegate) {
    this.delegate = delegate;
    this.model = model;
    this.selectedBallIndex = -1;
    this.mouseIsDown = false;
    this.eps = new EventsCounter(delegate);
}

Controller.prototype.mouseDown = function (x, y) {
    var ballsHit = this.model.hitTest(x, y);
    if (ballsHit.length > 0) {
        this.selectedBallIndex = ballsHit[ballsHit.length - 1];
    } else {
        this.selectedBallIndex = -1;
    }
    this.mouseIsDown = true;
    this.makeDisplayList();
    this.eps.countEvent();
}

Controller.prototype.mouseMove = function (x, y) {
    if (this.mouseIsDown && this.selectedBallIndex) {
        this.model.setBallPosition(this.selectedBallIndex, x, y);
        this.makeDisplayList();
    }
    this.eps.countEvent();
}

Controller.prototype.mouseUp = function (x, y) {
    this.mouseIsDown = false;
    if (this.selectedBallIndex) {
        this.makeDisplayList();
    }
    this.eps.countEvent();
}

Controller.prototype.makeDisplayList = function () {
    var penetrationResults = penetrationTest(this.model.balls);
    var displayList = makeDisplayList(this.model.balls, penetrationResults, this.selectedBallIndex, this.mouseIsDown);
    if (this.delegate) {
        this.delegate.displayListChanged(displayList);
    }
}

var populateModel = function (model, viewWidth, viewHeight) {
    var getRandInt = function (min, max) {
        return Math.floor(Math.random() * (max - min)) + min;
    };
    
    var ballCount = 50;
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
        model.addBall(ball);
    }
}

var initApp = function (viewWidth, viewHeight, delegate) {
    var model = new Model();
    populateModel(model, viewWidth, viewHeight);
    var controller = new Controller(model, delegate);
    controller.makeDisplayList();
    return controller;
}
