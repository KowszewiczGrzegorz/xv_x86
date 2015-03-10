var commands = new Array();
var nbCommandeInHistory = 0;

function showMoreHistory(nb) {
    i = 0;
    while (i < nb && nbCommandsInHistory < commands.length) {
        document.getElementById('historyList').innerHTML += "    <p class=\"square\" id=\"historyLine\">"
            + "<a href=\"#\" onclick=\"document.getElementById('cmd').value='"
            + commands[nbCommandsInHistory] + "'; setCursor();return false;\">"
            + commands[nbCommandsInHistory] + "</a></p>\n";
        nbCommandsInHistory++;
        i++;
    }
    if (nbCommandsInHistory >= commands.length) {
        document.getElementById('moreButtonDiv').innerHTML = "";
    } else {
        document.getElementById('moreButtonDiv').innerHTML = ""
            + "<div class=\"moreButton historyButtons\">\n"
            + "<a href=\"javascript:showMoreHistory(" + (nb+2) + ")\"/>"
            + "<< (" + (commands.length-nbCommandsInHistory) + ") more >></a>"
            + "</div>\n";
    }
};

function add2History(cmd) {
    commands[commands.length] = cmd;
    var currentHistory = document.getElementById('historyList').innerHTML;
    document.getElementById('historyList').innerHTML = "    <p class=\"square\" id=\"historyLine\" "
            + "onclick=\"document.getElementById('cmd').value='"
            + cmd + "'; setCursor();\" >"
            + cmd + "</p>\n"
            + currentHistory;

}

function clearHistory() {
    nbCommandsInHistory = 0;
    commands = new Array();
    // document.getElementById('moreButtonDiv').innerHTML = "";
    document.getElementById('historyList').innerHTML = "";
    document.getElementById('fullHistory').style.display = 'none';
};

function setCursor () {
    var cmd_input = document.getElementById ('cmd');
    cmd_input.focus();
    // cmd_input.select();
}

function showConnectionLog() {
    $("#connectionLog").slideDown("slow");
}

function hideConnectionLog() {
    $("#connectionLog").slideUp("slow");
}

function submitEnter(myfield,e) {
    var keycode;
    if (window.event) 
        keycode = window.event.keyCode;
    else if (e) 
        keycode = e.which;
    else return true;

    if (keycode == 13) {
        sendCommand();
        return false;
    } else {
        return true;
    }
}

function sendCommand() {
    var cmd = document.getElementById('cmd').value;
    var xmlhttp;
    if (window.XMLHttpRequest) {
        xmlhttp = new XMLHttpRequest();
    } else {
        xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
    }
    xmlhttp.onreadystatechange = function() {
        if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
            document.getElementById("response").innerHTML = xmlhttp.responseText;
            $("#response").slideDown("fast");
            document.getElementById('cmd').disabled="";
            document.getElementById('connectionLogButton').disabled="";
            document.getElementById('fullHistory').style.display = 'block';
            $("#loading_img").fadeOut("slow");
        }
    }
    xmlhttp.open("POST", "commandes_exec.php", true);
    xmlhttp.setRequestHeader('Content-Type','application/x-www-form-urlencoded');
    xmlhttp.send("cmd="+cmd);

    document.getElementById('cmd').value = "";
    document.getElementById('cmd').disabled="disabled";
    document.getElementById('connectionLogButton').disabled="disabled";

    add2History(cmd);

    $("#loading_img").fadeIn("slow");
    if ($("#connectionLog").is(":visible"))
        $("#connectionLog").slideUp("fast");
}
