var keys = new Array();

function doKeyDown(evt){
    keys[evt.keyCode] = true;
    move();
}

function doKeyUp(evt){
    keys[evt.keyCode] = false;
    move();
}

function move() {
    var i = 0; 
    if (38 in keys && keys[38]){ //up
	i = i + 1;
    }
    if (40 in keys && keys[40]){ //down
        i = i + 2;        
    }
    if (37 in keys && keys[37]){ //left
	i = i + 4;    
    }
    if (39 in keys && keys[39]){ //right
	i = i + 8;
    }

    var text;
    switch(i) {
    case 0: text = "no move"; break;
    case 1: text = "straight line"; break;
    case 2: text = "backwards"; break;
    case 3: text = "??? front and back ???"; break;
    case 4: text = "left a lot forward"; break;
    case 5: text = "left a bit forward"; break;
    case 6: text = "turn on place left"; break;
    case 7: text = "??? front and back ???"; break;
    case 8: text = "right a lot forward"; break;
    case 9: text = "right a bit forward"; break;
    case 10: text = "turn on place right"; break;
    case 11: text = "??? front and back ???"; break;
    case 12: text = "??? left & right ???"; break;
    case 13: text = "??? left & right ???"; break;
    case 14: text = "??? left & right ???"; break;
    case 15: text = "??? left & right ???"; break;
    case 16: text = "??? left & right ???"; break;
    default: text = "wtf ???"; break;
    }

    document.getElementById('keyEvent').innerHTML = text;

    var tilt = 0;
    if (90 in keys && keys[90]) // z
        tilt = tilt + 1;
    if (83 in keys && keys[83]) // s
        tilt = tilt + 2;
    
    switch(tilt) {
    case 0: text = "neutral"; break;
    case 1: text = "up"; break;
    case 2: text = "down"; break;
    case 3: text = "up & down ???"; break;
    default: text = "wtf ???"; break;
    }

    document.getElementById('tiltEvent').innerHTML = text;
}

function init() {
    setInterval(move, 500);
}

window.addEventListener('keyup',doKeyUp,true);
window.addEventListener('keydown',doKeyDown,true);
init();

