var joystickImage;
var joystickTopImage;
var mouseDown;
var X = 0;
var Y = 0;
var offsetX = 0;
var offsetY = 0;

function drawJoystickTop(x, y) {
    var canvas = document.getElementById("joystick");
    var context = canvas.getContext('2d');
    context.drawImage(joystickImage, 0, 0);
    context.drawImage(joystickTopImage, 115 + x, 125 + y);
}

function clickDown(event) {
    mouseDown = true;

    var canvas = document.getElementById("joystick");
    offsetX = event.x - canvas.offsetLeft - 155 - X;
    offsetY = event.y - canvas.offsetTop - 165 - Y;

    var rau = Math.sqrt(offsetX*offsetX + offsetY*offsetY);
    var theta = Math.atan2(offsetY, offsetX);

    if (rau > 40)
        mouseDown = false;
}

function clickUp(event) {
    mouseDown = false;
    X = 0;
    Y = 0;
    drawJoystickTop(X, Y);
}

function mouseMove(event) {
    if (mouseDown == true) {
        var canvas = document.getElementById("joystick");
        var x = event.x - canvas.offsetLeft - 155 - offsetX;
        var y = event.y - canvas.offsetTop - 165 - offsetY;
        var rau = Math.sqrt(x*x + y*y);
        var theta = Math.atan2(y, x);

        if (rau > 50)
            rau = 50;

        X = rau * Math.cos(theta);
        Y = rau * Math.sin(theta);
        drawJoystickTop(X, Y);

        theta = -(Math.PI/2) - theta;
        if (theta < -Math.PI)
            theta = (2*Math.PI) + theta;

        var leftVel;
        var rightVel;
        if (theta > 0) {
            if (theta < (Math.PI/2)) {
                rightVel = 300.0;
                leftVel = 300.0 - (600.0 * (theta / (Math.PI/2)));
            } else {
                leftVel = -300.0;
                rightVel = 300 - (600 * ((theta-(Math.PI/2)) / (Math.PI/2)));
            }
        } else {
            if (theta > (-Math.PI/2)) {
                leftVel = 300.0;
                rightVel =  300.0 + (600.0 * (theta / (Math.PI/2)));
            } else {
                rightVel = -300.0;
                leftVel = 300 + (600 * ((theta+(Math.PI/2)) / (Math.PI/2)));
            }
        }
        leftVel = leftVel * (rau / 50.0);
        rightVel = rightVel * (rau / 50.0);

        document.getElementById('velocities').innerHTML = "leftVel = " + leftVel + ", rightVel = " + rightVel;
    }
}

function initJoystick() {
    var canvas = document.getElementById('joystick');
    var context = canvas.getContext('2d');

    joystickImage = new Image();
    joystickImage.onload = function() {
        context.drawImage(joystickImage, 0, 0);
        context.drawImage(joystickTopImage, 115, 125);
    };
    joystickImage.src = 'img/joystick.png';

    joystickTopImage = new Image();
    joystickTopImage.onload = function() {
        context.drawImage(joystickTopImage, 115, 125);
    };
    joystickTopImage.src = 'img/joystick-top.png';

    mouseDown = false;

    canvas.addEventListener("mousedown", clickDown, false);
    canvas.addEventListener("mouseup", clickUp, false);
    canvas.addEventListener("mousemove", mouseMove, false);
}
