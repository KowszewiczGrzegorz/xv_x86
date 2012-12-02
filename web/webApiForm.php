<html>
<head>
    <title>Web interface for Neato XV-25</title>
    <link rel="stylesheet" type="text/css" href="style.css">
    <script language="javascript" type="text/javascript">
        var commands = new Array();
<?php
echo "        nbCommandsInHistory = 0;\n";
$nbHistory = 0;
if (isset($_POST['cmd'])) {
    $nbHistory += 1;
    echo "        nbCommandsInHistory += 1;\n";
    echo "        commands[0] = \"" . htmlspecialchars($_POST['cmd']) . "\";\n";
}
if (isset($_POST['history'])) {
    // Slit post string and count
    /*
    if ($nb > 0) {
        echo "nbCommandsInHistory += " .   . "\n";

        for ($j = 0; $j < $nb; $j++)
            echo "        commands[" . ($j+1) . "] = \"" . htmlspecialchars($_POST['history']) . "\";\n";
    }
    */
}

?>
        function showMoreHistory(nb) {
            i = 0;
            while (i < nb && nbCommandsInHistory < commands.length) {
                document.getElementById('historyList').innerHTML += "    <p class=\"history\">"
                         + "<a href=\"#\" onclick=\"document.getElementById('cmd').value='" 
                         + commands[nbCommandsInHistory] + "';return false;\">" 
                         + commands[nbCommandsInHistory] + "</a></p>\n";
	        nbCommandsInHistory++;
                i++;
            }
            if (nbCommandsInHistory >= commands.length) {
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
    <form id="form_id" action="webApiForm.php" method="post">
        <input type="cmd" id="cmd" name="cmd" maxlength="30" size="30" /> <input type="submit" value="OK">
<?php
$history = "";
if (isset($_POST['history'])) {
    $history = htmlspecialchars($_POST['history']);
}
if (isset($_POST['cmd']))
    $history = htmlspecialchars($_POST['cmd']) . "," . $history;
echo "        <input type=\"hidden\" id=\"history\" name=\"history\" value=\"" . $history . "\" />\n";
?>
    </form>
</div>

<?php
error_reporting(E_ALL);

if (isset($_POST['cmd'])) {
    echo "<div class=\"form\">\n";
    echo "    <h2>" . htmlspecialchars($_POST['cmd']) . "</h2>\n";
    echo "    <div class=\"response\">\n";

    $service_port = 8112;
    $address = gethostbyname('127.0.0.1');

    echo "        Création du socket : <br/>\n";
    if (!($socket = socket_create(AF_INET, SOCK_STREAM, 0))) {
        echo "            socket_create() a échoué (erreur:" . socket_strerror(socket_last_error()) . ")<br/>\n";
    } else {
        echo "            OK.<br/>\n";
    }
    
    echo "        Essai de connexion à '$address' sur le port '$service_port' : <br/>\n";
    if (!(socket_connect($socket, $address, $service_port))) {
        echo "            socket_connect() a échoué (erreur:" . socket_strerror(socket_last_error($socket)) . ")<br/>\n";
    } else {
        echo "            OK.<br/>\n";
    }
    
    $in = htmlspecialchars($_POST['cmd']) . "\n";
    echo "        Envoi de la requête '" . htmlspecialchars($_POST['cmd']) . "\\n' : <br/>\n";
    if (socket_write($socket ,$in ,strlen($in)) === false) {
        echo "            socket_write() a échoué (erreur:" . socket_strerror(socket_last_error($socket)) . ")<br/>\n";
    } else {
        echo "            OK.<br/>\n";
    }
    
    echo "        Lecture de la réponse : <br/>\n";
    $out = '';
    while (1) {
        if(($out = socket_read($socket, 2048)) === false) {
            echo "            socket_read() a échoué (erreur:" . socket_strerror(socket_last_error($socket)) . ")<br/>\n";
            break;
        } else {
            echo $out;
        }
    }

    echo "        Fermeture du socket...<br/>\n";
    socket_close($socket);
    echo "        OK.<br/>\n\n";
    echo "    </div>\n";
}
?>
</div>

<?php
  /*
if (isset($_POST['response'])) {
    echo "<div class=\"form\">\n";
    echo "<h2>" . htmlspecialchars($_POST['cmd_response']) . "</h2>\n";
    echo "<p class=\"response\">\n";
    echo "" . str_replace("\n", "<br/>\n", htmlspecialchars($_POST['response'])) . "\n";
    echo "</p>\n":
    echo "</div>\n";
}
  */
?>

<?php
if (isset($_POST['cmd']) || isset($_POST['history'])) {
    echo "<div class=\"form\">\n";
    echo "    <h2>Historique</h2>\n";
    echo "    <div id=\"historyList\">\n";
    echo "        <p class=\"history\">\n";
    echo "            <a href=\"#\" onclick=\"document.getElementById('cmd').value='" .  htmlspecialchars($_POST['cmd'])  . "';return false;\">GetVersion</a>\n";
    echo "        </p>\n";
    echo "    </div>\n";
    echo "    <div class=\"centered\" id=\"moreButtonDiv\">\n";
    if (nbHistory > 1) {
        echo "        <div id=\"moreButton\">\n";
        echo "<a href=\"javascript:showMoreHistory(2)\"/><< more >></a>\n";
        echo "        </div>\n";
    }
    echo "    </div>\n";
    echo "</div>\n";
}
?>
</body>
</html>