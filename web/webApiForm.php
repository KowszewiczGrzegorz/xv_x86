<html>
<head>
    <title>Web interface for Neato XV-25</title>
    <link rel="stylesheet" type="text/css" href="style.css">
    <script language="javascript" type="text/javascript">
        nbCommandsInHistory = 2;
        var commands = new Array();
<?php

?>

    function showMoreHistory(nb) {
        i = 0;
        while (i < nb && nbCommandsInHistory < commands.length) {
            document.getElementById('historyList').innerHTML += "    <p class=\"history\"><a href=\"#\" onclick=\"document.getElementById('cmd').value='" 
                                                                + commands[nbCommandsInHistory] + "';return false;\">" 
                                                                + commands[nbCommandsInHistory] + "</a></p>\n";
	    nbCommandsInHistory++;
            i++;
        }
        if ( nbCommandsInHistory >= commands.length) {
            document.getElementById('moreButtonDiv').innerHTML = "";
        }
    };
</script>
</head>
<body>
<h1>Web interface for Neato XV-25</h1>

<img class="logo" src="xv-25.png" alt="neato XV-25" />

<div class="form">
    <h2>Commande</h2>
    <form id="form_id" action="webAPIForm.php" method="post">
        <input type="cmd" id="cmd" name="nom" maxlength="30" size="30" /> <input type="submit" value="OK">
    </form>

<?php
error_reporting(E_ALL);

if (isset($_POST['cmd'])) {

    echo "<div class=\"response\">\n";

    $service_port = 8112;
    $address = gethostbyname('127.0.0.1');

    echo "Création du socket : ";
    if (!($socket = socket_create(AF_INET, SOCK_STREAM, 0))) {
        echo "socket_create() a échoué (erreur:" . socket_strerror(socket_last_error()) . ")<br/>\n";
    } else {
        echo "OK.<br/>\n";
    }
    
    echo "Essai de connexion à '$address' sur le port '$service_port' : ";
    if (!(socket_connect($socket, $address, $service_port))) {
        echo "socket_connect() a échoué (erreur:" . socket_strerror(socket_last_error($socket)) . ")<br/>\n";
    } else {
        echo "OK.<br/>\n";
    }
    
    $in = htmlspecialchars($_POST['cmd']) . "\n";
    echo "Envoi de la requête '" . htmlspecialchars($_POST['cmd']) . "\\n' : ";
    if (socket_write($socket ,$in ,strlen($in)) === false) {
        echo "socket_write() a échoué (erreur:" . socket_strerror(socket_last_error($socket)) . ")<br/>\n";
    } else {
        echo "OK.<br/>\n";
    }
    
    echo "Lecture de la réponse : \n";
    $out = '';
    while (1) {
        if(($out = socket_read($socket, 2048)) === false) {
            echo "<br/>socket_read() a échoué (erreur:" . socket_strerror(socket_last_error($socket)) . ")<br/>\n";
            break;
        } else {
            echo $out;
        }
    }

    echo "Fermeture du socket...<br/>";
    socket_close($socket);
    echo "OK.<br/>\n\n";
    echo "</div>\n";
}
?>
</div>

<?php
if (isset($_POST['response'])) {
    echo "<div class="form">\n";
    echo "<h2>" . htmlspecialchars($_POST['cmd_response']) . "</h2>\n";
    echo "<p class=\"response\">\n";
    echo "" . str_replace("\n", "<br/>\n", htmlspecialchars($_POST['response'])) . "\n";
    echo "</p>\n":
    echo "</div>\n";
}
?>

<?php
if (isset($_POST['history'])) {
    echo "<div class=\"form\">\n";
    echo "<h2>Historique</h2>\n";
    echo "<div id=\"historyList\">\n";
    echo "    <p class=\"history\"><a href=\"#\" onclick=\"document.getElementById('cmd').value='GetVersion';return false;\">GetVersion</a></p>\n";
    echo "    <p class=\"history\"><a href=\"#\" onclick=\"document.getElementById('cmd').value='SetMotor';return false;\">SetMotor</a></p>\n";
    echo "</div>\n";
    echo "<div class=\"centered\" id=\"moreButtonDiv\"><div id=\"moreButton\"><a href=\"javascript:showMoreHistory(2)\"/><< more >></a></div></div>\n";
    echo "</div>\n";
}     

<?php

?>
</body>
</html>