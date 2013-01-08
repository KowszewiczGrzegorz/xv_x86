
<?php
error_reporting(E_ALL);

if (isset($_POST['cmd'])) {

    echo "<div class=\"form\" id=\"connectionLog\" style=\"cursor: pointer;\">\n";
    echo "    <h2>Connection Log</h2>\n";
    echo "    <div class=\"square\">\n";

    $ip = $_COOKIE['ipCookie'];
    $port = $_COOKIE['portCookie'];

    $service_port = $port;
    $address = gethostbyname($ip);
    $error = 0;

    if (!($socket = socket_create(AF_INET, SOCK_STREAM, 0)))
        $error = 1;
    echo "        <p class=\"". ((0 == $error) ? "ok" : "ko") . "\">\n";
    echo "        <b>Creation du socket</b><br/>\n";
    if (1 == $error)
        echo "            --> socket_create() a echoue (erreur:" . socket_strerror(socket_last_error()) . ")\n";
    else
        echo "            --> OK.\n";
    echo "        </p>\n";

    if (0 == $error) {
        if (!(socket_connect($socket, $address, $service_port)))
            $error = 1;
        echo "        <p class=\"". ((0 == $error) ? "ok" : "ko") . "\">\n";
        echo "        <b>Essai de connexion a '" . $address . "' sur le port '" . $service_port . "'</b><br/>\n";
        if (1 == $error)
            echo "            --> socket_connect() a echoue (erreur:" . socket_strerror(socket_last_error($socket)) . ")\n";
        else
            echo "            --> OK.\n";
        echo "        </p>\n";
    }

    if (0 == $error) {
        $in = htmlspecialchars($_POST['cmd']) . "\n";
        if (socket_write($socket ,$in ,strlen($in)) === false)
            $error = 1;
        echo "        <p class=\"". ((0 == $error) ? "ok" : "ko") . "\">\n";
        echo "        <b>Envoi de la requete '" . htmlspecialchars($_POST['cmd']) . "\\n'</b><br/>\n";
        if (1 == $error)
            echo "            --> socket_write() a echoue (erreur:" . socket_strerror(socket_last_error($socket)) . ")\n";
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
        echo "        <b>Lecture de la reponse</b><br/>\n";
        if (1 == $error)
                echo "            --> socket_read() a echoue (erreur:" . socket_strerror(socket_last_error($socket)) . ")\n";
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

    echo "<div id=\"responseText\" class=\"form\">\n";
    echo "    <h2>" . htmlspecialchars($_POST['cmd']) . "</h2>  \n";
    echo "    <div class=\"square\">\n";

    if (0 == $error) {
        echo $response;
    } else {
        echo "        Some error occured in the dialogue with the XV-25 ! <br/>\n";
        echo "        <b>Check the <a href=\"#\" onclick=\"toggleConnectionLog()\">connection logs</a></b>\n";
    }
    echo "    </div>\n";
    
    echo "        <div class=\"moreButton historyButtons\" id=\"connectionLogButton\"><< connection log >></div>\n";

    echo "</div>\n";
}
?>
