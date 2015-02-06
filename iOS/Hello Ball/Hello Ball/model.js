var Ball = function (x, y, radius, color) {
    this.x = x;
    this.y = y;
    this.radius = radius;
    this.color = color;
}

var Model = function () {
    this.delegate = null;
    this.selectedBallIndex = -1;
    
    this.balls = [
        new Ball(100, 100, 20, 0xFF0000),
        new Ball(200, 100, 30, 0x00FF00),
        new Ball(400, 300, 40, 0x0000FF),
        new Ball(250, 180, 50, 0xFF00FF),
        new Ball(120, 340, 20, 0x0055FF),
        new Ball(500, 600, 50, 0xBB2299),
        new Ball(400, 300, 40, 0x0000FF),
        new Ball(400, 300, 40, 0x0000FF),
        new Ball(400, 300, 40, 0x0000FF),
        new Ball(400, 300, 40, 0x0000FF),
        new Ball(400, 300, 40, 0x0000FF),
        new Ball(400, 300, 40, 0x0000FF),
        new Ball(400, 300, 40, 0x0000FF),
        new Ball(400, 300, 40, 0x0000FF),
        new Ball(400, 300, 40, 0x0000FF)
    ];
    
    this.setBallPosition = function (ballIndex, x, y) {
        this.balls[ballIndex].x = x;
        this.balls[ballIndex].y = y;
        if (this.delegate) {
            this.delegate.ballsChanged(this.balls);
        }
    }
    
    this.hitTest = function (x, y) {
        var i;
        var ball;
        for (i = this.balls.length - 1; i >= 0; i--) {
            ball = this.balls[i];
            if ((x >= (ball.x - ball.radius)) && (x <= (ball.x + ball.radius)) && (y >= (ball.y - ball.radius)) && (y <= (ball.y + ball.radius))) {
                return i;
            }
        }
        return -1;
    }
    
    this.setDelegate = function (delegate) {
        this.delegate = delegate;
        this.delegate.ballsChanged(this.balls);
        this.delegate.selectedBallChanged(this.selectedBallIndex);
    }
    
    this.selectBall = function (index) {
        this.selectedBallIndex = index;
        if (this.delegate) {
            this.delegate.selectedBallChanged(this.selectedBallIndex);
        }
    }
    
    this.mouseDown = function (x, y) {
        this.selectBall(this.hitTest(x, y));
    }
    
    this.mouseMove = function (dx, dy) {
        if (this.selectedBallIndex != -1) {
            var selectedBall = this.balls[this.selectedBallIndex];
            this.setBallPosition(this.selectedBallIndex, selectedBall.x + dx, selectedBall.y + dy);
        }
    }
    
    this.mouseUp = function (x, y) {
        this.selectBall(-1);
    }
}

var model = new Model();
