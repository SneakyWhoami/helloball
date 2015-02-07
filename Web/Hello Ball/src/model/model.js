'use strict';

var Ball = function (x, y, radius, color) {
    this.x = x;
    this.y = y;
    this.radius = radius;
    this.color = color;
    this.index;
    this.hits = [];
    this.numHits = 0;
};

Ball.prototype.setPosition = function (x, y) {
    this.x = x;
    this.y = y;
};

Ball.prototype.modRadius = function (radiusDelta) {
    this.radius += radiusDelta;
};

Ball.prototype.setColor = function (color) {
    this.radius = radius;
};

var Model = function () {
    this.balls = [];
    this.ballChanged = null;
};

Model.prototype.addBall = function (ball) {
    ball.index = this.balls.length + '';
    this.balls.push(ball);
    var changed = this.penetrationTest();
    changed.push(ball.index);
    if (this.ballChanged) {
        this.ballChanged(changed);
    }
};

Model.prototype.removeBallAtIndex = function (index) {
    this.balls.splice(index, 1);
    this.penetrationTest();
};

Model.prototype.setBallPosition = function (ballIndex, x, y) {
    this.balls[ballIndex].setPosition(x, y);
    var changed = this.penetrationTest();
    changed.push(ballIndex);
    if (this.ballChanged) {
        this.ballChanged(changed);
    }
};

Model.prototype.modBallRadius = function (ballIndex, radiusDelta) {
    this.balls[ballIndex].modRadius(radiusDelta);
    var changed = this.penetrationTest();
    changed.push(ballIndex);
    if (this.ballChanged) {
        this.ballChanged(changed);
    }
};

Model.prototype.setBallColor = function (ballIndex, color) {
    this.balls[ballIndex].setColor(color);
    if (this.ballChanged) {
        this.ballChanged(ballIndex);
    }
};

Model.prototype.hitTest = function (x, y) {
    return this.balls.filter(function (ball) {
        return Math.pow((x - ball.x), 2) + Math.pow((y - ball.y), 2) < Math.pow(ball.radius, 2);
    }).map(function (ball) {
        return ball.index;
    });
};

Model.prototype.penetrationTest = function () {
    var changed = [];
    this.balls.forEach(function (ball) {
        ball.hits = [];
    });
    var firstIdx = 0, secondIdx = 1;
    for (firstIdx = 0; firstIdx < this.balls.length - 1; firstIdx += 1) {
        var ball1 = this.balls[firstIdx];
        for (secondIdx = firstIdx + 1; secondIdx < this.balls.length; secondIdx += 1) {
            var ball2 = this.balls[secondIdx];
            if (Math.pow((ball1.x - ball2.x), 2) + Math.pow((ball1.y - ball2.y), 2) < Math.pow(ball1.radius + ball2.radius, 2)) {
                ball1.hits.push(ball2.index);
                ball2.hits.push(ball1.index);
            }
        }
    }

    this.balls.forEach(function (ball) {
        if (ball.hits.length !== ball.numHits) {
            ball.numHits = ball.hits.length;
            changed.push(ball.index);
        }
    });
    return changed;
};

Model.prototype.getDisplayList = function (ballIndex) {
    var res;
    if (typeof ballIndex === 'Number') {
        res = [this.balls[ballIndex]];
    } if (typeof ballIndex === 'Array') {
        res = this.balls.filter(function (ball) {
            return ballIndex.indexOf(ball.index) !== -1;
        });
    } else {
        res = this.balls;
    }
    return res;
};