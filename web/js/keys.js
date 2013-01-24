var keys = new Array();

function doKeyDown(evt){
    keys[evt.keyCode] = true;
}

function doKeyUp(evt){
    keys[evt.keyCode] = false;
}

function move() {   
    if (38 in keys && keys[38]){ //up
        
    }
    if (40 in keys && keys[40]){ //down
        
    }
    if (37 in keys && keys[37]){ //left
        
    }
    if (39 in keys && keys[39]){ //right
        
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
