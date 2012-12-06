var canvas_size = 500;
var scale_m_size = 200;
var center_x = canvas_size/2;
var center_y = canvas_size/2;
var xv_x_mm = 0, xv_y_mm = 0;
var xv_x = 0, xv_y = 0;
var xv_t = 0;
var xvImage;
var deadZone = 100;

function drawSquares(context) {
    context.save();
    context.translate(center_x, center_y);

    context.beginPath();
    context.strokeStyle = '#0099ff';
    for (x = 0; x < (canvas_size/2+center_x); x += scale_m_size) {
        context.moveTo(x, -center_y);
        context.lineTo(x, canvas_size-center_y);
    }
    for (x = -scale_m_size; x > -canvas_size/2-center_x && x > -canvas_size/2; x -= scale_m_size) {
        context.moveTo(x, -center_y);
        context.lineTo(x, canvas_size-center_y);
    }
    for (y = 0; y < canvas_size/2+center_y && y < center_y; y += scale_m_size) {
        context.moveTo(-center_x, y);
        context.lineTo(canvas_size-center_x, y);
    }
    for (y = -scale_m_size; y > -canvas_size/2-center_y && y > -center_y; y -= scale_m_size) {
        context.moveTo(-center_x, y);
        context.lineTo(canvas_size-center_x, y);
    }
    context.stroke();

    if (center_x > -scale_m_size/2 && center_x < canvas_size+scale_m_size/2 && center_y > -scale_m_size/4 && center_y < canvas_size+scale_m_size/4) {
        context.beginPath();
        context.strokeStyle = '#ff1100';
        context.moveTo(-scale_m_size/2, 0);
        context.lineTo(scale_m_size/2, 0);
        context.moveTo(0, -scale_m_size/2);
        context.lineTo(0, scale_m_size/2);
        context.stroke();
    }

    context.restore();
}

function drawXV(context) {
    context.save();
    context.translate(xv_x*scale_m_size+center_x, xv_y*scale_m_size+center_y);
    context.rotate(-xv_t);
    context.translate(-0.15*scale_m_size, -0.15*scale_m_size);
    context.drawImage(xvImage, 0, 0, 0.3*scale_m_size, 0.3*scale_m_size);
    context.restore();
}

function drawScan(context) {
    context.strokeStyle = '#0000ff';
    for (x = 110; x < (canvas_size-110); x += 10) {
        deltaX = Math.floor(Math.random()*11) - 5;
        deltaY = Math.floor(Math.random()*11) - 5;
        context.fillRect(x+deltaX,110+deltaY,2,2);
        context.fillRect(110+deltaX,x+deltaY,2,2);
        context.fillRect(canvas_size-110-deltaX,x+deltaY,2,2);
        context.fillRect(x+deltaX,canvas_size-110-deltaY,2,2);
    }
}

function drawScale(context) {
    if (scale_m_size <= 250) {
        context.beginPath();
        context.rect(100, 20, scale_m_size, 10);
        context.strokeStyle = '#CC0000';
        context.fillStyle = '#FF8080';
        context.lineWidth = 1;
        context.fill();
        context.stroke();   

        context.beginPath();
        context.fillStyle = '#CC0000';
        context.fillText('1m', 90+scale_m_size/2, 15); 
        context.stroke();
    }

    context.beginPath();
    context.strokeStyle = '#0099ff';        
    context.moveTo(100, 25);
    context.lineTo(350, 25);
    for (x = 100; x <= canvas_size-100 && x <= 350; x += scale_m_size) {
        context.moveTo(x, 10);
        context.lineTo(x, 40);
    }
    context.stroke();

    context.font = 'italic 12pt Calibri';
    context.fillStyle = '#606060';
    context.fillText('Échelle', 30, 30); 
    text = '1m = ' + scale_m_size + ' pixels'
    context.fillText(text, 365, 30);
}

function refreshDrawing() {
    xv_t += Math.PI/30.0;
    if (xv_t > 2*Math.PI)
        xv_t -= 2*Math.PI;
    xv_x += 0.06 * Math.cos(xv_t);
    xv_y -= 0.06 * Math.sin(xv_t);  

    if (xv_x*scale_m_size+center_x < deadZone) 
        center_x -= (xv_x*scale_m_size+center_x) - deadZone;
    else if  (xv_x*scale_m_size+center_x > canvas_size-deadZone)
        center_x -= (xv_x*scale_m_size+center_x) - (canvas_size-deadZone);

    if (xv_y*scale_m_size+center_y < deadZone) 
        center_y -= (xv_y*scale_m_size+center_y) - deadZone;
    else if  (xv_y*scale_m_size+center_y > canvas_size-deadZone)
        center_y -= (xv_y*scale_m_size+center_y) - (canvas_size-deadZone);

    var canvas = document.getElementById("xv");
    var context = canvas.getContext("2d");

    document.getElementById("xPos").innerHTML = Math.round(xv_x*100000)/100;
    document.getElementById("yPos").innerHTML = Math.round(-xv_y*100000)/100;
    document.getElementById("thetaPos").innerHTML = ((Math.round(xv_t*100))/100);

    context.clearRect(0, 0, canvas.width, canvas.height);
    drawSquares(context);
    drawXV(context);
//    drawScan(context);

    canvas = document.getElementById("xv-scale");
    context = canvas.getContext("2d");
    context.clearRect(0, 0, 500, 50);
    drawScale(context);
}

var periodicFunction;
function updatePeriod() {
    clearInterval(periodicFunction);
    periodicFunction = setInterval(refreshDrawing, (document.getElementById("periode").value*1000));
}

function updateScale() {
    scale = parseInt(document.getElementById("echelle").value);
    if (scale > 10 && scale < 500) {
        scale_m_size = scale;
    }
    refreshDrawing();
}

