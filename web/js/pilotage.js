
var joystickImage;
var joystickTopImage;
var joystickImage1;
var joystickImage2;
var joystickImage3;
var mouseDown;
var X = 0;
var Y = 0;
var offsetX = 0;
var offsetY = 0;
var leftVel = 0;
var rightVel = 0;
var periodicFunctionId;

var isIE = document.all;

function mouseX(event, canvas) {
    var x = isIE ? (event.clientX + document.body.scrollLeft) : event.pageX;
    return (x - canvas.offsetLeft);
}

function mouseY(event, canvas) {
    var y = isIE ? (event.clientY + document.body.scrollTop) : event.pageY;
    return (y - canvas.offsetTop);
}

function drawJoystick(x, y) {
    var canvas = document.getElementById("joystick");
    var context = canvas.getContext('2d');
    var baseX = 5;
    var baseY = 13;
    context.drawImage(joystickImage, 0, 0);
    var offset = 1 / 8;
    context.drawImage(joystickImage3, baseX + (x * offset), baseY + (y * offset));
    offset = 2 / 8;
    context.drawImage(joystickImage2, baseX + (x * offset), baseY + (y * offset));
    offset = 3 / 8;
    context.drawImage(joystickImage1, baseX + (x * offset), baseY + (y * offset));
    context.drawImage(joystickTopImage, 115 + x, 125 + y);
}

function clickDown(event) {
    var canvas = document.getElementById("joystick");
    var x = mouseX(event, canvas) - 68;
    var y = mouseY(event, canvas) - 52;
    var rau = Math.sqrt(x*x + y*y);

    if (rau < 25) {
        sendTestModeOnOff();
    } else {
        mouseDown = true;
        
        offsetX = mouseX(event, canvas) - 155 - X;
        offsetY = mouseY(event, canvas) - 165 - Y;
        
        rau = Math.sqrt(offsetX*offsetX + offsetY*offsetY);
        var theta = Math.atan2(offsetY, offsetX);
        
        if (rau > 40)
            mouseDown = false;
        
        periodicFunctionId = setInterval(periodicFunction, 500);
    }        
}

function clickUp(event) {
    if (mouseDown)
        clearInterval(periodicFunctionId);

    mouseDown = false;
    X = 0;
    Y = 0;
    drawJoystick(X, Y);
    document.getElementById('velocities').innerHTML = "leftVel = 0, rightVel = 0";
}

function mouseMove(event) {
    if (mouseDown == true) {
        var canvas = document.getElementById("joystick");
        var x = mouseX(event, canvas) - 155 - offsetX;
        var y = mouseY(event, canvas) - 165 - offsetY;
        var rau = Math.sqrt(x*x + y*y);
        var theta = Math.atan2(y, x);

        if (rau > 50)
            rau = 50;

        X = rau * Math.cos(theta);
        Y = rau * Math.sin(theta);
        drawJoystick(X, Y);

        theta = -(Math.PI/2) - theta;
        if (theta < -Math.PI)
            theta = (2*Math.PI) + theta;

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

        document.getElementById('velocities').innerHTML = "joystick leftVel = " + leftVel + ", rightVel = " + rightVel;
    }
}

function initJoystick() {
    var canvas = document.getElementById('joystick');
    var context = canvas.getContext('2d');

    joystickImage1 = new Image();
    joystickImage1.onload = function() {
        drawJoystick(0, 0);
    };
    joystickImage1.src = 'img/joystick_1.png';

    joystickImage2 = new Image();
    joystickImage2.onload = function() {
        drawJoystick(0, 0);
    };
    joystickImage2.src = 'img/joystick_2.png';

    joystickImage3 = new Image();
    joystickImage3.onload = function() {
        drawJoystick(0, 0);
    };
    joystickImage3.src = 'img/joystick_3.png';

    joystickTopImage = new Image();
    joystickTopImage.onload = function() {
        drawJoystick(0, 0);
    };
    joystickTopImage.src = 'img/joystick-top.png';

    joystickImage = new Image();
    joystickImage.onload = function() {
        drawJoystick(0, 0);
    };
    joystickImage.src = 'img/joystick.png';

    mouseDown = false;

    canvas.addEventListener("mousedown", clickDown, false);
    canvas.addEventListener("mouseup", clickUp, false);
    canvas.addEventListener("mousemove", mouseMove, false);
}

var cnt = 0;

function periodicFunction() {
    document.getElementById('realVelocities').innerHTML = "Commande leftVel = " + leftVel + ", rightVel = " + rightVel + " (" + cnt + ")";
    cnt = cnt + 1;
    sendVelocities();
}

function sendCommand(cmd) {
    var xmlhttp;
    if (window.XMLHttpRequest)
        xmlhttp = new XMLHttpRequest();
    else
        xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
    xmlhttp.open("POST", "sendCommand_simulated.php", true);
    xmlhttp.setRequestHeader('Content-Type','application/x-www-form-urlencoded');
    xmlhttp.send(cmd);
}

function sendVelocities() {
    sendCommand("SetMotor Speed 300 LWheelDist "+leftVel+" RWheelDist ="+rightVel);
}

var previousTestMode = 0;
function sendTestModeOnOff() {
    if (0 == previousTestMode) {
        sendCommand("TestMode On");
        previousTestMode = 1;
        alert('Set testmode On');
    } else {
        sendCommand("TestMode Off");
        previousTestMode = 0;
        alert('Set testmode Off');
    }
}