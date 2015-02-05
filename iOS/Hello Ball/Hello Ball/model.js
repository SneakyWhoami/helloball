var Ball = function (x, y, radius, color) {
    this.x = x;
    this.y = y;
    this.radius = radius;
    this.color = color;
}

var Model = function () {
    this.balls = [
        new Ball(100, 100, 20, 0xFF0000),
        new Ball(200, 100, 30, 0x00FF00),
        new Ball(200, 300, 40, 0x0000FF)
    ];
    
    this.setBallPosition = function (ballIndex, x, y) {
        this.balls[ballIndex].x = x;
        this.balls[ballIndex].y = y;
    }
    
    this.findBallAtPosition = function (x, y) {
        var i;
        var ball;
        for (i = this.balls.length - 1; i >= 0; i--) {
            ball = this.balls[i];
            if ((x >= (ball.x - ball.radius)) && (x <= (ball.x + ball.radius)) && (y >= (ball.y - ball.radius)) && (y >= (ball.y + ball.radius))) {
                return i;
            }
        }
        return -1;
    }
}

var model = new Model();
