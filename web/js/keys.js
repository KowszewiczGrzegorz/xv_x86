var keys = new Array();

function doKeyDown(evt){
    alert("key down");
    keys[evt.keyCode] = true;
}

function doKeyUp(evt){
    alert("key up");
    keys[evt.keyCode] = false;
}

function move() {   
    if (38 in keys && keys[38]){ //up
        document.getElementById('keyEvent').innerHTML = "Up was pushed";
    }
    if (40 in keys && keys[40]){ //down
        document.getElementById('keyEvent').innerHTML = "Down was pushed";        
    }
    if (37 in keys && keys[37]){ //left
        document.getElementById('keyEvent').innerHTML = "Left was pushed";        
    }
    if (39 in keys && keys[39]){ //right
        document.getElementById('keyEvent').innerHTML = "Right was pushed";        
    }
}

function init() {
    canvas = document.getElementById("keys");
    ctx = canvas.getContext("2d");
    return setInterval(move, 500);
}

init();
window.addEventListener('keyup',doKeyUp,true);
window.addEventListener('keydown',doKeyDown,true);
