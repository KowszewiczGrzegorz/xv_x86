function updateStatus() {
    window.webkitRequestAnimationFrame(updateStatus);

    var gamepads = navigator.webkitGamepads;

    var data = '';
    for (var padindex = 0; padindex < gamepads.length; ++padindex)
    {
        var pad = gamepads[padindex];
        if (!pad) continue;
        data += '<b>' + pad.index + "</b> : " + pad.id + "<br/>";
        for (var i = 0; i < pad.buttons.length; ++i)
            data += "button" + i + ": " + pad.buttons[i] + "<br/>";
        for (var i = 0; i < pad.axes.length; ++i)
            data += "axis" + i + ": " + pad.axes[i] + "<br/>";
    }
    document.getElementById('gamepadInfo').innerHTML = data;
}

window.webkitRequestAnimationFrame(updateStatus);

