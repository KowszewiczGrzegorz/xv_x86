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
    $historyList = explode (",",  htmlspecialchars($_POST['history']));
    if (count($historyList) > 1) {
        foreach ($historyList as $historyItem) {
            if (strcmp($historyItem, "") != 0) {
                if ($nbHistory <= 1)
                    echo "        nbCommandsInHistory += 1;\n";
                echo "        commands[" . $nbHistory . "] = \"" . $historyItem . "\";\n";
                $nbHistory++;
            }
        }
    }
}
?>
        function showMoreHistory(nb) {
            i = 0;
            while (i < nb && nbCommandsInHistory < commands.length) {
                document.getElementById('historyList').innerHTML += "    <p class=\"square\" id=\"historyLine\">"
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

       function toggleConnectionLog() {
           var e = document.getElementById('connectionLog');
           if (e.style.display == 'block')
               e.style.display = 'none';
           else
               e.style.display = 'block';
       };

       function hideConnectionLog() {
           document.getElementById('connectionLog').style.display = 'none';
       };

       function clearHistory() {
           document.getElementById('history').value = "";
           document.getElementById('moreButtonDiv').innerHTML = "";
           document.getElementById('historyList').innerHTML = "";
           document.getElementById('fullHistory').style.display = 'none';
       };

       window.onload = function() {
           var cmd_input = document.getElementById ('cmd');
           cmd_input.focus();
           cmd_input.select();
       }
    </script>
</head>
<body>
<h1>Web interface for Neato XV-25</h1>

<img class="logo" src="xv-25.png" alt="neato XV-25" />

<div class="form">
    <h2>Commande</h2>
    <form id="form_id" action="webApiForm.php" method="post">

<?php
    $ip = "127.0.0.1";
    if (isset($_POST['ip']) && strcmp($_POST['ip'], "") != 0)
        $ip = $_POST['ip'];
    echo "        IP <input type=\"text\" id=\"ip\" name=\"ip\" maxlength=\"10\" size=\"10\" value=\"" . $ip . "\" />\n";
    $port = 8112;
    if (isset($_POST['port']) && strcmp($_POST['port'], "") != 0)
        $port = $_POST['port'];
    echo " port <input type=\"text\" id=\"port\" name=\"port\" maxlength=\"4\" size=\"4\" value=\"" . $port . "\" /><br/><br/>\n";
?>
        <input type="text" id="cmd" name="cmd" maxlength="30" size="30" /> 
        <input type="submit" value="OK">
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
    echo "<div class=\"form\" id=\"connectionLog\"style=\"cursor: pointer;\" onclick=\"hideConnectionLog()\" >\n";
    echo "    <h2>Connection Log</h2>\n";
    echo "    <div class=\"square\">\n";

    $service_port = $port;
    $address = gethostbyname($ip);
    $error = 0;

    if (!($socket = socket_create(AF_INET, SOCK_STREAM, 0)))
        $error = 1;
    echo "        <p class=\"". ((0 == $error) ? "ok" : "ko") . "\">\n";
    echo "        <b>Création du socket</b><br/>\n";
    if (1 == $error)
        echo "            --> socket_create() a échoué (erreur:" . socket_strerror(socket_last_error()) . ")\n";
    else
        echo "            --> OK.\n";
    echo "        </p>\n";

    if (0 == $error) {
        if (!(socket_connect($socket, $address, $service_port)))
            $error = 1;
        echo "        <p class=\"". ((0 == $error) ? "ok" : "ko") . "\">\n";
        echo "        <b>Essai de connexion à '" . $address . "' sur le port '" . $service_port . "'</b><br/>\n";
        if (1 == $error)
            echo "            --> socket_connect() a échoué (erreur:" . socket_strerror(socket_last_error($socket)) . ")\n";
        else
            echo "            --> OK.\n";
        echo "        </p>\n";
    }

    if (0 == $error) {
        $in = htmlspecialchars($_POST['cmd']) . "\n";
        if (socket_write($socket ,$in ,strlen($in)) === false)
            $error = 1;
        echo "        <p class=\"". ((0 == $error) ? "ok" : "ko") . "\">\n";
        echo "        <b>Envoi de la requête '" . htmlspecialchars($_POST['cmd']) . "\\n'</b><br/>\n";
        if (1 == $error)
            echo "            --> socket_write() a échoué (erreur:" . socket_strerror(socket_last_error($socket)) . ")\n";
        else
            echo "            --> OK.\n";
        echo "        </p>\n";
    }

    if (0 == $error) {
        $out = "";
        $response = "";
        $receivedDone = 0;
        while (0 == $receivedDone && 0 == $error) {
            if (false === socket_recv($socket, $out, 1024, MSG_WAITALL)) {
                $error = 1;
            } else {
                $response .= $out;
                if (false !== ($endOfResponse = strpos($response, ",EndOfResponse"))) {
                    $receivedDone = 1;
                    $response = substr($response, 0, $endOfResponse);
                    $response = str_replace("~", "<br/>", $response);
                }
            }
        }
        echo "        <p class=\"". ((0 == $error) ? "ok" : "ko") . "\">\n";
        echo "        <b>Lecture de la réponse</b><br/>\n";
        if (1 == $error)
                echo "            --> socket_read() a échoué (erreur:" . socket_strerror(socket_last_error($socket)) . ")\n";
        else
            echo "--> " . $response;
        echo "        </p>\n";
    }

    echo "        <p class=\"ok\">\n";
    echo "        <b>Fermeture du socket</b>\n";
    socket_close($socket);
    echo "        </p>\n";
    echo "    </div>\n";
    echo "</div>\n\n";

    echo "<div class=\"form\">\n";
    echo "    <h2>" . htmlspecialchars($_POST['cmd']) . "</h2>  \n";
    echo "    <div class=\"square\">\n";

    if (0 == $error) {
        echo $response;
    } else {
        echo "        Some error occured in the dialogue with the XV-25 ! <br/>\n";
        echo "        <b>Check the <a href=\"#\" onclick=\"toggleConnectionLog()\">connection logs</a></b>\n";
    }
    echo "    </div>\n";
    
    echo "        <div class=\"moreButton historyButtons\" id=\"connectionLogButton\">\n";
    echo "            <a href=\"#\" onclick=\"toggleConnectionLog()\"><< connection log >></a>\n";
    echo "        </div>\n";

    echo "</div>\n";
}
?>

<?php
if (isset($_POST['cmd']) || isset($_POST['history'])) {
    echo "<div class=\"form\" id=\"fullHistory\">\n";
    echo "    <h2>Historique</h2>\n";
    echo "    <div id=\"historyList\">\n";
    echo "        <p class=\"square\" id=\"historyLine\">\n";
    echo "            <a href=\"#\" onclick=\"document.getElementById('cmd').value='" . htmlspecialchars($_POST['cmd']) . "';return false;\">" . htmlspecialchars($_POST['cmd']) . "</a>\n";
    echo "        </p>\n";
    if ($nbHistory > 1) {
        echo "        <p class=\"square\" id=\"historyLine\">\n";
        echo "            <a href=\"#\" onclick=\"document.getElementById('cmd').value='" . $historyList[0] . "';return false;\">" . $historyList[0] . "</a>\n";
        echo "        </p>\n";
    }
    echo "    </div>\n";
    echo "        <div id=\"moreButtonDiv\">\n";
    if ($nbHistory > 2) {
        echo "            <div class=\"moreButton historyButtons\">\n";
        echo "                <a href=\"javascript:showMoreHistory(2)\"/><< more >></a>\n";
        echo "            </div>\n";
    }
    echo "        </div>\n";
    echo "        <div class=\"moreButton historyButtons\">\n";
    echo "            <a href=\"javascript:clearHistory()\"/><< clear >></a>\n";
    echo "        </div>\n";
    echo "</div>\n";
}
?>
</body>
</html>