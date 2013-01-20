var gamepadActive = false;
 
function gamepadConnected (evt) {
    document.getElementById('gamepadInfo').innerHTML = "Gamepad connected";
    gamepadActive = true;
}

function gamepadDisconnected (evt) {
    document.getElementById('gamepadInfo').innerHTML = "Gamepad disconnected";
    gamepadActive = false;
}
 
function buttonPressed (evt, pressed) {
    document.getElementById('gamepadInfo').innerHTML = "Button " + evt + " was " + (pressed ? "pressed" : "released");
}

window.addEventListener('MozGamepadConnected', gamepadConnected);
window.addEventListener('MozGamepadDisconnected', gamepadDisconnected);
window.addEventListener("MozGamepadButtonDown", function(evt) { buttonPressed(evt, true); } );
window.addEventListener("MozGamepadButtonUp", function(evt) { buttonPressed(evt, false); } );