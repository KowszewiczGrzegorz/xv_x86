<html>
<head><title>Web API Test</title></head>
<body>
<?php
    error_reporting(E_ALL);

    echo "<h2>Web API Test</h2>\n";

    /* Lit le port du service WWW. */
    // $service_port = getservbyname('www', 'tcp');
    $service_port = 8112

    /* Lit l'adresse IP du serveur de destination */
    $address = gethostbyname('127.0.0.1');

    /* Crée un socket TCP/IP. */
    $socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
    if ($socket === false) {
        echo "socket_create() a échoué : raison :  " . socket_strerror(socket_last_error()) . "\n";
    } else {
        echo "OK.\n";
    }

    echo "Essai de connexion à '$address' sur le port '$service_port'...";
    $result = socket_connect($socket, $address, $service_port);
    if ($socket === false) {
        echo "socket_connect() a échoué : raison : ($result) " . socket_strerror(socket_last_error($socket)) . "\n";
    } else {
        echo "OK.\n";
    }

    $in = "GetVersion\n";

    echo "Envoi de la requête '$in'\n";
    socket_write($socket, $in, strlen($in));
    echo "OK.\n";

    echo "Lire la réponse : \n";
    $out = '';
    while ($out = socket_read($socket, 2048)) {
        echo $out;
    }

    echo "Fermeture du socket...";
    socket_close($socket);
    echo "OK.\n\n";
?>
</body>
</html>